local cjson = require "cjson"
local example_config = {
    server = "192.168.41.219",
    port = 3306,
    device = "en5" 
}

set_config(example_config)
function process_packet(packet)
    --local obj = cjson:decode(packet)
    print(packet)
end
