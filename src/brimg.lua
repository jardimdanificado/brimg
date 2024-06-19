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
    print(n)
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
    local position = #br.vm.file;
    
    for k,v in pairs({...}) do
        br.vm.file = br.vm.file .. v
    end

    return position;
end

br.decimal = function (...)
    local position = #br.vm.file;
    
    for k,v in pairs({...}) do
        br.vm.file = br.vm.file .. string.char(v)
    end

    return position;
end

br.short = function (...)
    local position = #br.vm.file;
    
    for k,v in pairs({...}) do
        br.vm.file = br.vm.file .. short_to_bytes(v)
    end

    return position;
end

br.int = function (...)
    local position = #br.vm.file;
    
    for k,v in pairs({...}) do
        if type(v) == "string" then
            return position;
        end
        br.vm.file = br.vm.file .. int_to_bytes(v)
    end

    return position;
end

br.long = function (...)
    local position = #br.vm.file;
    
    for k,v in pairs({...}) do
        br.vm.file = br.vm.file .. long_to_bytes(v)
    end

    return position;
end

br.float = function (...)
    local position = #br.vm.file;
    
    for k,v in pairs({...}) do
        br.vm.file = br.vm.file .. float_to_bytes(v)
    end

    return position;
end

br.double = function (...)
    local position = #br.vm.file;
    
    for k,v in pairs({...}) do
        br.vm.file = br.vm.file .. double_to_bytes(v)
    end

    return position;
end


br.STATE = function ()
    local position = #br.vm.file;
    br.vm.file = br.vm.file .. string.char(0);
    return position;
end

br.SET = function ()
    local position = #br.vm.file;
    br.vm.file = br.vm.file .. string.char(1);
    return position;
end

br.INSERT = function ()
    local position = #br.vm.file;
    br.vm.file = br.vm.file .. string.char(2);
    return position;
end

br.DELETE = function ()
    local position = #br.vm.file;
    br.vm.file = br.vm.file .. string.char(3);
    return position;
end

br.MOVE = function ()
    local position = #br.vm.file;
    br.vm.file = br.vm.file .. string.char(4);
    return position;
end

br.SWAP = function ()
    local position = #br.vm.file;
    br.vm.file = br.vm.file .. string.char(5);
    return position;
end

br.SHIFT = function ()
    local position = #br.vm.file;
    br.vm.file = br.vm.file .. string.char(6);
    return position;
end

br.RANDOM = function ()
    local position = #br.vm.file;
    br.vm.file = br.vm.file .. string.char(7);
    return position;
end

br.COPY = function ()
    local position = #br.vm.file;
    br.vm.file = br.vm.file .. string.char(8);
    return position;
end

br.FILL = function ()
    local position = #br.vm.file;
    br.vm.file = br.vm.file .. string.char(9);
    return position;
end

br.REVERSE = function ()
    local position = #br.vm.file;
    br.vm.file = br.vm.file .. string.char(10);
    return position;
end

br.SORT = function ()
    local position = #br.vm.file;
    br.vm.file = br.vm.file .. string.char(11);
    return position;
end

br.FIND = function ()
    local position = #br.vm.file;
    br.vm.file = br.vm.file .. string.char(12);
    return position;
end

br["IF"] = function ()
    local position = #br.vm.file;
    br.vm.file = br.vm.file .. string.char(13);
    return position;
end


br.EQUAL = function ()
    local position = #br.vm.file;
    br.vm.file = br.vm.file .. string.char(15);
    return position;
end

br.NOT_EQUAL = function ()
    local position = #br.vm.file;
    br.vm.file = br.vm.file .. string.char(16);
    return position;
end

br.GREATER = function ()
    local position = #br.vm.file;
    br.vm.file = br.vm.file .. string.char(17);
    return position;
end

br.LESS = function ()
    local position = #br.vm.file;
    br.vm.file = br.vm.file .. string.char(18);
    return position;
end

br.GREATER_OR_EQUAL = function ()
    local position = #br.vm.file;
    br.vm.file = br.vm.file .. string.char(19);
    return position;
end

br.LESS_OR_EQUAL = function ()
    local position = #br.vm.file;
    br.vm.file = br.vm.file .. string.char(20);
    return position;
end

br["AND"] = function ()
    local position = #br.vm.file;
    br.vm.file = br.vm.file .. string.char(21);
    return position;
end

br["OR"] = function ()
    local position = #br.vm.file;
    br.vm.file = br.vm.file .. string.char(22);
    return position;
end

br.PRINT = function ()
    local position = #br.vm.file;
    br.vm.file = br.vm.file .. string.char(23);
    return position;
end


br["GOTO"] = function ()
    local position = #br.vm.file;
    br.vm.file = br.vm.file .. string.char(24);
    return position;
end



br.ADD = function ()
    local position = #br.vm.file;
    br.vm.file = br.vm.file .. string.char(26);
    return position;
end

br.SUB = function ()
    local position = #br.vm.file;
    br.vm.file = br.vm.file .. string.char(27);
    return position;
end

br.MUL = function ()
    local position = #br.vm.file;
    br.vm.file = br.vm.file .. string.char(28);
    return position;
end

br.DIV = function ()
    local position = #br.vm.file;
    br.vm.file = br.vm.file .. string.char(29);
    return position;
end

br.INCREMENT = function ()
    local position = #br.vm.file;
    br.vm.file = br.vm.file .. string.char(30);
    return position;
end

br.DECREMENT = function ()
    local position = #br.vm.file;
    br.vm.file = br.vm.file .. string.char(31);
    return position;
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