
#include <string>
#include <ArduinoMqttClient.h>
#include <WiFiNINA.h>
#include <WiFi.h>
#include "arduino_secrets.h"
#include <Servo.h>

Servo myservo; 
int pos = 0; 
char ssid[] = SECRET_SSID;   
char pass[] = SECRET_PASS;   

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "192.168.0.17";
int port = 1883;
const char topic[]  = "station1";
char msg[6];
char left[]="left1";
char right[]="right";

void setup() {

  myservo.attach(9); 
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    delay(2000);
  }

  
  if (!mqttClient.connect(broker, port)) {
    while (1);
  }

  mqttClient.subscribe(topic);

}

void loop() {
  int messageSize = mqttClient.parseMessage();
  if (messageSize) {
   
    // use the Stream interface to print the contents
    int i=0;
    while (mqttClient.available()) {
    
      msg[i]=(char)mqttClient.read();
      i++;
    }

     if (strcmp(msg, left) == 0){
         for (pos = 0; pos <= 180; pos += 1){ // goes from 0 degrees to 180 degrees
          // in steps of 1 degree
          myservo.write(pos);              // tell servo to go to position in variable 'pos'
          delay(15);                       // waits 15ms for the servo to reach the position
        }
    }
    else if(strcmp(msg, right) == 0){
           for (pos = 180; pos >= 0; pos -= 1){ // goes from 180 degrees to 0 degrees
              myservo.write(pos);              // tell servo to go to position in variable 'pos'
              delay(15);                       // waits 15ms for the servo to reach the position
           }
        
        }

  }
}
