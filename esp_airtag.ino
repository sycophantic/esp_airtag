/*
   Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleScan.cpp
   Ported to Arduino ESP32 by Evandro Copercini
*/

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

#define btservice "7DFC9000-7D1C-4951-86AA-8D9728F8D66C"
#define btchar "7DFC9001-7D1C-4951-86AA-8D9728F8D66C"

int scanTime = 5; //In seconds
BLEScan* pBLEScan;

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      //Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());

      //  if (advertisedDevice.haveServiceUUID())
      //  {
      //    BLEUUID devUUID = advertisedDevice.getServiceUUID();
      //    Serial.print("Found ServiceUUID: ");
      //    Serial.println(devUUID.toString().c_str());
      //    Serial.println("");
      //  }
      if (advertisedDevice.haveManufacturerData() == true)
      {
        std::string strManufacturerData = advertisedDevice.getManufacturerData();

        uint8_t cManufacturerData[100];
        strManufacturerData.copy((char *)cManufacturerData, strManufacturerData.length(), 0);

        if (strManufacturerData.length() == 29 && cManufacturerData[0] == 0x4C && cManufacturerData[1] == 0x00 &&
            //cManufacturerData[2] == 0x12 && 
            cManufacturerData[3] == 0x19)
        {
          Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
          Serial.println("Found Apple Airtag Device");
          Serial.printf("strManufacturerData: %d ", strManufacturerData.length());
          for (int i = 0; i < strManufacturerData.length(); i++)
          {
            Serial.printf("[%X]", cManufacturerData[i]);
          }
          Serial.printf("\n");
          if (advertisedDevice.haveServiceUUID())
          {
            BLEUUID devUUID = advertisedDevice.getServiceUUID();
            Serial.print("Found ServiceUUID: ");
            Serial.println(devUUID.toString().c_str());
            Serial.println("");
            //if (advertisedDevice.getServiceUUID() == "")
            //  Serial.println("btservice matches");
          }
        }
      }

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
  //Serial.print("Devices found: ");
  //Serial.println(foundDevices.getCount());
  //Serial.println("Scan done!");
  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
  delay(2000);
}
