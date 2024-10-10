#include <WiFi.h>  //red wifi
#include <esp_wifi.h>
#include <WebServer.h>    // web comunicacion
#include <ArduinoJson.h>  // para archivos json
#include <ESPmDNS.h>      // no escribir ip
#include <Preferences.h>  //guardad coniguracion
#include "config.h"       // credenciales wifi
#include "pagina.h"       // partes de la pagina
#include <Arduino.h>      // tareas paraleo

#include <Wire.h>    //leer corriente
#include "INA226.h"  //leer corriente

#include <SPI.h>  //usar puerto SPI para SD
#include <SD.h>   //usar SD

#include <DHT.h>  //sensor de temperatura

#include <time.h>  //contar tiempo

#define CS_PIN 5
#define DHT_PIN 13  // s
#define ANG_PIN 36
#define MXA_PIN 27
#define MXB_PIN 14
#define MXC_PIN 12
#define LED_PIN 2
#define B1_PIN 32
#define B2_PIN 33
#define B3_PIN 15
#define B4_PIN 4
#define B5_PIN 16
#define B6_PIN 17
#define OVP_PIN 25
#define UVP_PIN 26

const int pin[] = { MXA_PIN, MXB_PIN, MXC_PIN, LED_PIN, B1_PIN, B2_PIN, B3_PIN, B4_PIN, B5_PIN, B6_PIN, OVP_PIN, UVP_PIN };
INA226 INA(0x40);
DHT dht(DHT_PIN, DHT11);

WebServer server(80);
Preferences pre;

TaskHandle_t Task1;
TaskHandle_t Task2;
SemaphoreHandle_t mutex;

float bat[6] = { 3.7, 3.8, 3.9, 4.0, 4.1, 4.2 };
float lec[9] = { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 23.4, 699, 20 };
float g[10] = {};
float conf[10] = {};
float absolut[6] = {};
bool boton[4] = {};
int cc = 255;
int dd = 255;
bool bancc = false;
bool bandd = false;

unsigned long ti;     // tiempo inicial
time_t ta;            // tiempo actual
struct tm* timeinfo;  // timepo para guardar
unsigned long tp;     // tiempo pasado
void tiempoin() {
  struct tm tiempo = { 0 };
  tiempo.tm_year = 2024 - 1900;  // Años desde 1900
  tiempo.tm_mon = 0;             // 0-11 (0 = Enero)
  tiempo.tm_mday = 1;            // 1-31
  tiempo.tm_hour = 0;
  tiempo.tm_min = 0;
  tiempo.tm_sec = 0;
  tp = mktime(&tiempo);

  ti = millis();
  ta = 0;
}

const char* CONFIG_FILE = "/config.json";

void estado() {
  DynamicJsonDocument doc(1024);
  doc["GS"] = 0;
  
  File configFile = SD.open(CONFIG_FILE, FILE_WRITE);
  if (!configFile) {
    Serial.println("Failed to open config file for writing");
    return;
  }
  
  serializeJson(doc, configFile);
  configFile.close();
}

void actualVariable() {
  File configFile = SD.open(CONFIG_FILE);
  if (!configFile) {
    Serial.println("Failed to open config file for reading");
    return;
  }
  
  DynamicJsonDocument doc(2048);
  DeserializationError error = deserializeJson(doc, configFile);
  configFile.close();
  
  if (error) {
    Serial.println("Failed to parse config file");
    return;
  }
  
  // Valores g[]
  g[0] = doc["GB1"] | 588.0f;
  g[1] = doc["GB2"] | 271.0f;
  g[2] = doc["GB3"] | 199.0f;
  g[3] = doc["GB4"] | 143.5f;
  g[4] = doc["GB5"] | 124.2f;
  g[5] = doc["GB6"] | 101.7f;
  g[6] = doc["GBI"] | 4.8f;
  g[7] = doc["GT"] | 1.0f;
  g[8] = doc["GANG"] | 0.0f;
  g[9] = doc["GS"] | 0.0f;
  
  // Valores conf[]
  conf[0] = doc["OVP"] | 4.2f;
  conf[1] = doc["OVPR"] | 4.0f;
  conf[2] = doc["UVP"] | 3.0f;
  conf[3] = doc["UVPR"] | 3.4f;
  conf[4] = doc["VBal"] | 4.15f;
  conf[5] = doc["CCP"] | 1500.0f;
  conf[6] = doc["DCP"] | 1500.0f;
  conf[7] = doc["TMin"] | 4.0f;
  conf[8] = doc["TMax"] | 60.0f;
  conf[9] = doc["Cap"] | 1800.0f;
  
  // Valores absolut[]
  absolut[0] = doc["VMax"] | 4.2f;
  absolut[1] = doc["VMin"] | 2.6f;
  absolut[2] = doc["IMax"] | 1500.0f;
  absolut[3] = doc["ACap"] | 1800.0f;
  absolut[4] = doc["MBal"] | 1.0f;
  absolut[5] = doc["MRec"] | 2.0f;
  
  // Valores boton[]
  boton[0] = doc["carga"] | false;
  boton[1] = doc["descarga"] | false;
  boton[2] = doc["balance"] | false;
  boton[3] = doc["emergencia"] | false;
}

