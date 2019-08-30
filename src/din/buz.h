/*
 * @Author: IoTcat (https://iotcat.me) 
 * @Date: 2019-08-20 09:58:28 
 * @Last Modified by: 
 * @Last Modified time: 2019-08-30 12:35:44
 */
#ifndef __BUZ_H__
#define __BUZ_H__

class Buz{
    public:
    Buz(const int& pin){
        this->_pin = pin;
    };
    ~Buz(){};

    void ini() {
        pinMode(this->_pin, OUTPUT);
        this->_fStatus = this->getStatus();
    };

    void loop(){
        if(millis() < this->_toT){
            analogWrite(this->_pin, (int)((float)((this->_toT - millis()) % 3000) * 255 / 3000));
            this->_status = true;
        }else{
            analogWrite(this->_pin, 0);
            this->_status = false;
        }
    };

    inline void on(const int& t = 12*60*1000){
        this->_toT = millis() + t;
    }

    inline void off(){
        this->_toT = 0;
    }

    inline void once(const int& times = 4){
        this->_toT = millis() + 3000 * times;
    }

    inline const bool getStatus() const{
        return this->_status;
    }

    inline const bool isStateChange() {
        if(this->_fStatus != this->getStatus()){
            this->_fStatus = this->getStatus();
            return true;
        }else{
            return false;
        }
    }

    private:
    static unsigned int _toT;
    unsigned int _pin; 
    bool _status, _fStatus;
};

unsigned int Buz::_toT = 0;

#endif //__BUZ_H__
