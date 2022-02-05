#include <WiFi.h>
#include <ESPAsyncWebServer.h>

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
Adafruit_BMP280 bme; // I2C

const byte led = 27;

AsyncWebServer server(80);

const char* ssid = "Wificarlos";
const char* password = "1234qwer123";

int contconexion = 0;

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Error 404: Page not found");
}

void setup(void) {
  delay(1000);
  Serial.begin(115200);

  ledcAttachPin(led, 0);
  ledcSetup(0, 5000, 8);
  ledcWrite(0, 128);

  if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1);
  }
  //--------- Acces Point ---------
  //WiFi.softAP(ssid, password);

  //IPAddress miIP = WiFi.softAPIP();
  //Serial.print("AP IP address: ");
  //Serial.println(miIP); //probar 192.168.4.1
  //--------- Fin Acces Point ---------

  //--------- Web server ---------
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED and contconexion < 50) {
    ++contconexion;
    delay(500);
    Serial.print(".");
  }
  if (contconexion < 50) {
    Serial.println("");
    Serial.println("WiFi conectado");
    Serial.println(WiFi.localIP()); //probar 192.168.4.1
  }
  else {
    Serial.println("");
    Serial.println("Error de conexion");
  }
  //------ fin del WS ------------


  //------------Envío de datos al cliente----------------
  server.on("/readSensors", HTTP_GET, [](AsyncWebServerRequest * request) {

    int h = hallRead();
    //int to0 = touchRead(T0);
    //int to3 = touchRead(T3);

    // GPIO22 -> SCL, GPIO21 -> SDA, 3.3v -> VCC; GND -> GND

    float temp = bme.readTemperature();
    float pres = (bme.readPressure()) / 100;
    Serial.print("Temperature = ");
    Serial.print(temp);
    Serial.println(" *C");

    Serial.print("Pressure = ");
    Serial.print(pres);
    Serial.println(" hPa");

    String potes_Json = "{\"h\":\"" + String(h) + "\", \"t\":\"" + String(temp) + "\", \"p\":\"" + String(pres) + "\"}";

    request->send(200, "application/json", potes_Json);

  });
  //------------Resepcion de datos del cliente----------------
  server.on("/led", HTTP_GET, [] (AsyncWebServerRequest * request) {

    String valR = request->arg("key");

    ledcWrite(0, valR.toInt());

    Serial.println("led:" + valR);

    String potes_Json = "{\"led\":\"" + String(valR) + "\"}";
    request->send(200, "application/json", potes_Json);
  });
  //-------------fin----------------------------------
  server.onNotFound(notFound); //pagina que no anda
  server.begin();//se inicia el server
}

void loop(void) {
}
