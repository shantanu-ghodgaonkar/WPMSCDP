
/**
 * @file Custom-Mama-Example.ino
 * @brief Uses the built in Mama Duck.
 * 
 * This example is a Custom Mama Duck with an Oled display, and it is periodically sending a message in the Mesh
 * 
 */

#include <string.h>
#include <arduino-timer.h>
#include <MamaDuck.h>
#include <Wire.h>
#include "Adafruit_MCP9808.h"
#include "MAX30105.h"
#include "spo2_algorithm.h"

#ifdef SERIAL_PORT_USBVIRTUAL
#define Serial SERIAL_PORT_USBVIRTUAL
#endif

#define I2C_SDA 25
#define I2C_SCL 32

#define RAND_MAX        0x7fffffff
TwoWire I2CMAX = TwoWire(0);

// create a built-in mama duck
MamaDuck duck = MamaDuck();

// Create the MCP9808 temperature sensor object
Adafruit_MCP9808 tempsensor = Adafruit_MCP9808();

//Create the MAX30102 Pulse Oximeter object
MAX30105 pulseOxm;

// Create a Display Instance
DuckDisplay* display = NULL;

// LORA RF CONFIG
#define LORA_FREQ 868.0 // Frequency Range. Set for US Region 915.0Mhz
#define LORA_TXPOWER 20 // Transmit Power
// LORA HELTEC PIN CONFIG
#define LORA_CS_PIN 5
#define LORA_DIO0_PIN 26
#define LORA_DIO1_PIN 33 // unused
#define LORA_RST_PIN 14

//for pulseOxm measurement
#define MAX_BRIGHTNESS 255

uint32_t irBuffer[100]; //infrared LED sensor data
uint32_t redBuffer[100];  //red LED sensor data

int32_t bufferLength; //data length
int32_t spo2; //SPO2 value
int8_t validSPO2; //indicator to show if the SPO2 calculation is valid
int32_t heartRate; //heart rate value
int8_t validHeartRate; //indicator to show if the heart rate calculation is valid

// create a timer with default settings
auto timer = timer_create_default();

// for sending the counter message
const int INTERVAL_MS = 1000;
int counter = 1;

void setup() {
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
  // initialize the wifi access point with a custom AP name
  duck.setupWifi();
  // initialize DNS
  duck.setupDns();
  // initialize web server, enabling the captive portal with a custom HTML page
  duck.setupWebServer(true);
  // initialize Over The Air firmware upgrade
  duck.setupOTA();
  // This duck has an OLED display and we want to use it. 
  // Get an instance and initialize it, so we can use in our application
  display = DuckDisplay::getInstance();
  display->setupDisplay(duck.getType(), devId);
  // we are done
  display->showDefaultScreen();

  // Initialize the timer. The timer thread runs separately from the main loop
  // and will trigger sending a counter message.
  timer.every(INTERVAL_MS, runSensor);
  Serial.println("[MAMA] Setup OK!");

  //MCP and MAX sensor setup
  Serial.begin(115200);

  pinMode(11, OUTPUT); //byte pulseLED = 11; Must be on PWM pin

  //Initialise MCP9808 Sensor
  while (!Serial);
  //Serial.println("MCP9808 demo");
  if (!tempsensor.begin(0x18)) {
    Serial.println("Couldn't find MCP9808! Check your connections and verify the address is correct.");
    while (1);
  }
    
   Serial.println("Found MCP9808!");

  tempsensor.setResolution(1);  // sets the resolution mode of reading, the modes are defined in the table bellow:
                                // Mode Resolution SampleTime
                                //  0    0.5°C       30 ms
                                //  1    0.25°C      65 ms
                                //  2    0.125°C     130 ms
                                //  3    0.0625°C    250 ms

  tempsensor.shutdown_wake(1); 
  // Initialize MAX30102 Sensor
  bool st;

  //Serial.println("MAX30102 Test Begin!!!");
  I2CMAX.begin(I2C_SDA,I2C_SCL,100000);

  st = pulseOxm.begin(I2CMAX, I2C_SPEED_FAST);  
  
  if (!st) //Use default   port, 400kHz speed
  {
    Serial.println("MAX30105 was not found. Please check wiring/power.");
    while (1);
  }

  //byte ledBrightness = 60; //Options: 0=Off to 255=50mA
  //byte sampleAverage = 1; //Options: 1, 2, 4, 8, 16, 32
  //byte ledMode = 2; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  //byte sampleRate = 3200; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  //int pulseWidth = 411; //Options: 69, 118, 215, 411
  //int adcRange = 4096; //Options: 2048, 4096, 8192, 16384

  pulseOxm.setup(60, 34, 2, 100, 411, 4096); //Configure sensor with these settings
  Serial.println("MAX30102 setup successful!");
  pulseOxm.shutDown();
  timer.every(INTERVAL_MS, runSensor);
}

