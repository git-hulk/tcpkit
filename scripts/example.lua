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
    if item.len > 0 then
        print(item.incoming, item.udp, item.tv_sec, item.tv_usec, item.src, item.sport, item.dst, item.dport, item.len, item.payload)
    end
end
