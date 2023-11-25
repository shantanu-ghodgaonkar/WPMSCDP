#include <Wire.h>
#include "Adafruit_MCP9808.h"
#include "MAX30105.h"
#include "spo2_algorithm.h"

#define I2C_SDA 25
#define I2C_SCL 32

TwoWire I2CMAX = TwoWire(0);

// Create the MCP9808 temperature sensor object
Adafruit_MCP9808 tempsensor = Adafruit_MCP9808();

//Create the MAX30102 Pulse Oximeter object
MAX30105 pulseOxm;

#define MAX_BRIGHTNESS 255

uint32_t irBuffer[100]; //infrared LED sensor data
uint32_t redBuffer[100];  //red LED sensor data

int32_t bufferLength; //data length
int32_t spo2; //SPO2 value
int8_t validSPO2; //indicator to show if the SPO2 calculation is valid
int32_t heartRate; //heart rate value
int8_t validHeartRate; //indicator to show if the heart rate calculation is valid

void setup() {
  Serial.begin(115200);

  pinMode(11, OUTPUT); //byte pulseLED = 11; Must be on PWM pin


  //Initialise MCP9808 Sensor
  while (!Serial);
  Serial.println("MCP9808 demo");
  if (!tempsensor.begin(0x18)) {
    Serial.println("Couldn't find MCP9808! Check your connections and verify the address is correct.");
    while (1);
  }
    
   Serial.println("Found MCP9808!");

  tempsensor.setResolution(1); // sets the resolution mode of reading, the modes are defined in the table bellow:
  // Mode Resolution SampleTime
  //  0    0.5°C       30 ms
  //  1    0.25°C      65 ms
  //  2    0.125°C     130 ms
  //  3    0.0625°C    250 ms

  tempsensor.shutdown_wake(1); 

  // Initialize MAX30102 Sensor
  Serial.println("MAX30102 Test Begin!!!");
  I2CMAX.begin(I2C_SDA,I2C_SCL,100000);

   bool st;

  st = pulseOxm.begin(I2CMAX, I2C_SPEED_FAST);
  if (!st)
  {
    Serial.println(F("MAX30105 was not found. Please check wiring/power."));
    while (1);
  }

  //byte ledBrightness = 60; //Options: 0=Off to 255=50mA
  //byte sampleAverage = 1; //Options: 1, 2, 4, 8, 16, 32
  //byte ledMode = 2; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  //byte sampleRate = 3200; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  //int pulseWidth = 411; //Options: 69, 118, 215, 411
  //int adcRange = 4096; //Options: 2048, 4096, 8192, 16384

  pulseOxm.setup(60, 34, 2, 100, 411, 4096); //Configure sensor with these settings
  pulseOxm.shutDown();
}
float c, f, cavg, favg;
int i;
void loop() {

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
}
