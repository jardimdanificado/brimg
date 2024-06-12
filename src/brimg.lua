br.loadfile = br.utils.file.load.text;
br.savefile = br.utils.file.save.text;

if br.utils.table.includes(arg, "-o") or br.utils.table.includes(arg, "--output") then
    local temp = br.utils.table.find(arg, "-o") or br.utils.table.find(arg, "--output")
    br.vm.outputpath = arg[temp + 1]
    table.remove(arg, temp)
    table.remove(arg, temp)
end

if br.utils.table.includes(arg, "--serialize") then
    local position = br.utils.table.find(arg, "--serialize");
    table.remove(arg, position);
    br.help(arg)
    br.filepath = arg[2]
    br.filetxt = br.utils.file.load.text(br.filepath)
    -- remove all lines that start with //
    br.filetxt = br.utils.string.replace(br.filetxt, "//.-\n", "");


    -- remove all newlines, tabs and double spaces
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
    br.help(arg)
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

if br.utils.table.includes(arg, "--help") or br.utils.table.includes(arg,"-h") then
    print("Usage: bruter <source file> [-o <output file>] [-h] [-v] [--help] [--version]")
    print("Options:")
    print("  --help        Display this information")
    print("  -h            Display this information")
    print("  --output      Output the file to the specified path")
    print("  -o            Output the file to the specified path")
    print("  --serialize   Serialize a file")
    print("  --deserialize Deserialize a file")
    --[[print("  --version  Display the version")]]
    --[[print("  -v         Display the version")]]

    os.exit(0)
end

br.vm.c_code_cache = "";

-- yeah byte has 1 byte size, but we use 4 bytes for it for alignment reasons;
-- same for short;
br.typesizes = {byte = 4, short = 4, int = 4, long = 8, float = 4, double = 8};

br.structs = {};

-- manually allocate each member of a struct; 
br.manual = function (...) 
    local args = {...};
    local obj = {};
    for i=2, #args, 3 do
        table.insert(obj, {index=args[i], type=args[i+1], name=args[i+2]});
    end
    obj.name = args[1];
    table.insert(br.structs, obj);
end;

br.auto = function (...)
    local args = {...};
    local obj = {};
    local index = 0;
    for i=2, #args, 2 do
        local _type = args[i];
        local name = args[i+1];
        local size = tonumber(_type) and (math.ceil(tonumber(_type) / 4) * 4) or br.typesizes[_type];
        table.insert(obj, {index=index, type=_type, name=name});
        index = index + size;
    end
    obj.name = args[1];
    table.insert(br.structs, obj);
end;


br.baked_structs = function ()
    local finaltxt = "";
    for structType ,obj in ipairs(br.structs) do
        local tmp = "typedef struct \n{\n";
        for k,v in ipairs(obj) do
            if tonumber(v.type) then 
                local size = string.match(v.type, "%d+");
                size = (math.ceil(tonumber(size) / 4) * 4)
                tmp = tmp .. "    byte " .. v.name .. "[" .. size .. "];\n";
            else
                tmp = tmp .. "    " .. v.type .. " " .. v.name .. ";\n";
            end
        end
        tmp = tmp .. "} " .. obj.name .. ";\n\n";
        tmp = tmp .. obj.name .. " get_" .. obj.name .. "(Disk *disk, int index)\n{\n";
        tmp = tmp .. "    " .. obj.name .. " _obj;\n";
        for k,v in ipairs(obj) do
            if tonumber(v.type) then 
                local size = string.match(v.type, "%d+");
                size = (math.ceil(tonumber(size) / 4) * 4)
                tmp = tmp .. "    " .. "memcpy(_obj." .. v.name .. ", get_string(*disk, index + " .. v.index .. ", " .. size .. "), " .. size .. ");\n";
            else
                tmp = tmp .. "    " .. "_obj." .. v.name .. " = get_" .. v.type .. "(*disk, index + " .. v.index .. ");\n";
            end
        end
        tmp = tmp .. "    " .. "return _obj;\n";
        tmp = tmp .. "}\n\n";
        
        tmp = tmp .. "void set_" .. obj.name .. "(Disk *disk, int index, " .. obj.name .. " _obj)\n{\n";
            for k,v in ipairs(obj) do
                if tonumber(v.type) then 
                    local size = string.match(v.type, "%d+");
                    size = (math.ceil(tonumber(size) / 4) * 4)
                    tmp = tmp .. "    " .. "set_string(disk, index + " .. v.index .. ", _obj." .. v.name .. ", " .. size .. ");\n";
                else
                    tmp = tmp .. "    " .. "set_" .. v.type .. "(disk, index + " .. v.index .. ", _obj." .. v.name .. ");\n";
                end
            end
            tmp = tmp .. "}\n\n";

        finaltxt = finaltxt .. tmp;
    end
    return finaltxt;
end;

local file = br.loadfile(arg[2]);

-- find #bruter "name" and replace it with the baked struct loaded from the file pointed by "name";
local baked = string.match(file, "#bruter%s\"(.-)\"");
if baked then
    local bakedfile = br.loadfile(baked);
    br.bruter.eval(bakedfile);
    bakedfile = br.baked_structs();
    file = string.gsub(file, "#bruter%s\"(.-)\"", bakedfile);
    br.savefile(arg[3], file);
end