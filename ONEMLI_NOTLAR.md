
local is_unix = package.config.sub(1, 1) == "/"

function Create_Folder_Structure(is_unix, project_type, project_name)

    local include_folder 
    local src_folder 
    local lib_folder
    local object_folder
    local bin_folder
    local history_folder 

    if is_unix then
        include_folder = project_name .. "/include"
        src_folder = project_name .. "/src"
        lib_folder = project_name .. "/lib"
        object_folder = project_name .. "/obj"
        bin_folder = project_name .. "/bin"
        history_folder = project_name .. "/hist"
    else
        include_folder = project_name .. "\\include"
        src_folder = project_name .. "\\src"
        lib_folder = project_name .. "\\lib"
        object_folder = project_name .. "\\obj"
        bin_folder = project_name .. "\\bin"
        history_folder = project_name .. "\\hist"
    end

    local function inform(color, type, message)
        if color == "red" then
            io.write("\27[31m" .. message .. " " .. project_name .. " " .. type .. " !" .. "\27[0m\n")
        elseif color == "green" then
            io.write("\27[32m" .. message .. " " .. project_name .. " " .. type .. " !" .. "\27[0m\n")
        else
            io.write("Color not defined")
        end
    end

    local function give_information(message)
        io.write("\n\27[94m" .. message .. "\27[0m\n")
    end

    if project_type == "executable" then
        local status

        if is_unix then
            status = os.execute("mkdir -p \"" .. include_folder .. "\"")
            os.execute("mkdir -p \"" .. src_folder .. "\"")
            os.execute("mkdir -p \"" .. lib_folder .. "\"")
            os.execute("mkdir -p \"" .. object_folder .. "\"")
            os.execute("mkdir -p \"" .. bin_folder .. "\"")
            os.execute("mkdir -p \"" .. history_folder .. "\"")
        else
            status = os.execute("mkdir \"" .. include_folder .. "\"")
            os.execute("mkdir \"" .. src_folder .. "\"")
            os.execute("mkdir \"" .. lib_folder .. "\"")
            os.execute("mkdir \"" .. object_folder .. "\"")
            os.execute("mkdir \"" .. bin_folder .. "\"")
            os.execute("mkdir \"" .. history_folder .. "\"")
        end
        if status then
            inform("green", "executable", "Succesfully created")
            give_information("Source files (.c/.cpp) -> src\nInclude files(.h/.hpp) -> include\nLibraries(.a/.so) -> lib\nObject files(.o/.obj) -> obj\nBinary files(.elf, .exe) -> bin\nHistory Files(files that remain from last build) -> hist\n")
        else 
            inform("red", "executable", "There is an error while creating")
        end
    elseif project_type == "static" then 
        local status

        if is_unix then
            status = os.execute("mkdir -p \"" .. include_folder .. "\"")
            os.execute("mkdir -p \"" .. src_folder .. "\"")
            os.execute("mkdir -p \"" .. lib_folder .. "\"")
            os.execute("mkdir -p \"" .. object_folder .. "\"")
            os.execute("mkdir -p \"" .. bin_folder .. "\"")
            os.execute("mkdir -p \"" .. history_folder .. "\"")
        else
            status = os.execute("mkdir \"" .. include_folder .. "\"")
            os.execute("mkdir \"" .. src_folder .. "\"")
            os.execute("mkdir \"" .. lib_folder .. "\"")
            os.execute("mkdir \"" .. object_folder .. "\"")
            os.execute("mkdir \"" .. bin_folder .. "\"")
            os.execute("mkdir \"" .. history_folder .. "\"")
        end

        if status then
            inform("green", "static library", "Succesfully created")
            give_information("Source files (.c/.cpp) -> src\nInclude files(.h/.hpp) -> include\nLibraries(ONLY STATIC .a) -> lib\nObject files(.o/.obj) -> obj\nBinary files(.a) -> bin\nHistory Files(files that remain from last build) -> hist\n")
        else 
            inform("red", "static library", "There is an error while creating")
        end
    elseif project_type == "shared" then
        local status

        if is_unix then
            status = os.execute("mkdir -p \"" .. include_folder .. "\"")
            os.execute("mkdir -p \"" .. src_folder .. "\"")
            os.execute("mkdir -p \"" .. lib_folder .. "\"")
            os.execute("mkdir -p \"" .. object_folder .. "\"")
            os.execute("mkdir -p \"" .. bin_folder .. "\"")
            os.execute("mkdir -p \"" .. history_folder .. "\"")
        else
            status = os.execute("mkdir \"" .. include_folder .. "\"")
            os.execute("mkdir \"" .. src_folder .. "\"")
            os.execute("mkdir \"" .. lib_folder .. "\"")
            os.execute("mkdir \"" .. object_folder .. "\"")
            os.execute("mkdir \"" .. bin_folder .. "\"")
            os.execute("mkdir \"" .. history_folder .. "\"")
        end
        if status then
            inform("green", "shared library", "Succesfully created")
            give_information("Source files (.c/.cpp) -> src\nInclude files(.h/.hpp) -> include\nLibraries(ONLY STATIC .a) -> lib\nObject files(.o/.obj) -> obj\nBinary files(.so/.dll) -> bin\nHistory Files(files that remain from last build) -> hist\n")
        else 
            inform("red", "shared library", "There is an error while creating")
        end
    elseif project_type == "header" then
        local status 

        if is_unix then
            status = os.execute("mkdir -p \"" .. include_folder .. "\"")
        else
            status = os.execute("mkdir \"" .. include_folder .. "\"")
        end
        if status then
            inform("green", "static library", "Succesfully created")
            give_information("Include files(.h/.hpp) -> include")
        else 
            inform("red", "static library", "There is an error while creating")
        end
    end

