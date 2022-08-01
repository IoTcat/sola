module.exports =  function(G){
    Object.keys(G).forEach(i=>global[i]=G[i]);
    
    const radar = require('./radar.js');


    const operator = function(n){
        let history = new Array(n).fill(3);
        return function(v){
            history.shift();
            history.push(v);
            return history.reduce((a, b) => a + b, 0) / history.length;
        }
    }
    

    return Object.keys(ZONES).map(function(zone){
        let cache = 0;
        let op_in = operator(ZONES[zone].in.n);
        let op_in_short = operator(ZONES[zone].in.hasOwnProperty('n_short')?ZONES[zone].in.n_short:1);
        let op_out = operator(ZONES[zone].out.n);
        let state = 0;
        let state_real = 0;
        let lock = false;
        let lock_short = false;
        
    
        client.on('message', function (topic, message) {
            if(topic !== '/hass/'+zone+'/snsr/light')return;
            state_real = parseInt(message);
            state = state_real;
        });
        let timer_lock = undefined;
        let timer_lock_short = undefined;
        let judge = ()=>{
            let v_in_short = op_in_short(cache);
            if(ZONES[zone].in.hasOwnProperty('lock_short') && v_in_short > ZONES[zone].in.th_short){
                if(!state){
                    client.publish('/ai/'+zone+'/light', '1');
                    state = 1;
                    log(zone+'@in_short', cache, 'avg:', v_in_short);
                    lock_short = true;
                    clearTimeout(timer_lock_short);
                    timer_lock_short = setTimeout(()=>{
                       lock_short = false; 
                    }, ZONES[zone].in.lock_short*1000);
                }
            } 
            let v_in = op_in(cache, zone === 'din');
            let v_out = op_out(cache, zone === 'din');
            if(v_in > ZONES[zone].in.th){
                if(!state || lock_short) {
                    client.publish('/ai/'+zone+'/light', '1');
                    state = 1;
                    log(zone+'@in', cache, 'avg:', v_in);
                    lock = true;
                    clearTimeout(timer_lock);
                    timer_lock = setTimeout(()=>{
                       lock = false; 
                    }, ZONES[zone].in.lock*1000);
                }
            }else if(v_out < ZONES[zone].out.th){
                if(state && !lock && !lock_short){
                    client.publish('/ai/'+zone+'/light', '0');
                    state = 0;
                    log(zone+'@out', cache, 'avg:', v_out);
                }
            }
        }
        radar(client, zone, function(data){
            cache =parseInt(data.substring(20, 22), 16);    
            client.publish('/ai/'+zone+'/peo', cache.toString());
            //console.log(zone, cache)
            //judge();
        });
        setInterval(judge, 1000);
    
        return function(){
           state = state_real; 
        }
    });
    
    
}