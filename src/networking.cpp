#include "networking.h"

HardwareSerial Serial2(SIM800_TX, SIM800_RX);

// Your GPRS credentials, if any
const char apn[] = "Internet";
const char gprsUser[] = "";
const char gprsPass[] = "";

// MQTT details
const char* broker = MQTT_SERVER;
const char* mqtt_user = MQTT_USER;
const char* mqtt_pass = MQTT_PASS;

const char* mqtt_client_name = "GsmClient";
const char* topicLed = "GsmClientTest/led";
const char* topicInit = "GsmClientTest/init";
const char* topicLedStatus = "GsmClientTest/ledStatus";

// Declare necessary objects
#ifdef DUMP_AT_COMMANDS
  #include <StreamDebugger.h>
  StreamDebugger debugger(SerialAT, SerialMon);
  TinyGsm modem(debugger);
#else
TinyGsm modem(SerialAT);
#endif

TinyGsmClient client(modem);
PubSubClient mqtt(client);


void mqttCallback(char* topic, byte* payload, unsigned int len){
    // For now only print the message to debug serial
    SerialMon.print("Message arrived [");
    SerialMon.print(topic);
    SerialMon.print("]: ");
    SerialMon.write(payload, len);
    SerialMon.println();    
}

bool mqttConnect(){
    SerialMon.print("Connecting to ");
    SerialMon.print(broker);

    // Connect to MQTT Broker
    bool status = mqtt.connect(mqtt_client_name, mqtt_user, mqtt_pass);

    if (status == false) {
    SerialMon.println(" fail");
    return false;
    }
    SerialMon.println(" success");
    mqtt.publish(topicInit, "GsmClientTest started");
    mqtt.subscribe(topicLed);
    return mqtt.connected();
}

void InitialiseModem(){
    // Cycle the power to the Gsm module to ensure proper startup
    SerialMon.println("Starting the GSM modem");
    StartModem();
    SerialMon.println("Modem Started");
    // !!!!!!!!!!!

    
    // Set GSM module baud rate
    // TinyGsmAutoBaud(SerialAT,GSM_AUTOBAUD_MIN,GSM_AUTOBAUD_MAX);
    SerialAT.begin(9600);
    // delay(3000); // not sure if this is really required

    // Restart takes quite some time
    // To skip it, call init() instead of restart()
    SerialMon.println("Initializing modem...");
    // modem.restart();
    modem.init();

    String modemInfo = modem.getModemInfo();
    SerialMon.print("Modem Info: ");
    SerialMon.println(modemInfo);

    #if TINY_GSM_USE_GPRS
    // Unlock your SIM card with a PIN if needed
    if ( GSM_PIN && modem.getSimStatus() != 3 ) {
    modem.simUnlock(GSM_PIN);
    }
    #endif

    #if TINY_GSM_USE_WIFI
    // Wifi connection parameters must be set before waiting for the network
    SerialMon.print(F("Setting SSID/password..."));
    if (!modem.networkConnect(wifiSSID, wifiPass)) {
    SerialMon.println(" fail");
    delay(10000);
    return;
    }
    SerialMon.println(" success");
    #endif

    #if TINY_GSM_USE_GPRS && defined TINY_GSM_MODEM_XBEE
    // The XBee must run the gprsConnect function BEFORE waiting for network!
    modem.gprsConnect(apn, gprsUser, gprsPass);
    #endif

    SerialMon.print("Waiting for network...");
    if (!modem.waitForNetwork()) {
    SerialMon.println(" fail");
    delay(10000);
    return;
    }
    SerialMon.println(" success");

    if (modem.isNetworkConnected()) {
    SerialMon.println("Network connected");
    }

    #if TINY_GSM_USE_GPRS
    // GPRS connection parameters are usually set after network registration
    SerialMon.print(F("Connecting to "));
    SerialMon.print(apn);
    if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
        SerialMon.println(" fail");
        delay(10000);
        return;
    }
    SerialMon.println(" success");

    if (modem.isGprsConnected()) {
    SerialMon.println("GPRS connected");
    }
    #endif

    // MQTT Broker setup
    mqtt.setServer(broker, 1883);
    mqtt.setCallback(mqttCallback);

}
void StartModem(){
    pinMode(SIM800_EN, OUTPUT);
    pinMode(SIM800_PWRKEY, OUTPUT);
    pinMode(SIM800_STAT, INPUT_FLOATING);

    // Initially set both pins to low
    digitalWrite(SIM800_EN, LOW);
    digitalWrite(SIM800_PWRKEY, LOW);

    digitalWrite(SIM800_EN, HIGH); // enable power to GSM module
    digitalWrite(SIM800_PWRKEY, HIGH); 
    delay(500); // wait for power to stabilise

    digitalWrite(SIM800_PWRKEY, LOW);
    delay(1200); // low signal for at least 1s 

    digitalWrite(SIM800_PWRKEY, HIGH);

    while(!digitalRead(SIM800_STAT)){
        delay(100);
        SerialMon.println(digitalRead(SIM800_STAT));
    }
    SerialMon.println("Modem Started");

}
void StopModem(){
    pinMode(SIM800_EN, OUTPUT);
    pinMode(SIM800_PWRKEY, OUTPUT);
    pinMode(SIM800_STAT, INPUT_FLOATING);

    // Initially set both pins should be high
    digitalWrite(SIM800_EN, HIGH); 
    digitalWrite(SIM800_PWRKEY, HIGH); 
    delay(10); // wait for power to stabilise

    digitalWrite(SIM800_PWRKEY, LOW);
    // delay(1700); // low signal for at least 1.5s 
    // wait untill STATUS is not low
    while(digitalRead(SIM800_STAT)){
        delay(100);
        SerialMon.println(digitalRead(SIM800_STAT));
    }
    digitalWrite(SIM800_PWRKEY, HIGH);
    delay(10);
    // Now turn the power off
    digitalWrite(SIM800_EN, LOW);
    digitalWrite(SIM800_PWRKEY, LOW);

}
