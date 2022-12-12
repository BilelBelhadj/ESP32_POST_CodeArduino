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
#include <Wire.h>
#include <Adafruit_AHTX0.h>  
#include <stdio.h>

//declaration des constantes
const char* ssid = name;
const char* password = pwd;
const char* serverName = "http://192.168.2.113:8000/recieve";

//declaration des variables
char tmpStr [3] = {0};
char httpRequestData [100] = {0};

unsigned long lastTime = 0;
unsigned long timerDelay = 20000;

String requestChain = "";

int n = 0;

Adafruit_AHTX0 aht;
sensors_event_t humidity, temp;


void setup() {

  //demarrer le moniteur serie
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

  if (aht.begin()) {
    Serial.println("Found AHT20");
  } else {
    Serial.println("Didn't find AHT20");
  }
}

void loop() {
  
  //executer le code chaque delay choisi
  if ((millis() - lastTime) > timerDelay) {
    
    if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;
      
      //notifier le serveur sur le demarrage de requete
      http.begin(client, serverName);
      
      //choix de Hearder
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");

      //template de requete pour ajouter la temperature a la fin
      String templateStr = "micro=MKR1010&capteur=TMP36&valeur="; 

      //capture de temperature avec la bonne forme
      aht.getEvent(&humidity, &temp); 
      float temperature = ceil(temp.temperature * 100.0) / 100.0;
      n = sprintf(tmpStr, "%f", temperature);

      //la chaine a envoyer sous la forme "form" pour que ca sera accepter par le serveur django
      requestChain = templateStr + tmpStr;

      //envoyer la requete vers le serveur et enregistrer le code de retour "code pour la bonne reception et stockage de donnees 200"
      int httpResponseCode = http.POST(requestChain);

      //afficher le code
      Serial.print("HT TPResponse code: ");
      Serial.println(httpResponseCode);
      
      //fermer le protocole http
      http.end();
    }else{
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}

