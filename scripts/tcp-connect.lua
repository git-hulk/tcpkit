local syn_table = {}

print(string.format("\n         Time                                 IP:Port                          Retries     Conenct Cost(s)"))
print(string.format("--------------------------  ---------------------------------------------- -------------- ---------------------"))
function process(packet)
    if packet.size ~= 0 then
        return -- // skip the packet with payload
    end

    if packet.request and packet.flags == 2 then
        -- syn packet
        local key = packet.sip..":"..packet.sport.." => "..packet.dip..":"..packet.dport
        if not syn_table[key] then
            syn_table[key] = {
                seq = packet.seq, count = 0, tv_sec = packet.tv_sec, tv_usec = packet.tv_usec
            }
            return
        end
        if syn_table[key].seq == packet.seq then
            -- duplicate syn
            syn_table[key].count = syn_table[key].count + 1
        end
        return
    end
    if (not packet.request and packet.flags == 18) -- syn + ack
       or (bit32.band(packet.flags, 4) ~= 0) then -- rst
        local key = packet.dip..":"..packet.dport.." => "..packet.sip..":"..packet.sport
            local first_syn_packet = syn_table[key]
            -- only print the connection with retransmit syn packet
            if first_syn_packet and first_syn_packet.count > 0 then
            local time_str = os.date('%Y-%m-%d %H:%M:%S', packet.tv_sec).."."..packet.tv_usec
            if packet.tv_usec < first_syn_packet.tv_usec then
                packet.tv_sec = packet.tv_sec - 1
                packet.tv_usec = packet.tv_usec + 1000000
            end
            print(string.format("%26s %47s %6d %12d.%06d",
                                time_str,
                                key,
                                syn_table[key].count,
                                packet.tv_sec-first_syn_packet.tv_sec,
                                packet.tv_usec-first_syn_packet.tv_usec
            ))
        end
        syn_table[key] = nil
        return
    end
end
