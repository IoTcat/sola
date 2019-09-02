/*
 * @Author: IoTcat (https://iotcat.me) 
 * @Date: 2019-08-20 11:15:20 
 * @Last Modified by: 
 * @Last Modified time: 2019-08-20 11:28:17
 */

#ifndef __PIR_H__
#define __PIR_H__

class Pir{
public:
    Pir(const int& pin){
        this->_pin = pin;
    };
    ~Pir(){};

    void ini() {
        pinMode(this->_pin, INPUT);
        this->_fState = this->getState();
    };
    void begin() {
        this->ini();
    };

    void loop(){

    };

    const bool isPeopleIn(){
        if(this->_fState == false && this->getState() == true){
            this->_fState = true;
            return true;
        }
        return false;
    }

    const bool isPeopleOut(){
        if(this->_fState == true && this->getState() == false){
            this->_fState = false;
            return true;
        }
        return false;
    };

    const bool getState(){
        return digitalRead(this->_pin);
    };

private:
    unsigned short _pin;
    bool _fState;

};


#endif //__PIR_H__
