#include "C1RMCore.h"

// HardwareSerial Serial2(2);
String comma = ",";
String quotes = "\"";

Network::Network(bool displayMsg){
  //Anything yoy need when initiating object goes here
}

void Network::SerialInit() {
  Serial2.begin(57600, SERIAL_8N1, 13, 15);
}

bool Network::radioEnable (bool enable) { // True - Enable Cellular Radio, False - Disable Cellular Radio.
	Serial2.print("AT+CFUN=");
	if (enable) {
   		Serial2.println("1");
 	} else {
   		Serial2.println("0");
 	}
	CS_MODEM_RES res = serial_res(500,F("OK"));
	return(res.status);
}


bool Network::networkAttach (bool enable) { // True - Attach to Cellular Network, False - Dettach from Cellular Service.
	Serial2.print("AT+CREG=");
	if (enable) {
   		Serial2.println("1");
 	} else {
   		Serial2.println("0");
 	}
	CS_MODEM_RES res = serial_res(500,F("OK"));
	return(res.status);
}

bool Network::setPDN(int ipType, String apn) { // IP Type: 1 - IPV4, 2 - IPV6, 3 - IPV4V6, 5 - No IP (NB-IoT Paging).
	Serial2.print("AT+CFGDFTPDN=");
	Serial2.println(ipType + comma + quotes + apn + quotes);
	CS_MODEM_RES res = serial_res(500,F("OK"));
	return(res.status);
}

bool Network::enablePacketData (bool enable) { // True - Attach to Data Service, False - Dettach from Data Service.
	Serial2.print("AT+CGACT=");
	if (enable) {
   		Serial2.println("1,1");
 	} else {
   		Serial2.println("0");
 	}
	CS_MODEM_RES res = serial_res(500,F("OK"));
	return(res.status);
}

bool Network::setDNSAddr(String primaryDNS, String secondaryDNS) { // Manual configuration for Primary and Secondary DNS IP Address.
	Serial2.print("AT+CDNSCFG=");
	Serial2.println(primaryDNS + comma + secondaryDNS);
	CS_MODEM_RES res = serial_res(500,F("OK"));
	return(res.status);
}

bool Network::enablePSM (bool enable, String interval, String start_timer) { // True - Module enter in to Power Save Mode, False - Power Save Mode Disabled.
	Serial2.print("AT+CPSMS=");
	if (enable) {
   		Serial2.println("1,,," + quotes + interval + quotes + comma + quotes + start_timer + quotes);
 	} else {
   		Serial2.println("0");
 	}
	CS_MODEM_RES res = serial_res(500,F("OK"));
	return(res.status);
}

bool Network::enableEDRX (String mode, String network_type, String edrx_val) { // Mode 0 - Disable eDRX, 1 - Enable eDRX, 2 - Enable eDRX with unsolicited result code.
	Serial2.print("AT+CEDRXS=");
   	Serial2.println(mode + comma + network_type + comma + quotes + edrx_val + quotes);
	CS_MODEM_RES res = serial_res(500,F("OK"));
	return(res.status);
}

bool Network::reboot() { // Reboots the Modem. This is cold reboot.
	Serial2.println("AT+TRB");
	CS_MODEM_RES res = serial_res(500,F("REBOOTING"));
	return(res.status);
}

bool Network::shutdown() { // Modem will be powered down. Toggle RESET PIN externally to Switch ON.
	Serial2.println("AT+CPOF");
	CS_MODEM_RES res = serial_res(500,F("OFF"));
	return(res.status);
}

bool Network::modemReset() { // Modem will go for Hard Reset. The same function can be used to Switch ON Modem after a Shutdown or as warm reboot.
// To be completed.
}

// Get Functions.

bool Network::isModemAvailable() { // Returns true if a valid modem response is received.
	Serial2.println("AT");
	CS_MODEM_RES res = serial_res(500,F("OK"));
	return(res.status);
}

bool Network::isNetworkAttached() { // Returns true if the modem is registered to the network.
	Serial2.println("AT+CREG?");
	CS_MODEM_RES res = serial_res(500,F("+CREG"));
	String tmp;
	bool netStat;
	String data;
	if(res.status) {
		data = res.data;
		if(res.data.indexOf(F("+CREG"))!=-1) {
			int first = res.data.indexOf(F(":"));
			int last = res.data.indexOf(F(","));
			tmp = res.data.substring(first+2,last+2);
		} 
		if (tmp == F("1,1") || tmp == F("1,5")) {
			netStat = true;
		} else {
			netStat = false;
		}
	}
	res = serial_res(500,F("OK"));
	return(netStat);
}

