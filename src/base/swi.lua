SWI = function(PO, PI, f)
local T, sa, sc, sl, su, sd, G2 = 2, {}, 1, 0, F.n('su'), F.n('sd'), F.n('G2');
gpio.mode(PO, gpio.OUTPUT);
gpio.mode(PI, gpio.INPUT);
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
            G2 = 1;
            F.s('G2', G2);
            f(0);
        end
    elseif math.abs(s - sd) >= T and math.abs(s - su) < T then
        if G2 == 1 then
            G2 = 0;
            F.s('G2', G2);
            f(1);
        end
    else
        if s > sl then
            su = s;
            sd = sl;
        else
            su = sl;
            sd = s;
        end
        F.s('su', su);
        F.s('sd', sd);
    end
    sl = s;
    sc= 1;
end
end);
end