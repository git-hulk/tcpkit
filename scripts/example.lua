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
    if item.len >= 0 then
        print(item.tv_sec, item.tv_usec, item.src, item.sport, item.dst, item.dport, item.len, item.payload)
    end
end
