

/*===================================================================== CAVLI WIRELESS P32 Firmware v1 ========================================================================*/
#include <C1RMCore.h>
Network Network(true);
#define RXD2 27
#define TXD2 26


void setup() { 
  Serial.begin(115200);
  Serial2.begin(57600, SERIAL_8N1, RXD2, TXD2);
  Serial.println("Testing.....\n");
  Network.radioEnable(true); 
  String modeminfo = Network.getModemInfo();
  Serial.println(modeminfo);
  String Status = Network.getRadioStatus();
  Serial.println(Status);
  String net =Network.getNetworkStatus();
  Serial.println(net);
  String strength = Network.getSignalStrength();
  Serial.println(strength);
  Network.networkAttach(true);
  Network.setPDN(1,"hubblethings.io");
  
}

 
void loop() {

}
