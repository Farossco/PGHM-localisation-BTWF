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
#include <string.h>

int scanTime = 5; //In seconds
BLEScan* pBLEScan;
//Ajout Clement
	double getDistance(BLEAdvertisedDevice Device);
  bool detecte(const char*  ManufacturerData,BLEAdvertisedDevice target);
  void debug(BLEAdvertisedDevice device);

	//Fin Ajout Clement

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {


      //Serial.printf(" Advertised Device: %s \n", advertisedDevice.toString().c_str());
 
      // Identification portable filtre

      std::string res="coucou";
      //const char *res ="coucou";
      char *pHex = BLEUtils::buildHexData(nullptr, (uint8_t*)advertisedDevice.getManufacturerData().data(), advertisedDevice.getManufacturerData().length());
      res= pHex;
    
		  
      //Serial.printf(" Manu data : %s, Match :  %d , distance %d \n",pHex,strcmp(pHex,"4c00100*"),getDistance(advertisedDevice));
      
      //debug(advertisedDevice);

     //if(strcmp(pHex,"4c00100*")>5){
       // Serial.printf("Portable détecté \n");
        //Serial.printf(" Manu data : %s, Match :  %d , distance %d , RSSI %d \n",pHex,strcmp(pHex,"4c00100*"),getDistance(advertisedDevice),advertisedDevice.getRSSI() );
      //}

      if(detecte("4c00100*",advertisedDevice)){
        Serial.printf("Portable détecté \n");
        Serial.printf(" Manu data : %s , distance %lf , RSSI %d \n",pHex,getDistance(advertisedDevice),advertisedDevice.getRSSI() );

          /// bool Approche(distance,last_distance)

      }
      free(pHex);
      
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

// Ajout Clement 
/**
 * @brief Calculate the distance.
 * @return The distance between the esp32 and the BT device
 */
double   getDistance(BLEAdvertisedDevice Device) {
	// init value localisation
      double   distance(1.0);
      double    power_meas(1.0);
      double   N_env(2.0);
      // debug
      power_meas=-69.000; //Device.getTXPower();

      // shadowing model to calculate distance from device emeting bluetooth
      distance = pow(10.0,(( power_meas - Device.getRSSI())/(10.0*N_env)));
	return distance;
} // getDistance

/**
 * @brief Compare Manu data to target.
 * @return Bool, true if match
 */
bool detecte(const char*  ManufacturerData,BLEAdvertisedDevice target){
        // Attention, il faut connaitre les données constructeurs de l'appareil que l'on cherche à détecter
        // Sachant qu'une partie la chaines de caractères est rendu aléatoire par le protocole
        // Il faut donc mettre en argument la 1ère partie fixe
          std::string res="coucou";
          bool match(false);
          char *pHex2 = BLEUtils::buildHexData(nullptr, (uint8_t*)target.getManufacturerData().data(), target.getManufacturerData().length());
          res= pHex2;
          match=strcmp(pHex2,ManufacturerData)>5;
          free(pHex2);

          return match;
      };

/**
 * @brief Print data from BT device scan to help debug.
 * @return void
 */
void debug(BLEAdvertisedDevice device){
 Serial.printf(" Advertised Device: %s , RSSI : %d \n", device.toString().c_str(),device.getRSSI());
//  Serial.printf(" Manu data : %s, Match :  %d , distance %d \n",pHex,strcmp(pHex,"4c001005*"),getDistance(device));

}

// Fin Clement 
