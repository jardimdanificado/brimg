local bit = br.utils.isjit() and require ("bit") or require ("bit32")

local function number_to_bytes(n, byte_count)
    local bytes = {}
    for i = 1, byte_count do
        table.insert(bytes, string.char(bit.band(n, 0xFF)))
        n = bit.rshift(n, 8)
    end
    return table.concat(bytes)
end

function short_to_bytes(n)
    return number_to_bytes(n, 2)
end

function int_to_bytes(n)
    return number_to_bytes(n, 4)
end

function long_to_bytes(n)
    return number_to_bytes(n, 8)
end

function float_to_bytes(n)
    local sign = 0
    if n < 0 then
        sign = 1
        n = -n
    end

    local mantissa, exponent = math.frexp(n)
    if n == 0 then
        mantissa = 0
        exponent = 0
    else
        mantissa = (mantissa * 2 - 1) * (2^23)
        exponent = exponent + 126
    end

    local b1 = bit.bor(bit.lshift(sign, 7), bit.rshift(exponent, 1))
    local b2 = bit.bor(bit.lshift(bit.band(exponent, 1), 7), bit.rshift(mantissa, 16))
    local b3 = bit.band(bit.rshift(mantissa, 8), 0xFF)
    local b4 = bit.band(mantissa, 0xFF)

    return string.char(b4, b3, b2, b1)
end

function double_to_bytes(n)
    local sign = 0
    if n < 0 then
        sign = 1
        n = -n
    end

    local mantissa, exponent = math.frexp(n)
    if n == 0 then
        mantissa = 0
        exponent = 0
    else
        mantissa = (mantissa * 2 - 1) * (2^52)
        exponent = exponent + 1022
    end

    local b1 = bit.bor(bit.lshift(sign, 7), bit.rshift(exponent, 4))
    local b2 = bit.bor(bit.lshift(bit.band(exponent, 0xF), 4), bit.rshift(mantissa, 48))
    local b3 = bit.band(bit.rshift(mantissa, 40), 0xFF)
    local b4 = bit.band(bit.rshift(mantissa, 32), 0xFF)
    local b5 = bit.band(bit.rshift(mantissa, 24), 0xFF)
    local b6 = bit.band(bit.rshift(mantissa, 16), 0xFF)
    local b7 = bit.band(bit.rshift(mantissa, 8), 0xFF)
    local b8 = bit.band(mantissa, 0xFF)

    return string.char(b8, b7, b6, b5, b4, b3, b2, b1)
end

br.vm.file = "";

br.literal = function (...)
    local _position = #br.vm.file;
    
    for k,v in pairs({...}) do
        br.vm.file = br.vm.file .. v
    end

    return _position;
end

br.decimal = function (...)
    local _position = #br.vm.file;
    
    for k,v in pairs({...}) do
        br.vm.file = br.vm.file .. string.char(v)
    end

    return _position;
end

br.dstring = function (...)
    local _position = #br.vm.file;
    
    for k,v in pairs({...}) do
        br.vm.file = br.vm.file .. string.char(v)
    end

    return _position;
end

br.short = function (...)
    local _position = #br.vm.file;
    
    for k,v in pairs({...}) do
        br.vm.file = br.vm.file .. short_to_bytes(v)
    end

    return _position;
end

br.int = function (...)
    local _position = #br.vm.file;
    
    for k,v in pairs({...}) do
        if type(v) == "string" then
            return _position;
        end
        br.vm.file = br.vm.file .. int_to_bytes(v)
    end

    return _position;
end

br.long = function (...)
    local _position = #br.vm.file;
    
    for k,v in pairs({...}) do
        br.vm.file = br.vm.file .. long_to_bytes(v)
    end

    return _position;
end

br.float = function (...)
    local _position = #br.vm.file;
    
    for k,v in pairs({...}) do
        br.vm.file = br.vm.file .. float_to_bytes(v)
    end

    return _position;
end

br.double = function (...)
    local _position = #br.vm.file;
    
    for k,v in pairs({...}) do
        br.vm.file = br.vm.file .. double_to_bytes(v)
    end

    return _position;
end





-- autos


br.STATE = function (data)
    local _position = #br.vm.file;
    if not data then
        br.vm.file = br.vm.file .. string.char(0);
        return _position;
    end
    br.vm.file = br.vm.file .. string.char(0) .. string.char(data);
    return _position;
end

