#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define WIFI_SSID "**********"
#define WIFI_PASSWORD "*************"

#define FIREBASE_AUTH "*************"
#define FIREBASE_HOST "**************"

const int AtivarBuzzer = 14;// Equivale a D5 na wemos
const int botao = 0; //Equivale a D3 na wemos
int estado = 0;

void setup() {
    Serial.begin(9600);

    Serial.begin(9600);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.printf("Conectando");
    while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);

    
    Serial.println();
    Serial.print("Conectado!");
    Serial.println(WiFi.localIP());
  
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    pinMode(AtivarBuzzer, OUTPUT);
    pinMode(botao, INPUT); 
    }
  
}

void loop() {
    bool recebido = false;  
    estado = Firebase.getInt("Buzzer");
    
    if (digitalRead(botao) == 0)
    {
        estado = !estado;
        Firebase.setInt("Buzzer", estado); 
    }
           
    Serial.print(estado); 
  
    int buzzerStatus = Firebase.getInt("Buzzer"); // Buscando dados no servidor
    if(buzzerStatus == 1){
      digitalWrite(AtivarBuzzer, HIGH);
    } else {
      digitalWrite(AtivarBuzzer, LOW);
    }
 }
   
