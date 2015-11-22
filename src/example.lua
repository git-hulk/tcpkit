local cjson = require "cjson"
local example_config = {
    server = "1.1.1.1",
    port = 11211,
    device = "eth0" 
}

set_config(example_config)
function process_packet(ip)
    print(ip)
end
