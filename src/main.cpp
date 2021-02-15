#include <Arduino.h>
/*
   Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleScan.cpp
   Ported to Arduino ESP32 by Evandro Copercini
*/

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

#include <math.h>

int scanTime = 5; //In seconds
BLEScan* pBLEScan;

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      Serial.printf(" Advertised Device: %s \n", advertisedDevice.toString().c_str());
      // init value localisation
      double distance(1);
      int power_meas(1);
      int N_env(1);
      // debug
      power_meas=-69; //advertisedDevice.getTXPower();

      // shadowing model to calculate distance from device emeting bluetooth
      distance = pow((( power_meas - advertisedDevice.getRSSI())/(10*N_env)),10);
      Serial.printf(" ### Debug power meas = %d, RSSI = %d " , power_meas, advertisedDevice.getRSSI());
      Serial.printf("Distance :  %f, Fonction %d metre \n",distance, advertisedDevice.getDistance());
      // Identification portable filtre
      //char *pHex = BLEUtils::buildHexData(nullptr, (uint8_t*)getManufacturerData().data(), getManufacturerData().length());

      //Serial.printf(" Manu data : %s, Match :  %d \n",advertisedDevice.getManufacturerData().c_str(),advertisedDevice.getManufacturerData()=="4c001005**");
      //  Serial.printf(" Manu data : %s, Match :  %d \n",advertisedDevice.getManufacturerData().c_str(),"4c0010055564545645645156"=="4c0010055*");
    }
};

void setup() {
  Serial.begin(115200);
  Serial.println("Scanning...");

  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);  // less or equal setInterval value
}

void loop() {
  // put your main code here, to run repeatedly:
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  Serial.print("Devices found: ");
  Serial.println(foundDevices.getCount());
  Serial.println("Scan done! \n ");
  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
  delay(2000);
}