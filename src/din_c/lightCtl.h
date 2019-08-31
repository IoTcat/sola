/*
 * @Author: IoTcat (https://iotcat.me) 
 * @Date: 2019-08-30 13:36:43 
 * @Last Modified by: 
 * @Last Modified time: 2019-08-30 14:17:59
 */

#ifndef __LIGHTCTL_H__
#define __LIGHTCTL_H__

class LightCtl{
public:
    LightCtl(Relay* _light, LED* _led){
        light = _light;
        led = _led;

        _fStatus = _status;
    };
    ~LightCtl(){
       delete light, led;
     };

    void on(){
        if(isOffline){
            if(nightMode() == 0){
                light->off();
                led->star();
            }
            if(nightMode() == 1){
                light->on();
                led->star();
            }
            if(nightMode() == 2){
                light->off();
                led->on();
            }
        }else{
            if(nightMode() == 0){
                light->off();
                led->breath();
            }
            if(nightMode() == 1){
                light->on();
                led->breath();
            }
            if(nightMode() == 2){
                light->off();
                led->on();
            }
        }
        _status = true;
    }
    void off(){
        if(isOffline){
            if(nightMode() == 0){
                light->off();
                led->star();
            }
            if(nightMode() == 1){
                light->off();
                led->star();
            }
            if(nightMode() == 2){
                light->off();
                led->star();
            }
        }else{
            light->off();
            led->off();
        }
        _status = false;
    }

    inline const bool toggle(){
        if(_status){
            off();
        }else{
            on();
        }
        return _status;
    }

    inline const bool isStateChange(){
        if(_fStatus != _status){
            _fStatus = _status;
            return true;
        }
        return false;
    }

    inline const bool getStatus(){
        return _status;
    }

private:
    Relay* light;
    LED* led;
    bool _status, _fStatus;

};





#endif //__LIGHTCTL_H__
