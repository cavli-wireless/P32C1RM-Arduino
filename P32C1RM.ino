#include "C1RMCore.h"
Network Network;
bool modemWait = false;
bool networkWait = false;
bool dataWait = false;
bool simWait = false;
int ledPin = 5;
String temp = "";
int tempCount = 0;


#ifdef __cplusplus
extern "C" {
#endif
uint8_t temprature_sens_read();
#ifdef __cplusplus
}
#endif
uint8_t temprature_sens_read();


void setup() { 
  // Network.modemReset();
  Serial.begin(115200);
  Network.SerialInit();
  pinMode(ledPin, OUTPUT);
  while(!Network.isModemAvailable()) {
    if(!modemWait) {
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
  while(!Network.isESIMReady()) {
    if(!simWait) {
      Serial.print("Waiting for eSIM.");
      simWait = true;
    } else {
      delay(500);
      Serial.print(".");
    }
  }
  Serial.println("eSIM: READY");
  while(!Network.isNetworkAttached()) {
    if(!networkWait) {
      Serial.print("Waiting for Network.");
      networkWait = true;
    } else {
      delay(500);
      Serial.print(".");
    }
  } Serial.println("Registered to Network");
  if(Network.setPDN(3,"hubblethings.io")) {
    Serial.println(Network.getDefaultPDN());
  }
  while(!Network.enablePacketData(true)) {
    if(!dataWait) {
      Serial.print("Waiting for GPRS.");
      dataWait = true;
    } else {
      delay(500);
      Serial.print(".");
    }
  }
  if(Network.getPacketDataStatus()) {
    Serial.println("Packet Data Attached");
    Serial.println(Network.getIPAddr());
    Serial.println(Network.getDNSAddr());
    Serial.println("Ping IP:"+Network.getPingStatus("google.com").addr+Network.getPingStatus("google.com").stats);
  }
  Serial.println("RF Strength="+Network.getRadioQuality().csq+", RSSI="+Network.getRadioQuality().rssi+", BER="+Network.getRadioQuality().ber);

  // Network.readSMS("2","ME");
  // Network.sendSMS("9745899513","Its me cavli Wireless");
  

  if(!(Network.createMQTT("broker.mqttdashboard.com","1883","1111", "60","0","test_123","123__4"))){
  }Serial.println("MQTT Connection Created!!");
  if(!(Network.publishMQTT("Hubble","Cavli-R&D-KOCHI\n","0","0","0"))){
  }Serial.println("MQTT Message Published Successfully");
  Network.subscribeMQTT("Hubble","0",true);
  // if(!(Network.disconnectMQTT())){
  // }Serial.println("MQTT Message Disconnected Successfully");

//  Network.createCoAP("8.8.8.8");
//  Network.sendCoAPData("2","1232");
//  Network.closeCoAP();

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
  if(tempCount == 60 || tempCount == 0){
    temp = (temprature_sens_read() - 32) / 1.8;
    Network.publishMQTT("Hubble",temp+"\n","0","0","0");
    tempCount = 0;
  }
  tempCount = tempCount + 1;
}
