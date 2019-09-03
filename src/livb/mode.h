/*
 * @Author: IoTcat (https://iotcat.me) 
 * @Date: 2019-08-30 11:14:59 
 * @Last Modified by: 
 * @Last Modified time: 2019-08-30 13:31:16
 */

#ifndef __MODE_H__
#define __MODE_H__

class Mode{
public:
    Mode() : _isNight(false), _fIsNight(false), _isAuto(true), _fIsAuto(true), \
    _isMidnight(false), _fIsMidnight(false), _isOffline(true), _fIsOffline(true){};
    ~Mode(){};

    inline void ini() const{
    }
    inline void begin() const{
        this->ini();
    }

    inline void core() const{};
    inline void loop() const{
        this->core();
    };

    inline const bool isNight() const{
        return this->_isNight;
    }
    inline const bool isNight(const bool& state){
        this->_isNight = state;
        return this->isNight();
    }
    inline const bool isNightChange(){
        if(this->_isNight != this->_fIsNight){
            this->_fIsNight = this->_isNight;
            return true;
        }else{
            return false;
        }
    }

    inline const bool isAuto() const{
        return this->_isAuto;
    }
    inline const bool isAuto(const bool& state){
        this->_isAuto = state;
        return this->isAuto();
    }
    inline const bool isAutoChange(){
        if(this->_isAuto != this->_fIsAuto){
            this->_fIsAuto = this->_isAuto;
            return true;
        }else{
            return false;
        }
    }

    inline const bool isMidnight() const{
        return this->_isMidnight;
    }
    inline const bool isMidnight(const bool& state){
        this->_isMidnight = state;
        return this->isMidnight();
    }
    inline const bool isMidnightChange(){
        if(this->_isMidnight != this->_fIsMidnight){
            this->_fIsMidnight = this->_isMidnight;
            return true;
        }else{
            return false;
        }
    }

    inline const bool isOffline() const{
        return this->_isOffline;
    }
    inline const bool isOffline(const bool& state){
        this->_isOffline = state;
        return this->isOffline();
    }
    inline const bool isOfflineChange(){
        if(this->_isOffline != this->_fIsOffline){
            this->_fIsOffline = this->_isOffline;
            return true;
        }else{
            return false;
        }
    }

    inline const unsigned short nightMode(){
        if(!this->isNight() && !this->isMidnight()){
            return 0;
        }
        if(this->isNight() && !this->isMidnight()){
            return 1;
        }
        if(this->isMidnight()){
            return 2;
        }
        return 0;
    }


private:
    bool _isNight, _isAuto, _isMidnight, _isOffline;
    bool _fIsNight, _fIsAuto, _fIsMidnight, _fIsOffline;
    

};



#endif //__MODE_H__