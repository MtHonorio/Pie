# Pie

![](https://github.com/dlrds/Pie/blob/master/Imagens/logo.jpg)
Projeto final para a disciplina de Introdução à Engenharia de Computação ministrada pelo Professor Anand Subramanian do Centro de Informatica da Universidade Federal da Paraıba

## Objetivos

O  projeto visa ativar um dispositivo de som remotamente e conseguir as coordenadas pelo módulo GYNEO6MV2


## Código da Wemos que envia as coordenadas e tem o dispositivo ativo
```c
#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define WIFI_SSID "***********" //Nome da rede
#define WIFI_PASSWORD "*************" //Senha da rede

#define FIREBASE_AUTH "*******************" //Token
#define FIREBASE_HOST "*******************"

int AtivarBuzzer = 0;//Equivale a D3 na wemos

SoftwareSerial serial1(12, 13); // RX, TX  //ATENÇÃO, COLOCAR TX NA 12 E RX NA 13
                                //SOFT. SERIAL PERMITE USAR OUTRAS PORTAS SEM SER A TX E RX 
                                //NA WEMOS: GPIO12 = D6, GPIO13 = D7
TinyGPS gps1;

   void setup() {
   serial1.begin(9600);
   Serial.begin(9600);

   Serial.println("Testando GPS...");   //TESTE DE CONEXÃO

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
  }
    

}

  void loop() {
  bool recebido = false;

  while (serial1.available()) { //AO RECEBER SINAL
     char cIn = serial1.read();
     recebido = gps1.encode(cIn);
  }

    if (recebido) {
     
     //Latitude e Longitude
     long latitude, longitude, l2;
     float latitudeFirebase, longitudeFirebase;
     gps1.get_position(&latitude, &longitude);     //PEGAR LAT. E LONG.

     if (latitude != TinyGPS::GPS_INVALID_F_ANGLE) {
        latitudeFirebase = float(latitude) / 100000, 7;
        Firebase.setFloat("Coordenadas/Latitude", latitudeFirebase);  
     }

     if (longitude != TinyGPS::GPS_INVALID_F_ANGLE) {
        longitudeFirebase = float(longitude) / 100000, 7;
        Firebase.setFloat("Coordenadas/Longitude", longitudeFirebase);    
   
    
     }

     int ledStatus = Firebase.getInt("Buzzer");
     String ledStatus2 = Firebase.getString("Buzzer");

     Serial.print("nop");
     Serial.print(ledStatus2);
     
   if(ledStatus == 1 || ledStatus2 == "1"){
    digitalWrite(AtivarBuzzer, HIGH);
  } else {
    digitalWrite(AtivarBuzzer, LOW);
  }
  
   }
  } 
```
## Código da Wemos que recebe


```c
#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define WIFI_SSID "**********"
#define WIFI_PASSWORD "*************"

#define FIREBASE_AUTH "*************"
#define FIREBASE_HOST "**************"

const int AtivarBuzzer = 14;// Equivale a D5 na wemos
const int botao = 0; //Equivale a D0 na wemos
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
   
```

## Circuitos
![](https://github.com/dlrds/Pie/blob/master/Imagens/Circuito.png)
