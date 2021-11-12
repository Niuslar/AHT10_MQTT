import paho.mqtt.client as mqtt
from time import sleep
from datetime import datetime
from pathlib import Path

def create_file(date):
    my_file=Path(f"{date}.csv")
    my_file2=Path(f"{date}-bedroom.csv")
    if not my_file.is_file():
        f = open(f"{date}.csv", "w")
        f.write("Date,Time,Temperature,Humidity\n")
        f.close()
    if not my_file2.is_file():
        f2 = open(f"{date}-bedroom.csv", "w")
        f2.write("Date,Time,Temperature,Humidity\n")
        f2.close()

# Callback to see if the connection worked 
def on_connect(client, userdata, flags, rc):
    if rc==0:
        print("connected OK Returned code=",rc)
        
    else:
        print("Bad connection Returned code=",rc)

    client.subscribe("main/hum")
    client.subscribe("main/temp")
    client.subscribe("bed/temp")
    client.subscribe("bed/hum")
       

# Callback when a message is received 
def on_message(client, userdata, msg):
    if(msg.topic == "main/hum"):
        hum_main_str = str(msg.payload).split("'")
        global hum_main
        hum_main = float(hum_main_str[1])
        print(hum_main)
    if(msg.topic == "main/temp"):
        temp_main_str = str(msg.payload).split("'")
        global temp_main
        temp_main = float(temp_main_str[1])
        print(temp_main)
    if(msg.topic == "bed/temp"):
        temp_bed_str = str(msg.payload).split("'")
        global temp_bed
        temp_bed = float(temp_bed_str[1])
        print(temp_bed)
    if(msg.topic == "bed/hum"):
        hum_bed_str = str(msg.payload).split("'")
        global hum_bed
        hum_bed = float(hum_bed_str[1])
        print(hum_bed)

 
temp_main = 0
hum_main = 0
temp_bed = 0
hum_bed = 0

  

def main():
    global temp_main
    global hum_main
    global temp_bed
    global hum_bed

    # MQTT Client configuration
    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_message = on_message
    client.username_pw_set(username="username",password="password")
    client.connect("HOST", "PORT", 60)

    # MQTT Loop starts in another thread to allow main to keep doing stuff 
    client.loop_start()

    dateObj = datetime.now()
    date = ""
    
    # Ininite loop to write data every 2 minutes
    while True:
        currentDateObj = datetime.now()
        currentDate = f'{currentDateObj.day}-{currentDateObj.month}-{currentDateObj.year}'
        currentTime = f'{currentDateObj.hour}:{currentDateObj.minute}:{currentDateObj.second}'
        #check if we are in the same day
        if(currentDate == date):
            f = open(f"{date}.csv","a")
            f2 = open(f"{date}-bedroom.csv","a")
            f.write(f"{date},{currentTime},{temp_main},{hum_main}\n")
            f2.write(f"{date},{currentTime},{temp_bed},{hum_bed}\n")
            f.close()
            f2.close()
        #if it is not the same day it creates new files
        else:
            date = currentDate
            create_file(date)

        sleep(120)

if __name__ == "__main__":
    main()

