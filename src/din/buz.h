/*
 * @Author: IoTcat (https://iotcat.me) 
 * @Date: 2019-08-20 09:58:28 
 * @Last Modified by: 
 * @Last Modified time: 2019-08-20 10:07:51
 */
#ifndef __BUZ_H__
#define __BUZ_H__

class Buz{
    public:
    Buz(const int& pin){
        this->_pin = pin;
    };
    ~Buz(){};

    void ini() const{
        pinMode(this->_pin, OUTPUT);
    };

    void loop() const{
        if(millis() < this->_toT){
            analogWrite(this->_pin, (int)((float)((this->_toT - millis()) % 3000) * 255 / 3000));
        }else{
            analogWrite(this->_pin, 0);
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

    private:
    static unsigned int _toT;
    unsigned int _pin; 
};

unsigned int Buz::_toT = 0;

#endif //__BUZ_H__
