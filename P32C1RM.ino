#include "C1RMCore.h"
Network Network(true);
bool modemWait = false;

void setup() { 
  Serial.begin(115200);
  Network.SerialInit();
  while(!Network.isModemAvailable()) {
    if(!modemWait) {
      Serial.print("Waiting for modem.");
      modemWait = true;
    } else {
      Serial.print(".");
    }
  }
  Serial.println("Modem Ready");
  Serial.println(Network.getModemInfo());
  Serial.println(Network.getIMEI());
  Serial.println(Network.getICCID());
  Serial.println(Network.getIMSI());
  while(!Network.isNetworkAttached()) {
  } Serial.println("Registered to Network");
  if(Network.setPDN(3,"hubblethings.io")) {
    Serial.println(Network.getDefaultPDN());
  }
  while(!Network.enablePacketData(true)) {
  }
  if(Network.getPacketDataStatus()) {
    Serial.println("Packet Data Attached");
    Serial.println(Network.getIPAddr());
    Serial.println(Network.getDNSAddr());
    Serial.println("Ping IP:"+Network.getPingStatus("google.com").addr+Network.getPingStatus("google.com").stats);
  }
  Serial.println("RF Strength="+Network.getRadioQuality().csq+", RSSI="+Network.getRadioQuality().rssi+", BER="+Network.getRadioQuality().ber);
}

void loop() {

}
