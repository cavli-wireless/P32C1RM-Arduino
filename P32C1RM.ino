<<<<<<< Updated upstream
#include "C1RMCore.h"
Network Network;
bool modemWait = false;
bool networkWait = false;
bool dataWait = false;
bool simWait = false;
int ledPin = 5;
=======
#include "lib/C1RM.h"

Network Response(true);

>>>>>>> Stashed changes
void setup() { 
  Network.modemReset();
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
