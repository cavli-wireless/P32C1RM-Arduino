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
  if(Network.setPDN(1,"airtelgprs.com")) {
    Serial.println(Network.getDefaultPDN());
  }
  while(!Network.enablePacketData(true)) {
  }
  Serial.println("Packet Data Attached");
  Serial.println(Network.getPingStatus("google.com").status);

  
//  Network.sendSMS("9745899513","Its me cavli Wireless");
//  Network.readSMS("2","ME");

  //Network.disconnectMQTT();
  if(!(Network.createMQTT("broker.mqttdashboard.com","1883","1111", "60","0","test_123","123__4"))){
  }Serial.println("MQTT Connection Created!!");
  if(!(Network.publishMQTT("Hubble","Cavli-R&D-KOCHI","0","0","0"))){
  }Serial.println("MQTT Message Published Successfully");
  Network.subscribeMQTT("Hubble123","0",true);
  if(!(Network.disconnectMQTT())){
  }Serial.println("MQTT Message Disconnected Successfully");

//  Network.createCoAP("8.8.8.8");
  Network.sendCoAPData("2","1232");
  Network.closeCoAP();
  
}

 
void loop() {

}
