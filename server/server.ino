#include <WiFi.h>        //red wifi
#include <WebServer.h>   // web comunicacion
#include <ESPmDNS.h>     // no escribir ip 
#include <Preferences.h> //guardad coniguracion
#include "config.h"      // credenciales wifi
#include "union.h"      // partes de la pagina
#include <Arduino.h>  // tareas paraleo

#include <Wire.h>             //leer corriente
#include <Adafruit_INA219.h>  //leer corriente

#include <SPI.h>              //usar puerto SPI para SD
#include <SD.h>               //usar SD

#include <DHT.h>              //sensor de temperatura

#define CS_PIN 5           
#define DHT_PIN 13 // s
#define ANG 36
#define MXA 27
#define MXB 14
#define MXC 12
#define LED 2
#define B1 32
#define B2 33
#define B3 15
#define B4 4
#define B5 16
#define B6 17
#define OVP 25
#define UVP 26

const int pin[] = { MXA, MXB, MXC, LED, B1, B2, B3, B4, B5, B6, OVP, UVP };
Adafruit_INA219 ina219(0x41);
DHT dht(DHT_PIN, DHT11);

WebServer server(80);
Preferences p;

TaskHandle_t Task1;
TaskHandle_t Task2;
SemaphoreHandle_t mutex;

const int nConf = 9;    // numero de lectoruas configuracion en web
const int nAng = 5;     // numero de entradas analogas
const int nSal = 10;     // numero de salidas
const int nBot = 4;     // numero de botones web
const int nBat = 6;     // numero de baterias
const int nLec = 8;     // numero de lecturas
const int pwmf = 1000;

volatile float lec[nLec];
volatile float conf[nConf];       // lectura configuracion web [ovp, ovpr, uvp, uvpr, maxCC, ...]
volatile bool boton[nBot];       // lectura configuracion web [carga, descara, balance, emergncia]
float g[nLec + 1] = {1,1,1,1,1,1,1,1,1};       // Ganancia de cada entrada analogica
//const float g[nLec]= {567.3,221.9,219.8,126.1,109.2,75.58,1,1}; // ganancia ajustada
float bat[nBat];
int  cc = 255;
int dd = 255;


void handleNotFound() {
    digitalWrite(LED,1);
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += (server.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";

    for (uint8_t i = 0; i < server.args(); i++) {
        message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }

    server.send(404, "text/plain", message);
    digitalWrite(LED,0);
}
//--------------Fin raices pagina web ------//
//-------------- Inicio funciones web -------//
void configuracion(){
    digitalWrite(LED,1);
    p.begin("my-app", false);
    String cade = server.arg("plain");
    String par[nConf];
    int npar = 0;
    while (cade.length() > 0){
        int posComa = cade.indexOf(",");
        if(posComa != -1){
        par[npar] = cade.substring(0, posComa);
        cade = cade.substring(posComa + 1);
        }else{
        par[npar] = cade;
        cade = "";
        }
        npar++;
    }
    for( int i = 0; i < nConf; i++ ){
        conf[i] = par[i].toFloat();
        String key = "conf" + String(i);
        const char* keyc = key.c_str();
        p.putFloat(keyc, conf[i]);
    }
    p.end();
    digitalWrite(LED,0);
    server.send(200,"text/plain","todo correcto");
}
void ganancia(){
    digitalWrite(LED,1);
    p.begin("my-app", false);
    String cade = server.arg("plain");
    String par[nLec + 1];
    int npar = 0;
    while (cade.length() > 0){
        int posComa = cade.indexOf(",");
        if(posComa != -1){
        par[npar] = cade.substring(0, posComa);
        cade = cade.substring(posComa + 1);
        }else{
        par[npar] = cade;
        cade = "";
        }
        npar++;
    }
    float gt[nLec + 1];
    for( int i = 0; i <= nLec; i++ ){
        gt[i] = par[i].toFloat();
        Serial.print(gt[i]);
        g[i] = gt[i];
        String key = "gain" + String(i);
        const char* keyc = key.c_str();
        p.putFloat(keyc, g[i]);
    }
    p.end();
    digitalWrite(LED,0);
    server.send(200,"text/plain","todo correcto");
}
void botones(){
    digitalWrite(LED,1);
    p.begin("my-app", false);
    String cade = server.arg("plain");
    String par[nBot];
    int npar = 0;
    while (cade.length() > 0){
        int posComa = cade.indexOf(",");
        if(posComa != -1){
        par[npar] = cade.substring(0, posComa);
        cade = cade.substring(posComa + 1);
        }else{
        par[npar] = cade;
        cade = "";
        }
        npar++;
    }
    for( int i = 0; i < nBot; i++ ){
        boton[i] = (par[i] == "true");
        String key = "boton" + String(i);
        const char* keyc = key.c_str();
        p.putBool(keyc, boton[i]);
    }
    p.end();
    server.send(200,"text/plain","todo correcto");
    digitalWrite(LED,0);  
}
void handleInformacion(){
    digitalWrite(LED,1);
    String data = "";
    if (g[8] < 1 ){
        for (int i = 0; i < nBat; i++){
            data += String(bat[i],2)+",";
        }
    }else{
        for (int i = 0; i < nBat; i++){
            data += String(lec[i],2)+",";
        }
    }
    data += String((lec[5]),1)+",";
    data += String(lec[6],2)+","; //corriente
    data += String(lec[7],0) ; //temperatura 
    server.send(200, "text/plain",data); 
    digitalWrite(LED,0);
}
void database(){
    p.begin("my-app", false);
    for (int i = 0; i < nConf; i++) {
        String key = "conf" + String(i);
        const char* keyc = key.c_str();
        conf[i] = p.getFloat(keyc, 0.0);
    }
    for (int i = 0; i < nBot; i++) {
        String key = "boton" + String(i);
        const char* keyc = key.c_str();
        boton[i] = p.getBool(keyc, false);
    }
    p.end();
    String data = "";
    for(int i=0; i<nBot ; i++){
        data += String(boton[i])+",";
    }
    for (int i = 0; i<nConf; i++ ){
        data += String(conf[i]) + ",";
    }
    server.send(200, "text/plain",data);
}

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
    static const uint8_t canales[] = { 4, 6, 7, 5, 0, 3 };
    for (uint8_t i = 0; i < 6; i++) {
        canal(canales[i]);
        lec[i] = analogRead(ANG);
    }
    lec[6] = ina219.getCurrent_mA();

    float temp = dht.readTemperature();
    if (!isnan(temp)) {
        lec[7] = temp;
    }
    for (int i = 0; i < 8; i++) {
        lec[i] = lec[i] / g[i];
    }
    bat[0] = lec[0];
    for (int i = 1; i < nBat; i++){
        bat[i] = lec[i] - lec[i-1];  
    }
}

