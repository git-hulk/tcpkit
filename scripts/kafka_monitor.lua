package.path = "../scripts/kafka_decoder.lua;"..package.path
--local cjson = require "cjson"

local packet_hash_sec = {}
local packet_hash_usec = {}
local packet_hash_cmd = {}

local request_map = {
    "PRODUCE",
    "FETCH", 
    "OFFSETS", 
    "METADATA", 
    "LEADER_AND_ISR", 
    "STOP_REPLICA", 
    "UPDATE_METADATA", 
    "CONTROLLER_SHUTDOWN", 
    "OFFSET_COMMIT", 
    "OFFSET_FETCH", 
    "CONSUMER_METADATA", 
    "JOIN_GROUP", 
    "HEART_BEAT"
}

function read_topic(decoder)
    local topic_count, err  = decoder:read_int32()
    if err then
        return ""
    end
    local topics = " Topics: "
    for i = 1, topic_count, 1 do
        topics, err = topics .. " "..decoder:read_short_string()
    end

    return topics
end
function offsets_request_topic(decoder)
    local replica_id = decoder:read_int32()
    return read_topic(decoder)
end

function metadata_request_topic(decoder)
     return read_topic(decoder)
end

function fetch_request_topic(decoder)
    local replica_id = decoder:read_int32()
    local max_wait = decoder:read_int32()
    local min_bytes = decoder:read_int32()
    local topic_count, err  = decoder:read_int32()
    local topics = " Topics: "
    for i = 1, topic_count, 1 do
        topics, err = topics .. " "..decoder:read_short_string()
        local partitions = decoder:read_int32()
        for i = 1, partitions, 1 do
            decoder:read_int32()
            decoder:read_int64()
            decoder:read_int32()
        end

    end
    return topics 
end

function produce_request_topic(decoder)
    local required_acks = decoder:read_int16()
    if required_acks == 0 then
        packet_hash_sec[key] = nil 
        packet_hash_usec[key] = nil 
        packet_hash_cmd[key] = nil 
        return ""
    end

    local ack_timeout = decoder:read_int32()
    return read_topic(decoder)
end

function parse_request(payload, size)
    local M = require("kafka_decoder")
    local decoder = M.new(payload, size)
    local total_size = decoder:read_int32()
    local req_type = decoder:read_int16()
    local version = decoder:read_int16()
    local cor_id = decoder:read_int32()
    local client_id = decoder:read_short_string()
    local req_name = "UNKNOWN"

    if req_type > 3 or total_size > size then
        return "UNKNOWN"
    end
    if req_type >= -1 then
        req_name = request_map[req_type + 1]
    end

    local ret = req_name
    if req_type == 0 then
        ret = ret .. produce_request_topic(decoder)
    elseif req_type == 1 then
        ret = ret .. fetch_request_topic(decoder)
    elseif req_type == 2 then
        ret = ret .. offsets_request_topic(decoder)
    elseif req_type == 3 then
        ret = ret .. metadata_request_topic(decoder)
    end
    
    return ret 
end

function calculate_request_cost(key, item)
    if packet_hash_cmd[key] then
        local time_str = os.date('%Y-%m-%d %H:%M:%S', item.tv_sec).."."..string.format("%06d",item.tv_usec)
        local cost = (item.tv_sec - packet_hash_sec[key]) * 1000
        cost = cost +  (item.tv_usec - packet_hash_usec[key]) / 1000
        print(string.format("%s | %36s | %s | %3.3fms", time_str, key, packet_hash_cmd[key], cost))
        packet_hash_sec[key] = nil 
        packet_hash_usec[key] = nil 
        packet_hash_cmd[key] = nil 
    end
end

function store_request(key, item)
    local req = parse_request(item.payload, item.len) 
    if req ~= "UNKNOWN" then
    packet_hash_sec[key] = item.tv_sec
    packet_hash_usec[key] = item.tv_usec
    packet_hash_cmd[key] = req
    end
end

function handle_incoming(item)
    if item.is_client == 1 then
        key = item.dst.. ":" .. item.dport .. " | " .. item.src.. ":" .. item.sport
        calculate_request_cost(key, item)
    else
        key = item.src.. ":" .. item.sport .. " | " .. item.dst.. ":" .. item.dport
        store_request(key, item)
    end
end

function handle_outgoing(item)
    if item.is_client == 1 then
        key = item.src.. ":" .. item.sport .. " | " .. item.dst.. ":" .. item.dport
        store_request(key, item)
    else
        key = item.dst.. ":" .. item.dport .. " | " .. item.src.. ":" .. item.sport
        calculate_request_cost(key, item)
    end
end

function process_packet(item)
    -- ignore tcp flow packet
    if item.len == 0 then
        return
    end

    if item.incoming == 1 then
        handle_incoming(item)
    else
        handle_outgoing(item)
    end
end
