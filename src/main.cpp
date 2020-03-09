#include <Arduino.h>
#include "hardware.h"
#include "secret.h"
#include "networking.h"
// #include <PubSubClient.h>
extern PubSubClient mqtt;

void setup(){
    Serial.begin(9600);
    delay(3000);
    Serial.println("Starting...");
    InitialiseModem();
    mqtt.publish("gsmModem/test", "test");
    StopModem();
}

void loop(){
    mqtt.loop();
}
