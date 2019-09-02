/*
 * @Author: IoTcat (https://iotcat.me) 
 * @Date: 2019-08-20 09:58:21 
 * @Last Modified by: 
 * @Last Modified time: 2019-08-30 13:12:01
 */
#ifndef __LED_H__
#define __LED_H__

class LED {
   public:

    LED(const int& pin){
        this->_pin = pin;
        this->_mode = "on";
    };
    ~LED(){};

    inline void on(){
        digitalWrite(this->_pin, HIGH);
        this->_mode = "on";
    }

    inline void off(){
        digitalWrite(this->_pin, LOW);
        this->_mode = "off";
    }

    inline const bool toggle(){
        digitalWrite(this->_pin, !digitalRead(this->_pin));
        this->_mode = (digitalRead(this->_pin)) ? "on" : "off";
        return digitalRead(this->_pin);
    }

    inline void ini(){
        pinMode(this->_pin, OUTPUT);
        this->_fStatus = this->getMode();
    }

    inline void loop() const{
        if(this->_mode == "on" || this->_mode == "off"){
            return;
        }

        if(this->_mode == "star"){
            if(millis() % this->_period < this->_period / 2){
                digitalWrite(this->_pin, HIGH);
            }else{
                digitalWrite(this->_pin, LOW);
            }
        }

        if(this->_mode == "breath"){
            int val;
            if(millis() % this->_period < this->_period / 2){
                val = millis() % this->_period;
                val = (float)val * 255 / (this->_period / 2);
            }else{
                val = this->_period - millis() % this->_period;
                val = (float)val * 255 / (this->_period / 2);
            }
            analogWrite(this->_pin, val);
        }

    }

    void star(const unsigned int& period = 2000){
        this->_mode = "star";
        this->_period = period;
    }

    void breath(const unsigned int& period = 3000){
        this->_mode = "breath";
        this->_period = period;
    }

    inline const String getMode() const{
        return this->_mode;
    }

    inline const bool isStateChange(){
        if(this->_fStatus != this->getMode()){
            this->_fStatus = this->getMode();
            return true;
        }else{
            return false;
        }
    }

   private:
   unsigned short _pin;
   String _mode, _fStatus;
   unsigned int _period;

};

#endif  //__LED_H__
