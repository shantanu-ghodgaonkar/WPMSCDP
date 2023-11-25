#include <WiFi.h>
#include<string>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include <arduino-timer.h>
#include <MamaDuck.h>

#ifdef SERIAL_PORT_USBVIRTUAL
#define Serial SERIAL_PORT_USBVIRTUAL
#endif

// create a built-in mama duck
MamaDuck duck = MamaDuck();

// LORA RF CONFIG
#define LORA_FREQ 868.0 // Frequency Range. Set for US Region 915.0Mhz
#define LORA_TXPOWER 20 // Transmit Power
// LORA HELTEC PIN CONFIG
#define LORA_CS_PIN 5
#define LORA_DIO0_PIN 26
#define LORA_DIO1_PIN 33 // unused
#define LORA_RST_PIN 14


// create a timer with default settings
auto timer = timer_create_default();

// for sending the counter message
const int INTERVAL_MS = 60000;
int counter = 1;


// Replace with your network credentials
const char* ssid     = "ESP32-Access-Point";
//const char* password = "123456789";

// Set web server port number to 80
AsyncWebServer server(80);

String getName();

String getTemp();

String getHr();

String getSpo2();

String processor(const String& var);


//Task Handler Objects
TaskHandle_t Task1;
TaskHandle_t Task2;


void setup() {
  Serial.begin(115200); 

  
  //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
                    Task2code,   /* Task function. */
                    "Task1",     /* name of task. */
                    20000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task1,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */                  
  delay(500); 

  //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
                    Task1code,   /* Task function. */
                    "Task2",     /* name of task. */
                    20000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task2,      /* Task handle to keep track of created task */
                    1);          /* pin task to core 1 */
    delay(500); 
}


//Task1code
void Task1code( void * pvParameters ){

  
  //Async webserver code
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

//Task2code
void Task2code( void * pvParameters ){
 
      //Duck initialisation code
  // The Device ID must be unique and 8 bytes long. Typically the ID is stored
  // in a secure nvram, or provided to the duck during provisioning/registration
  std::string deviceId("MAMA0001");
  std::vector<byte> devId;
  devId.insert(devId.end(), deviceId.begin(), deviceId.end()); 
  //Set the Device ID
  duck.setDeviceId(devId);
  // initialize the serial component with the hardware supported baudrate
  duck.setupSerial(115200);
  // initialize the LoRa radio with specific settings. This will overwrites settings defined in the CDP config file cdpcfg.h
  duck.setupRadio(LORA_FREQ, LORA_CS_PIN, LORA_RST_PIN, LORA_DIO0_PIN, LORA_DIO1_PIN, LORA_TXPOWER);

  // Initialize the timer. The timer thread runs separately from the main loop
  // and will trigger sending a counter message.
  timer.every(INTERVAL_MS, runSensor);
  Serial.println("[MAMA] Setup OK!");
  for(;;){
      timer.tick();
  // Use the default run(). The Mama duck is designed to also forward data it receives
  // from other ducks, across the network. It has a basic routing mechanism built-in
  // to prevent messages from hoping endlessly.
      duck.run();
  }
}

void loop() {
  
}

bool runSensor(void *) {
  
  String sensorVal = "";

  duck.sendData(topics::sensor, sensorVal);
  Serial.println("\nrunSensor Function");

  return true;
}


String getName() {
  return String("ABCD");
}

String getTemp() {
  
  return String(random(95, 105));
}

String getHr() {
    return String(random(65, 75));
}

String getSpo2() {
  return String(random(95, 99));
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
