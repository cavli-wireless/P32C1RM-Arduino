#include "C1RM.h"

#define RXD2 27
#define TXD2 26
TaskHandle_t Task1, Task2;
SemaphoreHandle_t baton;
void codeForTask1( void * parameter );
void codeForTask2( void * parameter );
String comma = ",";
String quotes = "\"";
String cmd;
String Resp;
String TrimResp; 
int Trimvalue;
int i;

Network::Response(bool displayMsg){
  //Anything yoy need when initiating object goes here
}

String readstringin(){
  int flag =0;
  int i=0;
  String inputString;
  for(;;){
    if (Serial2.available())
    {
      for(i=0;i<1;i++){
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

void Network::TaskCreatepinnedCore(){
  baton = xSemaphoreCreateMutex();
  xTaskCreatePinnedToCore(codeForTask1,"led1Task",1000,NULL,1,&Task1,1);
  delay(500);  // needed to start-up task1
 
  xTaskCreatePinnedToCore(codeForTask2,"led2Task",1000,NULL,1,&Task2,0);
  delay(500);

  
}

void codeForTask1( void * parameter )
{
  for (;;) {
    Serial.print("LED Task which runs on Core ");
    Serial.print( xPortGetCoreID());
    Serial.print("\n\r");  
    digitalWrite(2,HIGH);
    delay(1000);
    digitalWrite(2,LOW);
    delay(1000);
  }
}

void codeForTask2( void * parameter )
{
  for (;;) {
    Serial.print("Finish Loop Task which runs on Core ");
    Serial.print( xPortGetCoreID());
    Serial.print(" Time \r\n ");
    delay(10);  
  }
}
String CheckTrimResponse(String TrimResponse,String stringcheck,String posresp,String negresp)
{
  if (TrimResponse == stringcheck){
    Serial.println(posresp);
  } else {
    Serial.println(negresp);
  }
  return TrimResponse;
}

String callBackResponse(String cmd,String stringcheck,String posresp,String negresp)
{
  String TrimResponse;
  while(1)
  { 
  Serial2.println(cmd);
  delay(100);
  Resp = readstringin();
  // Serial.println(Resp);
  delay(100);
  TrimResponse = Resp.substring(2,4);
  // Serial.println(TrimResponse);
  if (TrimResponse == stringcheck)
  {
    Serial.println(posresp);
    break;
  }
  else 
  {
    Serial.println(negresp);
  }
  delay(500);
  }
  return TrimResponse;
}


void Core::serialBegin()
{
    Serial2.begin(57600, SERIAL_8N1, RXD2, TXD2);
}

String Core::getModemInfo()
{
    callBackResponse("AT","OK","MODULE ON","MODULE OFF");
    Serial2.println("AT+");
    delay(100);
    Serial2.println("ATI");
    delay(100);
    String RespATI  = readstringin();
    String TrimATIResponse = RespATI.substring(20,220);
    Serial2.println("AT+CCID");
    delay(50);
    String RespCCID  = readstringin();
    String TrimCCIDResponse = RespCCID.substring(18,39);
    Serial2.println("AT+GSN");
    delay(50);
    String RespIMEI  = readstringin();
    String TrimIMEIResponse = RespIMEI.substring(10,29);
    String Response = "Device Info:\r\n" + TrimATIResponse;
    return Response;
}
//====================================================================== GENERAL FUNCTIONS ================================================================================
String Core::queryTime()
{
  Serial2.println("AT+CCLK?");
  delay(500); 
  String Resp = readstringin();
  Trimvalue = Resp.indexOf('"');
  TrimResp = Resp.substring(Trimvalue,Trimvalue+23);
  // Serial.println(TrimResp);
  return TrimResp;
}

String Core::queryIMEI()
{
  Serial2.println("AT+GSN=0");
  delay(400); 
  Resp  = readstringin();
  Trimvalue = Resp.indexOf(':');
  TrimResp = Resp.substring(Trimvalue,Trimvalue+10);
  // Serial.println(TrimResp);
  return TrimResp;
}

String Core::queryModuleModel()
{
  Serial2.println("AT+CGMM");
  delay(400); 
  Resp  = readstringin();
  Trimvalue = Resp.indexOf('M');
  TrimResp = Resp.substring(Trimvalue,Trimvalue+22);
  // Serial.println(TrimResp);
  return TrimResp;
}

String Core::queryManufacture()
{
  Serial2.println("AT+CGMI");
  delay(400); 
  Resp  = readstringin();
  Trimvalue = Resp.indexOf('M');
  TrimResp = Resp.substring(Trimvalue,Trimvalue+33);
  // Serial.println(TrimResp);
  return TrimResp;
}



//====================================================================== INTERNET FUNCTIONS ================================================================================
String Core::getNetConnection(String type, String pdptype, String apn)
{
  Serial2.println("AT+CFUN=1\r\n");
  delay(500);
  String FunResp = readstringin();
  checkNetworkRegistraion();
  configurePDP(type,pdptype,apn);
  checkDataAttachement();
  Serial2.println("AT+CGPADDR\r\n");
  delay(500);
  String CGPADDRResponse  = readstringin();
  int Trimvalue = CGPADDRResponse.indexOf(':');
  String TrimResp = CGPADDRResponse.substring(Trimvalue+5,Trimvalue+18);
  return TrimResp;
}
String Core::checkSignalStrength()
{
    Serial2.println("AT+CSQ\r\n");
    delay(1000);
    String RespCSQ = readstringin();
    String TrimCSQResponse = RespCSQ.substring(8,13);
    Serial.print("Signal Strength:");
    Serial.print(TrimCSQResponse);
    return TrimCSQResponse;
}

String Core::checkNetworkRegistraion()
{
  while(1)
  {
    checkSignalStrength();
    Serial2.println("AT+CEREG?\r\n");
    delay(1000);
    String RespCREG  = readstringin();
    delay(100);
    String TrimCREGResponse = RespCREG.substring(10,13);
    String TrimCREGResponse1 = RespCREG.substring(10,12);
    if(TrimCREGResponse1 == "1,")
    {
      Serial.print(" & ");
      Serial.print("Network Response:");
      String NetAttachResp = "[" + TrimCREGResponse + "]";
      Serial.println(NetAttachResp);
      if (TrimCREGResponse == "1,1")
        Serial.println("Registered to Home Network");
      if (TrimCREGResponse == "1,0")
        Serial.println("Not Registered");
      else if (TrimCREGResponse == "1,5")
        Serial.println("Registered to Roaming Network");
      else if (TrimCREGResponse == "1,2")
        Serial.println("Unregistered");
      else if (TrimCREGResponse == "1,3")
        Serial.println("Registration Denied");
      else if(TrimCREGResponse == "1,1" || TrimCREGResponse == "1,5")
      {
        Serial.println("Network Resgistraion Successfull!!!");
        Serial.println("\0");
        break;
    }
    else
    {
      Serial.println("Unknown!!"); 
      Serial.println("Network Resgistraion Unsuccessfull!!!");   
    }
  }
  Serial.println("Waiting for the Network Registration!");
  Serial.println("\0");
  }
  String TrimCREGRespons1;
  return TrimCREGRespons1;
}



String Core::configurePDP(String type, String pdptype, String apn)
{
  Serial2.println("Activating APN");
  String AtCommand  = "AT+CGDCONT=" + type + "," + "\"" + pdptype + "\"" + "," + "\"" + apn + "\"" + "\r\n";
  //Serial.println(AtCommand);
  Serial2.println(AtCommand);
  delay(100);
  String RespCGDCONT  = readstringin();
  // Serial.println(RespCGDCONT);
  Serial.print("APN:");
  Serial.println(apn);
  Serial.println("Activation Successfull");
  return AtCommand;
}

String Core::checkDataAttachement()
{
  while(1)
  {
    Serial2.println("AT+CGACT=1,1\r\n");
    delay(100);
    String RespCGACT  = readstringin();
    String TrimCGACTResponse = RespCGACT.substring(2,4);
    if (TrimCGACTResponse == "OK")
  {
      Serial.println("Data Connection Successfull");
      break;
    }
    else
    {
      Serial.println("Data Connection Unsuccessfull");   
    }
  }
  String TrimCGACTResponse1;
  return TrimCGACTResponse1;
}

String Core::ping(String ipaddr)
{
  String quotes = "\"";
  Serial.print("Pinging to ");
  Serial.println(ipaddr);
  String pingCommand = "at+ping=" + quotes + ipaddr + quotes;
  Serial2.println(pingCommand);
  delay(5000); 
  String Respping  = readstringin();
  Serial.println(Respping);
  return Respping;
}

String Core::configureDNS(String prim, String sec)
{
  String comma = ",";
  String DNSCommand = "AT+CDNSCFG=" + prim + comma + sec;
  Serial2.println(DNSCommand);
  delay(100); 
  String RespDNS  = readstringin();
  Serial.println(RespDNS);
  return RespDNS;
}

String Core::domainNameRes(String domain)
{
  String quotes = "\"";
  String domainNameResCommand = "AT+CDNSGIP=" + quotes + domain + quotes;
  Serial2.println(domainNameResCommand);
  delay(100); 
  String RespdomainNameRes  = readstringin();
  Serial.println(RespdomainNameRes);
  return RespdomainNameRes;
}
//====================================================================== SIM FUNCTIONS =====================================================================================
String Core::queryIMSI()
{
  String queryIMSIcmd = "AT+CIMI";
  Serial2.println(queryIMSIcmd);
  delay(400); 
  Resp  = readstringin();
  Trimvalue = Resp.indexOf('K');
  TrimResp = Resp.substring(Trimvalue-20,Trimvalue-5);
  return TrimResp;
}

String Core::queryICCID()
{
  String queryICCIDcmd = "AT+CCID";
  Serial2.println(queryICCIDcmd);
  delay(400); 
  Resp = readstringin();
  Trimvalue = Resp.indexOf('K');
  TrimResp = Resp.substring(Trimvalue-25,Trimvalue-5);
  return TrimResp;
}

//====================================================================== MQTT FUNCTIONS ====================================================================================
String Core::MQTTCreateConn(String mqttserver, String port, String clientID, String keepalive, String cleansession, String username, String password)
{
  Serial.print("Creating MQTT Connection to server: ");
  Serial.println(mqttserver);
  String quotes = "\"";
  String MQTTConnCreateCommand = "AT+MQTTCONN=" + quotes + mqttserver + quotes + "," + port + "," + quotes + clientID + quotes + "," + keepalive + "," + cleansession + "," + quotes + username + quotes + "," + quotes + password + quotes; 
  Serial2.println(MQTTConnCreateCommand);
  delay(1000);
  String MQTTConnCreate  = readstringin();
  //Serial.println(MQTTConnCreate);
  Serial.println("Connection created");
  return MQTTConnCreate;
}

String Core::MQTTPublish(String topic, String message, String qos, String duplicate, String retain)
{
  Serial.print("Publishing to Topic:");
  Serial.println(topic);
  Serial.print("Message: ");
  Serial.println(message);
  String quotes = "\"";
  String MQTTPublishCommand = "AT+MQTTPUB=" + quotes + topic + quotes + "," + quotes + message + quotes + "," + qos + "," + duplicate + "," + retain;
  MQTTFallback(MQTTPublishCommand,"OK","Message Publihsed!!","Message Publish failed!!",8000);
  delay(500);
  return MQTTPublishCommand;
   
}

String Core::MQTTSubscribe(String topic, String qos)
{
  Serial.print("Subscribing to Topic:");
  Serial.println(topic);
  String MQTTSubscribeCommand = "AT+MQTTSUBUNSUB=" + quotes + topic + quotes + comma + "1" + comma + qos;
  MQTTFallback(MQTTSubscribeCommand,"OK","Subscription Successfull!","Subscription Unsuccessfull!",5000);
  return "\0"; 
}

String Core::MQTTReception()
{
  String inputString;
  for(;;){
    if (Serial2.available())
    {
      inputString=Serial2.readString();
      Serial.print("MQTTData:");
      //Serial.println(inputString);
      return inputString;
      }    
    }
}


String Core::MQTTDisconnect()
{
  cmd =  "AT+MQTTDISCONN";
  Resp = readstringin();
  Serial.println(Resp);
  return Resp;
}

String Core::MQTTUnsubscribe(String topic, String qos)
{
  cmd = "AT+MQTTSUBUNSUB=" + quotes + topic + quotes + comma + "0";
  MQTTFallback(cmd,"OK","UnSubscription Successfull!","UnSubscription Unsuccessfull!",2000);
  return "\0"; 
}

String Core::MQTTFallback(String cmd,String stringcheck,String posresp,String negresp, int dly)
{ 
  int i=0;int flag=0;
  String TrimMQTTConnPub;
  for(i=0;i<2;i++)
  { 
  Serial2.println(cmd);
  delay(dly);
  String MQTTConnPub = readstringin();
  int MQTTTrim = MQTTConnPub.indexOf('K');
  TrimMQTTConnPub = MQTTConnPub.substring(MQTTTrim-1,MQTTTrim+1);
  if(TrimMQTTConnPub == stringcheck)
  {
    Serial.println(posresp);
    flag=1;
    break;
  }
  else
  {
    Serial.println(negresp);
    Serial.println("PLease check the MQTT connection!!");
  }
  if(flag==1)
  break;
  }
  return TrimMQTTConnPub;
}


//====================================================================== SMS FUNCTIONS =====================================================================================
String Core::checkSMSSender()
{
  Serial.println("Checking SMS Sender number");
  Serial2.println("AT+CSCA?\r\n");
  delay(500);
  String RespCSCA  = readstringin();
  Trimvalue = RespCSCA.indexOf('"');
  TrimResp = RespCSCA.substring(Trimvalue+1,Trimvalue+14);
  return TrimResp;
}

String Core::SMSRead(String index, String storage)
{
  Serial.println("Reading SMS");
  Serial2.println("AT+CMGF=1");
  Serial2.println("AT+CPMS?");
  delay(100);
  String RespCPMS  = readstringin();
  cmd =  "AT+CPMS=" + storage; 
  Serial2.println(cmd); 
  String SMSIndexcmd = "AT+CMGR=" + index;
  Serial2.println(SMSIndexcmd);
  delay(100);
  String RespSMSIndex  = readstringin();
  Serial.println(RespSMSIndex);
  return RespSMSIndex;
}

String Core::SMSSend(String number, String message)
{
  Serial.println("Sending SMS");
  Serial2.println("AT+CMGF=1\r\n");
  delay(100);
  String RespCMGF  = readstringin();
  Serial.println(RespCMGF);
  String CMGScmd = "AT+CMGS=" + quotes + number + quotes;
  while(1)
  {
  Serial2.println(CMGScmd);
  delay(1000);
  String passmessage = message + "\x1a";
  Serial2.println(passmessage);
  delay(100);
  String RespCMGS  = readstringin();
  //Serial.println(RespCMGS);
  Trimvalue = RespCMGS.indexOf('K');
  TrimResp = RespCMGS.substring(Trimvalue-1,Trimvalue+1);
  //Serial.print(TrimResp);
  if(TrimResp == "OK")
  {
    Serial.println("Message send successfull!!");
    break;
  }
  else
  {
    Serial.println("Message send unsuccessfull!!");
  }
  }
  return TrimResp;
}

//====================================================================== NETWORK COMMANDS =================================================================================
String Core::enableEDRX(String setmode, String act_type, String req_value)
{
  String quotes = "\"";
  String comma = ",";
  String CMGScmd = "AT+CEDRXS=" + setmode + comma + act_type + comma + quotes + req_value + quotes;
  Serial2.println(CMGScmd);
  delay(400);
  Resp = readstringin();
  Trimvalue = Resp.indexOf('K');
  TrimResp = Resp.substring(Trimvalue-1,Trimvalue+1);
  if(TrimResp == "OK")
  {
    Serial.println("eDRX is enabled!");
  }
  else
  {
    Serial.println("eDRX enable Unsuccessful!");
  }
  return TrimResp;
}

String Core::disableEDRX()
{
  Serial2.println("AT+CEDRXS=0\r\n");
  delay(100);
  Resp  = readstringin();
  Trimvalue = Resp.indexOf('K');
  TrimResp = Resp.substring(Trimvalue-1,Trimvalue+1);
  if(TrimResp == "OK")
  {
    Serial.println("eDRX is disabled!");
  }
  else
  {
    Serial.println("eDRX disable Unsuccessful!");
  }
  return TrimResp;
}

String Core::enablePSM(String setmode, String cycle_reqtimer, String act_timer)
{
  String quotes = "\"";
  String comma = ",";
  Serial.println("Entering PSM Mode");
  String PSMcmd = "AT+CPSMS=1,,," + quotes + cycle_reqtimer + quotes + comma + quotes + act_timer + quotes;
  Serial2.println(PSMcmd);
  delay(400);
  Resp  = readstringin();
  Trimvalue = Resp.indexOf('K');
  TrimResp = Resp.substring(Trimvalue-1,Trimvalue+1);
  if(TrimResp == "OK")
  {
    Serial.println("PSM is enabled!");
  }
  else
  {
    Serial.println("PSM enable Unsuccessful!");
  }
  return TrimResp;
  return TrimResp;
}

String Core::disablePSM()
{
  Serial2.println("AT+CPSMS=0");
  delay(400);
  String Resp  = readstringin();
  Trimvalue = Resp.indexOf('K');
  TrimResp = Resp.substring(Trimvalue-1,Trimvalue+1);
  if(TrimResp == "OK")
  {
    Serial.println("PSM is disabled!");
  }
  else
  {
    Serial.println("PSM disable Unsuccessful!");
  }
  return TrimResp;
}


String Core::queryOperator()
{
  Serial2.println("AT+COPS?\r\n");
  delay(400);
  Resp  = readstringin();
  Trimvalue = Resp.indexOf('K');
  TrimResp = Resp.substring(Trimvalue-14,Trimvalue-9);
  Serial.println(TrimResp);
  return Resp;
}

String Core::querySignal()
{
  Serial2.println("AT+CSQ");
  delay(400);
  Resp  = readstringin();
  Trimvalue = Resp.indexOf('K');
  TrimResp = Resp.substring(Trimvalue-10,Trimvalue-5);
  Serial.println(TrimResp);
  return TrimResp;
}

String Core::querySignalConn()
{
  Serial2.println("AT+CSCON?");
  delay(400);
  Resp = readstringin();
  Trimvalue = Resp.indexOf('K');
  TrimResp = Resp.substring(Trimvalue-10,Trimvalue-4);
  Serial.println(TrimResp);
  return TrimResp;
}

//========================================================================== CONTROL COMMANDS ===============================================================================================

String Core::setModuleStatus(String modstatus)
{
  String RespModulestatuscmd = "AT+CFUN=" + modstatus; 
  Serial2.println(RespModulestatuscmd);
  delay(100);
  String RespsetModuleStatus = readstringin();
  Serial.println(RespsetModuleStatus);
  return RespsetModuleStatus;
}

String Core::shutdownModule()
{
  String Modulestatuscmd = "AT+CPOF"; 
  Serial2.println(Modulestatuscmd);
  delay(100);
  String RespshutdownModule = readstringin();
  Serial.println(RespshutdownModule);
  return RespshutdownModule;
}

String Core::setModuleSleepLevel(String sleeplevel)
{
  String ModuleSleepLevelcmd = "AT+NVSETPM=" + sleeplevel; 
  Serial2.println(ModuleSleepLevelcmd);
  delay(100);
  String RespsetModuleSleepLevel = readstringin();
  Serial.println(RespsetModuleSleepLevel);
  return RespsetModuleSleepLevel;
}

String Core::setFreqBand(String no, String band)
{
  String comma = ",";
  String setFreqBandcmd = "AT+NVSETBAND=" + no + comma + band; 
  Serial2.println(setFreqBandcmd);
  delay(100);
  String RespsetFreqBand = readstringin();
  Serial.println(RespsetFreqBand);
  return RespsetFreqBand;
}

String Core::hardResetModem()
{
  pinMode(23, OUTPUT);
  Serial.println("Reseting Module!");
  digitalWrite(23, HIGH);
  delay(1000);
  digitalWrite(23,LOW);
  delay(3000);
  Serial2.println("AT");
  delay(100);
  Resp = readstringin();
  String info = getModemInfo();
  Serial.println(info);

}

String Core::intPSM()
{
  pinMode(18, OUTPUT);
  Serial.println("Interrupting PSM Mode");
  digitalWrite(18, HIGH);
  delay(1000);
  digitalWrite(18,LOW);
  delay(3000);
  Serial2.println("AT");
  delay(100);
  Resp = readstringin();
}
//============================================================================= COAP COMMANDS ===============================================================================================
String Core::createCOAPConn(String ip)
{
  String cmd = "AT+NCDPOPEN=" + ip; 
  Serial2.println(cmd);
  delay(100);
  String RespsetFreqBand = readstringin();
  Serial.println(RespsetFreqBand);
  return RespsetFreqBand;
}

String Core::sendCOAPData(String coap_length, String coap_data)
{
  String cmd = "AT+NMGS=" + coap_length + comma + coap_data; 
  Serial2.println(cmd);
  delay(100);
  String Resp = readstringin();
  Serial.println(Resp);
  return Resp;
}

String Core::COAPReception()
{
  Serial2.println("AT+NMGR");
  String inputString;
  for(;;){
    if (Serial2.available())
    {
      inputString=Serial2.readString();
      //Serial.print("COAPData:");
      //Serial.println(inputString);
      return inputString;
      }    
    }
}

String Core::endCOAPConn()
{
  String cmd = "AT+NCDPCLOSE"; 
  Serial2.println(cmd);
  delay(100);
  String Resp = readstringin();
  Serial.println(Resp);
  return Resp;
}

String Core::setCOAPReport(String coap_mode)
{
  String cmd = "AT+NNMI=" + coap_mode; 
  Serial2.println(cmd);
  delay(100);
  String Resp = readstringin();
  Serial.println(Resp);
  return Resp;
}

//========================================================================== UDP COMMANDS ===================================================================================================

String Core::createUDPConn(String type, String protocol,String port, String recv,String socketid)
{
  cmd = "AT+NCDPOPEN=" + quotes + type + quotes + comma + protocol +comma + port + comma + recv + comma +socketid; 
  Serial2.println(cmd);
  delay(100);
  Resp = readstringin();
  Serial.println(Resp);
  return Resp;
}

String Core::sendUDPData(String socketid, String raddr,String rport, String len, String udpdata)
{
  cmd = "AT+NCDPOPEN=" +  socketid + comma + quotes + raddr + quotes +  comma + rport + comma + len + comma + quotes + udpdata + quotes; 
  Serial2.println(cmd);
  delay(100);
  Resp = readstringin();
  Serial.println(Resp);
  return Resp;
}

String Core::recieveUDPData(String socketid, String req_len)
{
  cmd = "AT+NCDPOPEN=" + socketid + comma + req_len; 
  Serial2.println(cmd);
  delay(100);
  Resp = readstringin();
  Serial.println(Resp);
  return Resp;
}

String Core::sendUDPError(String socketid, String req_len)
{
  cmd = "+TSONMI:" + socketid + comma + req_len; 
  Serial2.println(cmd);
  delay(100);
  Resp = readstringin();
  Serial.println(Resp);
  return Resp;
}

String Core::endUDPConn()
{
  cmd = "AT+TSOCL=1"; 
  Serial2.println(cmd);
  delay(100);
  Resp = readstringin();
  Serial.println(Resp);
  return Resp;
}

//========================================================================== TCP COMMANDS ===================================================================================================
String Core::setTCPConn(String tcpmode)
{
  cmd = "AT+CIPMUX=" + tcpmode; 
  Serial2.println(cmd);
  delay(100);
  Resp = readstringin();
  Serial.println(Resp);
  return Resp;
}
String Core::queryIPConn()
{
  cmd = "AT+CIPSTATUS"; 
  Serial2.println(cmd);
  delay(100);
  Resp = readstringin();
  Serial.println(Resp);
  return Resp;
}
String Core::initIPConn(String conmode, String addr, String port)
{
  cmd = "AT+CIPSTART=" + quotes + conmode + quotes + comma + quotes + addr + quotes + comma + port; 
  Serial2.println(cmd);
  delay(100);
  Resp = readstringin();
  Serial.println(Resp);
  return Resp;
}
String Core::sendSingleModeIPdata(String ipdata, String len)
{
  cmd = "AT+CIPSEND" ; 
  Serial2.println(cmd);
  String cmd1 = ipdata + "\x1a";
  Serial2.println(cmd1);
  delay(100);
  Resp = readstringin();
  Serial.println(Resp);
  String cmd2 = "AT+CIPSEND" + len;
  Serial2.println(cmd1);
  delay(100);
  String Resp1 = readstringin();
  Serial.println(Resp1);
  return Resp1;
}

String Core::sendMultiModeIPdata(String ipdata, String len, String conn)
{
  cmd = "AT+CIPSEND" ; 
  Serial2.println(cmd);
  String cmd1 = ipdata + "\x1a";
  Serial2.println(cmd1);
  delay(100);
  Resp = readstringin();
  Serial.println(Resp);
  String cmd2 = "AT+CIPSEND" + conn + comma + len;
  Serial2.println(cmd1);
  delay(100);
  String Resp1 = readstringin();
  Serial.println(Resp1);
  return Resp1;
}
String Core::sendSingleModeIPDataStat()
{
  cmd = "AT+CIPACK"; 
  Serial2.println(cmd);
  delay(100);
  Resp = readstringin();
  Serial.println(Resp);
  return Resp;
}

String Core::sendMultiModeIPDataStat(String conn)
{
  cmd = "AT+CIPACK" + conn; 
  Serial2.println(cmd);
  delay(100);
  Resp = readstringin();
  Serial.println(Resp);
  return Resp;
}
String Core::recieveSingleModeIPData(String recivemode)
{
  cmd = "AT+CIPRXGET=" + recivemode; 
  Serial2.println(cmd);
  delay(100);
  Resp = readstringin();
  Serial.println(Resp);
  return Resp;
}

String Core::recieveIPMultiModeData(String recivemode, String len)
{
  cmd = "AT+CIPRXGET=" + recivemode + comma + len; 
  Serial2.println(cmd);
  delay(100);
  Resp = readstringin();
  Serial.println(Resp);
  return Resp;
}
String Core::endIPConn()
{
  cmd = "AT+CIPCLOSE"; 
  Serial2.println(cmd);
  delay(100);
  Resp = readstringin();
  Serial.println(Resp);
  return Resp;
}
String Core::endNetConn()
{
  cmd = "AT+CIPSHUT"; 
  Serial2.println(cmd);
  delay(100);
  Resp = readstringin();
  Serial.println(Resp);
  return Resp;
}
String Core::setLocalIPPort(String ipmode, String port)
{
  cmd = "AT+CLPORT=" + quotes + ipmode + quotes + comma + port; 
  Serial2.println(cmd);
  delay(100);
  Resp = readstringin();
  Serial.println(Resp);
  return Resp;
}

String Core::setSendPrompt(String promptmode)
{
  cmd = "AT+CIPSPRT=" + promptmode; 
  Serial2.println(cmd);
  delay(100);
  Resp = readstringin();
  Serial.println(Resp);
  return Resp;
}

String Core::setRecieverHeader(String recievemode)
{
  cmd = "AT+CIPHEAD=" + recievemode; 
  Serial2.println(cmd);
  delay(100);
  Resp = readstringin();
  Serial.println(Resp);
  return Resp;
}

String Core::setRecieverDisplay(String dispalymode)
{
  cmd = "AT+CIPSHOWTP=" + dispalymode; 
  Serial2.println(cmd);
  delay(100);
  Resp = readstringin();
  Serial.println(Resp);
  return Resp;
}
String Core::setDispalyIP(String displayaddrmode)
{
  cmd = "AT+CIPSRIP=" + displayaddrmode; 
  Serial2.println(cmd);
  delay(100);
  Resp = readstringin();
  Serial.println(Resp);
  return Resp;
}
String Core::queryTransParam()
{
  cmd = "AT+CIPCCFG?";
  Serial2.println(cmd);
  delay(100);
  Resp = readstringin();
  Serial.println(Resp);
  return Resp;
}

String Core::setTCPHeartBeatParam()
{
  cmd = "AT+CIPTKA?";
  Serial2.println(cmd);
  delay(100);
  Resp = readstringin();
  Serial.println(Resp);
  return Resp;
}
//==================================================================================  HTTP Service  =========================================================================================
String Core::initHTTPService()
{
  cmd = "AT+HTTPINIT";
  Serial2.println(cmd);
  delay(100);
  Resp = readstringin();
  Serial.println(Resp);
  return Resp;
}

String Core::terminateHTTPService()
{
  cmd = "AT+HTTPTERM";
  Serial2.println(cmd);
  delay(100);
  Resp = readstringin();
  Serial.println(Resp);
  return Resp;
}

String Core::setHTTPParam(String tag, String value)
{
  cmd = "AT+HTTPPARA=" + quotes + tag + quotes + comma + quotes + value + quotes;
  Serial2.println(cmd);
  delay(100);
  Resp = readstringin();
  Serial.println(Resp);
  return Resp;
}

String Core::inputHTTPData(String httpdata)
{
  cmd = "AT+HTTPDATA"; 
  Serial2.println(cmd);
  String cmd1 = httpdata + "\x1a";
  Serial2.println(cmd1);
  delay(100);
  Resp = readstringin();
  Serial.println(Resp);
  return Resp;
}

String Core::setHTTPSCertifacte(String flag, String certificate)
{
  cmd = "AT+ HTTPSSETCRT=" + flag;
  Serial2.println(cmd);
  String cmd1 = certificate + "\x1a";
  Serial2.println(cmd1);
  delay(100);
  Resp = readstringin();
  Serial.println(Resp);
  return Resp;
}

String Core::setHTTPMethod(String flag)
{
  cmd = "AT+HTTPACTION=" + flag;
  Serial2.println(cmd);
  delay(100);
  Resp = readstringin();
  Serial.println(Resp);
  return Resp;
}

String Core::readHTTPServerResp(String addr, String bytesize)
{
  cmd = "AT+HTTPREAD=" + quotes + addr + quotes + comma + quotes + bytesize + quotes;
  Serial2.println(cmd);
  delay(100);
  Resp = readstringin();
  Serial.println(Resp);
  return Resp;
}

String Core::readHTTPStatus()
{
  cmd = "AT+HTTPSTATUS";
  Serial2.println(cmd);
  delay(100);
  Resp = readstringin();
  Serial.println(Resp);
  return Resp;
}

String Core::getHTTP()
{
  cmd = "AT+HTTPGET";
  Serial2.println(cmd);
  delay(100);
  Resp = readstringin();
  Serial.println(Resp);
  return Resp;
}

String Core::downloadFilesHTTP()
{
  cmd = "AT+HTTPDOWNLOAD";
  Serial2.println(cmd);
  delay(100);
  Resp = readstringin();
  Serial.println(Resp);
  return Resp;
}
//==================================================================================  HTTP Commands set2  ===================================================================================
String Core::initHTTP2Service()
{
  cmd = "AT+INITHTTP";
  Serial2.println(cmd);
  delay(100);
  Resp = readstringin();
  Serial.println(Resp);
  return Resp;
}

String Core::terminateHTTP2Service()
{
  cmd = "AT+TERMHTTP";
  Serial2.println(cmd);
  delay(100);
  Resp = readstringin();
  Serial.println(Resp);
  return Resp;
}


String Core::setHTTPAuthority(String url, String username, String password)
{
  cmd = "AT+HTTPAUTHOR=" + quotes + url + quotes + comma + quotes + username + quotes + comma + quotes + password + quotes;
  Serial2.println(cmd);
  delay(100);
  Resp = readstringin();
  Serial.println(Resp);
  return Resp;
}

String Core::postdataHTTP(String url, String contenttype, String bodycontent)
{
  cmd = "AT+HTTPPOST=" + quotes + url + quotes + comma + quotes + contenttype + quotes + comma + quotes + bodycontent + quotes;
  Serial2.println(cmd);
  delay(100);
  Resp = readstringin();
  Serial.println(Resp);
  return Resp;
}

String Core::putdataHTTP(String url, String contenttype, String contentname, String bodycontent)
{
  cmd = "AT+HTTPPOST=" + quotes + url + quotes + comma + quotes + contenttype + quotes + comma + quotes + contentname + quotes + comma + quotes + bodycontent + quotes; 
  Serial2.println(cmd);
  delay(100);
  Resp = readstringin();
  Serial.println(Resp);
  return Resp;
}

String Core::readHTTPheaderinfo(String url)
{
  cmd = "AT+HTTPHEAD=" + quotes + url + quotes;
  Serial2.println(cmd);
  delay(100);
  Resp = readstringin();
  Serial.println(Resp);
  return Resp;
}

String Core::queryHTTPMethods(String url)
{
  cmd = "AT+HTTPOPTIONS =" + quotes + url + quotes;
  Serial2.println(cmd);
  delay(100);
  Resp = readstringin();
  Serial.println(Resp);
  return Resp;
}


String Core::getHTTPServerPath(String url)
{
  cmd = "AT+HTTPTRACE=" + quotes + url + quotes;
  Serial2.println(cmd);
  delay(100);
  Resp = readstringin();
  Serial.println(Resp);
  return Resp;
}

String Core::deleteHTTPRes(String url, String file)
{
  cmd = "AT+HTTPDELETE=" + quotes + url + quotes + comma + quotes + file + quotes;
  Serial2.println(cmd);
  delay(100);
  Resp = readstringin();
  Serial.println(Resp);
  return Resp;
}

//==================================================================================  END  ==================================================================================================
