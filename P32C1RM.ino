#include "C1RMCore.h"
Network Network(true);

bool modemWait = false;
String modemWaitString = "Waiting for modem.";

void setup() { 
  Serial.begin(115200);
  Network.SerialInit();
  while(!Network.isModemAvailable()) {
    if(!modemWait) {
      Serial.print(modemWaitString);
      modemWait = true;
    } else {
      Serial.print(".");
    }
  } 
  Serial.println("Modem Ready");
  Serial.println(Network.getModemInfo());
  Serial.println(Network.getIMEI());

  while(!Network.isNetworkAttached()) {
  } Serial.println("Registered to Network");
  Serial.println("# Get CSQ Signal: csq= " + Network.getRadioQuality().csq + ", rssi= " + Network.getRadioQuality().rssi + ", ber= " + Network.getRadioQuality().ber);
  if(Network.setPDN(3,"hubblethings.io")) {
    Serial.println(Network.getDefaultPDN());
  }
  while(!Network.enablePacketData(true)) {
  }
  Serial.println("Packet Data Attached");
  Serial.println(Network.getPingStatus("google.com").status);
}

 
void loop() {

}
