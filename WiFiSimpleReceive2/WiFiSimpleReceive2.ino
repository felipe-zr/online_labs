/*
  ArduinoMqttClient - WiFi Simple Receive

  This example connects to a MQTT broker and subscribes to a single topic.
  When a message is received it prints the message to the Serial Monitor.

  The circuit:
  - Arduino MKR 1000, MKR 1010 or Uno WiFi Rev2 board

  This example code is in the public domain.
*/
#include <string>
#include <ArduinoMqttClient.h>
//#include <WiFiNINA.h>
#include <WiFi.h>
#include <Servo.h>
#include "arduino_secrets.h"

Servo myservo;

//define pin led
const int RED_LED_PIN = 15;
const int GREEN_LED_PIN = 14;
// defines pins numbers ultrasonic sensor
const int trigPin = 9;
const int echoPin = 10;
// defines variables
long duration;
int distance;

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;    // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)

// To connect with SSL/TLS:
// 1) Change WiFiClient to WiFiSSLClient.
// 2) Change port value from 1883 to 8883.
// 3) Change broker value to a server with a known SSL/TLS root certificate 
//    flashed in the WiFi module.

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "192.168.0.17";
int        port     = 1883;
const char topic[]  = "station2";
char msg[6];
char distances[]="dista";
char inc_dist[]="idist";
char dec_dist[]="ddist";
char red_led[]="redld";
char green_led[]="greld";
int red_state=0;
int green_state=0;
int pos=180;
void setup() {
  //Initialize serial and wait for port to open:
  //Serial.begin(9600);
 // while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
 // }
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  myservo.attach(12);
 
  // Serial.begin(9600); // Starts the serial communication
 
  // attempt to connect to WiFi network:
  // Serial.print("Attempting to connect to WPA SSID: ");
  //Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed, retry
    //  Serial.print(".");
    delay(2000);
  }

  //Serial.println("You're connected to the network");
  //Serial.println();

  // You can provide a unique client ID, if not set the library uses Arduino-millis()
  // Each client must have a unique client ID
  // mqttClient.setId("clientId");

  // You can provide a username and password for authentication
  // mqttClient.setUsernamePassword("username", "password");

  // Serial.print("Attempting to connect to the MQTT broker: ");
  // Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
   // Serial.print("MQTT connection failed! Error code = ");
   // Serial.println(mqttClient.connectError());

    while (1);
  }

 // Serial.println("You're connected to the MQTT broker!");
 // Serial.println();

 // Serial.print("Subscribing to topic: ");
 // Serial.println(topic);
 // Serial.println();

 // subscribe to a topic
    mqttClient.subscribe(topic);

 // topics can be unsubscribed using:
 // mqttClient.unsubscribe(topic);

 // Serial.print("Waiting for messages on topic: ");
 // Serial.println(topic);
 // Serial.println();
}

void loop() {
  int messageSize = mqttClient.parseMessage();
  if (messageSize) {
    // we received a message, print out the topic and contents
    // Serial.print("Received a message with topic '");
    // Serial.print(mqttClient.messageTopic());
    // Serial.print("', length ");
    // Serial.print(messageSize);
    // Serial.println(" bytes:");

    // use the Stream interface to print the contents
    int i=0;

    while (mqttClient.available()) {
       msg[i]=(char)mqttClient.read();
       i++;
    }
    // Serial.println(msg);
     if (strcmp(msg, distances) == 0){
	    for (pos = 180; pos >= 0; pos -= 5) { // goes from 180 degrees to 0 degrees
	    myservo.write(pos);                 // tell servo to go to position in variable 'pos'
	     delay(45);
            // waits 45 ms for the servo to reach the position
	    		    
            // Measure distance
	    // Clears the trigPin
	    digitalWrite(trigPin, LOW);
	    delayMicroseconds(2);
	    // Sets the trigPin on HIGH state for 10 micro seconds
	    digitalWrite(trigPin, HIGH);
	    delayMicroseconds(10);
	    digitalWrite(trigPin, LOW);
	    // Reads the echoPin, returns the sound wave travel time in microseconds
	    duration = pulseIn(echoPin, HIGH);
	    // Calculating the distance
	    distance = duration * 0.034 / 2;
            // Send distance
	    mqttClient.beginMessage(topic);
            mqttClient.print(distance);
            mqttClient.endMessage();
	    delay(100);
          }
         //reset actuator position
	 for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
	    // in steps of 1 degree
	    myservo.write(pos);              // tell servo to go to position in variable 'pos'
	    delay(15);                       // waits 15 ms for the servo to reach the position
	  }
      // Prints the distance on the Serial Monitor
      // Serial.print("Distance: ");
      // Serial.println(distance);
    }
    else if (strcmp(msg, inc_dist) == 0){
            int val;
	    for (val = 1; val <= 18; val=val+1) { // goes from 0 degrees to 180 degrees
	    // in steps of 1 degree
            pos=pos+val;
	    myservo.write(pos);              // tell servo to go to position in variable 'pos'
	    sleep(15);  			// waits 15 ms for the servo to reach the position
	    
	    }                     
    }
    else if (strcmp(msg, dec_dist) == 0){
            int val;
	    for (val = 1; val <= 18; val=val+1) { // goes from 0 degrees to 180 degrees
	    // in steps of 1 degree
            pos=pos-val;
	    myservo.write(pos);              // tell servo to go to position in variable 'pos'
	    sleep(15); 
 	    }
    }
    else if (strcmp(msg, red_led) == 0){
            if (red_state ==0){
              digitalWrite(RED_LED_PIN, HIGH);
              red_state =1;
            }
            else if (red_state ==1){
                    digitalWrite(RED_LED_PIN, LOW);
                    red_state =0;
                  }
        }else if (strcmp(msg, green_led) == 0){
                    if (green_state ==0){
                          digitalWrite(GREEN_LED_PIN, HIGH);
                          green_state =1;
                        }
                        else if (green_state ==1){
                                digitalWrite(GREEN_LED_PIN, LOW);
                                green_state =0;
                               }
             }
  // Serial.println();
  // Serial.println();
  }
}
