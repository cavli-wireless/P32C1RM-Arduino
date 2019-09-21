#include "C1RMCore.h"

String comma = ",";
String quotes = "\"";
String cmd;
String Resp;
String TrimResp;
String AtCommand; 
int Trimvalue;
int i;

Network::Network(bool displayMsg){
  //Anything yoy need when initiating object goes here
}

String readstringin() {
  int flag =0;
  int i=0;
  String inputString;
  for(;;) {
    if (Serial2.available()) {
      for(i=0;i<1;i++) {
      inputString=Serial2.readString();
      }
      flag = 1;
      break;     
    }   
   return inputString;
   inputString="\0";
   if(flag==1)
    break;
  }
}

String CheckTrimResponse(String TrimResponse,String stringcheck,String posresp,String negresp) {
  if (TrimResponse == stringcheck) {
    Serial.println(posresp);
  } else {
    Serial.println(negresp);
  }
  return TrimResponse;
}

String callBackResponse(String cmd,String stringcheck,String posresp,String negresp) {
  String TrimResponse;
  while(1) { 
    Serial2.println(cmd);
    delay(100);
    Resp = readstringin();
    Trimvalue = Resp.indexOf('K');
    TrimResp = Resp.substring(Trimvalue-1,Trimvalue+1);
    if (TrimResp == stringcheck) {
      Serial.println(posresp);
      break;
    } else {
      Serial.println(negresp);
    }
    delay(500);
  }
  return TrimResponse;
}

void Network::radioEnable (bool enable) { // True - Enable Cellular Radio, False - Disable Cellular Radio.
  if (enable) {
    Serial2.println("AT+CFUN=1");
    delay(500);
  } else {
    Serial2.println("AT+CFUN=0");
    delay(500);
  }
}

void Network::networkAttach (bool enable) { // True - Attach to Cellular Network, False - Dettach from Cellular Service.
  if (enable) {
    Serial2.println("AT+CREG=1");
    delay(500);
  } else {
    Serial2.println("AT+CREG=0");
    delay(500);
  }
}

String Network::getRadioStatus () { // Inform the current Radio mode status.
  Serial2.println("AT+CFUN?");
  delay(500);
  Resp = readstringin();
  Trimvalue = Resp.indexOf('+');
  TrimResp = Resp.substring(Trimvalue,Trimvalue+8);
  return TrimResp;
}

String Network::getModemInfo () { // Displays details Modem Information.
  callBackResponse("AT","OK","MODULE ON","MODULE OFF");
  Serial2.println("ATI");
  delay(500);
  String RespATI  = readstringin();
  Serial2.println(RespATI);
  Trimvalue = RespATI.indexOf('M');
  TrimResp = RespATI.substring(Trimvalue,Trimvalue+151);
  Serial2.println("AT+CCID");
  delay(200);
  String RespCCID  = readstringin();
  String TrimCCIDResponse = RespCCID.substring(18,39);
  Serial2.println("AT+GSN");
  delay(200);
  String RespIMEI  = readstringin();
  String TrimIMEIResponse = RespIMEI.substring(10,29);
  String Response = "Device Info:\r\n" + TrimResp;
  return Response;
}

String Network::getSignalStrength() { // Displays the Radio Signal Strength.
  Serial2.println("AT+CSQ");
  delay(500);
  Resp = readstringin();
  Trimvalue = Resp.indexOf(':');
  TrimResp = Resp.substring(Trimvalue+2,Trimvalue+7);
  return TrimResp;
}



String Network::getNetworkStatus() { // Displays current network registration status.
  Serial2.println("AT+CREG?");
  delay(500);
  Resp = readstringin();
  Trimvalue = Resp.indexOf(':');
  TrimResp = Resp.substring(Trimvalue+2,Trimvalue+5);
  return TrimResp;

}

void Network::setPDN(int ipType, String apn) { // IP Type: 1 - IPV4, 2 - IPV6, 3 - IPV4V6, 4 - No IP (NB-IoT Paging).
  Serial.print("Setting APN to ");
  switch (ipType) {
    case 1:
      AtCommand  = "AT+CFGDFTPDN=" + "1" + comma + quotes + apn + quotes;
      Serial2.println(AtCommand);
      Serial.println(apn);
      break;
    case 2:
      AtCommand  = "AT+CFGDFTPDN=" + "2" + comma + quotes + apn + quotes;
      Serial2.println(AtCommand);
      Serial.println(apn);
      break;
    case 3:
      AtCommand  = "AT+CFGDFTPDN=" + "3" + comma + quotes + apn + quotes;
      Serial2.println(AtCommand);
      Serial.println(apn);
      break;
    case 5:
      AtCommand  = "AT+CFGDFTPDN=" + "5" + comma + quotes + apn + quotes;
      Serial2.println(AtCommand);
      Serial.println(apn);
      break;
    default:
      break;
  }
}

// String Network::getDefaultPDN() // Dsiplays default PDN configured.
// {
//     Serial2.println("AT+CFGDFTPDN?");
//     delay(500);
//     Resp = readstringin();
//     Serial.println(Resp);
//     int Trimvalue_1 = Resp.indexOf('1');
//     int Trimvalue_2 = Resp.indexOf('2');
//     int Trimvalue_3 = Resp.indexOf('3');
//     int Trimvalue_4 = Resp.indexOf('5');
//     int semiindex1 = Resp.indexOf(';');
//     int semiindex2 = Resp.indexOf(';',semiindex1+1);
//     int semiindex3 = Resp.indexOf(';',semiindex2+1);
//     int semiindex4 = Resp.indexOf(';',semiindex3+1);
//     int semiindex5 = Resp.indexOf(';',semiindex4+1);
//     String TrimResp_IPV4 = Resp.substring(Trimvalue+6,semiindex2);
//     String TrimResp_IPV6 = Resp.substring(Trimvalue+6,semiindex3);
//     String TrimResp_IPV4V6 = Resp.substring(Trimvalue+6,semiindex4);
//     String TrimResp_NOIP = Resp.substring(Trimvalue+6,semiindex5);
//     Serial.println(TrimResp_IPV4);
//     Serial.println(TrimResp_IPV6);
//     Serial.println(TrimResp_IPV4V6);
//     Serial.println(TrimResp_NOIP);
//     return TrimResp;
// }

