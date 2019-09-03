/*
 * @Author: IoTcat (https://iotcat.me) 
 * @Date: 2019-08-20 10:04:18 
 * @Last Modified by: 
 * @Last Modified time: 2019-08-20 10:40:35
 */

#ifndef __SWI_H__
#define __SWI_H__

class Swi{
public:
    Swi(const int& pin0, const int& pin1){
        _pin0 = pin0;
        _pin1 = pin1;
    };    
    ~Swi(){};

    static inline void ini() {
        pinMode(_pin0, INPUT);
        pinMode(_pin1, OUTPUT);
        _fState = state();
    };

    static inline void begin() {
        ini();
    };

    static inline void loop(){
        const bool sta = state();

        if(sta != _fState){
            if(sta == true){
                if(_isDefOn) _on();
            }else{
                if(_isDefOff) _off();
            }
            if(_isDefToggle) _toggle();
            _fState = sta;
        }
    }

    static inline void on(const String& event, void (*f)(void)){
        if(event == "on"){
            _on = f;
            _isDefOn = true;
        }
        if(event == "off"){
            _off = f;
            _isDefOff = true;
        }
        if(event == "toggle"){
            _toggle = f;
            _isDefToggle = true;
        }
    }

    static const bool state(const unsigned int& total = 600, const unsigned int& threshole = 500){
        unsigned int cnt = 0;
        for(unsigned int i = 0; i < total; i ++){
            digitalWrite(_pin1, i % 2);
            if(digitalRead(_pin0) == i%2){
                cnt ++;
            }
        }

        if(cnt >= threshole){
            return true;
        }else{
            return false;
        }
    }

private:
    static unsigned short _pin0, _pin1;
    static bool _fState;
    static bool _isDefOn, _isDefOff, _isDefToggle;
    static void (*_on)(void);
    static void (*_off)(void);
    static void (*_toggle)(void);


};

unsigned short Swi::_pin0, Swi::_pin1;
bool Swi::_fState, Swi::_isDefOn = false, Swi::_isDefOff = false, Swi::_isDefToggle = false;
void (*Swi::_on)(void);
void (*Swi::_off)(void);
void (*Swi::_toggle)(void);

#endif //__SWI_H__
