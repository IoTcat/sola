-- SOLA Lua Init Script on NodeMCU
__main = coroutine.create(function(__run)
    --Packages Used: file, timer, node, wifi, net, mqtt, gpio, uart
    --Global Constant
    -----------------
    HOSTNAME = file.getcontents('cfg/HOSTNAME') or 'default';
    WIFI_SSID = file.getcontents('cfg/WIFI_SSID') or 'sola';
    WIFI_PASSWD = file.getcontents('cfg/WIFI_PASSWD') or 'c5ce4dfbbd7f';
    MQTT_SERVER = file.getcontents('cfg/MQTT_SERVER') or '192.168.3.4';
    MQTT_PORT = file.getcontents('cfg/MQTT_PORT') or '1884';
    SCRIPT = 'script.lua';
    MQTT_KEEP_ALIVE = 60;
    MQTT_TOPIC_PREFIX = '/hass/'..HOSTNAME..'/';
    MQTT_RECONNECT_DELAY = 10 * 1000;
    MQTT_PUBLISH_INTERVAL = 100;
    SCRIPT_ID = 'DEFAULT';
    
    --Global Variables
    -----------------
    mqtt_publish_queue = {};
    FLAG_mqtt_status = false;
    
    --Global Predefined Func
    RECV = nil;
    REG = function(topic, payload)
        mqtt_publish_queue[topic] = payload;
    end
    

    --Load User-defined Script
    -----------------
    if file.exists(SCRIPT) ~= nil then
        --handle syntex error
        local status, func, errmsg= pcall(loadfile, SCRIPT);
        --handle runtime error
        if errmsg == nil then
            status, errmsg = pcall(func);
        end
        if errmsg then
            REG(MQTT_TOPIC_PREFIX..'debug/script_errmsg', errmsg);
        end
    end



    --WiFi Setup
    ------------
    --Set Mode to STA
    wifi.setmode(wifi.STATION);
    --Set HostName to WIOT-<nid>
    wifi.sta.sethostname(HOSTNAME);
    --Configure the WiFi
    local station_cfg = {};
    station_cfg.ssid = WIFI_SSID;
    station_cfg.pwd = WIFI_PASSWD;
    station_cfg.save = true;
    wifi.sta.config(station_cfg);
    --Connect to AP
    tmr.create():alarm(1000, tmr.ALARM_AUTO, function(timer)
        --print('Setting WiFi');
        if wifi.sta.getip() ~= nil then --When WiFi is Ready
            --print('WiFi OK');
            --Release Timer Resources
            timer:unregister();
            --Resume Main Process
            coroutine.resume(__main);
        end
    end);
    --Suspend Main Process Until WiFi is ready
    coroutine.yield();


    --MQTT Setup
    --------------
    --create MQTT client obj
    m = mqtt.Client(HOSTNAME, MQTT_KEEP_ALIVE);
    --MQTT funcs
    function mqtt_connect()
        m:close();
        m:connect(MQTT_SERVER, MQTT_PORT, false);
    end
    --MQTT events
    m:on("connect", function(m) 
        FLAG_mqtt_status = true;
        m:subscribe({[MQTT_TOPIC_PREFIX..'ctl/#']=0, [MQTT_TOPIC_PREFIX..'debug/#']=0});
    end);
    m:on("connfail", function(m, errCode) 
        FLAG_mqtt_status = false;
        tmr.create():alarm(MQTT_RECONNECT_DELAY, tmr.ALARM_SINGLE, mqtt_connect);
    end);
    m:on("offline", function(m) 
        FLAG_mqtt_status = false;
        tmr.create():alarm(MQTT_RECONNECT_DELAY, tmr.ALARM_SINGLE, mqtt_connect);
    end);
    m:on("message", function(m, topic, data)
        --eval debug
        if topic == MQTT_TOPIC_PREFIX..'debug/eval' then
            local status, func, errmsg = pcall(loadstring, data);
            if errmsg == nil then
                status, errmsg = pcall(func);
            end
            REG(MQTT_TOPIC_PREFIX..'debug/eval_result', errmsg);
        end
        --ota debug
        local pos = string.find(topic, '@');
        if pos and string.sub(topic, 1, pos-1) == MQTT_TOPIC_PREFIX..'debug/ota' then
            file.putcontents(string.sub(topic, pos+1, #topic), data);
        end
        --render to RECV
        pcall(RECV, m, topic, data);
    end);
    m:on("overflow", function(m, topic, data) 
        REG(MQTT_TOPIC_PREFIX..'overflow', topic);
    end);
    --start mqtt
    mqtt_connect();
    --publish the cached mqtt publish queue
    tmr.create():alarm(MQTT_PUBLISH_INTERVAL, tmr.ALARM_AUTO, function()
        if FLAG_mqtt_status then
            local topic, payload = next(mqtt_publish_queue);
            if topic then
                m:publish(topic, payload, 0, 0);
                mqtt_publish_queue[topic] = nil;
            end
        end
    end);
end);
--Launch Main Process
coroutine.resume(__main);