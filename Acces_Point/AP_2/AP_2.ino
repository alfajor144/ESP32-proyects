#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>

WebServer server(80);

const char* ssid = "ESP32-Access-Point";
const char* password = "1234";

const int output27 = 27;

void handleRoot() {
  pinMode(output27, OUTPUT);
  digitalWrite(output27, LOW);
  String s;
  s = "<!DOCTYPE html>";
  s += "<html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"></head><body>";
  s += "<h1>ESP32 - AP</h1>";
  s += "<h3>THE LED is OFF</h3>";
  s += "<p><a href=\"/on\"><button style=\"/font-family:century gothic;color:White;font-size:20px;background-color:rgb(200,200,200);\" class=\"button\">LED on</button></a></p>";
  s += "</body></html>";
  
 server.send(200, "text/html", s); //Send web page
}
void page_on(){
  pinMode(output27, OUTPUT);
  digitalWrite(output27, HIGH);
  String s;
  s = "<!DOCTYPE html>";
  s += "<html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"></head><body>";
  s += "<h1>ESP32 - AP</h1>";
  s += "<h3>THE LED is ON</h3>";
  s += "<p><a href=\"/off\"><button style=\"/font-family:century gothic;color:White;font-size:20px;background-color:rgb(10,10,10);\" class=\"button\">LED off</button></a></p>";
  s += "</body></html>";
  
 server.send(200, "text/html", s); //Send web page
}
void page_off(){
  pinMode(output27, OUTPUT);
  digitalWrite(output27, LOW);
  String s; 
  s = "<!DOCTYPE html>";
  s += "<html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"></head><body>";
  s += "<h1>ESP32 - AP</h1>";
  s += "<h3>THE LED is OFF</h3>";
  s += "<p><a href=\"/on\"><button style=\"/font-family:century gothic;color:Black;font-size:20px;background-color:rgb(200,200,200);\" class=\"button\">LED on</button></a></p>";
  s += "</body></html>";
  
 server.send(200, "text/html", s); //Send web page
}
//===============================================================
// Setup
//===============================================================

void setup(void){
  delay(1000);
  Serial.begin(115200);
  WiFi.softAP(ssid, password);

  IPAddress miIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(miIP); //probar 192.168.4.1 
//----------------------------------------------------------------
 
  server.on("/", handleRoot);      //This is display page
  server.on("/on", page_on);
  server.on("/off", page_off);
 
  server.begin();                  //Start server
  Serial.println("HTTP server started");
}

//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
void loop(void){
  server.handleClient();
  delay(1);
}
