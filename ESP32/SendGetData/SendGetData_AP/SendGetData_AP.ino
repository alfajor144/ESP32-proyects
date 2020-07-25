#include <WiFi.h>
#include <ESPAsyncWebServer.h>
 
#include "index.h" 
 
const byte ledR = 27;  
const byte ledG = 26;  
const byte ledB = 25;  

AsyncWebServer server(80);

const char* ssid = "ESP32-AP";
const char* password = "1234";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Error 404: Page not found");
}

void setup(void){  
  delay(1000);
  Serial.begin(115200);
  
  ledcAttachPin(ledR,0);
  ledcAttachPin(ledG,1);
  ledcAttachPin(ledB,2);
  
  ledcSetup(0,5000,8);
  ledcSetup(1,5000,8);
  ledcSetup(2,5000,8);
  
  ledcWrite(0,128); //R
  ledcWrite(1,128); //G
  ledcWrite(2,128); //B
  
  WiFi.softAP(ssid, password);

  IPAddress miIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(miIP); //probar 192.168.4.1 

  //-------Pagina principal---------------
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", MAIN_page);
  });

  //------------Resepcion de datos del cliente----------------
  server.on("/set", HTTP_GET, [] (AsyncWebServerRequest *request) {
    
    String valR = request->arg("key"); 
    String valG = request->arg("key2");
    String valB = request->arg("key3");
    
    ledcWrite(0,valR.toInt());
    ledcWrite(1,valG.toInt());
    ledcWrite(2,valB.toInt()); 

    Serial.println("R:"+valR+", G: "+valG+", B:"+valB);
  });
  
  //------------Envío de datos al cliente----------------
  server.on("/readSensors", HTTP_GET, [](AsyncWebServerRequest *request){
  /* Sensor | max Value
   * h        -50 a 50 (hall sensor)
   * T0       90
   * T3       84
   * T4       93
   * T5       101
   * T6       102
   * T7       106
   * T8       111
   * T9       113
   */
   int h = hallRead();
   int to0 = touchRead(T0);
   int to3 = touchRead(T3); 
   /*
   int to4 = touchRead(T4); 
   int to5 = touchRead(T5); 
   int to6 = touchRead(T6);
   int to7 = touchRead(T7);
   int to8 = touchRead(T8);
   int to9 = touchRead(T9);
*/

   int to4 = 23; 
   int to5 = 24; 
   int to6 = 34;
   int to7 = 45;
   int to8 = 56;
   int to9= 56;
   
   String potes_Json = "{\"h\":\"" + String(h) + "\", \"to0\":\""+ String(to0)+"\", \"to3\":\""+ String(to3) +"\", \"to4\":\"" + String(to4) +"\", \"to5\":\"" + String(to5)+"\", \"to6\":\""+String(to6)+"\", \"to7\":\""+String(to7)+"\", \"to8\":\""+String(to8)+"\", \"to9\":\""+String(to9)+"\"}";
   
   request->send(200, "application/json", potes_Json); 
                            
  });
  //-------------fin----------------------------------
  server.onNotFound(notFound); //pagina que no anda
  server.begin();//se inicia el server
}

void loop(void){
}
