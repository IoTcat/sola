F = {};
local F_PREFIX = 'data/';
F.g = function(k)
    return file.getcontents(F_PREFIX..k);
end
F.s = function(k, v)
    file.putcontents(F_PREFIX..k, v);
end
F.n = function(k)
    return tonumber(F.g(k) or 0);
end