void handleGetConfig() {
  File configFile = SD.open(CONFIG_FILE);
  if (!configFile) {
    server.send(500, "text/plain", "Failed to open config file");
    return;
  }
  
  String json = configFile.readString();
  configFile.close();
  
  server.send(200, "application/json", json);
}
//----------------------------------------


// ... (mantener las otras funciones como están) ...

void configuracion() {
  digitalWrite(LED_PIN, HIGH);
  String newJson = server.arg("plain");
  Serial.println("Nuevos datos recibidos: " + newJson);

  // Documento JSON para los nuevos datos
  DynamicJsonDocument newDoc(1024);
  DeserializationError error = deserializeJson(newDoc, newJson);
  
  if (error) {
    Serial.println("Error al parsear JSON nuevo");
    server.send(400, "text/plain", "Error en el formato JSON");
    digitalWrite(LED_PIN, LOW);
    return;
  }

  // Cargar configuración existente
  DynamicJsonDocument existingDoc(2048);
  File configFile = SD.open(CONFIG_FILE);
  if (configFile) {
    DeserializationError existingError = deserializeJson(existingDoc, configFile);
    configFile.close();
    
    if (existingError) {
      Serial.println("Error al parsear JSON existente");
    }
  }

  // Actualizar solo los campos recibidos
  JsonObject newObj = newDoc.as<JsonObject>();
  for (JsonPair p : newObj) {
    const char* key = p.key().c_str();
    JsonVariant value = p.value();

    // Verificar si es un booleano
    if (strcmp(key, "carga") == 0 || 
        strcmp(key, "descarga") == 0 || 
        strcmp(key, "balance") == 0 || 
        strcmp(key, "emergencia") == 0) {
      bool boolValue = value.as<bool>();
      existingDoc[key] = boolValue;
      Serial.print("Actualizando booleano - Key: ");
      Serial.print(key);
      Serial.print(" Valor: ");
      Serial.println(boolValue);
    } else {
      // Es un valor flotante
      float floatValue = value.as<float>();
      existingDoc[key] = floatValue;
      Serial.print("Actualizando flotante - Key: ");
      Serial.print(key);
      Serial.print(" Valor: ");
      Serial.println(floatValue);
    }
  }

  // Guardar la configuración actualizada
  configFile = SD.open(CONFIG_FILE, FILE_WRITE);
  if (!configFile) {
    server.send(500, "text/plain", "Error al abrir archivo para escritura");
    digitalWrite(LED_PIN, LOW);
    return;
  }

  serializeJson(existingDoc, configFile);
  configFile.close();

  server.send(200, "text/plain", "Configuración actualizada correctamente");
  actualVariable();
  digitalWrite(LED_PIN, LOW);
}

// Función auxiliar para imprimir el contenido actual del archivo (útil para depuración)
void printCurrentConfig() {
  File configFile = SD.open(CONFIG_FILE);
  if (configFile) {
    Serial.println("Configuración actual:");
    while (configFile.available()) {
      Serial.write(configFile.read());
    }
    Serial.println();
    configFile.close();
  }
}
//----------------------------------------------------------------

void lecturas() {
  StaticJsonDocument<200> docLec;                         //crear documento json
  JsonArray data = docLec.createNestedArray("lecturas");  //añadir a documento
  for (int i = 0; i < 6; i++) {
    String key = "bat" + String(i + 1);
    docLec[key] = bat[i];
  }
  docLec["Total"] = lec[5];
  docLec["Current"] = lec[6];
  docLec["Temperature"] = lec[7];

  // Serializa el objeto JSON a una cadena
  String json;
  serializeJson(docLec, json);
  server.send(200, "application/json", json);
}

void handleNotFound() {
  digitalWrite(LED_PIN, 1);
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
  digitalWrite(LED_PIN, 0);
}

void canal(uint8_t valor) {
  digitalWrite(MXA_PIN, valor & 0x01);
  digitalWrite(MXB_PIN, (valor >> 1) & 0x01);
  digitalWrite(MXC_PIN, (valor >> 2) & 0x01);
}

