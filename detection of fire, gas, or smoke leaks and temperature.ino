//Project Group 10
//Pendeteksi Api, Gas, Suhu
//Hikmah Nurmala dan Tri Lestari
//3SC8

//==============INISIALISASI LIBRARY==========================
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include "DHT.h"
#define DHTTYPE DHT11
#define BLYNK_PRINT Serial
#define dht_pin D3

BlynkTimer timer;
DHT dht(dht_pin, DHTTYPE);

//==================KONEKSI KE BLYNK===========================================

char auth[] = "M0R0e6R-arUjdf8D4PLKSiLai32W_Dbr"; //Auth code sent via Email
char ssid[] = "OPPO F7"; //Wifi name
char pass[] = "13099913";  //Wifi Password

//==================INISIALISASI====================================
const int pinAlarm = 2; // pin D2 buzzer kaki (+)
const int redLED = 4; //pin D4 LED kaki (+)
int flag=0; //value data api
int sensor = A0; //pin A0 mq2
int data; //u/membuat variabel data alarm buzzer dan led

//==================Fungsi Deteksi API, GAS, SUHU==========================================

void notifyOnFire()
{
  //======notifikasi API di Blynk===========
  int isButtonPressed = digitalRead(D1);
  if (isButtonPressed==1 && flag==0) {
    Serial.println("Tari Fire in the Room");
    Blynk.notify("Alert : Tari Fire in the Room");
    flag=1;
  }
  else if (isButtonPressed==0)
  {
    flag=0;
  }

  //======notifikasi GAS/ASAP di Blynk===========
  int sensorVal = analogRead(sensor);
  Serial.println(sensorVal);
  Blynk.virtualWrite(V2, sensorVal);
  if (sensorVal > 600)
  {
    Blynk.notify("Smoke Detected in home");
    Serial.println(sensorVal);
  }
}

//=======================VOID SETUP======================================

void setup()
{
  Serial.begin(9600);
  dht.begin();
  Blynk.begin(auth, ssid, pass);
  pinMode(D1,INPUT_PULLUP); // set D1 sebagai input pin deteksi API
  pinMode(sensor, INPUT); //set A0 sebagai input pin deteksi GAS
  pinMode(D2, OUTPUT); //set D2 sebagai output buzzer
  pinMode(D4, OUTPUT); //set D4 sebagai output LED Red
  
  timer.setInterval(1000L,notifyOnFire);
  //KONEKSI WIFI
  WiFi.begin(ssid, pass);
  {
        delay(500);
        Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi Connected");
}

//=====================VOID LOOP========================================

void loop()
{
  data = digitalRead(flag);

  //kondisi logic menyalakan buzzer dan led
  if (data == LOW)
  {
    digitalWrite(pinAlarm, LOW);
    digitalWrite(redLED, LOW);
    delay(1000);  
  } else
  {
    digitalWrite(redLED, HIGH);
    digitalWrite(pinAlarm, HIGH);
    delay(1000);
  }

  //Membaca suhu
  float s = dht.readTemperature();
  float h =dht.readHumidity();
  Blynk.virtualWrite(V0, s);
  Blynk.virtualWrite(V1, h);

  Blynk.run();
  timer.run();
}
