#ifndef WIFI_H
#define WIFI_H

#define WIFI_SSID      "SSID"
#define WIFI_PASS      "PASSWORD"


#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

#define EXAMPLE_ESP_MAXIMUM_RETRY  5

void wifi_init_sta(void);

#endif /*WIFI_H*/