String Network::getModemInfo () {
	Serial2.println(F("ATI"));
	CS_MODEM_RES res = serial_res(500,F("OK"));
	String out = res.temp;
    out.replace(F("OK"),"");
	out = out.substring(0,out.length());
	res = serial_res(500,F("OK"));
	return (out);
}

String Network::getIMEI() { // Displays IMEI Number.
	Serial2.println("AT+CGSN");
	CS_MODEM_RES res = serial_res(500,F("OK"));
	String out = res.temp;
    out.replace(F("OK"),"");
	out = out.substring(0,out.length());
	res = serial_res(500,F("OK"));
	return (out);
}

String Network::getICCID() { // Displays ICCID of the active SIM Card.
	Serial2.println("AT+CCID");
	CS_MODEM_RES res = serial_res(500,F("OK"));
	String out = res.temp;
    out.replace(F("OK"),"");
	out = out.substring(0,out.length());
	res = serial_res(500,F("OK"));
	return (out);
}

String Network::getIMSI() { // Displays IMSI of the active SIM Card.
	Serial2.println("AT+CIMI");
	CS_MODEM_RES res = serial_res(500,F("OK"));
	String out = res.temp;
    out.replace(F("OK"),"");
	out = out.substring(0,out.length());
	res = serial_res(500,F("OK"));
	return (out);
}

String Network::getNetTime() { // Dsipalys the time received from the network.
// To be reviewed.
}

radio Network::getRadioQuality() {
  Serial2.println(F("AT+CSQ"));
	CS_MODEM_RES res = serial_res(500,F("+CSQ"));
	Serial.println(res.data);
	radio sig;
	int x = 0;
	String tmp;
	if(res.status) {
		if(res.data.indexOf(F("+CSQ"))!=-1) {
			int index = res.data.indexOf(F(":"));
			int index2 = res.data.indexOf(F(","));
			tmp = res.data.substring(index+1,index2);
			if (tmp == F("99")) {
				sig.csq = F("N/A");
				sig.rssi = F("N/A");
			} else {
				sig.csq = tmp;
				x = tmp.toInt();
				x = (2*x)-113;
				sig.rssi = String(x);
			}
			sig.ber  = res.data.substring(index2+1);
		}
	}
	res = serial_res(500,F("OK"));
	return(sig);
}

String Network::getDefaultPDN() { // Dsiplays default PDN configured.
	Serial2.println(F("AT+CFGDFTPDN?"));
	CS_MODEM_RES res = serial_res(500,F("+CFGDFTPDN"));
	String tmp;
	if(res.status) {
		if(res.data.indexOf(F("+CFGDFTPDN"))!=-1) {
			int type_first = res.data.indexOf(F("="));
			int type_last = res.data.indexOf(F(";"));
			tmp = res.data.substring(type_first+1,type_last);
			tmp = tmp+",apn=";
			int pdn_first = res.data.indexOf(tmp);
			int pdn_last = res.data.indexOf(F(";"),pdn_first+1);
			tmp = res.data.substring(pdn_first+tmp.length(),pdn_last);
		}
	}
	res = serial_res(500,F("OK"));
	return (tmp);
}

ping Network::getPingStatus(String hostname) { // Ping to a hostname and get the ping quality report.
	ping pingr;
	String data = "";
	Serial2.print(F("AT+PING="));
	Serial2.println(quotes + hostname + quotes);
	CS_MODEM_RES res = serial_res(10000,F("OK"));
	if(res.status) {
		data = res.data;
		Serial.println(data);
		int index = data.indexOf(F(":"));
		int index2 = data.indexOf(F(","));
		int index3 = data.indexOf(F(","),index2+1);
		pingr.status = true;
		pingr.addr = data.substring(index+1,index2);
		pingr.ttl = data.substring(index2+1,index3);
		pingr.rtt = data.substring(index3+1,data.length());
		Serial.println("# Ping IP:"+pingr.addr + ",ttl= " + pingr.ttl + ",rtt= " + pingr.rtt);

	}else {Serial.println("# Ping Failed");}
	res = serial_res(500,F("OK"));
	return pingr;
}

bool Network::sendSMS(String destNumber, String message) // Sends SMS to the given Destination Number.
{
	Serial2.println("AT+CMGF=1");
	CS_MODEM_RES res = serial_res(500,F("OK"));
	Serial2.print(F("AT+CMGS="));
	Serial2.println(quotes + destNumber + quotes);
	Serial2.println(message + "\x1a");
	res = serial_res(20000,F("OK"));
	Serial.println(res.data);
	return res.status;
}


