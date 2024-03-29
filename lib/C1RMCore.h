#ifndef t1
#define t1

#if (ARDUINO >=100)
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

struct CS_MODEM_RES {
	unsigned char status;
	String data;
	String temp;
};

struct radio {
	String csq;
	String rssi;
	String ber;
};

struct ping {
	bool status;
	String addr;
	String stats;
};

class Network {
  public:
  // Public API Functions starts here.
    void SerialInit(); // Modem Serial Comm Initiate. Mandatory function to be called before all other API functions.
    
  // Network Function Method
    bool radioEnable (bool enable); // True - Enable Cellular Radio, False - Disable Cellular Radio. 
    bool networkAttach (bool enable); // True - Attach to Cellular Network, False - Dettach from Cellular Service.
    bool setNetworkAuto ();
    bool prefRadioPriority (int net_type); // Change network priority for radio scanning. 1 - Preffered NB-IoT, 2 - Preffered EGPRS.
    bool prefRadioMode (int mode); // 1 - Single mode 2 - Dual Mode. Single or Dual Radio with network preference taken from the prefRadioPriority.
    bool setPDN(int ipType, String apn); // IP Type: 1 - IPV4, 2 - IPV6, 3 - IPV4V6, 5 - No IP (NB-IoT Paging).
    bool enablePacketData (bool enable); // True - Attach to Data Service, False - Dettach from Data Service.
    bool setDNSAddr(String primaryDNS, String secondaryDNS); // Manual configuration for Primary and Secondary DNS IP Address.
    bool enablePSM (bool enable, String interval, String start_timer); // True - Module enter in to Power Save Mode, False - Power Save Mode Disabled.
    bool enableEDRX (String mode, String interval, String edrx_val); // Mode 0 - Disable eDRX, 1 - Enable eDRX, 2 - Enable eDRX with unsolicited result code.
    bool reboot(); // Reboots the Modem. This is a cold reboot.
    bool shutdown(); // Modem will be powered down. Toggle RESET PIN externally to Switch ON.
    void modemReset(); // Modem will go for Hard Reset. The same function can be used to Switch ON Modem after a Shutdown or as warm reboot.
    bool modemWakeUp(); // Wake Up the modem from Sleep Mode.

  // Get Info/Status methods.
    bool isModemAvailable(); // Returns true if a valid modem response is received.
    bool isESIMReady(); // Returns true if the eSIM is ready for registration.
    bool isNetworkAttached(); // Returns true if the modem is registered to the network.
    radio getRadioQuality(); // Displays the Radio Signal Strength.
    String getModemInfo(); // Displays details Modem Information.
    String getIMEI(); // Displays IMEI Number.
    String getICCID(); // Displays ICCID of the active SIM Card.
    String getIMSI(); // Displays IMSI of the active SIM Card.
    String getNetTime(); // Dsipalys the time received from the network.
    String getDefaultPDN(); // Dsiplays default PDN configured.
    bool getPacketDataStatus(); // Displays Packet Data status.
    // String getPSMStatus(); // Displays current Power Save Mode Status.
    // String getLowPowerStatus(); // Displays current Low Power Mode (eDRX Mode) Status.
    String getDNSAddr(); // Get current active DNS Addresses.
    String getIPAddr(); // Get the current modem IP address.
    ping getPingStatus(String hostname); // Ping to a hostname and get the ping quality report.
    String getSMSCenterNumber(); // Displays the SMS Center Number from the SIM Card.
    String getNetworkOperator(); // Displays the currently attached Service Provider Name.
    
  // SMS methods
    String readSMS(String index, String storageType); // Prints the SMS received and stored in a specific strorage (SIM or Modem Flash) with the index number.
    bool sendSMS(String destNumber, String message); // Sends SMS to the given Destination Number.

  // MQTT methods
    bool createMQTT(String mqttserver, String port, String clientID, String keepalive, String cleansession, String username, String password); // Create a new MQTT connection to the specified hostname and credintials.
    bool publishMQTT(String topic, String message, String qos, String duplicate, String retain); // Publish MQTT Message to a specified topic unding the previously created MQTT connection.
    bool subscribeMQTT(String topic, String qos, bool enable); // Subcribe to a MQTT Topic. If enable is false, the topic will be unsubscribed from the connection.
    bool disconnectMQTT(); // Disconnect from the created MQTT server.

  // CoAP methods
  bool createCoAP(String coapserver);                // Create new CoAP connection.
  bool sendCoAPData(String data, String dataLength); // Send data to CoAP server.
  void receiveCoAPData(String enable);               // If enable is true, inidicates incoming message notifications and the received message.
  void closeCoAP();                                  // Close CoAP server connection.

  // HTTP/HTTPS methods

  // TCP IP methods
  bool initSingleTCP(String ipmode, String addr, int port);            //Initialize single TCP connection,Set Mode- "TCP"/"UDP", IP address, Port
  bool initMultiTCP(int connum, String ipmode, String addr, int port); //Initialize Multi TCP connection, Set Transmission mode:0-AT mode 1-Transperent mode, Connection number- 0-7, Mode- "TCP"/"UDP", IP address, Port
  bool sendTCPData(int mode, int num, String message);                 //Sends TCP Data, Mode: 0 -Single mode 1- Multimode, Connection number - 0-7, Input message to be sent
  String recieveTCPData(int mode, int num);                            //Recieves TCP Data, Mode: 0 -Single mode 1- Multimode, Connection number - 0-7, Recieved message is returned to the fucntion
  void closeTCP(int mode, int connum);                                 //Close TCP Connection Mode: 0 -Single mode 1- Multimode, Connection number - 0-7
  void shutdownTCP();                                                  //Shutdown TCP connection

  // UDP methods
  bool creatUDP(int port, int recievechar);                                     //Creates UDP socket connection, set port - 0-65635 (except 5683), recievechar: 0 - disable data reception 1 - enable data reception,   
  bool sendUDP(int socketid, String raddr, int rport, int length, String data); //Sends UDP data, set socket id, raddr - IP address of the other party, rport - Port of the other party, length - Pending packet length, data - Packet, data can be HEX format or quoted string
  String recieveUDP(int socketid, int reqlength);                               //Recieves UDP data, set socket id, reqlength - Maximum receiving data
  void endUDP();                                                                //end UDP socket connection
  
  private:
    CS_MODEM_RES serial_res(long timeout, String chk_string);
};

#endif