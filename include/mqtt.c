#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_system.h"
#include "esp_event.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

#include "esp_log.h"
#include "mqtt.h"

static const char *TAG = "MQTT_MODULE";


static esp_err_t mqtt_event_handler_cb(esp_mqtt_event_handle_t event)
{

    switch (event->event_id) {
        case MQTT_EVENT_BEFORE_CONNECT:
            ESP_LOGI(TAG, "The client is initialized and about to start connecting to the broker.");
            break;
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "The client has successfully established a connection to the broker.");
            break;
        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAG, "The broker has acknowledged the client’s subscribe request.");
            break;     
        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG, "The broker has acknowledged the client’s publish message.");
            break;               
        default:
            ESP_LOGI(TAG, "Other event id:%d", event->event_id);
            break;
    }
    return ESP_OK;
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    mqtt_event_handler_cb(event_data);
}

void mqtt_app_start(esp_mqtt_client_handle_t client)
{

    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
    esp_mqtt_client_start(client);
}
