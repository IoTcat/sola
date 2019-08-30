/*
 * @Author: IoTcat (https://iotcat.me) 
 * @Date: 2019-08-30 11:00:00 
 * @Last Modified by: 
 * @Last Modified time: 2019-08-30 11:13:18
 */

#ifndef __RELAY_H__
#define __RELAY_H__

class Relay{
public:
    Relay(const int& pin, const bool& onStatus = true){
        this->_pin = pin;
        this->_onStatus = onStatus;
    };
    ~Relay(){};

    inline void ini() const{
        pinMode(this->_pin, OUTPUT);
    };
    inline void begin() const{
        this->ini();
    };
    inline void core() const{};
    inline void loop() const{
        this->core();
    }

    inline void on() const{
        digitalWrite(this->_pin, _onStatus);
    }
    inline void off() const{
        digitalWrite(this->_pin, !_onStatus);
    }
    inline const bool toggle() const{
        digitalWrite(this->_pin, !digitalRead(this->_pin));
        return this->getStatus();
    }
    inline const bool getStatus() const{
        return (digitalRead(this->_pin) == this->_onStatus) ? true : false;
    }

private:
    unsigned short _pin;
    bool _onStatus;

};



#endif //__RELAY_H__