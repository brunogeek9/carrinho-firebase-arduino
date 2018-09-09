#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

// WIFI
#define WIFI_SSID     "mediotec"  
#define WIFI_PASSWORD "mediotec"

// Configuração firebase
#define FIREBASE_HOST   "carrinhofirebase.firebaseio.com"
#define FIREBASE_AUTH   "mY0uaKFXyfueSLYgERjKSYoeyJccBej5MsNQVgy3"
//pinagem nodemcu
const int d0 = D1;
const int d1 = D2;
const int d2 = D3;
const int d3 = D4;
const int d4 = D5;
const int d5 = D6;

void setup() {
  pinMode(d0,OUTPUT);
  pinMode(d1,OUTPUT);
  pinMode(d2,OUTPUT);
  pinMode(d3,OUTPUT);
  pinMode(d4,OUTPUT);
  pinMode(d5,OUTPUT);
  Serial.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.setString("estado","parado");
  Firebase.setInt("velocidade",300);
}

void parar(){
  analogWrite(d0,0);
  analogWrite(d5,0); 
}

void velocidade(int vel){
  if(vel > 1023){
    vel = 1023;
  }
  analogWrite(d0,vel);
  analogWrite(d5,vel);
}

void tras(int vel){
  velocidade(vel);
  digitalWrite(d1,HIGH);
  digitalWrite(d2,LOW);
  digitalWrite(d3,LOW);
  digitalWrite(d4,HIGH);   
}

void frente(int vel){
  velocidade(vel); 
  digitalWrite(d1,LOW);
  digitalWrite(d2,HIGH);
  digitalWrite(d3,HIGH);
  digitalWrite(d4,LOW);
}

void direita(int vel){
  velocidade(vel); 
  digitalWrite(d1,HIGH);
  digitalWrite(d2,LOW);
  digitalWrite(d3,LOW);
  digitalWrite(d4,HIGH);
}

void esquerda(int vel){
  velocidade(vel); 
  digitalWrite(d1,LOW);
  digitalWrite(d2,HIGH);
  digitalWrite(d3,LOW);
  digitalWrite(d4,HIGH);
  
}

void falha(){
  if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());
      return;
    }
}

void monitoraFirebase(){
      String estado = Firebase.getString("estado");
      int vel = Firebase.getInt("velocidade");
      if(estado == "frente"){    
          frente(vel);
      }else if(estado == "tras"){
          tras(vel);      
      }else if(estado == "direita"){
          direita(vel);      
      }else if(estado == "esquerda"){
          esquerda(vel);      
      }else{
        parar();
      }
}
void loop() {
    monitoraFirebase();
    delay(500);
}
