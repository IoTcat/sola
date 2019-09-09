
#define LORA_SOCKET_IP "1.0.0.1"
#include "lora-mqtt.h"

LoRaMQTT mqtt;

void(* resetFunc) (void) = 0;

void setup() {

    Serial.begin(115200);

    if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

    mqtt.ini();
    mqtt.subscribe("#");
    mqtt.onReceived(mqttRes);

    Serial.println("started..");

}

void loop() {

    if(millis() > 180000) resetFunc();
    mqtt.core();
}

void mqttRes(String subject, String content){
  Serial.println("res");
  Serial.println(subject);
  Serial.println(content);

    mqtt.publish("res/json", "lalala");

    delay(1000);
    resetFunc();


}



