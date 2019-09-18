#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define WIFI_SSID "***********" //Nome da rede
#define WIFI_PASSWORD "*************" //Senha da rede

#define FIREBASE_AUTH "Xbe3qyqVaQcW4rdfMVZKld0hrB9od58m9NiLs9MS" //Token
#define FIREBASE_HOST "projetopie.firebaseio.com"

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
  
