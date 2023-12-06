#include "DHT.h"
#include <BluetoothSerial.h>

#define DHTPIN 4
#define DHTTYPE DHT11
#define SLAVE_ID "1" // Unique ID for each slave

DHT dht(DHTPIN, DHTTYPE);
BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_Slave" + String(SLAVE_ID));
  Serial.println(F("Slave has started!"));
  dht.begin();
}

void loop() {
  sendDataViaBluetooth();

  // Rest of your code for reading DHT sensor data goes here

  delay(10000); // Adjust the delay according to your requirements
}

void sendDataViaBluetooth() {
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (it's a very slow sensor)
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Create a JSON-like string for the sensor data with ID
  String sensorData = "{\"id\":\"" + String(SLAVE_ID) + "\",\"temperature\":" + String(t) + ",\"humidity\":" + String(h) + "}\n";
  Serial.println(sensorData);

  // Send sensor data to master via Bluetooth
  SerialBT.print(sensorData);
}
