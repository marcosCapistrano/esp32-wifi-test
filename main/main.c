#include "connect.h"
#include "web-server.h"

void app_main(void)
{
	wifi_init();
	web_server_init();
}