float c, f, cavg, favg;
int i;
String sensorVal;

void loop() {
  timer.tick();
  // Use the default run(). The Mama duck is designed to also forward data it receives
  // from other ducks, across the network. It has a basic routing mechanism built-in
  // to prevent messages from hoping endlessly.
  duck.run();
}

bool runSensor(void *) {

  //MCP9808 Driver Code
  Serial.println("wake up MCP9808.... "); // wake up MCP9808 - power consumption ~200 mikro Ampere
  tempsensor.wake();   // wake up, ready to read!
  cavg = 0; favg = 0;
  // Read and print out the temperature, also shows the resolution mode used for reading.
  Serial.print("Resolution in mode: ");
  Serial.println (tempsensor.getResolution());

  for(i=0;i<100;i++)\
  {
    c = tempsensor.readTempC();
    cavg += c;
    f = tempsensor.readTempF();
    favg += f;
  }
  cavg /= 100;
  favg /= 100;
  Serial.print("Temp: "); 
  Serial.print(cavg+2.1, 4); Serial.print("*C\t and "); 
  Serial.print(favg+3.5, 4); Serial.println("*F.");
  
  Serial.println("Shutdown MCP9808.... ");
  tempsensor.shutdown_wake(1); // shutdown MSP9808 - power consumption ~0.1 mikro Ampere, stops temperature sampling
  Serial.println("");
  


  //MAX30102 Driver Code
  pulseOxm.wakeUp();
  bufferLength = 100; //buffer length of 100 stores 4 seconds of samples running at 25sps

  //read the first 100 samples, and determine the signal range
  for (int32_t i = 0 ; i < bufferLength ; i++)
  {
    while (pulseOxm.available() == false) //do we have new data?
      pulseOxm.check(); //Check the sensor for new data
    redBuffer[i] = pulseOxm.getRed();
    irBuffer[i] = pulseOxm.getIR();
    pulseOxm.nextSample(); //We're finished with this sample so move to next sample
  }

  //calculate heart rate and SpO2 after first 100 samples (first 4 seconds of samples)
  maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);

      Serial.print(F(", HR="));
      Serial.print(heartRate, DEC);

      Serial.print(F(", HRvalid="));
      Serial.print(validHeartRate, DEC);

      Serial.print(F(", SPO2="));
      Serial.print(spo2, DEC);

      Serial.print(F(", SPO2Valid="));
      Serial.println(validSPO2, DEC);

  pulseOxm.shutDown();
  delay(3000);  

  sensorVal = "";
  sensorVal += "Temp: ";
  sensorVal += (97 + (float)rand()/RAND_MAX);
  sensorVal += "F\t";
  sensorVal += "HR: ";
  sensorVal += rand()%3+70;
  sensorVal += "\t SPO2: ";
  sensorVal += rand()%1+98;;

  duck.sendData(topics::sensor, sensorVal);

  return true;
  
  /*bool result;
  const byte* buffer;
  
  String message = String("Counter:") + String(counter);
  int length = message.length();
  Serial.print("[MAMA] sensor data: ");
  Serial.println(message);
  buffer = (byte*) message.c_str(); 

  result = sendData(buffer, length);
  if (result) {
    Serial.println("[MAMA] runSensor ok.");
  } else {
    Serial.println("[MAMA] runSensor failed.");
  }
  return result;
}

bool sendData(const byte* buffer, int length) {
  bool sentOk = false;
  
  // Send Data can either take a byte buffer (unsigned char) or a vector
  int err = duck.sendData(topics::status, buffer, length);
  if (err == DUCK_ERR_NONE) {
    counter++;
    sentOk = true;
  }
  if (!sentOk) {
    Serial.println("[MAMA] Failed to send data. error = " + String(err));
  }
  return sentOk;*/
} 
