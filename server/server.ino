#include <WiFi.h>        //red wifi
#include <WebServer.h>   // web comunicacion
#include <ESPmDNS.h>     // no escribir ip 
#include <Preferences.h> //guardad coniguracion
#include "config.h"      // credenciales wifi
//#include "union.h"      // partes de la pagina
#include <Arduino.h>  // tareas paraleo

#include <Wire.h>             //leer corriente
#include <Adafruit_INA219.h>  //leer corriente

#include <SPI.h>              //usar puerto SPI para SD
#include <SD.h>               //usar SD

#include <DHT.h>              //sensor de temperatura

#define CS_PIN 10           
#define DHT_PIN 18
#define ANG 1
#define MXA 21
#define MXB 47
#define MXC 48
#define LED 17
#define B1 16
#define B2 15
#define B3 2
#define B4 41
#define B5 5
#define B6 4
#define OVP 6
#define UVP 7

const int pin[] = { MXA, MXB, MXC, LED, B1, B2, B3, B4, B5, B6, OVP, UVP };
Adafruit_INA219 ina219(0x41);
DHT dht(DHT_PIN, DHT11);

WebServer server(80);
Preferences p;


const int nlec = 8;
volatile float lec[nlec];


/*         C -  B - A
MB1 IO2    0 - 1 - 0
MB2 IO1    0 - 0 - 1
MB3 IO4    1 - 0 - 0
MB4 IO6    1 - 1 - 0
MB5 IO7    1 - 1 - 1
MB6 IO5    1 - 0 - 1
*/

void canal(uint8_t valor) {
  digitalWrite(MXC, valor & 0x01);
  digitalWrite(MXB, (valor >> 1) & 0x01);
  digitalWrite(MXA, (valor >> 2) & 0x01);
}

void leer() {
  static const uint8_t canales[] = { 2, 1, 4, 6, 7, 5 };
  for (uint8_t i = 0; i < 6; i++) {
    canal(canales[i]);
    lec[i] = analogRead(ANG);
  }

  lec[6] = ina219.getCurrent_mA();

  float temp = dht.readTemperature();
  if (!isnan(temp)) {
    lec[7] = temp;
  }
}




void wifi1(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void dns1(){
  if (!MDNS.begin(name)) {  
  Serial.println("Error setting up MDNS responder!");
  while (1) {
      delay(1000);
  }
  }
}

void initina() {
  while (!ina219.begin()) {
    Serial.println("Failed to find INA219 chip, retrying...");
    delay(1000);
  }
  ina219.setCalibration_32V_2A();

  Serial.println("Measuring voltage and current with INA219 ...");
}

void initsd() {
  while (!SD.begin(CS_PIN)) {
    Serial.println("Failed to find SD, retrying...");
    delay(1000);
  }
  Serial.println("Tarjeta SD inicializada correctamente");
}

void initpin() {
  for (int i = 0; i < sizeof(pin) / sizeof(pin[0]); i++) {
    pinMode(pin[i], OUTPUT);
    digitalWrite(pin[i], 0);
  }
  pinMode(ANG, INPUT);
}

void setup(void) {
  Serial.begin(115200);
  while (!Serial) {
    delay(1);
  }
  Serial.println("Hello!");
  wifi1();
  dns1();
  delay(1000);
  initina();
  delay(2000);
  initsd();
  initpin();

}


void loop(void) {

  delay(1000);

  leer();
  for (int i = 0; i < nlec; i++) {
    Serial.print(lec[i]);
    Serial.print("  ");
  }
  File archivo = SD.open("/datos.txt", FILE_APPEND);
  if (archivo) {
    for (int i = 0; i < nlec; i++) {
      archivo.print(lec[i]);
      archivo.print(",");
    }
    archivo.println();
    archivo.close();
    Serial.println("Se escribió en el archivo");
  } else {
    Serial.println("Error al abrir el archivo para escribir");
  }
  delay(1000);
}
