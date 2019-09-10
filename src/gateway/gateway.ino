#include <SPI.h>
#include <LoRa.h>
#include <WiFi.h>

#define LORA_SOCKET_IP "1.0.0.1"

#include "lora-mqtt.h"


#define MQTT_MAX_TRANSFER_SIZE 99999

#include <PubSubClient.h>
#include <U8x8lib.h>
#include "ovo.h"


// WIFI_LoRa_32 ports
// GPIO5  -- SX1278's SCK
// GPIO19 -- SX1278's MISO
// GPIO27 -- SX1278's MOSI
// GPIO18 -- SX1278's CS
// GPIO14 -- SX1278's RESET
// GPIO26 -- SX1278's IRQ(Interrupt Request)

#define SS      18
#define RST     14
#define DI0     26
#define Band 433E6

//#define syncWord 0xF3

//============================
//CHANGE THIS FOR EACH ARDUINO
String nodeId ="goodGateway";
//============================

const char* ssid = "yimian-iot";
const char* password =  "1234567890.";
const char* mqtt_server = "192.168.3.4";//change this to the mqtt server

char* topicIn="#";//change this to the outgoing messages
String cache_mqtt_publish = "";
String cache_mqtt_subject = "";
bool isStarted = false;
unsigned int g_cnt = 0;


WiFiClient espClient;
PubSubClient client(espClient);

LoRaMQTT mqtt;

char outMsg[200];
long lastMsg = 0;

U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);

void oled_print(const String& s){
    u8x8.drawString(0, 4, "                ");
    u8x8.drawString((16 - s.length()) / 2, 4, s.c_str());
  }

void router_print(const String& s){
    u8x8.drawString(0, 7, "                ");
    u8x8.drawString(0, 6, "                ");
    if(s.length() <= 16){
       u8x8.drawString((16 - s.length()) / 2, 6, s.c_str());
       return;
    }
    u8x8.drawString(0, 6, s.substring(0, 16).c_str());
    u8x8.drawString(0, 7, s.substring(16, s.length()).c_str());
  }

void mode_print(const String& s){
    u8x8.drawString(0, 1, "                ");
    u8x8.drawString((16 - s.length()) / 2, 1, s.c_str());
  }

  void ini_print(){
    u8x8.drawString(0, 3, "---");
    u8x8.drawString(13, 3, "---");
    u8x8.drawString(0, 5, "---");
    u8x8.drawString(13, 5, "---");
  }

void setup() {
  Serial.begin(115200);
  
  u8x8.begin();
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  
  LoRa.setPins(SS, RST, DI0);

  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("."); 
    oled_print("No WiFi..");
  }
 

  Serial.println("");
  Serial.println("WiFi connected");
  oled_print("WiFi ok!");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  client.setServer(mqtt_server, 1883);//default port, change it...
  client.setCallback(callback);

Serial.println("LoRa Receiver");

  if (!LoRa.begin(Band)) {
    Serial.println("Starting LoRa failed!");
    oled_print("No LoRa..");
  }

  mqtt.ini();
  mqtt.subscribe("#");
  mqtt.onReceived(mqttRes);
  
  Serial.println("Lora Started...\n");
  oled_print("LoRa ok..");
  Serial.print("Connecting to ");
  Serial.println(ssid);
}



void callback(char* topic, byte* payload, unsigned int length) {

  g_cnt ++;
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String s = "";
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
    s += (char)payload[i];
  }
  Serial.println("");

  router_print(String(topic) + " " + s);

  if(String(topic) == "hass/ctl/wc0/valve") mqtt.publish(String(topic), s);
  if(String(topic) == "hass/refresh") mqtt.publish(String(topic), s);

  if(String(topic) == "hass/snsr/hall/light"){
      if(s == "1") oled_print("hall light on");
      if(s == "0") oled_print("hall light off");
   }
  if(String(topic) == "hass/snsr/din/light"){
      if(s == "1") oled_print("din light on");
      if(s == "0") oled_print("din light off");
   }
   if(String(topic) == "hass/snsr/liv/light"){
      if(s == "1") oled_print("liv light on");
      if(s == "0") oled_print("liv light off");
   }
   if(String(topic) == "hass/snsr/kit/light"){
      if(s == "1") oled_print("kit light on");
      if(s == "0") oled_print("kit light off");
   }

   if(String(topic) == "hass/ctl/kit/mode/isNight"){
      if(s == "0") mode_print("Day Mode");
      if(s == "1") mode_print("Night Mode");
   }

  if(String(topic) == "hass/snsr/hall/doorTelSwi"){
      if(s == "0") oled_print("Door Tel Off");
      if(s == "1") oled_print("Door Tel On");
   }


  if(String(topic) == "hass/snsr/hall/doorSwi"){
      if(s == "1") oled_print("Unit Door On");
   }
   
   if(String(topic) == "hass/ctl/kit/mode/isMidnight"){
      if(s == "1") mode_print("MidNight Mode");
   }

     if(String(topic).substring(0, 4) == "peo/"){
      oled_print(String(topic).substring(4, String(topic).length()) + " - num " + s);
   }
   if(String(topic) == "peo/hall"){
      oled_print("hall - num " + s);
   }
   if(String(topic) == "peo/din"){
      oled_print("din - num " + s);
   }
   if(String(topic) == "peo/liv"){
      oled_print("liv - num " + s);
   }
   if(String(topic) == "peo/kit"){
      oled_print("kit - num " + s);
   }
   if(g_cnt > 1000) ESP.restart();
  //send to LoRa nodes..
  //sendToNode();

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
        
    String clientId = nodeId;    
    
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      oled_print("MQTT ok..");
      ini_print();
      isStarted = true;
      // Once connected, publish an announcement...
      //client.publish(topicOut, "Hello from the Gateway!");
      // ... and resubscribe
      client.subscribe(topicIn);
    } else {
      oled_print("No MQTT..");
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 3 seconds before retrying
      if(isStarted) ESP.restart();
      delay(3000);
    }
  }
}



void f(){
  
  if(cache_mqtt_publish != "")  {

    client.publish(cache_mqtt_subject.c_str(), cache_mqtt_publish.c_str(), cache_mqtt_publish.length());
    cache_mqtt_publish = "";
    cache_mqtt_subject = "";
  }
}

void mqttRes(String subject, String content){
  Serial.println("LoRaMQTT::"+subject+"::"+content);
  cache_mqtt_subject = subject;
  cache_mqtt_publish = content;

}


void loop() {

  if (!client.connected()) 
  {
    reconnect();
  }
  client.loop();
  mqtt.core();
  f();
}

