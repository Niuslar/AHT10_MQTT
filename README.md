# MQTT with Raspberry Pi and ESP32 

This project had two main goals:
 1. Learn more about MQTT for IoT projects
 2. Get used to the ESP32 development framework
  
To use MQTT, I decided to use a Raspberry Pi as the broker using Mosquitto. 
Once the broker was set up, I worked on the ESP32 side of the project. 
Doing this using the Arduino Framework would have been much easier, but I wanted to use the ESP-IDF to understand better how things worked. 

The first step was to adapt my AHT10 temperature and humidity driver from an ATMega328 to work with the ESP32. 
After this, I used the ESP-IDF WiFi and MQTT examples as base, and was able to connect the ESP32 to the broker and publish the temperature and humidity data. 
To display the data I used the Paho Javascript library and used websockets to subscribe to the different topics.

The result is a simple website that shows the temperature and humidity readings in real time and a graph with the day's history. 

The data used to create the graph comes from a .csv file created with a Python script (mqtt_read.py) running on the Raspberry Pi. The script reads the sensors' values and every two minutes it appends the data to a .csv file named after the date. Each day a new file is created for each sensor. 

![Screenshot from 2021-11-11 17-04-49](https://user-images.githubusercontent.com/34103285/141339095-ad1bf19b-9a84-40e1-83bf-b9c639753dd5.png)


![Screenshot from 2021-11-11 17-03-16](https://user-images.githubusercontent.com/34103285/141338847-c3fa0bff-13f9-4a4f-9219-f0f043fb2043.png)
