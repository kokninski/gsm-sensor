#ifndef state_machine_h
#define state_machine_h
#include <PubSubClient.h>

enum state{
    WAIT,
    SEND_DATA_MQTT,
    READ_DATA_MQTT,
    STORE_DATA_SD,
    READ_DATA_SD,
    READ_SENSOR
};

class StateMachine {
    private:
        PubSubClient* mqtt_client;
        state _state;
        unsigned long time;
        unsigned long prev_time;
        unsigned long interval;

        void wait();
        int send_data_mqtt();
        int read_data_mqtt();
        int store_data_sd();
        int read_data_sd();
        int read_sensor();

    public:
        StateMachine(PubSubClient&);
        void loop();
        
};

#endif