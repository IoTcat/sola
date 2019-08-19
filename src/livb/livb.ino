#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <EEPROM.h>

#include "led.h"
#include "buz.h"

const char* ssid = "yimian-iot";
const char* password = "1234567890.";
const char* mqtt_server = "192.168.3.4";  // change this to the mqtt server

const char* topicIn = "hass/ctl/liv/#";  // change this to the outgoing messages
const String clientId = "liv";


WiFiClient espClient;
PubSubClient client(espClient);

Buz buz(D13);

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

    buz.ini();


    EEPROM.write(1320, 1);
    Serial.println(EEPROM.read(1320));
}

void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    String s = "";
    for (int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
        s += (char)payload[i];
    }
    Serial.println("");
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
            client.subscribe(topicIn);
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 3 seconds before retrying
            delay(3000);
        }
    }
}

void loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();

    buz.loop();
}

