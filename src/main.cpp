/*
  Titre      : ESP32 POST request django framework
  Auteur     : Bilel Belhadj
  Date       : 06/12/2022
  Description: le ESP32 envoi une requete contien des donnees a une django endpoint
                -tutoriel utilisee : https://randomnerdtutorials.com/esp32-http-get-post-arduino/
  Version    : 0.0.1
*/

#include <WiFi.h>
#include <HTTPClient.h>
#include <WifiLogin.h>

const char* ssid = name;
const char* password = pwd;

const char* serverName = "http://192.168.2.113:8000/recieve";

unsigned long lastTime = 0;
unsigned long timerDelay = 5000;

void setup() {
  Serial.begin(9600);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");

  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("Timer set to 5 seconds.");
}

void loop() {
  
  if ((millis() - lastTime) > timerDelay) {
    
    if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;
    
      
      http.begin(client, serverName);
      
      
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      
      String httpRequestData = "micro=MKR1010&capteur=TMP36&valeur=18";           
      
      int httpResponseCode = http.POST(httpRequestData);

      Serial.print("HT TPResponse code: ");
      Serial.println(httpResponseCode);
      
      http.end();
    }else{
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}

