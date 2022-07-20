RLY = function(P, f)
    local v = F.n('rly'..P);
    gpio.mode(P, gpio.OUTPUT);
    gpio.write(P, v);
    return function(c)
        if c ~= nil then
            v = tonumber(c) % 2;
            gpio.write(P, v);
            F.s('rly'..P, v);
            if f ~= nil then f(v); end
        end
        return v;
    end
end