module.exports = function(G){
    Object.keys(G).forEach(i=>global[i]=G[i]);
    let state_valve = undefined;
    let timer_valve = undefined;
    let tmr_warning = undefined;
    client.on('message', function(topic, message){
        if(topic === '/hass/bath/snsr/valve'){
            state_valve = parseInt(message);
            return;
        }
        if(topic !== '/ai/bath/enforced_valve')return;
        state_valve = undefined;
        client.publish('/hass/bath/ctl/valve', message, {qos: 1, retain: true});
        clearTimeout(timer_valve);
        timer_valve = setTimeout(()=>{
            if(state_valve !== parseInt(message)){
                let cnt = WARN_TIMES*2;
                FLAG.lock = true;
                clearInterval(tmr_warning);
                tmr_warning = setInterval(()=>{
                    Object.keys(ZONES).forEach(zone=>client.publish('/hass/'+zone+'/ctl/light', String(cnt%2)));
                    cnt--;
                    log('Valve', 'Warning', 'CNT Left:', cnt);
                    if(cnt === -1){
                        clearInterval(tmr_warning);
                        FLAG.lock = false;
                        refresh.forEach(f=>f());
                    }
                }, 3000);
            }else{
                log('Valve', 'Confirmed', parseInt(message));
            }
        }, 5000);
    
    });
    
}