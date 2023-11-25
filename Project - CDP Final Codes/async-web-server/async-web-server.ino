// Load Wi-Fi li// Load Wi-Fi library
#include <WiFi.h>
//#include<string>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"

// Replace with your network credentials
const char* ssid     = "ESP32-Access-Point";
//const char* password = "123456789";

// Set web server port number to 80
AsyncWebServer server(80);


String getName() {
  return String("ABCD");
}

String getTemp() {
  return String(random(96.00, 100.00));
}

String getHr() {
  return String(random(65, 75));
}

String getSpo2() {
  return String(random(95, 100));
}

// Set data in placeholder
String processor(const String& var) {
  if (var == "NAME") {
    return getName();
  } else if (var == "TEMPERATURE") {
    return getTemp();
  } else if (var == "HEARTRATE") {
    return getHr();
  } else if (var == "SPO2") {
    return getSpo2();
  } else {
    return String();
  }
}

TaskHandle_t Task1;


void setup() {
  Serial.begin(115200);


    //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
                    Task1code,   /* Task function. */
                    "Task1",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task1,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */                  
  delay(500); 
}


//Task1code: blinks an LED every 1000 ms
void Task1code( void * pvParameters ){
  
  //initialise SPIFFS
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Setting AP (Access Point)…");
  WiFi.mode(WIFI_AP);
  // Remove the password parameter, if you want the AP (Access Point) to be open
  if(!WiFi.softAP(ssid) /*, password);*/) {
    delay(100);
  }

  IPAddress Ip(192, 168, 4, 1);
  IPAddress NMask(255, 255, 255, 0);


  WiFi.softAPConfig(Ip, Ip, NMask);
  if (!WiFi.softAP(ssid, ""))
  {
   Serial.println("WiFi.softAP failed.(Password too short?)");
   return;
  }
  //IPAddress myIP = WiFi.softAPIP();
  Serial.println();
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());
  server.begin();  
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  server.on("/bit-symbol.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/bit-symbol.png", "image/png");
  });
  server.on("/vtu-symbol.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/vtu-symbol.png", "image/png");
  });

  server.on("/name", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", getName().c_str());
  });
  server.on("/temp", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", getTemp().c_str());
  });
  server.on("/hr", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", getHr().c_str());
  });
  server.on("/spo2", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", getSpo2().c_str());
  });

  for(;;){
    delay(1);
  } 
}
void loop(){

  
}
