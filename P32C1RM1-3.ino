#include "C1RMCore.h"
Network Network;
bool modemWait = false;
bool networkWait = false;
bool dataWait = false;
bool simWait = false;
int ledPin = 5;
void setup() {
  Serial.begin(115200);
//  Network.modemReset();
  Network.SerialInit();
  pinMode(ledPin, OUTPUT);
  while (!Network.isModemAvailable()) {
    if (!modemWait) {
      Serial.print("Waiting for modem.");
      modemWait = true;
    } else {
      Serial.print(".");
    }
  }
  Serial.println("Modem Ready");
  delay(10000);
  Serial.println(Network.getModemInfo());
  Serial.println(Network.getIMEI());
  Serial.println(Network.getICCID());
  while (!Network.isESIMReady()) {
    if (!simWait) {
      Serial.print("Waiting for eSIM.");
      simWait = true;
    } else {
      delay(500);
      Serial.print(".");
    }
  }
  Serial.println("eSIM: READY");

  while (!Network.isNetworkAttached()) {
    if (!networkWait) {
      Serial.print("Waiting for Network.");
      networkWait = true;
    } else {
      delay(500);
      Serial.print(".");
    }
  } Serial.println("Registered to Network");
  if (Network.setPDN(3, "hubblethings.io")) {
    Serial.println(Network.getDefaultPDN());
  }
  while (!Network.enablePacketData(true)) {
    if (!dataWait) {
      Serial.print("Waiting for GPRS.");
      dataWait = true;
    } else {
      delay(500);
      Serial.print(".");
    }
  }
  if (Network.getPacketDataStatus()) {
    Serial.println("Packet Data Attached");
    Serial.println(Network.getIPAddr());
    Serial.println(Network.getDNSAddr());
    //    Serial.println("Ping IP:"+Network.getPingStatus("google.com").addr+Network.getPingStatus("google.com").stats);
  }
  Serial.println("RF Strength=" + Network.getRadioQuality().csq + ", RSSI=" + Network.getRadioQuality().rssi + ", BER=" + Network.getRadioQuality().ber);

//=============================================================== SMS Functions ==================================================================================================
//    String readsms = Network.readSMS("5","ME");
//    Serial.println(readsms);  
//    if((Network.sendSMS("9745899513","Its me cavli Wireless"))==1)
//    Serial.println("SMS Send Successfull");
//    
//=============================================================== MQTT Functions ==================================================================================================
//  if((Network.createMQTT("broker.mqttdashboard.com", "1883", "1111", "60", "0", "test_123", "123__4"))==1) 
//  Serial.println("MQTT Connection Successful!!");
//  else
//  Serial.println("MQTT Connection Unsuccessful!!");
//  while(!(Network.publishMQTT("Hubble", "Cavli-R&D-KOCHI", "0", "0", "0"))) {
//  } Serial.println("MQTT Message Published Successfully");
//  while (!(Network.subscribeMQTT("Hubble123", "0", false))) {
//  } Serial.println("MQTT Unsubscription Successfull");
//  while (!Network.subscribeMQTT("Hubble123", "0", true)) {
//  } Serial.println("MQTT Subscription Successfull");
//  while (!(Network.disconnectMQTT())) {
//  } Serial.println("MQTT Connection Disconnected Successfully");


//=============================================================== COAP Functions ==================================================================================================
  //  Network.createCoAP("8.8.8.8");
  //  Network.sendCoAPData("2","1232");
  //  Network.closeCoAP();

//=============================================================== TCP Functions ==================================================================================================
//    Network.initSingleTCP("TCP", "121.41.100.43", 1234);
////  Network.initMultiTCP(1, "TCP", "121.41.100.43", 1234);
//    bool res = Network.sendTCPData(0,0,"hELLOO");
//    Serial.println(res);
//    String res1 = Network.recieveTCPData(0,0);
//    Serial.println(res1);
//    Network.closeTCP(0,0);
//    Network.shutdownTCP();
//=============================================================== UDP Functions ==================================================================================================
//    Network.creatUDP(56,1);
//    Network.sendUDP(1,"1.1.1.1",7000,7,"test123");
//    Network.recieveUDP(1,128);
//    Network.endUDP();
}


void loop() {
  digitalWrite(ledPin, HIGH);
  delay(100);
  digitalWrite(ledPin, LOW);
  delay(100);
  digitalWrite(ledPin, HIGH);
  delay(100);
  digitalWrite(ledPin, LOW);
  delay(700);
}
