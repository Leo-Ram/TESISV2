
#include "INA3221.h"          // usar INA
#include <SPI.h>              //usar puerto SPI para SD
#include <SD.h>               //usar SD
#define CS_PIN 10
#include <time.h>        //contar tiempo

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Definir las dimensiones de la pantalla
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

// Definir el pin de reset de la pantalla, si no tiene uno, pon -1  

// Crear una instancia del objeto de la pantalla OLED
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);



unsigned long ti; // tiempo inicial
time_t ta;    // tiempo actual
struct tm *timeinfo;  // timepo para guardar
unsigned long tp;  // tiempo pasado

void tiempoin(){
    struct tm tiempo = {0};
    tiempo.tm_year = 2024 - 1900;  // Años desde 1900
    tiempo.tm_mon = 0;             // 0-11 (0 = Enero)
    tiempo.tm_mday = 1;            // 1-31
    tiempo.tm_hour = 0;
    tiempo.tm_min = 0;
    tiempo.tm_sec = 0;
    tp = mktime(&tiempo);
}


INA3221 INA(0x40);

const int ch = 3;

float lecv[ch] ;
float lecc[ch] ;

void ina3221init(){
  int c = 0;
  while((!INA.begin()) && (c < 10)){
    Serial.println("Failed to find INA3221 chip, retrying...");
    Serial.println(c);
    delay(1000);
    c++;
  }
  INA.setShuntR(0, 0.100);
  INA.setShuntR(1, 0.100);
  INA.setShuntR(2, 0.100);
  Serial.println("INA3221 iniciado");
}
void initsd() {
  int c = 0;
  while ((!SD.begin(CS_PIN)) && (c < 10)) {
    Serial.println("Failed to find SD, retrying...");
    Serial.println(c);
    delay(1000);
    c++;
  }
  Serial.println("Tarjeta SD inicializada correctamente");
}

void leerina(){
  for (int i=0; i<ch; i++){
    lecv[i] = INA.getBusVoltage(i) - 0.2;
    lecc[i] = INA.getCurrent_mA(i);
  } 
}

void escribirsd(){
    ta = tp + ((millis() - ti) / 1000);
    timeinfo = gmtime(&ta); // tiempo para guardar
    File dataFile = SD.open("/datos.txt", FILE_WRITE);
    if (dataFile) {
        dataFile.seek(dataFile.size());
        for (int i=0; i<ch ; i++) {
          dataFile.print(lecv[i]);
          dataFile.print(",");
          dataFile.print(lecc[i]);
          dataFile.print(",");
        }
        dataFile.print(timeinfo);
        dataFile.println(); 
        dataFile.close();
        //Serial.println("Data saved");
    } else {
        Serial.println("Error opening data file");
    }
}

void serialp() {
  Serial.println("\nCH\tV\tI");
  for (int i=0; i<ch; i++){
      Serial.print(i);
      Serial.print("\t");  // Separador de columnas
      Serial.print(lecv[i]);
      Serial.print("\t");  // Separador de columnas
      Serial.print(lecc[i]);
      Serial.println();  // Salto de línea
  }
}

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  ina3221init();
  initsd();
  tiempoin();

  // Iniciar la pantalla OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("No se pudo iniciar la pantalla OLED"));
    while (true); // Detener el programa si falla la inicialización
  }

  // Limpiar el buffer de la pantalla
  display.clearDisplay();
  
  // Mostrar un texto de prueba en la pantalla
  display.setTextSize(1.5);      // Tamaño del texto
  display.setTextColor(SSD1306_WHITE); // Color del texto
  display.setCursor(0, 0);     // Posición del texto
  display.println(F("Hola Mundo!"));
  
  // Actualizar la pantalla con lo que hemos escrito
  display.display();

}

void dispPrint(){
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
      // Mostrar los valores del vector lecv en la primera fila
    for (int i = 0; i < 3; i++) {
      display.setCursor(i * 40, 0); // Colocar el texto (división de 128px en 3 columnas)
      display.print(lecv[i]);
    }

    // Mostrar los valores del vector lecc en la segunda fila
    for (int i = 0; i < 3; i++) {
      display.setCursor(i * 40, 16); // Fila inferior (y = 16 para la segunda fila)
      display.print((int)lecc[i]);
    }

    // Actualizar la pantalla con los valores
    display.display();

}


void loop()
{
  leerina();
  escribirsd();
  delay(500);
//  serialp();  
  dispPrint(); 
}


//  -- END OF FILE --