br.SET = function (position, ...)
    local data = type(({...})[1]) == "string" and ({...})[1] or dstring(...);
    local size = #data;
    local _position = #br.vm.file;

    if not position then
        br.vm.file = br.vm.file .. string.char(1);
        return _position;
    end

    br.vm.file = br.vm.file .. string.char(1) .. int_to_bytes(position) .. int_to_bytes(size) .. data;
    return _position;
end

br.INSERT = function (position, ...)
    local data = type(({...})[1]) == "string" and ({...})[1] or br.dstring(...);
    local _position = #br.vm.file;
    local size = #data;

    if not position then
        br.vm.file = br.vm.file .. string.char(2);
        return _position;
    end

    br.vm.file = br.vm.file .. string.char(2) .. int_to_bytes(position) .. int_to_bytes(size) .. data;
    return _position;
end

br.DELETE = function (position, size)
    local _position = #br.vm.file;
    if not position then
        br.vm.file = br.vm.file .. string.char(3);
        return _position;
    end
    br.vm.file = br.vm.file .. string.char(3) .. int_to_bytes(position) .. int_to_bytes(size);
    return _position;
end

br.MOVE = function (origin, destiny, size)
    local _position = #br.vm.file;
    if not origin then
        br.vm.file = br.vm.file .. string.char(4);
        return _position;
    end
    br.vm.file = br.vm.file .. string.char(4) .. int_to_bytes(origin) .. int_to_bytes(destiny) .. int_to_bytes(size);
    return _position;
end

br.SWAP = function (origin, destiny, size)
    local _position = #br.vm.file;
    if not origin then
        br.vm.file = br.vm.file .. string.char(5);
        return _position;
    end
    br.vm.file = br.vm.file .. string.char(5) .. int_to_bytes(origin) .. int_to_bytes(destiny) .. int_to_bytes(size);
    return _position;
end

br.SHIFT = function (position, size, direction)
    local _position = #br.vm.file;
    if not position then
        br.vm.file = br.vm.file .. string.char(6);
        return _position;
    end
    br.vm.file = br.vm.file .. string.char(6) .. int_to_bytes(position) .. int_to_bytes(size) .. string.char(direction);
    return _position;
end

br.RANDOM = function (position, size)
    local _position = #br.vm.file;
    if not position then
        br.vm.file = br.vm.file .. string.char(7);
        return _position;
    end
    br.vm.file = br.vm.file .. string.char(7) .. int_to_bytes(position) .. int_to_bytes(size);
    return _position;
end

br.COPY = function (origin, destiny, size)
    local _position = #br.vm.file;
    if not origin then
        br.vm.file = br.vm.file .. string.char(8);
        return _position;
    end
    br.vm.file = br.vm.file .. string.char(8) .. int_to_bytes(origin) .. int_to_bytes(destiny) .. int_to_bytes(size);
    return _position;
end

br.FILL = function (position, size, data)
    local _position = #br.vm.file;
    if not position then
        br.vm.file = br.vm.file .. string.char(9);
        return _position;
    end
    br.vm.file = br.vm.file .. string.char(9) .. int_to_bytes(position) .. int_to_bytes(size) .. string.char(data);
    return _position;
end

br.REVERSE = function (position, size)
    local _position = #br.vm.file;
    if not position then
        br.vm.file = br.vm.file .. string.char(10);
        return _position;
    end
    br.vm.file = br.vm.file .. string.char(10) .. int_to_bytes(position) .. int_to_bytes(size);
    return _position;
end

br.SORT = function (position, size)
    local _position = #br.vm.file;
    if not position then
        br.vm.file = br.vm.file .. string.char(11);
        return _position;
    end
    br.vm.file = br.vm.file .. string.char(11) .. int_to_bytes(position) .. int_to_bytes(size);
    return _position;
end

br.FIND = function (position, size, result, ...)
    local pattern = type(({...})[1]) == "string" and ({...})[1] or br.dstring(...);
    local patternsize = #pattern;
    local _position = #br.vm.file;
    if not position then
        br.vm.file = br.vm.file .. string.char(12);
        return _position;
    end
    br.vm.file = br.vm.file .. string.char(12) .. int_to_bytes(position) .. int_to_bytes(size) .. int_to_bytes(result) .. int_to_bytes(patternsize) .. pattern;
    return _position;
end

