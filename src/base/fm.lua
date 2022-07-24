FM = function(PIN, f)
    local v = F.n('fm'..PIN);
    gpio.mode(PIN, gpio.INT);
    gpio.trig(PIN, 'up', function()
        v = v + 1;
        if v%450 == 0 then
            f(v/450);
            F.s('fm'..PIN, v);
        end
    end);
    return function()
        return v;
    end
end