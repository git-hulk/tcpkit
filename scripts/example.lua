function process_packet(item)
    -- item.tv_sec 
    -- time.tv_usec
    -- item.src
    -- item.sport
    -- item.dst
    -- item.dport
    -- item.payload
    -- item.incoming
    -- item.udp
    -- item.seq
    -- item.ack
    -- item.flags
    if item.len > 0 then
        local time_str = os.date('%Y-%m-%d %H:%M:%S', item.tv_sec).."."..item.tv_usec
        local network_str = item.src .. ":" .. item.sport .. "=>" .. item.dst .. ":" .. item.dport
        print(time_str, network_str, item.len, item.payload)
    end
end