br.IF = function (position, size)
    local _position = #br.vm.file;
    if not position then
        br.vm.file = br.vm.file .. string.char(13);
        return _position;
    end
    br.vm.file = br.vm.file .. string.char(13) .. int_to_bytes(position) .. int_to_bytes(size);
    return _position;
end


br.EQUAL = function (position1, position2, size, result)
    local _position = #br.vm.file;
    if not position1 then
        br.vm.file = br.vm.file .. string.char(15);
        return _position;
    end
    br.vm.file = br.vm.file .. string.char(15) .. int_to_bytes(position1) .. int_to_bytes(position2) .. int_to_bytes(size) .. int_to_bytes(result);
    return _position;
end

br.NOT_EQUAL = function (position1, position2, size, result)
    local _position = #br.vm.file;
    if not position1 then
        br.vm.file = br.vm.file .. string.char(16);
        return _position;
    end

    br.vm.file = br.vm.file .. string.char(16) .. int_to_bytes(position1) .. int_to_bytes(position2) .. int_to_bytes(size) .. int_to_bytes(result);
    return _position;
end

br.GREATER = function (position1, position2, size, result)
    local _position = #br.vm.file;
    if not position1 then
        br.vm.file = br.vm.file .. string.char(17);
        return _position;
    end
    br.vm.file = br.vm.file .. string.char(17) .. int_to_bytes(position1) .. int_to_bytes(position2) .. int_to_bytes(size) .. int_to_bytes(result);
    return _position;
end

br.LESS = function (position1, position2, size, result)
    local _position = #br.vm.file;
    if not position1 then
        br.vm.file = br.vm.file .. string.char(18);
        return _position;
    end
    br.vm.file = br.vm.file .. string.char(18) .. int_to_bytes(position1) .. int_to_bytes(position2) .. int_to_bytes(size) .. int_to_bytes(result);
    return _position;
end

br.GREATER_OR_EQUAL = function (position1, position2, size, result)
    local _position = #br.vm.file;
    if not position1 then
        br.vm.file = br.vm.file .. string.char(19);
        return _position;
    end
    br.vm.file = br.vm.file .. string.char(19) .. int_to_bytes(position1) .. int_to_bytes(position2) .. int_to_bytes(size) .. int_to_bytes(result);
    return _position;
end

br.LESS_OR_EQUAL = function (position1, position2, size, result)
    local _position = #br.vm.file;
    if not position1 then
        br.vm.file = br.vm.file .. string.char(20);
        return _position;
    end
    br.vm.file = br.vm.file .. string.char(20) .. int_to_bytes(position1) .. int_to_bytes(position2) .. int_to_bytes(size) .. int_to_bytes(result);
    return _position;
end

br.AND = function (position, size, result)
    local _position = #br.vm.file;
    if not position then
        br.vm.file = br.vm.file .. string.char(21);
        return _position;
    end
    br.vm.file = br.vm.file .. string.char(21) .. int_to_bytes(position) .. int_to_bytes(size) .. int_to_bytes(result);
    return _position;
end

br.OR = function (position, size, result)
    local _position = #br.vm.file;
    if not position then
        br.vm.file = br.vm.file .. string.char(22);
        return _position;
    end
    br.vm.file = br.vm.file .. string.char(22) .. int_to_bytes(position) .. int_to_bytes(size) .. int_to_bytes(result);
    return _position;
end

br.PRINT = function (position, size)
    local _position = #br.vm.file;
    if not position then
        br.vm.file = br.vm.file .. string.char(23);
        return _position;
    end
    br.vm.file = br.vm.file .. string.char(23) .. int_to_bytes(position) .. int_to_bytes(size);
    return _position;
end

br.GOTO = function (position)
    local _position = #br.vm.file;
    if not position then
        br.vm.file = br.vm.file .. string.char(24);
        return _position;
    end
    br.vm.file = br.vm.file .. string.char(24) .. int_to_bytes(position);
    return _position;
end



br.ADD = function (position1, position2, type)
    local _position = #br.vm.file;
    if not position1 then
        br.vm.file = br.vm.file .. string.char(26);
        return _position;
    end
    br.vm.file = br.vm.file .. string.char(26) .. int_to_bytes(position1) .. int_to_bytes(position2) .. string.char(type);
    return _position;
end

br.SUB = function (position1, position2, type)
    local _position = #br.vm.file;
    if not position1 then
        br.vm.file = br.vm.file .. string.char(27);
        return _position;
    end
    br.vm.file = br.vm.file .. string.char(27) .. int_to_bytes(position1) .. int_to_bytes(position2) .. string.char(type);
    return _position;
