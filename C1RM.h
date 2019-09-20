#ifndef t1
#define t1

#if (ARDUINO >=100)
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif


class Network {
  public:
    Response(bool displayMsg=false);
    
    // Network Function Method
    void radioEnable (bool enable); // True - Enable Cellular Radio, False - Disable Cellular Radio. 
    void networkAttach (bool enable); // True - Attach to Cellular Network, False - Dettach from Cellular Service.
    void setPDN(int ipType, String apn); // IP Type: 1 - IPV4, 2 - IPV6, 3 - IPV4V6, 5 - No IP (NB-IoT Paging).
    void packetData (bool enable); // True - Attach to Data Service, False - Dettach from Data Service.
    void setDNSAddr(String primaryDNS, String secondaryDNS); // Manual configuration for Primary and Secondary DNS IP Address.
    void setPSM (bool enable, String interval); // True - Module enter in to Power Save Mode, False - Power Save Mode Disabled.
    void setLowPower (bool enable, String interval); // True - Enable eDRX with specified timer, False - Disables eDRX.
    void reboot(); // Reboots the Modem.
    void shutdown(); // Modem will be pwered down. Toggle RESET PIN from the controller to Switch ON.
    void modemReset(); // Modem will go for Hard Reset. The same function can be used to Switch ON Modem after a Shutdown.

    // Get Info methods
    String getModemInfo(); // Displays details Modem Information.
    String getSerialNumber(); // Displays Serial Number.
    String getIMEI(); // Displays IMEI Number.
    String getICCID(); // Displays ICCID of the active SIM Card.
    String getIMSI(); // Displays IMSI of the active SIM Card.
    String getNetTime(); // Dsipalys the time received from the network.
    String getSignalStrength(); // Displays the Radio Signal Strength.
    String getRadioStatus(); // Inform the current Radio mode status.
    String getNetworkStatus(); // Displays current network registration status.
    String getDefaultPDN(); // Dsiplays default PDN configured.
    String getPacketDataStatus(); // Displays Packet Data status.
    String getPSMStatus(); // Displays current Power Save Mode Status.
    String getLowPowerStatus(); // Displays current Low Power Mode (eDRX Mode) Status.
    String getDNSAddr(); // Get current active DNS Addresses.
    String getIPAddr(); // Get the current modem IP address.
    String getPingStatus(String hostname, String pingCount); // Ping to a hostname and get the ping quality report.
    String getSMSCenterNumber(); // Displays the SMS Center Number from the SIM Card.

    // SMS methods
    String readSMS(String index, String storageType); // Prints the SMS received and stored in a specific strorage (SIM or Modem Flash) with the index number.
    bool sendSMS(String destNumber, String message); // Sends SMS to the given Destination Number.

    // MQTT methods
    bool createMQTT(String mqttserver, String port, String clientID, String keepalive, String cleansession, String username, String password); // Create a new MQTT connection to the specified hostname and credintials.
    bool publishMQTT(String topic, String message, String qos, String duplicate, String retain); // Publish MQTT Message to a specified topic unding the previously created MQTT connection.
    void subscribeMQTT(String topic, String qos, String enable); // Subcribe to a MQTT Topic. If enable is false, the topic will be unsubscribed from the connection.
    void disconnectMQTT(); // Disconnect from the created MQTT server.

    // CoAP methods
    bool createCoAP(String coapserver); // Create new CoAP connection.
    bool sendCoAPData(String data, String dataLength); // Send data to CoAP server.
    void receiveCoAPData(String enable); // If enable is true, inidicates incoming message notifications and the received message.
    void closeCoAP(); // Close CoAP server connection.


    // HTTP/HTTPS methods


    // TCP IP methods


    // UDP methods







  String readstringin();
  String CheckTrimResponse(String TrimResponse,String stringcheck,String posresp,String negresp);
  String callBackResponse(String cmd,String stringcheck,String posresp,String negresp);
  String MQTTFallback(String cmd,String stringcheck,String posresp,String negresp,int dly);
  void TaskCreatepinnedCore();
  void serialBegin();
  String getModemInfo();
  String queryTime();
  String queryIMEI();
  String queryModuleModel();
  String queryManufacture();
  String getNetConnection(String type, String pdptype, String apn);
  String checkSignalStrength();
  String checkNetworkRegistraion();
  String configurePDP(String type, String pdptype, String apn);
  String checkDataAttachement();
  String ping(String ipaddr);
  String queryIMSI();
  String queryICCID();  
  String configureDNS(String prim, String sec);
  String domainNameRes(String domain);
  String MQTTCreateConn(String mqttserver, String port, String clientID, String keepalive, String cleansession, String username, String password);
  String MQTTPublish(String topic, String message, String qos, String duplicate, String retain);
  String MQTTSubscribe(String topic, String qos);
  String MQTTReception();
  String MQTTDisconnect();
  String MQTTUnsubscribe(String topic, String qos);
  String checkSMSSender();
  String SMSRead(String index, String storage);
  String SMSSend(String number, String message);
  String enableEDRX(String setmode, String act_type, String req_value);
  String disableEDRX();
  String enablePSM(String setmode, String cycle_reqtimer, String act_timer);
  String disablePSM();
  String queryOperator();
  String querySignal();
  String querySignalConn();
  String setModuleStatus(String modstatus);
  String shutdownModule();
  String setModuleSleepLevel(String sleeplevel);
  String setFreqBand(String no, String band);
  String hardResetModem();
  String intPSM();
  String createCOAPConn(String ip);
  String sendCOAPData(String coap_length, String coap_data);
  String COAPReception();
  String endCOAPConn();
  String setCOAPReport(String coap_mode);
  String createUDPConn(String type, String protocol,String port, String recv,String socketid);
  String sendUDPData(String socketid, String raddr,String rport, String len, String udpdata);
  String recieveUDPData(String socketid, String req_len);
  String sendUDPError(String socketid, String req_len);
  String endUDPConn();
  String setTCPConn(String tcpmode);
  String queryIPConn();
  String initIPConn(String conmode, String addr, String port);
  String sendSingleModeIPdata(String ipdata, String len);
  String sendMultiModeIPdata(String ipdata, String len, String conn);
  String sendSingleModeIPDataStat();
  String sendMultiModeIPDataStat(String conn);
  String recieveSingleModeIPData(String recivemode);
  String recieveIPMultiModeData(String recivemode, String len);
  String endIPConn();
  String endNetConn();
  String setLocalIPPort(String ipmode, String port);
  String setSendPrompt(String promptmode);
  String setRecieverHeader(String recievemode);
  String setRecieverDisplay(String dispalymode);
  String setDispalyIP(String displayaddrmode);
  String queryTransParam();
  String setTCPHeartBeatParam();  
  String initHTTPService();
  String terminateHTTPService();
  String setHTTPParam(String tag, String value);
  String postdataHTTP(String url, String contenttype, String bodycontent);
  String inputHTTPData(String httpdata);
  String setHTTPSCertifacte(String flag, String certificate);
  String setHTTPMethod(String flag);
  String readHTTPServerResp(String addr, String bytesize);
  String readHTTPStatus();
  String getHTTP();
  String downloadFilesHTTP();
  String initHTTP2Service();
  String terminateHTTP2Service();
  String setHTTPAuthority(String url, String username, String password);
  String putdataHTTP(String url, String contenttype, String contentname, String bodycontent);
  String readHTTPheaderinfo(String url);
  String queryHTTPMethods(String url);
  String getHTTPServerPath(String url);
  String deleteHTTPRes(String url, String file);
  
  private:
};



#endif
