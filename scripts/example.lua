function process_packet(item)
    -- item.tv_sec 
    -- time.tv_usec
    -- item.src
    -- item.sport
    -- item.dst
    -- item.dport
    -- item.payload
    if item.len > 0 then
        print(item.payload)
    end
end