void leer() {
  static const uint8_t canales[] = { 4, 6, 7, 5, 0, 3 };
  for (uint8_t i = 0; i < 6; i++) {
    canal(canales[i]);
    vTaskDelay(pdMS_TO_TICKS(100));
    ;
    lec[i] = analogRead(ANG_PIN);
  }
  lec[6] = INA.getCurrent_mA();
  float temp = dht.readTemperature();
  if (!isnan(temp)) {
    lec[7] = temp;
  }
  for (int i = 0; i < 8; i++) {
    lec[i] = lec[i] / g[i];
  }
  bat[0] = lec[0];
  for (int i = 1; i < 6; i++) {
    bat[i] = lec[i] - lec[i - 1];
  }
  escribirsd();
}

//----------------------------------------------------------------control

void apagar() {
  digitalWrite(B1_PIN, LOW);
  digitalWrite(B2_PIN, LOW);
  digitalWrite(B3_PIN, LOW);
  digitalWrite(B4_PIN, LOW);
  digitalWrite(B5_PIN, LOW);
  digitalWrite(B6_PIN, LOW);
}

void control() {
  for (int i = 0; i < 4; i++) {
    Serial.print("bot ");
    Serial.print(boton[i]);
  }
  Serial.println();
  if (isTemperatureInvalid() || isEmergencyActive()) {
    shutDownSystem();
    return;
  } else {
    bool batteryStates[5] = { true, true, true, true, true };
    bool balanceStates[6] = { false, false, false, false, false, false };
    updateBatteryStates(batteryStates, balanceStates);

    handleCharging(batteryStates);
    handleDischarging(batteryStates);
    handleBalancing(balanceStates, batteryStates);
  }
}

bool isTemperatureInvalid() {
  return (lec[7] < conf[7]) || (lec[7] > conf[8]);
}

bool isEmergencyActive() {
  return boton[3];
}

void shutDownSystem() {
  apagar();
  analogWrite(OVP_PIN, 255);
  analogWrite(UVP_PIN, isTemperatureInvalid() ? 255 : 0);
}

void updateBatteryStates(bool batteryStates[], bool balanceStates[]) {
  for (int i = 0; i < 6; i++) {
    batteryStates[0] &= (bat[i] < conf[0]);
    batteryStates[1] &= (bat[i] < conf[1]);
    batteryStates[2] &= (bat[i] > conf[2]);
    batteryStates[3] &= (bat[i] > conf[3]);
    batteryStates[4] &= (bat[i] > conf[4]);
    balanceStates[i] = (bat[i] > conf[4]);
  }
}

void handleCharging(bool batteryStates[]) {
  if (boton[0]) {
    if (batteryStates[0] & batteryStates[1]) {
      cc = 0;
      analogWrite(OVP_PIN, cc);
      //cc = ajuste(cc,conf[5],true);
      bancc = false;
    } else if (batteryStates[0] & !batteryStates[1]) {
      if (!bancc) {
        cc = 0;
        analogWrite(OVP_PIN, cc);
        //cc = ajuste(cc,conf[5],true);
      } else {
        cc = 255;
        analogWrite(OVP_PIN, cc);
      }
    } else {
      cc = 255;
      analogWrite(OVP_PIN, cc);
      bancc = true;
    }
  } else {
    cc = 255;
    analogWrite(OVP_PIN, cc);
  }
}

void handleDischarging(bool batteryStates[]) {
  if (boton[1]) {
    if (batteryStates[2] & batteryStates[3]) {
      dd = 0;
      analogWrite(UVP_PIN, cc);
      //dd = ajuste(dd,conf[6],false);
      bandd = false;
    } else if (batteryStates[2] & !batteryStates[3]) {
      if (!bandd) {
        dd = 0;
        analogWrite(UVP_PIN, dd);
        //dd = ajuste(dd,conf[6],false);
      } else {
        dd = 255;
        analogWrite(UVP_PIN, dd);
      }
    } else {
      dd = 255;
      analogWrite(UVP_PIN, dd);
      bandd = true;
    }
  } else {
    dd = 255;
    analogWrite(UVP_PIN, dd);
  }
}

void handleBalancing(bool balanceStates[], bool batteryStates[]) {
  if (boton[2] && !batteryStates[4] && (lec[6] > 350)) {
    digitalWrite(B1_PIN, balanceStates[0] ? HIGH : LOW);
    digitalWrite(B2_PIN, balanceStates[1] ? HIGH : LOW);
    digitalWrite(B3_PIN, balanceStates[2] ? HIGH : LOW);
    digitalWrite(B4_PIN, balanceStates[3] ? HIGH : LOW);
    digitalWrite(B5_PIN, balanceStates[4] ? HIGH : LOW);
    digitalWrite(B6_PIN, balanceStates[5] ? HIGH : LOW);

  } else {
    apagar();
  }
}

//----------------------------------------------------------------contron ends

