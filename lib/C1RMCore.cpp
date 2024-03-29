#include "C1RMCore.h"

//HardwareSerial Serial2(2);
String comma = ",";
String quotes = "\"";

void Network::SerialInit() { // Modem Serial Comm Initiate. Mandatory function to be called before all other API functions.
  Serial2.begin(57600, SERIAL_8N1, 27, 26);
}

bool Network::radioEnable (bool enable) { // True - Enable Cellular Radio, False - Disable Cellular Radio.
	if(isModemAvailable()) {
		Serial2.print("AT+CFUN=");
		if (enable) {
			Serial2.println("1");
		} else {
			Serial2.println("0");
		}
		CS_MODEM_RES res = serial_res(500,F("OK"));
		return(res.status);
	} else {
		Serial.println("Modem is Unavailable or Busy");
		return(false);
	}
}


bool Network::networkAttach (bool enable) { // True - Attach to Cellular Network, False - Dettach from Cellular Service.
	if(isModemAvailable()) {
		Serial2.print("AT+CREG=");
		if (enable) {
			Serial2.println("1");
		} else {
			Serial2.println("0");
		}
		CS_MODEM_RES res = serial_res(500,F("OK"));
		return(res.status);
	} else {
		Serial.println("Modem is Unavailable or Busy");
		return(false);
	}
}

bool Network::prefRadioPriority (int net_type) { // Change network priority for radio scanning. 1 - Preffered NB-IoT, 2 - Preffered EGPRS.
	Serial2.print("AT+CFGRATPRIO=");
	if(net_type == 1) {
		Serial2.println("4");
	} else if (net_type == 2) {
		Serial2.println("2");
	}
	CS_MODEM_RES res = serial_res(500,F("OK"));
	reboot();
	delay(1000);
	return(res.status);
}
 
bool Network::prefRadioMode (int mode) { // 1 - Single mode 2 - Dual Mode. Single or Dual Radio with network preference taken from the prefRadioPriority.
	CS_MODEM_RES res;
	if(mode == 1) {
		String rat_prio = "";
		Serial2.println("AT+CFGRATPRIO?");
		res = serial_res(500,F("+CFGRATPRIO"));
		int rat_start = res.data.indexOf(F(":"));
		rat_prio = res.data.substring(rat_start+2,rat_start+3);
		res = serial_res(500,F("OK"));
		Serial2.print("AT+CFGDUALMODE=");
		if(rat_prio == "2") {
			Serial2.println("1,0");
		} else if (rat_prio == "4") {
			Serial2.println("1,0");
		}
	} else if (mode == 2) {
		Serial2.print("AT+CFGDUALMODE=");
		Serial2.println("1,1");
	}
	res = serial_res(500,F("OK"));
	reboot();
	delay(1000);
	return(res.status);
}

bool Network::setPDN(int ipType, String apn) { // IP Type: 1 - IPV4, 2 - IPV6, 3 - IPV4V6, 5 - No IP (NB-IoT Paging).
	Serial2.print("AT+CFGDFTPDN=");
	Serial2.println(ipType + comma + quotes + apn + quotes);
	CS_MODEM_RES res = serial_res(500,F("OK"));
	return(res.status);
}

