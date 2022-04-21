idf_component_register(SRCS "web-server.c"
                    INCLUDE_DIRS "include"
                    REQUIRES esp_http_server spiffs)
