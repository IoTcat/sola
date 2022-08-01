module.exports = function(G){
    Object.keys(G).forEach(i=>global[i]=G[i]);
    let night = 0;
    let ai = 0;
    client.on('message', function (topic, message) {
        if(topic === '/mode/night') night = parseInt(message);
        if(topic === '/mode/ai') ai = parseInt(message);
    });

    Object.keys(ZONES).forEach(zone=>{
        
        let timer_unfrozen = undefined;
        let frozen = false;
        let retained_msg = true;
        client.on('message', function (topic, message) {
            if(topic === '/hass/'+zone+'/snsr/swi'){
            if(retained_msg){
                    retained_msg = false;
                    return;
            }
            frozen = true;
            clearTimeout(timer_unfrozen);
                timer_unfrozen = setTimeout(()=>{
                    frozen = false;
                    refresh.forEach(f=>f());
                }, ZONES[zone].frozen*1000);
                return;
            }
            if(topic !== '/ai/'+zone+'/light')return;
            if(!ai || !night) return;
            if(frozen) return;
            if(FLAG.lock) return;
            client.publish('/hass/'+zone+'/ctl/light', message);
        });
    });
}