void apagar (){
    digitalWrite(B1, LOW);
    digitalWrite(B2, LOW);
    digitalWrite(B3, LOW);
    digitalWrite(B4, LOW);
    digitalWrite(B5, LOW);
    digitalWrite(B6, LOW);
}

void control() {

    if (isTemperatureInvalid() || isEmergencyActive()) {
        shutDownSystem();
        return;
    }

    bool batteryStates[5] = {true, true, true, true, true};
    bool balanceStates[nBat] = {false};
    updateBatteryStates(batteryStates, balanceStates);

    handleCharging(batteryStates);
    handleDischarging(batteryStates);
    handleBalancing(balanceStates,batteryStates);
}

bool isTemperatureInvalid() {
    return (lec[7] < conf[7]) || (lec[7] > conf[8]);
}

bool isEmergencyActive() {
    return boton[3];
}

void shutDownSystem() {
    apagar();
    analogWrite(OVP, 0);
    analogWrite(UVP, isEmergencyActive() ? 255 : 0);
}

void updateBatteryStates(bool batteryStates[], bool balanceStates[]) {
    for (int i = 0; i < nBat; i++) {
        batteryStates[0] &= (bat[i] < conf[0]);
        batteryStates[1] &= (bat[i] < conf[1]);
        batteryStates[2] &= (bat[i] > conf[2]);
        batteryStates[3] &= (bat[i] > conf[3]);
        batteryStates[4] &= (bat[i] > conf[4]);
        balanceStates[i] = (bat[i] > conf[4]);
    }
}

void handleCharging(bool batteryStates[]) {
    if (batteryStates[0] && boton[0] && batteryStates[1]) {
        if (lec[6] > conf[5] && cc > 0) {
            while (lec[6] > conf[5] && cc > 0) {
                cc = max(0, cc - 5);
                lec[6] = analogRead(ANG);
                analogWrite(OVP, cc);
            }
        } else {
            cc = min(255, cc + 5);
            analogWrite(OVP, cc);
        }
    } else {
        analogWrite(OVP, 0);
    }
}

void handleDischarging(bool batteryStates[]) {
    if (batteryStates[2] && boton[1] && batteryStates[3]) {
        if (lec[6] < -conf[6] && dd > 0) {
            while (lec[6] < -conf[6] && dd > 0) {
                digitalWrite(UVP, 1);
                dd = max(0, dd - 5);
                lec[6] = analogRead(ANG);
                analogWrite(UVP, dd);
            }
        } else {
            dd = min(255, dd + 5);
            analogWrite(UVP, dd);
        }
    } else {
        analogWrite(UVP, 0);
    }
}

