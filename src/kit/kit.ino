/*
 * @Author: IoTcat (https://iotcat.me) 
 * @Date: 2019-08-20 09:57:58 
 * @Last Modified by: 
 * @Last Modified time: 2019-08-30 14:44:11
 */

#include <PubSubClient.h>
#include <ESP8266WiFi.h>

#include "led.h"
//#include "buz.h"
#include "swi.h"
#include "pir.h"
#include "relay.h"
#include "mode.h"
#include "lightCtl.h"

const char* ssid = "yimian-iot";
const char* password = "1234567890.";
const char* mqtt_server = "192.168.3.4";  // change this to the mqtt server

const char* topicInCtl = "hass/ctl/kit/#";  // change this to the outgoing messages
const char* topicInRefresh = "hass/refresh";

const String clientId = "kit";
const unsigned short pirNum = 4;

WiFiClient espClient;
PubSubClient client(espClient);

//Buz buz(D3);
Swi swi(D1, D2);
Pir p[pirNum] = {D6, D13, D11, D4};
LED led(D0);
Relay light(D5, true);
Mode mode;
LightCtl lightCtl(&light, &led, &mode);

int pirCnt = 0; //计数区域内激活的pir个数
unsigned long int LastActTime = 0;

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

    swi.on("toggle", swiToggle);


    //buz.ini();
    swi.ini();
    for(auto i : p){
        i.ini();
    }
    led.ini();
    light.ini();
    mode.ini();

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
    mqtt_mode(topic, s);
    mqtt_lightCtl(topic, s);
    mqtt_light(topic, s);
    //mqtt_buz(topic, s);

}