String Network::readSMS(String index, String storageType) // Prints the SMS received and stored in a specific strorage (SIM or Modem Flash) with the index number.
{
	Serial2.print(F("AT+CPMS="));
	Serial2.println(quotes + storageType + quotes);
	CS_MODEM_RES res = serial_res(10000,F("OK"));
	Serial2.print(F("AT+CMGR="));
	Serial2.println(index);
	res = serial_res(10000,F("OK"));
	String out = res.temp;
    out.replace(F("OK"),"");
	out = out.substring(0,out.length());
	Serial.println(out);
	return out;
}

bool Network::createMQTT(String mqttserver, String port, String clientID, String keepalive, String cleansession, String username, String password) // Create a new MQTT connection to the specified hostname and credintials.
{
	Serial2.print(F("AT+MQTTCONN="));
	Serial2.println(quotes + mqttserver + quotes + comma + port + comma + quotes + clientID + quotes + comma + keepalive + comma + cleansession + comma + quotes + username + quotes + comma + quotes + password + quotes);
	CS_MODEM_RES res = serial_res(10000,F("OK"));
	return res.status;
}

bool Network::publishMQTT(String topic, String message, String qos, String duplicate, String retain) // Publish MQTT Message to a specified topic unding the previously created MQTT connection.
{
	Serial2.print(F("AT+MQTTPUB="));
	Serial2.println(quotes + topic + quotes + comma +  quotes + message + quotes + comma + qos + comma + duplicate + comma + retain);
	CS_MODEM_RES res = serial_res(10000,F("OK"));
	return res.status;
}

bool Network::subscribeMQTT(String topic, String qos, bool enable) // Subcribe to a MQTT Topic. If enable is false, the topic will be unsubscribed from the connection.
{
	Serial2.print(F("AT+MQTTSUBUNSUB="));
	if(enable)
	{
	Serial2.println(quotes + topic + quotes + comma + "1" + comma + qos);
	}
	else
	{
	Serial2.println(quotes + topic + quotes + comma + "0");
	}
	CS_MODEM_RES res = serial_res(10000,F("OK"));
	return res.status;
}

bool Network::disconnectMQTT() // Disconnect from the created MQTT server.
{
	Serial2.println("AT+MQTTDISCONN");
	CS_MODEM_RES res = serial_res(500,F("OK"));
	return res.status;
}

bool Network::createCoAP(String coapserver) // Create new CoAP connection.
{
	Serial2.print(F("AT+NCDPOPEN="));
	Serial2.println(quotes + coapserver + quotes);
	CS_MODEM_RES res = serial_res(500,F("OK"));
	return res.status;
}

bool Network::sendCoAPData(String data, String dataLength) // Send data to CoAP server.
{
	Serial2.print(F("AT+NMGS="));
	Serial2.println(data + comma + dataLength);
	CS_MODEM_RES res = serial_res(500,F("OK"));
	return res.status;
}


void Network::receiveCoAPData(String enable)
{
	Serial2.println("AT+NMGR");
	CS_MODEM_RES res = serial_res(500,F("OK"));
	res = serial_res(10000,F("OK"));
	String out = res.temp;
    out.replace(F("OK"),"");
	out = out.substring(0,out.length());
}

void Network::closeCoAP()
{
	Serial2.print(F("AT+NCDPCLOSE"));
	CS_MODEM_RES res = serial_res(500,F("OK"));
}

CS_MODEM_RES Network::serial_res(long timeout,String chk_string) {
	unsigned long pv_ok = millis();
	unsigned long current_ok = millis();
	String input;
	unsigned char until=1;
	unsigned char res=-1;
	CS_MODEM_RES res_;
	res_.temp="";
	res_.data = "";

	while(until) {
		if(Serial2.available()) {
			input = Serial2.readStringUntil('\n');
			res_.temp+=input;
			if(input.indexOf(chk_string)!=-1) {
				res=1;
				until=0;
			} else if(input.indexOf(F("ERROR"))!=-1) {
				res=0;
				until=0;
			}
		}
		current_ok = millis();
		if (current_ok - pv_ok>= timeout) {
			until=0;
			res=0;
			pv_ok = current_ok;
		}
	}
	res_.status = res;
	res_.data = input;
	return(res_);
}









