/*===================================================================== CAVLI WIRELESS P32 Firmware v1 ========================================================================*/
#ifndef t1
#define t1

#if (ARDUINO >=100)
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif


class Network {
  public:
  Network(bool displayMsg=false);
    
    // Network Function Method
    void radioEnable (bool enable); // True - Enable Cellular Radio, False - Disable Cellular Radio. 
    void networkAttach (bool enable); // True - Attach to Cellular Network, False - Dettach from Cellular Service.
    void setPDN(int ipType, String apn); // IP Type: 1 - IPV4, 2 - IPV6, 3 - IPV4V6, 5 - No IP (NB-IoT Paging).
    // void packetData (bool enable); // True - Attach to Data Service, False - Dettach from Data Service.
    // void setDNSAddr(String primaryDNS, String secondaryDNS); // Manual configuration for Primary and Secondary DNS IP Address.
    // void setPSM (bool enable, String interval); // True - Module enter in to Power Save Mode, False - Power Save Mode Disabled.
    // void setLowPower (bool enable, String interval); // True - Enable eDRX with specified timer, False - Disables eDRX.
    // void reboot(); // Reboots the Modem.
    // void shutdown(); // Modem will be pwered down. Toggle RESET PIN from the controller to Switch ON.
    // void modemReset(); // Modem will go for Hard Reset. The same function can be used to Switch ON Modem after a Shutdown.


        // Get Info methods
    String getModemInfo(); // Displays details Modem Information.
    // String getSerialNumber(); // Displays Serial Number.
    // String getIMEI(); // Displays IMEI Number.
    // String getICCID(); // Displays ICCID of the active SIM Card.
    // String getIMSI(); // Displays IMSI of the active SIM Card.
    // String getNetTime(); // Dsipalys the time received from the network.
    String getSignalStrength(); // Displays the Radio Signal Strength.
    String getRadioStatus(); // Inform the current Radio mode status.
    String getNetworkStatus(); // Displays current network registration status.
    // String getDefaultPDN(); // Dsiplays default PDN configured.
    // String getPacketDataStatus(); // Displays Packet Data status.
    // String getPSMStatus(); // Displays current Power Save Mode Status.
    // String getLowPowerStatus(); // Displays current Low Power Mode (eDRX Mode) Status.
    // String getDNSAddr(); // Get current active DNS Addresses.
    // String getIPAddr(); // Get the current modem IP address.
    // String getPingStatus(String hostname, String pingCount); // Ping to a hostname and get the ping quality report.
    // String getSMSCenterNumber(); // Displays the SMS Center Number from the SIM Card.
  
  private:
};



#endif