void reconnect() {
    // Loop until we're reconnected
    while (!client.connected()) {
        mode.isOffline(true);
        Serial.print("Attempting MQTT connection...");

        // Attempt to connect
        if (client.connect(clientId.c_str())) {
            Serial.println("connected");
            mode.isOffline(false);
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





void loop() {

    // mqtt core
    if (!client.connected()) {
        reconnect();
    }
    client.loop();

    // component core
    //buz.loop();
    swi.loop();
    led.loop();

    // pir snsr
    for(unsigned short i = 0; i < pirNum; i ++){
        p[i].loop();
        if(p[i].isPeopleIn()){
            client.publish(String("hass/snsr/"+clientId+"/p"+i).c_str(), "1");
            pirCnt ++;
        }
        if(p[i].isPeopleOut()){
            client.publish(String("hass/snsr/"+clientId+"/p"+i).c_str(), "0");
            pirCnt --;
        }
    }

    // Auto & offline Mode
    if(pirCnt < 0 || pirCnt > pirNum) pirCnt = 0;
    if(mode.isAuto() && mode.isOffline()){
        if(pirCnt > 1) {
            lightCtl.on();
            LastActTime = millis();
        }
        else if(pirCnt == 0 && LastActTime > millis()-5000) lightCtl.off();
    }

    // lightCtl trigger
    if(lightCtl.isStateChange()){
        client.publish(String("hass/snsr/"+clientId+"/lightCtl").c_str(), String(lightCtl.getStatus()).c_str());
    }
    // light trigger
    if(light.isStateChange()){
        client.publish(String("hass/snsr/"+clientId+"/light").c_str(), String(light.getStatus()).c_str());
    }
   // buz trigger
   /*if(buz.isStateChange()){
        client.publish(String("hass/snsr/"+clientId+"/buz").c_str(), String(buz.getStatus()).c_str());
   } */
    // led trigger
    if(led.isStateChange()){
        client.publish(String("hass/snsr/"+clientId+"/led").c_str(), led.getMode().c_str());
    }
    // mode trigger
    if(mode.isNightChange()){
        client.publish(String("hass/snsr/"+clientId+"/mode/isNight").c_str(), String(mode.isNight()).c_str());
    } 
    if(mode.isAutoChange()){
        client.publish(String("hass/snsr/"+clientId+"/mode/isAuto").c_str(), String(mode.isAuto()).c_str());
    }
    if(mode.isMidnightChange()){
        client.publish(String("hass/snsr/"+clientId+"/mode/isMidnight").c_str(), String(mode.isMidnight()).c_str());
    }
    if(mode.isOfflineChange()){
        client.publish(String("hass/snsr/"+clientId+"/mode/isOffline").c_str(), String(mode.isOffline()).c_str());
    }

}


/**** mqtt func ****/

void mqtt_refresh(const String& subject, const String& content){

    // send all status
    if(subject == topicInRefresh){
        client.publish(String("hass/snsr/"+clientId+"/lightCtl").c_str(), String(lightCtl.getStatus()).c_str());
        client.publish(String("hass/snsr/"+clientId+"/mode/isNight").c_str(), String(mode.isNight()).c_str());
        client.publish(String("hass/snsr/"+clientId+"/mode/isAuto").c_str(), String(mode.isAuto()).c_str());
        client.publish(String("hass/snsr/"+clientId+"/mode/isMidnight").c_str(), String(mode.isMidnight()).c_str());
        client.publish(String("hass/snsr/"+clientId+"/mode/isOffline").c_str(), String(mode.isOffline()).c_str());
        client.publish(String("hass/snsr/"+clientId+"/light").c_str(), String(light.getStatus()).c_str());
        //client.publish(String("hass/snsr/"+clientId+"/buz").c_str(), String(buz.getStatus()).c_str());
        client.publish(String("hass/snsr/"+clientId+"/swi").c_str(), String(swi.state()).c_str());
        for(unsigned short i = 0; i < pirNum; i ++){
            client.publish(String("hass/snsr/"+clientId+"/p"+i).c_str(), String(p[i].getState()).c_str());
        }
        client.publish(String("hass/snsr/"+clientId+"/led").c_str(), led.getMode().c_str());
    }

}

void mqtt_mode(const String& subject, const String& content){

    if(subject == String("hass/ctl/"+clientId+"/mode/isNight")){
        if(content == "0"){
            mode.isNight(false);
        }
        if(content == "1"){
            mode.isNight(true);
        }
    }
    if(subject == String("hass/ctl/"+clientId+"/mode/isAuto")){
        if(content == "0"){
            mode.isAuto(false);
        }
        if(content == "1"){
            mode.isAuto(true);
        }
    }
    if(subject == String("hass/ctl/"+clientId+"/mode/isMidnight")){
        if(content == "0"){
            mode.isMidnight(false);
        }
        if(content == "1"){
            mode.isMidnight(true);
        }
    }
    if(subject == String("hass/ctl/"+clientId+"/mode/isOffline")){
        if(content == "0"){
            mode.isOffline(false);
        }
        if(content == "1"){
            mode.isOffline(true);
        }
    }

}

void mqtt_lightCtl(const String& subject, const String& content){
    if(mode.isAuto() && !mode.isOffline() && subject == String("hass/ctl/"+clientId+"/lightCtl")){
        if(content == "0"){
            lightCtl.off();
        }
        if(content == "1"){
            lightCtl.on();
        }
    }
}

void mqtt_light(const String& subject, const String& content){
    if(subject == String("hass/ctl/"+clientId+"/light")){
        if(content == "0"){
            light.off();
        }
        if(content == "1"){
            light.on();
        }
    }
}


/*void mqtt_buz(const String& subject, const String& content){
    if(subject == String("hass/ctl/"+clientId+"/buz")){
        if(content == "0"){
            buz.off();
        }
        if(content == "1"){
            buz.on();
        }
    }
}
*/

/**** swi ****/
void swiToggle(){
    client.publish(String("hass/snsr/"+clientId+"/swi").c_str(), String(swi.state()).c_str());
    //lightCtl.toggle();
    light.toggle();
}



/**** lightCtl ****/
