FM = function(PIN, f)
    local v = F.n('fm'..PIN);
    gpio.mode(PIN, gpio.INT);
    gpio.trig(PIN, 'up', function()
        v = v + 1;
        F.s('fm'..PIN, v);
        f(v);
    end);
    return function()
        return v;
    end
end