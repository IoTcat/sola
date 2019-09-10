/*
 * @Author: IoTcat (https://iotcat.me) 
 * @Date: 2019-08-20 09:57:58 
 * @Last Modified by: 
 * @Last Modified time: 2019-09-10 14:44:11
 */

#include <PubSubClient.h>
#include <ESP8266WiFi.h>

#include "relay.h"

const char* ssid = "yimian-iot";
const char* password = "1234567890.";
const char* mqtt_server = "192.168.3.4";  // change this to the mqtt server

const char* topicInCtl = "hass/ctl/wc0/#";  // change this to the outgoing messages
const char* topicInRefresh = "hass/refresh";

const String clientId = "wc0";

WiFiClient espClient;
PubSubClient client(espClient);

Relay valve(D5, false);

volatile double waterFlow = 0, markFlow = 0;
long unsigned int LastSentTime = millis();


void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);

    attachInterrupt(0, pulse, RISING);  //DIGITAL Pin 2: Interrupt 0

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    pinMode(D10, OUTPUT);
    digitalWrite(D10, HIGH);
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    client.setServer(mqtt_server, 1883);  // default port, change it...
    client.setCallback(callback);

    valve.ini();


}



void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    String s = "";
    for (int i = 0; i < length; i++) {
        s += (char)payload[i];
    }
    Serial.print(s);
    Serial.println("");

    mqtt_refresh(topic, s);
    mqtt_valve(topic, s);
    mqtt_flowmeter(topic, s);
}

void reconnect() {
    // Loop until we're reconnected
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");

        // Attempt to connect
        if (client.connect(clientId.c_str())) {
            Serial.println("connected");
            // Once connected, publish an announcement...
            // client.publish(topicOut, "Hello from the Gateway!");
            // ... and resubscribe
            client.subscribe(topicInCtl);
            client.subscribe(topicInRefresh);
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 3 seconds before retrying
            delay(3000);
        }
    }
}

void pulse()   //measure the quantity of square wave
{
  waterFlow += 1.0 / 450.0;
}




void loop() {

    // mqtt core
    if (!client.connected()) {
        reconnect();
    }
    client.loop();

    // component core

    // valve trigger
    if(valve.isStateChange()){
        client.publish(String("hass/snsr/"+clientId+"/valve").c_str(), String(valve.getStatus()).c_str());
    }

    if(waterFlow > markFlow + 0.2 && LastSentTime < millis() - 10 * 1000){
        client.publish(String("hass/snsr/"+clientId+"/flowmeter").c_str(), String(waterFlow - markFlow).c_str());
        LastSentTime = millis();
    }

}


/**** mqtt func ****/

void mqtt_refresh(const String& subject, const String& content){

    // send all status
    if(subject == topicInRefresh){
        client.publish(String("hass/snsr/"+clientId+"/valve").c_str(), String(valve.getStatus()).c_str());
        client.publish(String("hass/snsr/"+clientId+"/flowmeter").c_str(), String(waterFlow - markFlow).c_str());
    }

}
void mqtt_valve(const String& subject, const String& content){
    if(subject == String("hass/ctl/"+clientId+"/valve")){
        if(content == "0"){
            valve.off();
        }
        if(content == "1"){
            valve.on();
        }
    }
}

void mqtt_flowmeter(const String& subject, const String& content){
    if(subject == String("hass/ctl/"+clientId+"/flowmeter")){
        client.publish(String("hass/snsr/"+clientId+"/flowmeter").c_str(), String(waterFlow - markFlow).c_str());
    }
    if(subject == String("hass/ctl/"+clientId+"/flowmeterGotten")){
        markFlow = waterFlow;
    }
}
