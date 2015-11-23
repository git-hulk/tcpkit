local cjson = require "cjson"

-- Set config
local mc_redis_delay_config = {
    server = "1.1.1.1",
    port = 11211,
    device = "eth0" 
}

set_config(mc_redis_delay_config )

local delay_hash = {}
function process_packet(ip)
    -- ip is a json string.
    local ip_obj = cjson.decode(ip)
    local key

    if ip_obj.direct == 1 then
        -- incoming packet
        if ip_obj.is_client == 1 then
            key = ip_obj.src .. "-" .. ip_obj.sport .. "-" .. ip_obj.dst .. "-" .. ip_obj.dport
            if delay_hash[key] then
                print("Client query data cost " .. (ip_obj.timestamp - delay_hash[key]) .. "ms")
            end
        else
            key = ip_obj.dst .. "-" .. ip_obj.dport .. "-" .. ip_obj.src .. "-" .. ip_obj.sport
            -- unit ms
            delay_hash[key] = ip_obj.timestamp
        end
    else 
        -- outgoing packet
        if ip_obj.is_client == 1 then
            key = ip_obj.dst .. "-" .. ip_obj.dport .. "-" .. ip_obj.src .. "-" .. ip_obj.sport
            delay_hash[key] = ip_obj.timestamp
        else
            key = ip_obj.src.. "-" .. ip_obj.sport .. "-" .. ip_obj.dst .. "-" .. ip_obj.dport
            if delay_hash[key] then
                print("Server handle packet cost " .. (ip_obj.timestamp - delay_hash[key]) .. "ms")
            end
        end
    end
end
