#ifndef __SENSOR_H__
#define __SENSOR_H__

class Sensor{
public:
    Sensor(const int& pin, const bool& isAnalog = false){
        this->_pin = pin;
        this->_isAnalog = isAnalog;
    };
    ~Sensor(){};

    inline void ini(){
        pinMode(this->_pin, INPUT);
        this->getVal();
        this->_fVal = this->_val;
    }

    inline void begin(){
        this->ini();
    }

    inline void core(){

    }

    inline void loop(){
        this->core();
    }


    inline const int getVal(){
        if(this->_isAnalog){
            this->_val = analogRead(this->_pin);
        }else{
            this->_val = digitalRead(this->_pin);
        }
        return this->_val;
    }

    inline const bool isValChange(const unsigned short& err = 0){
        this->getVal();
        if(this->_val > this->_fVal + err || this->_val < this->_fVal - err){
            this->_fVal = this->_val;
            return true;
        }
        return false;
    }

private:
    unsigned short _pin;
    bool _isAnalog;
    int _val, _fVal;

};





#endif //__SENSOR_H__