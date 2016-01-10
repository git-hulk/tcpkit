local cjson = require "cjson"
local example_config = {
    server = "192.168.41.219",
    port = 3306,
    device = "en5" 
}

set_config(example_config)
function process_packet(item)
    -- item.tv_sec 
    -- time.tv_usec
    -- item.src
    -- item.sport
    -- item.dst
    -- item.dport
    -- item.payload
    print("---- packet ----")
end