end

function Compile_Project()
    local project_path

    if is_unix then
        project_path = io.popen("pwd"):read("l")
    else 
        project_path = io.popen("cd"):read("l")
    end

    local include_folder 
    local src_folder 
    local lib_folder
    local object_folder
    local bin_folder
    local history_folder 

    if is_unix then
        include_folder = project_path .. "/include"
        src_folder = project_path .. "/src"
        lib_folder = project_path .. "/lib"
        object_folder = project_path .. "/obj"
        bin_folder = project_path .. "/bin"
        history_folder = project_path .. "/hist"
    else
        include_folder = project_path .. "\\include"
        src_folder = project_path .. "\\src"
        lib_folder = project_path .. "\\lib"
        object_folder = project_path .. "\\obj"
        bin_folder = project_path .. "\\bin"
        history_folder = project_path .. "\\hist"
    end

    local source_file_paths = {}

    if is_unix then
        local command = 'ls -p "' .. src_folder .. '" | grep -v /'
        local files = io.popen(command)
        if files then
            for file_name in files:lines() do
                table.insert(source_file_paths, src_folder .. "/" .. file_name)
            end
            files:close() 
        end
    else 
        local command = 'dir "' .. src_folder .. '" /b'
        local files = io.popen(command)
        if files then
            for file_name in files:lines() do
                table.insert(source_file_paths, src_folder .. "\\" .. file_name)
            end
            files:close()
        end
    end

    local command = "g++ -c "

    for i, v in ipairs(source_file_paths) do
        command = command .. v .. " "
    end

    os.execute(command)

    local object_file_paths = {}

    if is_unix then
        local command = 'ls -p "' .. project_path .. '" | grep -v /'
        local files = io.popen(command)
        if files then
            for file_name in files:lines() do
                if file_name:sub(-1) == "o" then
                    table.insert(object_file_paths, file_name)
                end
            end
            files:close() 
        end
    else 
        local command = 'dir "' .. project_path .. '" /b'
        local files = io.popen(command)
        if files then
            for file_name in files:lines() do
                if file_name:sub(-1) == "o" then
                    table.insert(object_file_paths, file_name)
                end
            end
            files:close()
        end
    end

    for i, v in ipairs(object_file_paths) do
        if is_unix then
            os.execute("mv " .. project_path .. "/" .. v .. " " .. object_folder .. "/" .. v)
        else
            os.execute("move " .. project_path .. "\\" .. v .. " " .. object_folder .. "\\" .. v)
        end
    end
end

function Link_Project()
    local project_path

    if is_unix then
        project_path = io.popen("pwd"):read("l")
    else 
        project_path = io.popen("cd"):read("l")
    end

    local include_folder 
    local src_folder 
    local lib_folder
    local object_folder
    local bin_folder
    local history_folder 

    if is_unix then
        include_folder = project_path .. "/include"
        src_folder = project_path .. "/src"
        lib_folder = project_path .. "/lib"
        object_folder = project_path .. "/obj"
        bin_folder = project_path .. "/bin"
        history_folder = project_path .. "/hist"
    else
        include_folder = project_path .. "\\include"
        src_folder = project_path .. "\\src"
        lib_folder = project_path .. "\\lib"
        object_folder = project_path .. "\\obj"
        bin_folder = project_path .. "\\bin"
        history_folder = project_path .. "\\hist"
    end

    local object_files = {}

    if is_unix then
        local command = 'ls -p "' .. object_folder .. '" | grep -v /'
        local files = io.popen(command)
        if files then
            for file_name in files:lines() do
                if file_name:sub(-1) == "o" then
                    table.insert(object_files, file_name)
                end
            end
            files:close() 
        end
    else 
        local command = 'dir "' .. object_folder .. '" /b'
        local files = io.popen(command)
        if files then
            for file_name in files:lines() do
                if file_name:sub(-1) == "o" then
                    table.insert(object_files, file_name)
                end
            end
            files:close()
        end
    end

    local command = "g++ "

    for i, v in ipairs(object_files) do
        if is_unix then
            command = command .. " " .. object_folder .. "/" .. v
        else
            command = command .. " " .. object_folder .. "\\" .. v
        end
    end

    command = command .. " -o executable.exe"

    os.execute(command)
end

if arg[1] == "create" then
    local project_type = arg[2]
    local project_name = arg[3]

    Create_Folder_Structure(is_unix, project_type, project_name)
elseif arg[1] == "compile" then
    Compile_Project()
elseif arg[1] == "link" then
    Link_Project()
elseif arg[1] == "build" then
    Compile_Project()
    Link_Project()
end
