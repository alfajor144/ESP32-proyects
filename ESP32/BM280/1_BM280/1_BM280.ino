#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "index.h"

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
Adafruit_BMP280 bme; // I2C

const byte ledR = 27;
const byte ledG = 26;
const byte ledB = 25;

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

  ledcAttachPin(ledR, 0);
  ledcAttachPin(ledG, 1);
  ledcAttachPin(ledB, 2);

  ledcSetup(0, 5000, 8);
  ledcSetup(1, 5000, 8);
  ledcSetup(2, 5000, 8);

  ledcWrite(0, 0); //R
  ledcWrite(1, 0); //G
  ledcWrite(2, 0); //B

  WiFi.softAP(ssid, password);

  IPAddress miIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(miIP); //probar 192.168.4.1

  //-------Pagina principal---------------
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", MAIN_page);
  });

  //------------Resepcion de datos del cliente (INPUTS)---------------
  server.on("/comandos", HTTP_POST, [] (AsyncWebServerRequest * request) {

    String valR = request->arg("comand");
    String valG = request->arg("codigo");
    String valB = request->arg("passw");

    Serial.println("Comand:" + valR + ", Codigo: " + valG + ", PassW:" + valB);
  });

  //------------Resepcion de datos del cliente (SLIDERS)---------------
  server.on("/set", HTTP_GET, [] (AsyncWebServerRequest * request) {

    String valR = request->arg("key");
    String valG = request->arg("key2");
    String valB = request->arg("key3");

    ledcWrite(0, valR.toInt());
    ledcWrite(1, valG.toInt());
    ledcWrite(2, valB.toInt());

    Serial.println("R:" + valR + ", G: " + valG + ", B:" + valB);
  });

  //------------Resepcion de datos del cliente (BOTONES)----------------
  server.on("/pulsoValue", HTTP_GET, [] (AsyncWebServerRequest * request) {

    String valR = request->arg("pulso");

    if (valR == "u") {
      Serial.println("-> Up");
    }
    else if (valR == "l") {
      Serial.println("-> Left");
    }
    else if (valR == "c") {
      Serial.println("-> Center");
    }
    else if (valR == "r") {
      Serial.println("-> Right");
    }
    else if (valR == "d") {
      Serial.println("-> Down");
    }
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
