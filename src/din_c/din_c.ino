/*
 * @Author: IoTcat (https://iotcat.me) 
 * @Date: 2019-08-20 09:57:58 
 * @Last Modified by: 
 * @Last Modified time: 2019-08-30 14:44:11
 */

#include <PubSubClient.h>
#include <ESP8266WiFi.h>



const char* ssid = "yimian-iot";
const char* password = "1234567890.";
const char* mqtt_server = "192.168.3.4";  // change this to the mqtt server

const char* topicInCtl = "hass/ctl/din/#";  // change this to the outgoing messages
const char* topicInRefresh = "hass/refresh";

const String clientId = "din";

/**** Mode Func ****/
bool isNight = false, isAuto = true, isMidnight = false, isOffline = true;
bool fIsNight = false, fIsAuto = true, fIsMidnight = false, fIsOffline = true;

const bool isNightChange(){
    if(isNight != fIsNight){
        fIsNight = isNight;
        return true;
    }
    return false;
}

const bool isAutoChange(){
    if(isAuto != fIsAuto){
        fIsAuto = isAuto;
        return true;
    }
    return false;
}

const bool isMidnightChange(){
    if(isMidnight != fIsMidnight){
        fIsMidnight = isMidnight;
        return true;
    }
    return false;
}

const bool isOfflineChange(){
    if(isOffline != fIsOffline){
        fIsOffline = isOffline;
        return true;
    }
    return false;
}

const unsigned short nightMode(){
    if(!isNight && !isMidnight){
        return 0;
    }
    if(isNight && !isMidnight){
        return 1;
    }
    if(isMidnight){
        return 2;
    }
    return 0;
}



#include "led.h"
#include "relay.h"
#include "lightCtl.h"
#include "swi.h"



WiFiClient espClient;
PubSubClient client(espClient);

LED led(D0);
Relay light(D5);
LightCtl lightCtl(&light, &led);
Swi swi(D1, D2);


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


    led.ini();
    light.ini();
    swi.ini();


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
    mqtt_buz(topic, s);

}

void reconnect() {
    // Loop until we're reconnected
    while (!client.connected()) {
        isOffline = true;
        Serial.print("Attempting MQTT connection...");

        // Attempt to connect
        if (client.connect(clientId.c_str())) {
            isOffline = false;
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





void loop() {

    // mqtt core
    if (!client.connected()) {
        reconnect();
    }
    client.loop();
    led.loop();
    light.loop();
    swi.loop();



    trigger();

}


/**** mqtt func ****/

void mqtt_refresh(const String& subject, const String& content){

    // send all status
    if(subject == topicInRefresh){
         client.publish(String("hass/snsr/"+clientId+"/mode/isNight").c_str(), String(isNight).c_str()); 
         client.publish(String("hass/snsr/"+clientId+"/mode/isAuto").c_str(), String(isAuto).c_str()); 
         client.publish(String("hass/snsr/"+clientId+"/mode/isMidnight").c_str(), String(isMidnight).c_str()); 
         client.publish(String("hass/snsr/"+clientId+"/mode/isOffline").c_str(), String(isOffline).c_str()); 
         client.publish(String("hass/snsr/"+clientId+"/lightCtl").c_str(), String(lightCtl.getStatus()).c_str());
         client.publish(String("hass/snsr/"+clientId+"/light").c_str(), String(light.getStatus()).c_str());
         client.publish(String("hass/snsr/"+clientId+"/led").c_str(), led.getMode().c_str());
    }

}

void mqtt_mode(const String& subject, const String& content){

    if(subject == String("hass/ctl/"+clientId+"/mode/isNight")){
        if(content == "0"){
            isNight = false;
        }
        if(content == "1"){
            isNight = true;
        }
    }
    if(subject == String("hass/ctl/"+clientId+"/mode/isAuto")){
        if(content == "0"){
            isAuto = false;
        }
        if(content == "1"){
            isAuto = true;
        }
    }
    if(subject == String("hass/ctl/"+clientId+"/mode/isMidnight")){
        if(content == "0"){
            isMidnight = false;
        }
        if(content == "1"){
            isMidnight = true;
        }
    }
    if(subject == String("hass/ctl/"+clientId+"/mode/isOffline")){
        if(content == "0"){
            isOffline = false;
        }
        if(content == "1"){
            isOffline = true;
        }
    }
}

void mqtt_lightCtl(const String& subject, const String& content){
    if(isAuto && !isOffline && subject == String("hass/ctl/"+clientId+"/lightCtl")){
        if(content == "0"){
            lightCtl.off();
        }
        if(content == "1"){
            lightCtl.on();
        }
    }    
}

void mqtt_buz(const String& subject, const String& content){
}


/**** swi ****/
void swiToggle(){
}



/**** lightCtl ****/




/**** trigger ****/
void trigger(){
    if(isNightChange()){
       client.publish(String("hass/snsr/"+clientId+"/mode/isNight").c_str(), String(isNight).c_str()); 
    }
    if(isAutoChange()){
       client.publish(String("hass/snsr/"+clientId+"/mode/isAuto").c_str(), String(isAuto).c_str()); 
    }
    if(isMidnightChange()){
       client.publish(String("hass/snsr/"+clientId+"/mode/isMidnight").c_str(), String(isMidnight).c_str()); 
    }
    if(isOfflineChange()){
       client.publish(String("hass/snsr/"+clientId+"/mode/isOffline").c_str(), String(isOffline).c_str()); 
    }

    // lightCtl trigger
    if(lightCtl.isStateChange()){
        client.publish(String("hass/snsr/"+clientId+"/lightCtl").c_str(), String(lightCtl.getStatus()).c_str());
    }
    // light trigger
    if(light.isStateChange()){
        client.publish(String("hass/snsr/"+clientId+"/light").c_str(), String(light.getStatus()).c_str());
    }

    // led trigger
    if(led.isStateChange()){
        client.publish(String("hass/snsr/"+clientId+"/led").c_str(), led.getMode().c_str());
    }
}
