T, sa, sc, sl, su, sd = 2, {}, 1, 0, 0, 0;
G2 = 0;
gpio.mode(1, gpio.OUTPUT);
gpio.mode(2, gpio.OUTPUT);
gpio.mode(5, gpio.INPUT);
tmr.create():alarm(5, tmr.ALARM_AUTO, function()
    gpio.write(2, sc%2);
    sa[sc] = (gpio.read(5) + sc%2) % 2;
    sc = sc + 1;
    if sc > 20 then
        local s = 0;
        for _,v in pairs(sa) do
            s = s + v;
        end
        if math.abs(s - sd) < T and math.abs(s - su) >= T then
            if G2 == 0 then
                gpio.write(1, 1);
                G2 = 1;
            end
        elseif math.abs(s - sd) >= T and math.abs(s - su) < T then
            if G2 == 1 then
                gpio.write(1, 0);
                G2 = 0;
            end
        else
            if s > sl then
                su = s;
                sd = sl;
            else
                su = sl;
                sd = s;
            end
        end
        sl = s;
        sc= 1;
    end
end);