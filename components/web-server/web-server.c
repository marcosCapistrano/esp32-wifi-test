#include "web-server.h"

#include <stdio.h>

#include "esp_http_server.h"
#include "esp_log.h"
#include "esp_spiffs.h"

static const char *TAG = "WEB_SERVER";

static esp_err_t on_default_url(httpd_req_t *req) {
    ESP_LOGI(TAG, "URL: %s", req->uri);

    esp_vfs_spiffs_conf_t esp_vfs_spiffs_conf = {
        .base_path = "/spiffs",
        .partition_label = NULL,
        .max_files = 5,
        .format_if_mount_failed = true,
    };

    esp_vfs_spiffs_register(&esp_vfs_spiffs_conf);

    char path[600];
    if (strcmp(req->uri, "/") == 0) {
        strcpy(path, "/spiffs/index.html");
    } else {
				sprintf(path, "/spiffs%s", req->uri);
		}
		char *ext = strrchr(path, '.');
		if(strcmp(ext, ".css") == 0) {
			httpd_resp_set_type(req, "text/css");
		} else if(strcmp(ext, ".js") == 0) {
			httpd_resp_set_type(req, "text/javascript");
		} else if(strcmp(ext, ".png") == 0) {
			httpd_resp_set_type(req, "image/png");
		}

    FILE *file = fopen(path, "r");
    if (file == NULL) {
        httpd_resp_send_404(req);
        esp_vfs_spiffs_unregister(NULL);
        return ESP_OK;
    }

		char line_read[256];
		while(fgets(line_read, sizeof(line_read), file)) {
			httpd_resp_sendstr_chunk(req, line_read);
		}
		httpd_resp_sendstr_chunk(req, NULL);

    esp_vfs_spiffs_unregister(NULL);
    return ESP_OK;
}

void web_server_init() {
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
		config.uri_match_fn = httpd_uri_match_wildcard;

    ESP_ERROR_CHECK(httpd_start(&server, &config));

		/* DEIXE ESSE ROUTE WILDCARD APOS TODAS AS ROTAS */
    httpd_uri_t default_url = {
        .uri = "/*",
        .method = HTTP_GET,
        .handler = on_default_url};

    httpd_register_uri_handler(server, &default_url);
}