void escribirsd() {
  ta = tp + ((millis() - ti) / 1000);
  timeinfo = gmtime(&ta);  // tiempo para guardar
  File dataFile = SD.open("/datos.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.seek(dataFile.size());
    for (int i = 0; i < 8; i++) {
      if (i < 6) {
        dataFile.print(bat[i]);
        dataFile.print(",");
      } else {
        dataFile.print(lec[i]);
        dataFile.print(",");
      }
    }
    dataFile.print(lec[5]);
    dataFile.print(",");
    dataFile.print(timeinfo);
    dataFile.println();
    dataFile.close();
    //Serial.println("Data saved");
  } else {
    Serial.println("Error opening data file");
  }
}

void setupServer() {
  server.on("/", []() {
    server.send(200, "text/html", paginaHTML);
  });
  server.on("/style.css", []() {
    server.send(200, "text/css", paginaCSS);
  });
  server.on("/script.javascript", []() {
    server.send(200, "text/javascript", paginaJS);
  });
  //no base
  server.on("/conf", HTTP_POST, []() {
    configuracion();
  });
  server.on("/lec", HTTP_GET, []() {
    lecturas();
  });
  server.on("/datasaved", HTTP_GET, []() {
    handleGetConfig();
  });
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void Task1code(void* pvParameters) {
  TickType_t xLastWakeTime;
  const TickType_t xFrequency = pdMS_TO_TICKS(1000);  // 1 segundo de ciclo total

  xLastWakeTime = xTaskGetTickCount();

  for (;;) {
    // Ejecutar leer()
    if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
      leer();
      xSemaphoreGive(mutex);
    }
    // Esperar 500ms
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(500));
    // Ejecutar control()
    if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
      control();
      //Serial.println("control");
      xSemaphoreGive(mutex);
    }
    // Esperar hasta completar 1 segundo desde el inicio del ciclo
    vTaskDelayUntil(&xLastWakeTime, xFrequency);
  }
}

void Task2code(void* pvParameters) {
  setupServer();  // Función para configurar el servidor

  for (;;) {
    if (xSemaphoreTake(mutex, pdMS_TO_TICKS(10)) == pdTRUE) {
      server.handleClient();
      xSemaphoreGive(mutex);
    }
    vTaskDelay(pdMS_TO_TICKS(1));  // Pequeña pausa para no saturar el CPU
  }
}

void wifi1() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  int c = 0;
  while ((WiFi.status() != WL_CONNECTED) && (c < 20)) {
    delay(1000);
    Serial.print(".");
    c++;
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  dns1();
}

void dns1() {
  int c = 0;
  while ((!MDNS.begin(name)) && (c < 20)) {
    Serial.println("Error setting up MDNS responder!");
    delay(1000);
    c++;
  }
}

void initina() {
  Wire.begin();
  int c = 0;
  while ((!INA.begin()) && (c < 10)) {
    Serial.println("Failed to find INA219 chip, retrying...");
    delay(1000);
    c++;
  }
  INA.setMaxCurrentShunt(5, 0.01, true);

  Serial.println("Measuring voltage and current with INA219 ...");
}

void initsd() {
  int c = 0;
  while ((!SD.begin(CS_PIN)) && (c < 10)) {
    Serial.println("Failed to find SD, retrying...");
    delay(1000);
    c++;
  }
  Serial.println("Tarjeta SD inicializada correctamente");
}

void initpin() {
  for (int i = 0; i < 10; i++) {
    pinMode(pin[i], OUTPUT);
    digitalWrite(pin[i], 0);
  }
  pinMode(OVP_PIN, OUTPUT);
  pinMode(UVP_PIN, OUTPUT);
  analogWrite(OVP_PIN, 255);
  analogWrite(UVP_PIN, 255);
  pinMode(ANG_PIN, INPUT);
}

void setup(void) {
  Serial.begin(115200);
  while (!Serial) {
    delay(1);
  }
  Serial.println("Hello!");
  initpin();
  estado();  // para que wifi no se apague
  wifi1();
  initina();
  initsd();
  actualVariable();
  tiempoin();

  mutex = xSemaphoreCreateMutex();
  // Crea las tareas en diferentes núcleos
  xTaskCreatePinnedToCore(
    Task1code,  // Función de la tarea
    "Task1",    // Nombre de la tarea
    10000,      // Tamaño del stack
    NULL,       // Parámetro de entrada
    1,          // Prioridad de la tarea
    &Task1,     // Manejador de la tarea
    0);         // Núcleo al que se asigna la tarea

  xTaskCreatePinnedToCore(
    Task2code,  // Función de la tarea
    "Task2",    // Nombre de la tarea
    10000,      // Tamaño del stack
    NULL,       // Parámetro de entrada
    1,          // Prioridad de la tarea
    &Task2,     // Manejador de la tarea
    1);         // Núcleo al que se asigna la tarea
}

void loop(void) {}