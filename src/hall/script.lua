P_SWI_O = 2;
P_SWI_I = 5;
P_RLY = 1;
P_TRIGER = 6;
M = MQTT_TOPIC_PREFIX;

dofile('utils.lua');
dofile('rly.lua');
dofile('swi.lua');

R = RLY(P_RLY, function(c)
    REG(M..'snsr/light', c);
end);
D = RLY(P_TRIGER, function(c)
    if c == 1 then
        REG(M..'snsr/door', c);
    end
end);
SWI(P_SWI_O, P_SWI_I, function(c)
    REG(M..'snsr/swi', c);
    R(R()+1);
end);



RECV = function(m, topic, data)
    if topic == M..'ctl/light' then
        R(data);
    end
    if topic == M..'ctl/door' then
        D(1);
        tmr.create():alarm(500, tmr.ALARM_SINGLE, function()
            D(0);
        end);
    end
end