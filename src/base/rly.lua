RLY = function(P, f, SHIFT)
    SHIFT = SHIFT or 0;
    local v = F.n('rly'..P);
    gpio.mode(P, gpio.OUTPUT);
    gpio.write(P, (v + SHIFT) % 2);
    return function(c)
        if c ~= nil then
            v = tonumber(c) % 2;
            gpio.write(P, (v + SHIFT) % 2);
            F.s('rly'..P, v);
            if f ~= nil then f(v); end
        end
        return v;
    end
end