end

br.MUL = function (position1, position2, type)
    local _position = #br.vm.file;
    if not position1 then
        br.vm.file = br.vm.file .. string.char(28);
        return _position;
    end
    br.vm.file = br.vm.file .. string.char(28) .. int_to_bytes(position1) .. int_to_bytes(position2) .. string.char(type);
    return _position;
end

br.DIV = function (position1, position2, type)
    local _position = #br.vm.file;
    if not position1 then
        br.vm.file = br.vm.file .. string.char(29);
        return _position;
    end
    br.vm.file = br.vm.file .. string.char(29) .. int_to_bytes(position1) .. int_to_bytes(position2) .. string.char(type);
    return _position;
end

br.INCREMENT = function (position, type)
    local _position = #br.vm.file;
    if not position then
        br.vm.file = br.vm.file .. string.char(30);
        return _position;
    end
    br.vm.file = br.vm.file .. string.char(30) .. int_to_bytes(position) .. string.char(type);
    return _position;
end

br.DECREMENT = function (position, type)
    local _position = #br.vm.file;
    br.vm.file = br.vm.file .. string.char(31) .. int_to_bytes(position) .. string.char(type);
    return _position;
end







if br.utils.table.includes(arg, "-o") or br.utils.table.includes(arg, "--output") then
    local temp = br.utils.table.find(arg, "-o") or br.utils.table.find(arg, "--output")
    br.vm.outputpath = arg[temp + 1]
    table.remove(arg, temp)
    table.remove(arg, temp)
end

if br.utils.table.includes(arg, "--serialize") then
    local position = br.utils.table.find(arg, "--serialize");
    table.remove(arg, position);
    br.filepath = arg[2]
    br.filetxt = br.utils.file.load.text(br.filepath)
    br.filetxt = br.utils.string.replace(br.filetxt, "//.-\n", "");


    br.filetxt = br.utils.string.replace(br.filetxt, "\n", " ");
    br.filetxt = br.utils.string.replace(br.filetxt, "\r", " ");
    br.filetxt = br.utils.string.replace(br.filetxt, "\t", " ");
    br.filetxt = br.utils.string.replace(br.filetxt, "%.", " ");
    br.filetxt = br.utils.string.replace(br.filetxt, "%,", " ");
    while br.utils.string.includes(br.filetxt, "  ") do
        br.filetxt = br.utils.string.replace(br.filetxt, "  ", " ");
    end
    
    br.splited = br.utils.string.split(br.filetxt, " ")
    br.result = ""
    for i = 1, #br.splited do
        if br.splited[i] and tonumber(br.splited[i]) then
            br.result = br.result .. string.char(tonumber(br.splited[i]))
        end
    end
    br.utils.file.save.text(br.vm.outputpath, br.result)
    os.exit()
end

if br.utils.table.includes(arg, "--deserialize") then
    local position = br.utils.table.find(arg, "--deserialize");
    table.remove(arg, position);
    br.filepath = arg[2]
    br.filetxt = br.utils.file.load.text(br.filepath)
    br.result = ""
    for i = 1, #br.filetxt do
        br.result = br.result .. string.byte(br.filetxt:sub(i, i)) .. " "
    end
    if br.vm.outputpath then
        br.utils.file.save.text(br.vm.outputpath, br.result)
    else
        br.utils.file.save.text(br.utils.string.concat(arg[2], ".txt"), br.result)
    end
    os.exit()
end

if br.utils.table.includes(arg, "--compile") then
    local position = br.utils.table.find(arg, "--compile");
    table.remove(arg, position);
    br.filepath = arg[2]
    br.filetxt = br.utils.file.load.text(br.filepath)
    br.filetxt = br.utils.string.replace(br.filetxt, "//.-\n", "");
    br.bruter.eval(br.filetxt)
    br.utils.file.save.text(br.vm.outputpath, br.vm.file)
end

if br.utils.table.includes(arg, "--help") or br.utils.table.includes(arg,"-h") then
    print("Usage: bruter <source file> [-o <output file>] [-h] [-v] [--help] [--version]")
    print("Options:")
    print("  --help        Display this information")
    print("  -h            Display this information")
    print("  --output      Output the file to the specified path")
    print("  -o            Output the file to the specified path")
    print("  --serialize   Serialize a file")
    print("  --deserialize Deserialize a file")

    os.exit(0)
end