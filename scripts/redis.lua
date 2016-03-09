--local cjson = require "cjson"

local packet_hash_sec = {}
local packet_hash_usec = {}
local packet_hash_cmd = {}

function parse_data(str)
    if string.byte(str, 1) ~= 42 then
        return string.sub(str, 1, string.len(str) - 1)
    end

    local result = ""
    for sub_str in string.gmatch(str, "(%g+)\r\n") do
        local first_char = string.byte(sub_str, 1)
        -- ascii('$') = 36, ascii('*') = 42
        if first_char ~= 36 and first_char ~= 42 then
            result = result.." "..sub_str
        end
    end
    return result
end

function handle_incoming(item)
    if item.is_client == 1 then
        key = item.dst.. ":" .. item.dport .. " | " .. item.src.. ":" .. item.sport
        if packet_hash_cmd[key] then
            local cost = (item.tv_sec - packet_hash_sec[key]) * 1000
            cost = cost +  (item.tv_usec - packet_hash_usec[key]) / 1000
            local cmd = parse_data(packet_hash_cmd[key]) 
            if string.len(cmd) > 20 then
                print(string.format("%36s | %1.20s... | %3.3fms", key, parse_data(packet_hash_cmd[key]), cost))
            else
                print(string.format("%36s | %-23s | %3.3fms", key, parse_data(packet_hash_cmd[key]), cost))
            end
            packet_hash_sec[key] = nil 
            packet_hash_usec[key] = nil 
            packet_hash_cmd[key] = nil 
        end
    else
        key = item.src.. ":" .. item.sport .. " | " .. item.dst.. ":" .. item.dport
        packet_hash_sec[key] = item.tv_sec
        packet_hash_usec[key] = item.tv_usec
        packet_hash_cmd[key] = parse_data(item.payload)
    end
end

function handle_outgoing(item)
    if item.is_client == 1 then
        key = item.src.. ":" .. item.sport .. " | " .. item.dst.. ":" .. item.dport
        packet_hash_sec[key] = item.tv_sec
        packet_hash_usec[key] = item.tv_usec
        packet_hash_cmd[key] = parse_data(item.payload)
    else
        key = item.dst.. ":" .. item.dport .. " | " .. item.src.. ":" .. item.sport
        if packet_hash_cmd[key] then
            local cost = (item.tv_sec - packet_hash_sec[key]) * 1000
            cost = cost +  (item.tv_usec - packet_hash_usec[key]) / 1000
            local cmd = parse_data(packet_hash_cmd[key]) 
            if string.len(cmd) > 20 then
                print(string.format("%36s\t%1.20s...\t%3.3fms", key, parse_data(packet_hash_cmd[key]), cost))
            else
                print(string.format("%36s\t%-23s\t%3.3fms", key, parse_data(packet_hash_cmd[key]), cost))
            end
            packet_hash_sec[key] = nil 
            packet_hash_usec[key] = nil 
            packet_hash_cmd[key] = nil 
        end
    end
end

function process_packet(item)
    if item.len == 0 then
        -- ignore tcp flow packet
        return
    end

    if item.direct == 1 then
        -- item.direct = 1 means incoming packet 
        handle_incoming(item)
    else
        -- item.direct = 0 means outgoing packet 
        handle_outgoing(item)
    end
end
