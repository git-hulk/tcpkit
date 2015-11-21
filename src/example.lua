local cjson = require "cjson"

local hash = {}

function process_packet(ip)
    local ipp = cjson.decode(ip)
    print(ip)
end
