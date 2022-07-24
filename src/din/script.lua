P_RLY = 1;
M = MQTT_TOPIC_PREFIX;

dofile('utils.lua');
dofile('rly.lua');
dofile('swi.lua');

R = RLY(P_RLY, function(c)
    REG(M..'snsr/light', c);
end, 1);
SWI(function(c)
    REG(M..'snsr/swi', c);
    R(R()+1);
end);

RECV = function(m, topic, data)
    if topic == M..'ctl/light' then
        R(data);
    end
end