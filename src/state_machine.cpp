#include "state_machine.h"
#include "networking.h"



StateMachine::StateMachine(PubSubClient& _mqtt){
    this->_state = WAIT;
    this->mqtt_client = &_mqtt;
    this->prev_time = millis();
    this->time = millis();
    this->interval = 5000U;
}

void StateMachine::loop(){
    switch (this->_state)
    {
    case WAIT:
        this->wait();
        break;
    case SEND_DATA_MQTT:
        this->send_data_mqtt();
        break;
    case READ_DATA_MQTT:
        this->read_data_mqtt();
        break;
    case STORE_DATA_SD:
        this->store_data_sd();
        break;
    case READ_DATA_SD:
        this->read_data_sd();
        break;
    case READ_SENSOR:
        this->read_sensor();
        break;
    default:
        break;
    }
}

void StateMachine::wait(){
    if(millis()-prev_time > this->interval){
        this->_state = SEND_DATA_MQTT;
        this->prev_time = millis();
    }
    else{
        delay(1);
    }
}

int StateMachine::send_data_mqtt(){
    InitialiseModem();
    int ret = mqtt_client->publish("gsmModem/test", "test");
    StopModem();
    this->prev_time = millis();
    this->_state = WAIT;
    return ret;
}

int StateMachine::read_data_mqtt(){
    // Start Modem to receive messages through mqtt
    InitialiseModem();
    delay(2000);
    StopModem;
    return 0;
}

int StateMachine::store_data_sd(){
    return 0;
}

int StateMachine::read_data_sd(){
    return 0;
}

int StateMachine::read_sensor(){
    return 0;
}

