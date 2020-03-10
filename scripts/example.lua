function process(packet)
    if packet.size ~= 0 then -- skip the syn and ack
        local time_str = os.date('%Y-%m-%d %H:%M:%S', packet.tv_sec).."."..packet.tv_usec
        print(string.format("%s %s:%d=>%s:%d %s %u %u %d %u %s",
            time_str,
            packet.sip, -- source ip
            packet.sport, -- source port
            packet.dip, -- destination ip
            packet.dport, -- destination port
            packet.seq, -- sequence number
            packet.ack, -- ack number
            packet.flags, -- flags, e.g. syn|ack|psh..
            packet.size, -- payload size
            packet.payload -- payload
        ))
    end
end
