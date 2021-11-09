#include <stdio.h>
#include "../include/aht10.h"
#include "../include/wifi.h"
#include "../include/mqtt.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "mqtt_client.h"


void app_main(void)
{

    static const char *TAG = "MQTT_AHT10";

    //Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);


    //Start WiFi
    ESP_LOGI(TAG, "ESP_WIFI_MODE_STA");
    wifi_init_sta();

    //AHT10 
    float temp, hum; 
    aht10_init();

    //MQTT
    esp_mqtt_client_config_t mqtt_cfg = {
        .uri = "mqtt://username:password@host:port",
    };

    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);

    mqtt_app_start(client);


    while(1)
    {
        temp = read_temperature();
        hum = read_humidity();
        char temp_str[30];
        char hum_str[30];

        sprintf(temp_str,"%.2f\n", temp);
        esp_mqtt_client_publish(client, "topic/temp", temp_str, 0, 0, 0);
        sprintf(hum_str,"%.2f\n", hum);
        esp_mqtt_client_publish(client, "topic/hum", hum_str, 0, 1, 0);
        vTaskDelay(2000/portTICK_PERIOD_MS);
    }
    return;

}
