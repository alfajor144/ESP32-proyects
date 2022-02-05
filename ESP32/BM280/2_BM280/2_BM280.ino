#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "index.h"

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
Adafruit_BMP280 bme; // I2C

AsyncWebServer server(80);

const char* ssid = "ESP32-AP";
const char* password = "1234";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Error 404: Page not found");
}

void setup(void) {
  delay(1000);
  Serial.begin(115200);

  if (!bme.begin(0x76)) {  
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1);
  }

  WiFi.softAP(ssid, password);

  IPAddress miIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(miIP); //probar 192.168.4.1

  //-------Pagina principal---------------
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", MAIN_page);
  });

  //------------Envío de datos al cliente----------------
  server.on("/readSensors", HTTP_GET, [](AsyncWebServerRequest * request) {

    int h = hallRead();
    //int to0 = touchRead(T0);
    //int to3 = touchRead(T3);

    // GPIO22 -> SCL, GPIO21 -> SDA, 3.3v -> VCC; GND -> GND

    float temp = bme.readTemperature();
    float pres = (bme.readPressure())/100;
    Serial.print("Temperature = ");
    Serial.print(temp);
    Serial.println(" *C");
    
    Serial.print("Pressure = ");
    Serial.print(pres);
    Serial.println(" hPa");

    String potes_Json = "{\"h\":\"" + String(h) + "\", \"t\":\"" + String(temp) + "\", \"p\":\"" + String(pres) + "\"}";

    request->send(200, "application/json", potes_Json);

  });
  //-------------fin----------------------------------
  server.onNotFound(notFound); //pagina que no anda
  server.begin();//se inicia el server
}

void loop(void) {
}
