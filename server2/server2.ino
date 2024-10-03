#include <WiFi.h>        //red wifi
#include <esp_wifi.h>
#include <WebServer.h>   // web comunicacion
#include <ArduinoJson.h>  // para archivos json
#include <ESPmDNS.h>     // no escribir ip 
#include <Preferences.h> //guardad coniguracion
#include "config.h"      // credenciales wifi
#include "pagina.h"      // partes de la pagina
#include <Arduino.h>  // tareas paraleo

#include <Wire.h>             //leer corriente
#include "INA226.h"  //leer corriente

#include <SPI.h>              //usar puerto SPI para SD
#include <SD.h>               //usar SD

#include <DHT.h>              //sensor de temperatura

#include <time.h>        //contar tiempo

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
INA226 INA(0x40);
DHT dht(DHT_PIN, DHT11);

WebServer server(80);
Preferences p;

TaskHandle_t Task1;
TaskHandle_t Task2;
SemaphoreHandle_t mutex;



void configuracion(){
    digitalWrite(LED,1);
    String json = server.arg("plain");
    Serial.println(json);

    // Crear un documento JSON
    StaticJsonDocument<1024> doc;

    DeserializationError error = deserializeJson(doc, json);
    //Serial.println(doc);
    if (error) {
        Serial.print("Error al parsear JSON: ");
        server.send(400, "text/plain", "Error en el formato JSON");
    } else {
        
        JsonObject obj = doc.as<JsonObject>();
        for (JsonPair p : obj) {
            const char* key = p.key().c_str();
            JsonVariant value = p.value();
            
            Serial.print("Clave: ");
            Serial.print(key);
            Serial.print(" - Valor: ");
            
            // Manejar diferentes tipos de valores
            if (value.is<int>()) {
                Serial.println(value.as<int>());
            } else if (value.is<float>()) {
                Serial.println(value.as<float>());
            } else if (value.is<const char*>()) {
                Serial.println(value.as<const char*>());
            }
        }
        // Procesar todas las variables mapeadas
    }
    server.send(200,"text/plain","todo correcto");
    digitalWrite(LED,0);
}

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


void setupServer() {
    server.on("/", [](){
    server.send(200, "text/html", paginaHTML);
    });
    server.on("/style.css", [](){
        server.send(200, "text/css", paginaCSS);
    });
    server.on("/script.javascript",[](){
        server.send(200, "text/javascript", paginaJS);
    });
    //no base
    server.on("/conf", HTTP_POST, []() {
        configuracion();
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
            //leer();
            //Serial.println("leer");
            xSemaphoreGive(mutex);
        }
        // Esperar 500ms
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(500));
        // Ejecutar control()
        if(xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
            //control();
            //Serial.println("control");
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
    dns1();
}

void dns1(){
    if (!MDNS.begin(name)) {  
    Serial.println("Error setting up MDNS responder!");
    while (1) {
        delay(1000);
    }
    }
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