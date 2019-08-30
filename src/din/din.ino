/*
 * @Author: IoTcat (https://iotcat.me) 
 * @Date: 2019-08-20 09:57:58 
 * @Last Modified by: 
 * @Last Modified time: 2019-08-20 11:11:31
 */

#include <PubSubClient.h>
#include <ESP8266WiFi.h>

#include "led.h"
#include "buz.h"
#include "swi.h"
#include "pir.h"

const char* ssid = "yimian-iot";
const char* password = "1234567890.";
const char* mqtt_server = "192.168.3.4";  // change this to the mqtt server

const char* topicInCtl = "hass/ctl/din/#";  // change this to the outgoing messages
const char* topicInSnsr = "hass/snsr/livb/#";
const char* topicInSnsr1 = "hass/snsr/hall/#";

const String clientId = "din";


WiFiClient espClient;
PubSubClient client(espClient);

Buz buz(D8);
Swi swi(D1, D2);
Pir p0(D6), p1(D13), p2(D11), p3(D12);
LED led(D0);

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    client.setServer(mqtt_server, 1883);  // default port, change it...
    client.setCallback(callback);

    swi.on("on", swiOn);
    swi.on("off", swiOff);

    buz.ini();
    swi.ini();
    p0.ini();
    p1.ini();
    p2.ini();
    p3.ini();
    led.ini();

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

    if(topic == "")

    if(s=="1"){
      buz.once();
    }else{
      buz.off();
    }
    client.publish("hass/snsr/liv/light", s.c_str(), 1);
    client.publish("hass/snsr/livd/test", String(analogRead(A0)).c_str());

    Serial.println(String(analogRead(A0)).c_str());
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
            client.subscribe(topicInSnsr);
            client.subscribe(topicInSnsr1);
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 3 seconds before retrying
            delay(3000);
        }
    }
}

void swiOn(){
    Serial.println("onononon");  
}

void swiOff(){
  
  Serial.println("offoffoff");  
}

void loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();

    buz.loop();
    swi.loop();
    p.loop();

    if(p.isPeopleIn()){
      Serial.println("ppppppin");
    }

    if(p.isPeopleOut()){
      Serial.println("pppppppout");
      }
}

