#include "C1RM.h"

Network Response(true);

void setup() { 
  Serial.begin(115200);
  core.serialBegin();
  Serial.println("Testing.....\n");
  String info = core.getModemInfo();
  Serial.println(info);

//  String NetResp = core.getNetConnection("1","IP","hubblethings.io");
//  Serial.print("IP Address:");
//  Serial.println(NetResp);

//  String sender = core.checkSMSSender();
//  Serial.println(sender);

//  core.MQTTCreateConn("broker.mqttdashboard.com", "1883","Cavli-P32TEST", "60","0","test_12453","123__4");
//   core.MQTTPublish("Hubble","p32c1rs-lanka","0","0","0");
//  core.MQTTPublish("Hubble","HELLOOO","0","0","0");
//  core.MQTTPublish("Hubble","hey","0","0","0");
//  core.MQTTPublish("Hubble","how are you","0","0","0");
//  core.MQTTPublish("Hubble","cavli wireless","0","0","0");
//  core.MQTTDisconnect();
//  core.MQTTSubscribe("Hubble","0");
//  core.MQTTUnsubscribe("Hubble","0");
//  while(1)
//  {
//   String MQTTResp = core.MQTTReception();
//   int flag =1;
//   if (flag==1)
//   {
//     Serial.println(MQTTResp);
//   }
//  else
//  {
//    //do nothing
//  }
//  }

 String time1 = core.queryTime();
 Serial.println(time1);
// String imei =  core.queryIMEI();
// Serial.println(imei);
 String model =  core.queryModuleModel();
 Serial.println(model);
 String manf =  core.queryManufacture();
 Serial.println(manf);
//
 String imsi = core.queryIMSI();
 Serial.println(imsi);
 String iccid = core.queryICCID();
 Serial.println(iccid);

// core.enableEDRX("1","5","0010");
// core.disableEDRX();
//
// core.enablePSM("1,,,","00000100","00001111");
// core.disablePSM();

 core.queryOperator();
 core.querySignal();
 core.querySignalConn();



//  core.ping("8.8.8.8");
//  core.checkSMSSender();
//  core.SMSRead("2","ME");
//  core.SMSSend("+919745899513","hi");
//
//  core.queryOperator();
//  core.querySignalConn();

//  core.hardResetModem();
//  printf("Success");

  


  
}

 
void loop() {

}
