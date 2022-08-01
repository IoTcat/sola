
module.exports = function(client, id, f){

    client.on('connect', function(){
        client.subscribe('/hass/'+id+'2/snsr/radar');
    });
    client.on('message', function (topic, message) {
        if(topic != '/hass/'+id+'2/snsr/radar') return;
        let data = message.toString('hex');
        if(data.substring(0, 8) !== 'f4f3f2f1' || data.substring(data.length-8, data.length) !== 'f8f7f6f5' || data.indexOf('f8f7f6f5f4f3f2f1') !== -1) return;
            data = data.substring(8, data.length-8);
        //data = Buffer.from(data, 'hex');
        f(data);
    })
}


