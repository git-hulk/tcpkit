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
    self.pos = self.pos + 1
    return string.byte(self.payload, 1), nil
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

function _M.read_short_string(self)
    local size, err = self:read_int16() 
    if err then
        return nil, err
    end
    if self.pos + size - 1 > self.size then
        return nil, "Read out of bound."
    end

    local start = self.pos 
    self.pos = self.pos + size

    return string.sub(self.payload, start, start + size), nil
end

return _M
