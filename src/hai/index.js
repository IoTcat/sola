// import configures
Object.keys(require('./config.js')).forEach(i=>global[i]=require('./config.js')[i]);

// load modules
const mqtt = require('mqtt');

// add modules
const log = function(){
    console.log(new Date().toLocaleString('zh-CN') , ...arguments);
}

// instances init
const client  = mqtt.connect(MQTT_SERVER);

// global vars / flags
const FLAG = {};
FLAG.lock = false;

// MQTT init
client.on('connect', function(){
    client.subscribe('/mode/night');
    client.subscribe('/mode/ai');
    Object.keys(ZONES).forEach(zone=>client.subscribe('/ai/'+zone+'/light'));
    Object.keys(ZONES).forEach(zone=>client.subscribe('/hass/'+zone+'/snsr/light'));
    Object.keys(ZONES).forEach(zone=>client.subscribe('/hass/'+zone+'/snsr/swi'));
    client.subscribe('/ai/bath/enforced_valve');
    client.subscribe('/hass/bath/snsr/valve');
});

//Load light
const refresh = require('./src/light.js')({
    ZONES: ZONES,
    client: client,
    FLAG: FLAG,
    log: log
});


//Load enforced-valve
require('./src/enforced-valve.js')({
    ZONES: ZONES,
    client: client,
    FLAG: FLAG,
    WARN_TIMES: WARN_TIMES,
    refresh: refresh,
    log: log
});

//Load mode ctl
require('./src/mode-ctl.js')({
    ZONES: ZONES,
    client: client,
    FLAG: FLAG,
    refresh: refresh,
    log: log
});
