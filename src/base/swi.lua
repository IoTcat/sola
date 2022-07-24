if adc.force_init_mode(adc.INIT_ADC) then
    node.restart()
    return
end
SWI = function(f, T, TH)
    local s, fv = F.n('swi'), adc.read(0);
    local trig = function(v)
        if s ~= v then
            f(v);
            s = v;
            F.s('swi', s);
        end
    end
    tmr.create():alarm(50, tmr.ALARM_AUTO, function()
        local v = adc.read(0);
        if DEBUG then 
            REG(M..'debug/swi', v..','..(v-fv));
        end
        if math.abs(fv - v) > (TH or 100) then
            if v > (T or 800) then
                trig(1);
            else
                trig(0);
            end
        end
        fv = v;
    end);    
end