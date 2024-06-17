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