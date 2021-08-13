#include <LPS.h>

#include <dht.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
File myFile;

#define dht_apin A1
unsigned long myTime;
LPS ps;
dht DHT;
bool on = false;

void setup() {
  Serial.begin(9600);
  
  Wire.begin();

  if (!ps.init())
  {
    Serial.println("Failed to autodetect pressure sensor!");
    while (1);
  }
  while (!Serial) {
  ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.print("Initializing SD card...");
  if (!SD.begin(10)) {
  Serial.println("initialization failed!");
  while (1);
  
  }
  ps.enableDefault();
}

void loop() {
  myFile = SD.open("test.txt", FILE_WRITE);
  // milisaniye,hava kalitesi,co2 miktarı,nem,sıcaklık,basınç,yükseklik,sıcaklık
  float sensorValue2 = analogRead(2);
  float sensorValue = analogRead(A0);
  myTime = millis();
  myFile.print(myTime);
  myFile.print(",");
  myFile.print(sensorValue2, DEC);
  myFile.print(",");
  myFile.print(sensorValue);
  myFile.print(",");
  DHT.read11(dht_apin);
  myFile.print(DHT.humidity);
  myFile.print(",");
  myFile.print(DHT.temperature); 
  float pressure = ps.readPressureMillibars();
  float altitude = ps.pressureToAltitudeMeters(pressure);
  float temperature = ps.readTemperatureC();
  myFile.print(",");
  myFile.print(pressure);
  myFile.print(",");
  myFile.print(altitude);
  myFile.print(",");
  myFile.println(temperature);
  myFile.close();

  delay(2000);
}
