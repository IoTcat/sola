P_RLY = 1;
P_FM = 5;
M = MQTT_TOPIC_PREFIX;
MQTT_KEEP_ALIVE = 10;

dofile('utils.lua');
dofile('rly.lua');
dofile('fm.lua');

R = RLY(P_RLY, function(c)
    REG(M..'snsr/valve', c);
end, 1);

FM(P_FM, function(v)
    REG(M..'snsr/flowmeter', v);
end);

RECV = function(m, topic, data)
    if topic == M..'ctl/valve' then
        R(data);
    end
end