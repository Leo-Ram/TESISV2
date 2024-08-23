#include <Wire.h>
#include <Adafruit_INA219.h>

#include <SPI.h>
#include <SD.h>

#include <DHT.h>

#define CS_PIN 10  
#define DHT_PIN 18
#define ANG 1


Adafruit_INA219 ina219(0x41);
DHT dht(DHT_PIN, DHT11);

void setup(void) 
{
  Serial.begin(115200);
  while (!Serial) {
      delay(1);
  }
    
  pinMode(ANG_PIN, INPUT);

  Serial.println("Hello!");
  
  if (! ina219.begin()) { 
    Serial.println("Failed to find INA219 chip");
    while (1) { delay(10); }
  }
  ina219.setCalibration_32V_2A();

  Serial.println("Measuring voltage and current with INA219 ...");
  delay(2000);

  if (!SD.begin(CS_PIN)) {
    Serial.println("No se pudo inicializar la tarjeta SD");
    return;
  }
  Serial.println("Tarjeta SD inicializada correctamente");

  // Escribe en un archivo
  File archivo = SD.open("/ejemplo.txt", FILE_WRITE);|
  if (archivo) {
    archivo.println("Hola desde el ESP32-S3!");
    archivo.close();
    Serial.println("Se escribió en el archivo");
  } else {
    Serial.println("Error al abrir el archivo para escribir");
  }

  // Lee del archivo
  archivo = SD.open("/ejemplo.txt");
  if (archivo) {
    Serial.println("Contenido del archivo:");
    while (archivo.available()) {
      Serial.write(archivo.read());
    }
    archivo.close();
  } else {
    Serial.println("Error al abrir el archivo para leer");
  }
}

void loop(void) 
{
  float shuntvoltage = 0;
  float busvoltage = 0;
  float current_mA = 0;
  float loadvoltage = 0;
  float power_mW = 0;

  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  power_mW = ina219.getPower_mW();
  loadvoltage = busvoltage + (shuntvoltage / 1000);
  
  Serial.print("Bus Voltage:   "); Serial.print(busvoltage); Serial.println(" V");
  Serial.print("Shunt Voltage: "); Serial.print(shuntvoltage); Serial.println(" mV");
  Serial.print("Load Voltage:  "); Serial.print(loadvoltage); Serial.println(" V");
  Serial.print("Current:       "); Serial.print(current_mA); Serial.println(" mA");
  Serial.print("Power:         "); Serial.print(power_mW); Serial.println(" mW");
  Serial.println("");

  delay(2000);
  Serial.println(dht.readTemperature());
  Serial.println(" ---- ");
  delay(1000);

  float lec = analogRead(ANG_PIN);
  Serial.print(" analogo = ");
  Serial.println(lec);
}




