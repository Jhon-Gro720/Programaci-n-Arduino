#include <DHT.h>
#define DHTPIN 2 //PIN DONDE CONECTAR LA SEÑAL DEL SENSOR
#define DHTTYPE DHT11
#define LDR 2
DHT dht(DHTPIN, DHTTYPE);

float t;
float tf;
float ld;
int ht;
int pinAgua=3;
int pinLuz=12;
int pinVentana1=13;
int datoRecibido;
int ledVentilador=10;
int leerCalor;
float valor_luz;
//float LDR;
int contadorRiego=0;
int contadorVentilacion=0;
int contadorLuz=0;

#include <Servo.h>
Servo servoMotor;

void setup() {
  Serial.begin(9600);
  pinMode(pinVentana1,OUTPUT);
  pinMode(pinLuz,OUTPUT);
  pinMode(LDR,INPUT);
  pinMode(pinAgua,OUTPUT);
  pinMode(ledVentilador,OUTPUT);
  dht.begin();

  servoMotor.attach(9);
  servoMotor.write(180);
}

void loop() {

  float h = dht.readHumidity();
   t = dht.readTemperature();
   tf = dht.readTemperature(true);
  int ht1=analogRead(A3);
  int ht2=analogRead(A4);
  valor_luz = analogRead(LDR); 
  ht=(ht1+ht2)/2;
  if(ht>=1000){
    ht=999;
  }
    
  ld = (5.0 * valor_luz * 100.0)/1024.0;
  Serial.print("H");
  Serial.print(h);
  Serial.print("\t");
  Serial.print("T");
  Serial.print(t);
  Serial.print("\t");
  Serial.print("F");
  Serial.print(tf);
  Serial.print("\t");
  Serial.print("S");
  Serial.print(ht);
  Serial.print("\t");
  Serial.print("L");
  Serial.println(ld);

  if(Serial.available()){
    datoRecibido=Serial.read();
    ventilacion();
    abrirAgua();
    aumentarCalor();
  }  
  delay(100);
}

void ventilacion(){
    if(datoRecibido=='a' &&contadorVentilacion==0){
      digitalWrite(pinVentana1,HIGH);
      digitalWrite(ledVentilador,HIGH);
      contadorVentilacion=1;
    }
    if(datoRecibido=='c' && contadorVentilacion==1){
      digitalWrite(pinVentana1,LOW);
      digitalWrite(ledVentilador,LOW);
      contadorVentilacion=0;
    }
}

void abrirAgua(){
    if(datoRecibido=='d'  && contadorRiego==0){
      digitalWrite(pinAgua,HIGH);
          for (int i = 180; i > 90; i--){
              servoMotor.write(i);
              delay(25);
              
          }
          contadorRiego=1; 
    }
    
    if(datoRecibido=='e' &&contadorRiego==1){
      digitalWrite(pinAgua,LOW);

      
        for (int i = 90; i < 180; i++){
          servoMotor.write(i);
          delay(25);
        }

      contadorRiego=0;
    }
  }

void aumentarCalor(){
    if(datoRecibido=='f' && contadorLuz==0){
      digitalWrite(pinLuz,HIGH);
      contadorLuz=1;
    } if(datoRecibido=='g' && contadorLuz==1){
      digitalWrite(pinLuz,LOW);
      contadorLuz=0;
    }
  }