void handleBalancing(bool balanceStates[], bool batteryStates[]) {
    if (boton[2] && !batteryStates[4]) {
        digitalWrite(B1, balanceStates[0] ? HIGH : LOW);
        digitalWrite(B2, balanceStates[1] ? HIGH : LOW);
        digitalWrite(B3, balanceStates[2] ? HIGH : LOW);
        digitalWrite(B4, balanceStates[3] ? HIGH : LOW);
        digitalWrite(B5, balanceStates[4] ? HIGH : LOW);
        digitalWrite(B6, balanceStates[5] ? HIGH : LOW);
        
    } else {
        apagar();
    }
}

void setupServer() {
    server.on("/", [](){
        server.send(200, "text/html", paginaHTML);
    });
    server.on("/style.css", [](){
        server.send(200, "text/css", paginaCSS);
    });
    server.on("/config", HTTP_POST, []() {
        configuracion();
    });
    server.on("/gain", HTTP_POST, []() {
        ganancia();
    });
    server.on("/boton", HTTP_POST, []() {
        botones();
    });
    server.on("/informacion", HTTP_GET, []() {
        handleInformacion();
    });
    server.on("/data", HTTP_GET, []() {
        database();
    });
    server.onNotFound(handleNotFound);
    server.begin();
    Serial.println("HTTP server started");
}

void Task1code(void * pvParameters) {
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = pdMS_TO_TICKS(1000); // 1 segundo de ciclo total

    xLastWakeTime = xTaskGetTickCount();

    for(;;) {
        // Ejecutar leer()
        if(xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
            leer();
            xSemaphoreGive(mutex);
        }
        // Esperar 500ms
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(500));
        // Ejecutar control()
        if(xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
            control();
            xSemaphoreGive(mutex);
        }
        // Esperar hasta completar 1 segundo desde el inicio del ciclo
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

void Task2code(void * pvParameters) {
    setupServer(); // Función para configurar el servidor

    for(;;) {
        if(xSemaphoreTake(mutex, pdMS_TO_TICKS(10)) == pdTRUE) {
            server.handleClient();
            xSemaphoreGive(mutex);
        }
        vTaskDelay(pdMS_TO_TICKS(1)); // Pequeña pausa para no saturar el CPU
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

void variables(){
    p.begin("my-app", false);
    for (int i = 0; i < nConf; i++) {
        String key = "conf" + String(i);
        const char* keyc = key.c_str();
        conf[i] = p.getFloat(keyc, 0.0);
        Serial.print(conf[i]);
    }
    Serial.println();
    for (int i = 0; i <= nLec; i++) {
        String key = "gain" + String(i);
        const char* keyc = key.c_str();
        g[i] = p.getFloat(keyc, 0.0);
        Serial.print(g[i]);
    }
    Serial.println();
    for (int i = 0; i < nBot; i++) {
        String key = "boton" + String(i);
        const char* keyc = key.c_str();
        boton[i] = p.getBool(keyc, false);
    }
    p.end();
}

void initpin() {
  for (int i = 0; i < sizeof(pin) / sizeof(pin[0]); i++) {
    pinMode(pin[i], OUTPUT);
    digitalWrite(pin[i], 0);
  }
  pinMode(ANG, INPUT);
}

void escribirsd(){
  File dataFile = SD.open("datos.txt", FILE_WRITE);
  if (dataFile) {
    for (int i = 0; i < 8; i++) {
      if (i < 6){
        dataFile.print(bat[i]);
        dataFile.print(",");
      }else{
        dataFile.print(lec[i]);
        dataFile.print(",");
      }  
    }
    dataFile.println(bat[0]);
    dataFile.close();
    Serial.println("Data saved");
  } else {
    Serial.println("Error opening data file");
  }
}

void setup(void) {
    Serial.begin(115200);
    while (!Serial) {
        delay(1);
    }
    Serial.println("Hello!");
    variables();
    wifi1();
    dns1();
    delay(1000);
    //initina();
    delay(2000);
    //initsd();
    initpin();

    mutex = xSemaphoreCreateMutex();
    // Crea las tareas en diferentes núcleos
    xTaskCreatePinnedToCore(
        Task1code,    // Función de la tarea
        "Task1",      // Nombre de la tarea
        10000,        // Tamaño del stack
        NULL,         // Parámetro de entrada
        1,            // Prioridad de la tarea
        &Task1,       // Manejador de la tarea
        0);           // Núcleo al que se asigna la tarea

    xTaskCreatePinnedToCore(
        Task2code,    // Función de la tarea
        "Task2",      // Nombre de la tarea
        10000,        // Tamaño del stack
        NULL,         // Parámetro de entrada
        1,            // Prioridad de la tarea
        &Task2,       // Manejador de la tarea
        1);           // Núcleo al que se asigna la tarea

}


void loop(void) {}
