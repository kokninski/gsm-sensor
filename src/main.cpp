#include <Arduino.h>
#include "hardware.h"
#include "secret.h"
#include "networking.h"
#include "state_machine.h"
// #include <PubSubClient.h>
extern PubSubClient mqtt;

StateMachine stateMachine(mqtt);

void setup(){
    Serial.begin(9600);
    delay(3000);
    Serial.println("Starting...");
    InitialiseModem();
    mqtt.publish("gsmModem/test", "Modem initialize test");
    StopModem();
}

void loop(){
    mqtt.loop();
    stateMachine.loop();
}
