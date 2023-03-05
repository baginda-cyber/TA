#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"
#define DHTPIN D2
#define DHTTYPE DHT11

// Update these with values suitable for your network.


const int relay = D1; // pin relay1
const int relay2 = D3; // pin relay2

const char* ssid = "Azure";
const char* password = "initugas12";
const char* mqtt_server = "192.168.43.127";   // Ip address mqtt

WiFiClient espClient;
PubSubClient client(espClient);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
   dht.begin();
   
  pinMode(relay, OUTPUT);
  pinMode(relay2, OUTPUT);
}

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("humi", "hello world");
      // ... and resubscribe
      client.subscribe("suhu");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  
  float h = dht.readHumidity(); 
  float t = dht.readTemperature(); 


    Serial.print("Publish message: ");


    char buffer[10];
  dtostrf(t,0, 0, buffer); //function untuk mengubah float kedalam char 
  client.publish("suhu",buffer);
  //Serial.println(buffer);
  dtostrf(h,0, 0, buffer);
  client.publish("humi",buffer);
  
delay(3000);
    Serial.println(h);
if(t <= 28){ //nilai 0 sampai 31 lampu hidup
 digitalWrite(relay, LOW);
 Serial.println("lampu hidup");
 delay(3000); 

    }
    else if (t >= 31){
      digitalWrite(relay, HIGH);      
      Serial.println("lampu mati");
      delay(3000);
      
    }




  else {

  Serial.println(" ");
 }
 
if (h <= 61 ) { // nilai dibawah 61 humidity hidup
  digitalWrite(relay2, LOW); 
  Serial.println("Fan Hidup");
  delay(3000);
    }    
    
  else if(h >= 64) {
   digitalWrite(relay2, HIGH);
   Serial.println("Fan Mati");
   delay(3000);
   }
   else{
    Serial.println(" ");
   }

}
