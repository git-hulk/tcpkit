local _M = {}
local mt = {__index = _M}

function _M.new(payload, size)
   local self = {} 
   self.payload = payload
   self.size = size
   self.pos = 1
   setmetatable(self, mt)
   return self
end

function _M.read_int8(self)
    if self.pos > self.size then
        return nil, "Read out of bound."
    end
    local b = string.byte(self.payload, self.pos), nil
    self.pos = self.pos + 1
    return b
end

function _M.read_int16(self)
    if self.pos + 1 > self.size then
        return nil, "Read out of bound."
    end

    local b1 = string.byte(self.payload, self.pos)
    local b2 = string.byte(self.payload, self.pos + 1)
    self.pos = self.pos + 2

    return bit32.lshift(b1, 8) + b2, nil
end

function _M.read_int32(self)
    if self.pos + 3 > self.size then
        return nil, "Read out of bound."
    end

    local b1 = string.byte(self.payload, self.pos)
    b1 = bit32.lshift(b1, 24)
    local b2 = string.byte(self.payload, self.pos + 1)
    b2 = bit32.lshift(b2, 16)
    local b3 = string.byte(self.payload, self.pos + 2)
    b3 = bit32.lshift(b3, 8)
    local b4 = string.byte(self.payload, self.pos + 3)
    self.pos = self.pos + 4

    return (b1 + b2 + b3 + b4), nil
end

-- unsupport int64 in lua
function _M.read_int64(self)
    local h, h_err = self:read_int32()
    local l, l_err = self:read_int32()
    if h_err or l_err then
        return nil, h_err or l_err 
    end

    return h * 4294967296 + l, nil
end

function _M.read_domain(self)
    local start
    local domain = ""
    local size, err = self:read_int8() 

    while size ~= 0 do
        if err then
            return nil, err 
        end
        if self.pos + size - 1 > self.size then
            return nil, "size is out of bound" 
        end
        start = self.pos 
        self.pos = self.pos + size
        domain = domain .. string.sub(self.payload, start, start + size -1).."."
        size, err = self:read_int8() 
    end
    return domain, nil
end

local qtype_map = {
    "A", -- 1
    "NS",  -- 2 
    "MD", -- 3
    "MD", -- 4
    "MF", -- 5
    "CNAME", -- 6
    "SOA", -- 7
    "MB",  -- 8
    "MG", -- 9
    "MR", -- 10
    "NULL", -- 11
    "WKS", -- 12
    "PTR", -- 13
    "HINFO", -- 14
    "MINFO", -- 15
    "MX", -- 16
    "TXT", -- 17
    [28] = "AAAA"
}

local storage = {}
function process_packet(item)
    -- dns query or response header is 24 bytes
    if item.len < 12  then
        return 0
    end
    local decoder = _M.new(item.payload, item.len)
    local id = decoder:read_int16()
    local code = decoder:read_int16()
    local qcount = decoder:read_int16()
    local acount = decoder:read_int16()
    local nscount = decoder:read_int16()
    local arcount = decoder:read_int16()
    local qr = bit32.rshift(code, 15)
    if qr == 0 then
        -- query
        local domains = ""
        for i = 1, qcount, 1 do
            domain, err= decoder:read_domain()
            qtype, err = decoder:read_int16()
            qname = qtype
            -- skip qclass
            decoder:read_int16()
            if qtype_map[qtype] then
                qname = qtype_map[qtype]
            end
            domains = domains .. " " .. domain .. " | " ..qname
        end
        local key = item.src..":"..item.sport.." => "..item.dst..":"..item.dport.." ".. id
        storage[key] = {domains=domains, tv_sec = item.tv_sec, tv_usec = item.tv_usec}
    else
        -- response
        local key = item.dst..":"..item.dport.." => "..item.src..":"..item.sport.." ".. id
        if storage[key] then
            query = storage[key]
            local cost = (item.tv_sec - query.tv_sec) * 1000 + (item.tv_usec - query.tv_usec) / 1000
            print(string.format("%s | %s | %3.3fms", key, query.domains, cost))
            storage[key] = nil
        else 
            print("recevie unknown response:", key)
        end
    end
end
