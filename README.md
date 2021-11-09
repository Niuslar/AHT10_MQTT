# MQTT with Raspberry Pi and ESP32 

This project had two main goals:
 1. Learn more about MQTT for IoT projects
 2. Get used to the ESP32 development framework
  
To use MQTT, I decided to use a Raspberry Pi as the broker using Mosquitto. 
Once the broker was set up, I adapted my AHT10 temperature and humidity driver from an ATMega328 to work with the ESP32. 
After this, I used the ESP-IDF examples as base, and was able to connect the ESP32 to the broker and send the temperature and humidity data. 
To display the data I used the Paho Javascript library and used websockets to subscribe to the different topics. 

This project was very helpful to get used to the ESP IoT Framework and understand concepts like Events, Event Groups and Event Loops better.

The result is a simple website that shows the temperature and humidity obtained from the sensor. 


![Screenshot from 2021-11-09 13-20-35](https://user-images.githubusercontent.com/34103285/140938859-9c85ab51-6811-4921-be93-f02f2557b97e.png)