bool Network::enablePacketData (bool enable) { // True - Attach to Data Service, False - Dettach from Data Service.
	if(!getPacketDataStatus()) {
		if(isModemAvailable()) {
			if(isNetworkAttached()) {
				if(getDefaultPDN()) {
					Serial2.print("AT+CGACT=");
					if (enable) {
						Serial2.println("1,1");
					} else {
						Serial2.println("0");
					}
					CS_MODEM_RES res = serial_res(500,F("OK"));
					return(res.status);
				} else {
					Serial.println("No default PDN found");
					return(false);
				}
			} else {
				Serial.println("No Network service available");
				return(false);
			}
		} else {
			Serial.println("Modem is Unavailable or Busy");
			return(false);
		}
	} 
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

void Network::modemReset() { // Modem will go for Hard Reset. The same function can be used to Switch ON Modem after a Shutdown or as warm reboot.
	int resetPin = 23;
	pinMode(resetPin, OUTPUT);
	digitalWrite(resetPin, HIGH);
  	delay(1000);
  	digitalWrite(resetPin, LOW);
	Serial.println("Modem Reset Complete.");
}

bool Network::modemWakeUp() { // Wake Up the modem from Sleep Mode.
	int wakePin = 18;
	pinMode(wakePin, OUTPUT);
	digitalWrite(wakePin, HIGH);
  	delay(1000);
  	digitalWrite(wakePin, LOW);
	Serial.println("Modem Wake up Initiated.");
}

// Get Functions.

bool Network::isModemAvailable() { // Returns true if a valid modem response is received.
	Serial2.println("AT");
	CS_MODEM_RES res = serial_res(500,F("OK"));
	return(res.status);
}

bool Network::isESIMReady() { // Returns true if the eSIM is ready for registration.
	Serial2.print("AT+SIM=");
	Serial2.println("0");
	CS_MODEM_RES res = serial_res(500,F("+USIM"));
	String data;
	String tmp;
	bool simStat;
	if(res.status) {
		data = res.data;
		int esim_first = data.indexOf(F(":"));
		tmp = data.substring(esim_first+2,esim_first+8);
		if(tmp == "NORMAL") {
			simStat = true;
		} else {
			simStat = false;
		}
	} else {
		simStat = false;
	}
	return (simStat);
}

bool Network::setNetworkAuto() {
	Serial2.print("AT+COPS=");
   	Serial2.println("0");
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

String Network::getModemInfo () { // Displays the Modem Manufacturer and Firmware information.
	CS_MODEM_RES res = serial_res(500,F("OK"));
	Serial2.println(F("ATI"));
	res = serial_res(500,F("OK"));
	String out = res.temp;
	int info_first = out.indexOf(F("Manufacturer:"));
	int info_last = out.indexOf(F("Build Date:"));
	out = out.substring(info_first,info_last+20);
	res = serial_res(500,F("OK"));
	return (out);
}

String Network::getIMEI() { // Displays IMEI Number.
	Serial2.println("AT+CGSN");
	CS_MODEM_RES res = serial_res(500,F("OK"));
	String out = res.temp;
	out = out.substring(1,16);
	res = serial_res(500,F("OK"));
	return (out);
}

String Network::getICCID() { // Displays ICCID of the active SIM Card.
	Serial2.println("AT+CCID");
	CS_MODEM_RES res = serial_res(500,F("+CCID"));
	String out = res.temp;
	int iccid_start = out.indexOf(F(":"))+2;
	out = out.substring(iccid_start,iccid_start+19);
	res = serial_res(500,F("OK"));
	return (out);
}

String Network::getIMSI() { // Displays IMSI of the active SIM Card.
	Serial2.println("AT+CIMI");
	CS_MODEM_RES res = serial_res(500,F("OK"));
	String out = res.temp;
	out = out.substring(1,16);
	res = serial_res(500,F("OK"));
	return (out);
}

String Network::getNetTime() { // Dsipalys the time received from the network.
// To be reviewed.
}

radio Network::getRadioQuality() {
  Serial2.println(F("AT+CSQ"));
	CS_MODEM_RES res = serial_res(500,F("+CSQ"));
	radio sig;
	int x = 0;
	String tmp;
	if(res.status) {
		if(res.data.indexOf(F("+CSQ"))!=-1) {
			int index = res.data.indexOf(F(":"));
			int index2 = res.data.indexOf(F(","));
			tmp = res.data.substring(index+2,index2);
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

bool Network::getPacketDataStatus() { // Displays Packet Data status.
	Serial2.println("AT+CGACT?");
	CS_MODEM_RES res = serial_res(500,F("+CGACT"));
	String tmp;
	bool netStat;
	String data;
	if(res.status) {
		data = res.data;
		if(res.data.indexOf(F("+CGACT"))!=-1) {
			int first = res.data.indexOf(F(":"));
			int last = res.data.indexOf(F(","));
			tmp = res.data.substring(first+2,last+2);
		} 
		if (tmp == F("1,1")) {
			netStat = true;
		} else {
			netStat = false;
		}
	}
	res = serial_res(500,F("OK"));
	return(netStat);
}

String Network::getIPAddr() { // Get the current modem IP address.
	Serial2.println("AT+CGPADDR");
	CS_MODEM_RES res = serial_res(500,F("+CGPADDR"));
	String out = res.temp;
	int addr_start = out.indexOf(F(":"))+5;
	int addr_end = out.indexOf(F("\","));
	out = out.substring(addr_start,addr_end);
	res = serial_res(500,F("OK"));
	return (out);
}

String Network::getDNSAddr() { // Get current active DNS Addresses.
	Serial2.println("AT+CGPDNSADDR=1");
	CS_MODEM_RES res = serial_res(500,F("+CGPDNSADDR"));
	String out = res.temp;
	int addr_start = out.indexOf(F(":"))+5;
	out = out.substring(addr_start,out.length());
	int addr_end = out.indexOf(F("\""));
	out = out.substring(0,addr_end);
	res = serial_res(500,F("OK"));
	return (out);
}

ping Network::getPingStatus(String hostname) { // Ping to a hostname and get the ping quality report.
	ping pingr;
	String data = "";
	Serial2.print(F("AT+PING="));
	Serial2.println(quotes + hostname + quotes);
	CS_MODEM_RES res = serial_res(10000,F("Ping"));
	if(res.status) {
		data = res.data;
		int ip_start = data.indexOf(F("for"))+4;
		int ip_end = data.indexOf(F("\n"),ip_start);
		pingr.addr = data.substring(ip_start,ip_end-2);
		res = serial_res(500,F("Packets:"));
		data = res.data;
		int stats_start = data.indexOf(F("Packets:"))+9;
		int stats_end = data.indexOf(F("\n"),stats_start);
		pingr.stats = data.substring(stats_start,stats_end-2);
		pingr.status = true;
	} else {
		Serial.println("# Ping Failed");
		pingr.status = false;
	}
	res = serial_res(500,F("+CSCON: 0"));
	return pingr;
}

String Network::getSMSCenterNumber() { // Displays the SMS Center Number from the SIM Card.
	Serial2.println("AT+CSCA?");
	CS_MODEM_RES res = serial_res(500,F("+CSCA"));
	String sca = res.data;
	int sca_start = sca.indexOf(F(":"))+3;
	int sca_end = sca.indexOf(F(",\""))-2;
	sca = sca.substring(sca_start,sca_end);
	res = serial_res(500,F("OK"));
	return (sca);
}

String Network::getNetworkOperator() { // Displays the currently attached Service Provider Name.
	if(isNetworkAttached()) {
		Serial2.println("AT+QSPN");
		CS_MODEM_RES res = serial_res(500,F("+QSPN"));
		String spn = res.data;
		int spn_start = spn.indexOf(F(":"))+5;
		int spn_end = spn.indexOf(F("\n"))-2;
		spn = spn.substring(spn_start,spn_end);
		res = serial_res(500,F("OK"));
		return (spn);
	} else {
		return ("No Service");
	}
}

bool Network::sendSMS(String destNumber, String message) 
{// Sends SMS to the given Destination Number.
	Serial2.println("AT+CMGF=1");
	CS_MODEM_RES res = serial_res(500, F("OK"));
	Serial2.print(F("AT+CMGS="));
	Serial2.println(quotes + destNumber + quotes);
	delay(100);
	Serial2.println(message + "\x1a");
	delay(100);
	res = serial_res(5000, F("OK"));
	Serial.println(res.data);
	Serial.println(res.status);
	return res.status;
}

String Network::readSMS(String index, String storageType) 
{// Prints the SMS received and stored in a specific strorage (SIM or Modem Flash) with the index number.
	Serial2.print(F("AT+CPMS="));
	Serial2.println(quotes + storageType + quotes);
	CS_MODEM_RES res = serial_res(10000, F("OK"));
	Serial2.print(F("AT+CMGR="));
	Serial2.println(index);
	res = serial_res(10000, F("OK"));
	String out = res.temp;
	out.replace(F("OK"), "");
	out = out.substring(0, out.length());
	return out;
}

bool Network::createMQTT(String mqttserver, String port, String clientID, String keepalive, String cleansession, String username, String password) { // Create a new MQTT connection to the specified hostname and credintials.
	Serial2.print(F("AT+MQTTCONN="));
	Serial2.println(quotes + mqttserver + quotes + comma + port + comma + quotes + clientID + quotes + comma + keepalive + comma + cleansession + comma + quotes + username + quotes + comma + quotes + password + quotes);
	CS_MODEM_RES res = serial_res(10000,F("OK"));
	return res.status;
}

bool Network::publishMQTT(String topic, String message, String qos, String duplicate, String retain) { // Publish MQTT Message to a specified topic unding the previously created MQTT connection.
	Serial2.print(F("AT+MQTTPUB="));
	Serial2.println(quotes + topic + quotes + comma +  quotes + message + quotes + comma + qos + comma + duplicate + comma + retain);
	CS_MODEM_RES res = serial_res(10000,F("OK"));
	return res.status;
}

bool Network::subscribeMQTT(String topic, String qos, bool enable) { // Subcribe to a MQTT Topic. If enable is false, the topic will be unsubscribed from the connection.
	Serial2.print(F("AT+MQTTSUBUNSUB="));
	if(enable) {
		Serial2.println(quotes + topic + quotes + comma + "1" + comma + qos);
	} else {
		Serial2.println(quotes + topic + quotes + comma + "0");
	}
	CS_MODEM_RES res = serial_res(10000,F("OK"));
	return res.status;
}

bool Network::disconnectMQTT() { // Disconnect from the created MQTT server.
	Serial2.println("AT+MQTTDISCONN");
	CS_MODEM_RES res = serial_res(500,F("OK"));
	return res.status;
}

bool Network::createCoAP(String coapserver) { // Create new CoAP connection.
	Serial2.print(F("AT+NCDPOPEN="));
	Serial2.println(quotes + coapserver + quotes);
	CS_MODEM_RES res = serial_res(500,F("OK"));
	return res.status;
}

bool Network::sendCoAPData(String data, String dataLength) { // Send data to CoAP server.
	Serial2.print(F("AT+NMGS="));
	Serial2.println(data + comma + dataLength);
	CS_MODEM_RES res = serial_res(500,F("OK"));
	return res.status;
}


// void Network::receiveCoAPData(String enable)  // Pending 
// {
// 	Serial2.println("AT+NMGR");
// 	CS_MODEM_RES res = serial_res(500,F("OK"));
// 	res = serial_res(10000,F("OK"));
// 	String out = res.temp;
//     out.replace(F("OK"),"");
// 	out = out.substring(0,out.length());
// 	enable = out;
// }


void Network::closeCoAP()
{//Close CoAP server connection.
	Serial2.print(F("AT+NCDPCLOSE"));
	CS_MODEM_RES res = serial_res(500, F("OK"));
}

bool Network::initSingleTCP(String ipmode, String addr, int port) 
{//Initialize single TCP connection
	CS_MODEM_RES resp;
	Serial2.println("AT+CIPMUX=0");
	CS_MODEM_RES res = serial_res(500, F("OK"));
	if (res.status == 1)
	{
		Serial2.print("AT+CIPSTART=");
		Serial2.println(quotes + ipmode + quotes + comma + quotes + addr + quotes + comma + port);
		delay(5000);
		resp = serial_res(10000, F("OK"));
	}
	return res.status;
}

bool Network::initMultiTCP(int conum, String ipmode, String addr, int port) 
{//Initialize Multi TCP connection
	CS_MODEM_RES resp;
	Serial2.println("AT+CIPMUX=1");
	delay(500);
	CS_MODEM_RES res = serial_res(500, F("OK"));
	Serial.println(res.data);
	if (res.status == 1)
	{
		Serial2.print(F("AT+CIPSTART="));
		Serial2.println(conum + comma + quotes + ipmode + quotes + comma + quotes + addr + quotes + comma + port);
		delay(5000);
		resp = serial_res(10000, F("OK"));
		Serial.println(res.data);
	}
	return resp.status;
}

bool Network::sendTCPData(int mode, int num, String message) 
{// Input message to be sent
	CS_MODEM_RES res;
	if (mode == 0)
	{
		Serial2.println("AT+CIPSEND");
		delay(100);
		String out = message + "\x1a";
		Serial2.println(out);
		delay(5000);
		res = serial_res(1000, F("SEND OK"));
	}
	else if (mode == 1)
	{
		Serial2.print("AT+CIPSEND=");
		Serial2.println(num);
		delay(100);
		String out = message + "\x1a";
		Serial2.println(out);
		delay(5000);
		res = serial_res(1000, F("SEND OK"));
	}
	return res.status;
}

String Network::recieveTCPData(int mode, int num) 
{//Recieves TCP data
	CS_MODEM_RES res;
	String out;
	if (mode == 0)
	{
		Serial2.println("AT+CIPRXGET=1");
		delay(1000);
		res = serial_res(1000, F("OK"));
		Serial2.println("AT+CIPRXGET=4");
		delay(1000);
		res = serial_res(1000, F("+CIPRXGET:"));
		out = res.temp;
		int addr_start = out.indexOf(F(":")) + 1;
		int addr_end = out.indexOf(F("O"));
		out = out.substring(addr_start, addr_end);
		Serial.println(out);
	}
	else if (mode == 1)
	{
		Serial2.print("AT+CIPRXGET=1");
		delay(100);
		res = serial_res(1000, F("OK"));
		String mout = num + comma + "4";
		Serial2.println(mout);
		delay(1000);
		res = serial_res(1000, F("+CIPRXGET:"));
		out = res.temp;
		int addr_start = out.indexOf(F(":")) + 1;
		int addr_end = out.indexOf(F("O"));
		out = out.substring(addr_start, addr_end);
	}
	return out;
}

void Network::closeTCP(int mode, int connum)
{//Close TCP Connection
	CS_MODEM_RES res;
	if (mode == 0)
	{
		Serial2.println("AT+CIPCLOSE");
		res = serial_res(500, F("OK"));
	}
	else if (mode == 1)
	{
		Serial2.print("AT+CIPCLOSE=");
		Serial2.println(connum);
		res = serial_res(500, F("OK"));
	}
}

void Network::shutdownTCP()
{//Shutdown TCP connection
	Serial2.println("AT+CIPSHUT");
	CS_MODEM_RES res = serial_res(500, F("OK"));
}

bool Network::creatUDP(int port, int recievechar)
{//Creates UDP socket connection
	Serial2.print("AT+TSOCR=");
	Serial2.println(quotes + "DGRAM" + quotes + comma + 17 + comma + port + comma + recievechar);
	CS_MODEM_RES res = serial_res(500, F("OK"));
	return res.status;
}

bool Network::sendUDP(int socketid, String raddr, int rport, int length, String data)
{//Sends UDP data
	Serial2.print("AT+TSOST=");
	Serial2.println(socketid + comma + quotes +  raddr + quotes + comma + rport + comma + length + comma + quotes + data + quotes);
	CS_MODEM_RES res = serial_res(500, F("OK"));
	return res.status;
}

String Network::recieveUDP(int socketid, int reqlength)
{//Recieves UDP data
	String tmp;
	CS_MODEM_RES res = serial_res(500, F("OK"));
	Serial.print("AT+TSORF=");
	Serial.println(socketid + comma + reqlength);
	res = serial_res(500, F("OK")); 
	String out = res.temp;
	int outindex = out.indexOf("O");
	Serial.println(outindex);
	tmp  = out.substring(outindex-16,outindex);
	return tmp;
}

void Network::endUDP()
{//end UDP socket connection
	Serial2.println("AT+TSOCL=1");
	CS_MODEM_RES res = serial_res(500, F("OK"));
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