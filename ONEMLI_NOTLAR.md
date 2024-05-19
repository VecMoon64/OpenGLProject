
local is_unix = package.config:sub(1, 1) == "/"

local seperator
local get_current_path_command
local create_dir_command 
local create_file_command 
local move_file_command
local delete_file_command
local run_command 
local copy_file_command
local copy_folder_command

local executable_extension
local static_extension
local shared_extension

if is_unix then
    get_current_path_command = "pwd"
    seperator = "/"
    create_dir_command = "mkdir -p "
    create_file_command = "touch "
    move_file_command = "mv "
    delete_file_command = "rm "
    executable_extension = ""
    static_extension = ".a"
    shared_extension = ".so"
    run_command = "./"
    copy_file_command = "cp "
else 
    get_current_path_command = "cd"
    seperator = "\\"
    create_dir_command = "mkdir "
    create_file_command = "type nul > "
    move_file_command = "move "
    delete_file_command = "del "
    executable_extension = ".exe"
    static_extension = ".a"
    shared_extension = ".dll"
    run_command = ".\\"
    copy_file_command = "copy "
end

local reset = "\27[0m"
local yellow = "\27[33m"
local green = "\27[32m"
local red = "\27[31m"
local cyan = "\27[36m"
local magenta = "\27[35m"

function Create_Folder_Structure(is_unix, project_type, project_name)

    local current_path = io.popen(get_current_path_command):read("l")

    if is_unix then
        current_path = io.popen("pwd"):read("l")
        seperator = "/"
        create_dir_command = "mkdir -p "
        create_file_command = "touch "
    else 
        current_path = io.popen("cd"):read("l")
        seperator = "\\"
        create_dir_command = "mkdir "
        create_file_command = "type nul > "
    end

    local root_folder
    local include_folder 
    local src_folder 
    local lib_folder
    local object_folder
    local history_folder 

    local conf_file 

    root_folder = current_path .. seperator .. project_name
    include_folder = current_path .. seperator .. project_name .. seperator .. "include"
    src_folder = current_path .. seperator .. project_name .. seperator .. "src"
    lib_folder = current_path .. seperator .. project_name .. seperator .. "lib"
    object_folder = current_path .. seperator .. project_name .. seperator .. "obj"
    history_folder = current_path .. seperator .. project_name .. seperator .. "hist"

    local function create_folders(include_enable, src_enable, lib_enable, object_enable, history_enable) 
        local status

        if include_enable then
            status = os.execute(create_dir_command .. "\"" .. include_folder .. "\"")
        end
        if src_enable then
            status = os.execute(create_dir_command .. "\"" .. src_folder .. "\"")
        end
        if lib_enable then
            status = os.execute(create_dir_command .. "\"" .. lib_folder .. "\"")
        end
        if object_enable then
            status = os.execute(create_dir_command .. "\"" .. object_folder .. "\"")
        end
        if history_enable then
            status = os.execute(create_dir_command .. "\"" .. history_folder .. "\"")
        end

        return status
    end

    if project_type == "executable" then
        print(cyan .. "\nCreating executable " .. project_name .. "'s project\n" .. reset)

        local status = create_folders(true, true, true, true, true)

        if status then
            print(green .. "Executable " .. project_name .. "'s project is succesfully created !" .. reset)
            print(yellow .. "\nSource files (.c/.cpp) -> src\nInclude files(.h/.hpp) -> include\nLibraries(.a/.so) -> lib\nObject files(.o/.obj) -> obj\nHistory Files(files that remain from last build) -> hist" .. reset)
            print(magenta .. "\nDont forget to cd into created folder !\n" .. reset)
        else 
            print(red .. "\nThere is an error while creating executable " .. project_name .. "'s project !" .. reset)
        end
    elseif project_type == "static" then 
        print(cyan .. "\nCreating static library " .. project_name .. "'s project\n"  .. reset)

        local status = create_folders(true, true, false, true, true)

        if status then
            print(green .. "Static library " .. project_name .. "'s project is succesfully created !" .. reset)
            print(yellow .. "\nSource files (.c/.cpp) -> src\nInclude files(.h/.hpp) -> include\nObject files(.o/.obj) -> obj\nHistory Files(files that remain from last build) -> hist" .. reset)
            print(magenta .. "\nDont forget to cd into created folder !" .. reset)
        else 
            print(red .. "\nThere is an error while creating static library " .. project_name .. "'s project !" .. reset)
        end
    elseif project_type == "shared" then
        print(cyan .. "\nCreating shared library " .. project_name .. "'s project\n"  .. reset)

        local status = create_folders(true, true, false, true, true)

        if status then
            print(green .. "Shared library " .. project_name .. "'s project is succesfully created !" .. reset)
            print(yellow .. "\nSource files (.c/.cpp) -> src\nInclude files(.h/.hpp) -> include\nObject files(.o/.obj) -> obj\nHistory Files(files that remain from last build) -> hist\n" .. reset)
            print(magenta .. "Dont forget to cd into created folder !" .. reset)
        else 
            print(red .. "\nThere is an error while creating shared library " .. project_name .. "'s project !" .. reset)
        end
    elseif project_type == "header" then
        print(cyan .. "\nCreating header library " .. project_name .. "'s project\n"  .. reset)

        local status = create_folders(true, false, false, false, true)

        if status then
            print(green .. "Header library " .. project_name .. "'s project is succesfully created !\n" .. reset)
            print(yellow .. "Include files(.h/.hpp) -> include\nHistory Files(files that remain from last build) -> hist\n" .. reset)
            print(magenta .. "Dont forget to cd into created folder !" .. reset)
        else 
            print(red .. "\nThere is an error while creating header library" .. project_name .. "'s project !" .. reset)
        end
    end

    local main_program = [[
#include "helper.h"

int main() {
    print_message("Hello from Moony !");
}
    ]]

    if project_type == "executable" then
        os.execute(create_file_command .. src_folder .. seperator .. "main.cpp")
        local main_file = io.open(src_folder .. seperator .. "main.cpp", "w")

        if main_file then
            main_file:write(main_program)
        end
    end

    local helper_program = [[
#include <iostream>

using namespace std;
        
void print_message(const char* message) {
    cout << message << endl;
}
    ]]

    if project_type == "executable" or project_type == "static" or project_type == "shared" then
        os.execute(create_file_command .. src_folder .. seperator .. "helper.cpp")
        local helper_file = io.open(src_folder .. seperator .. "helper.cpp", "w")
    
        if helper_file then
            helper_file:write(helper_program)
        end
    end

    local helper_header = [[
void print_message(const char* message);
    ]]

    if project_type == "executable" or project_type == "static" or project_type == "shared" or project_type == "header" then
        os.execute(create_file_command .. include_folder .. seperator .. "helper.h")
        local helper_header_file = io.open(include_folder .. seperator .. "helper.h", "w")
    
        if helper_header_file then
            helper_header_file:write(helper_header)
        end
    end

    os.execute(create_file_command .. project_name .. seperator .. "moony.conf")
    conf_file = io.open(project_name .. seperator .. "moony.conf","w")

    if conf_file then
        conf_file:write(string.format("project_name = %s\nproject_type = %s\nbinary_name = %s\nroot_path = %s\ninclude_path = %s\nsrc_path = %s\nlib_path = %s\nobject_path = %s\nhistory_path = %s\ncommits_enabled = %s\nlast_commit_number = 0\ncompiler_version = 14\nenable_warnings = true", project_name, project_type, project_name, root_folder, include_folder, src_folder, lib_folder, object_folder, history_folder, "false"))
    end
end

function Compile_Project(is_info_enabled)
    local current_path = io.popen(get_current_path_command):read("l")
    local conf_file = io.open(current_path .. seperator .. "moony.conf", "r")
    
    local project_name
    local project_type
    local root_folder
    local include_folder 
    local src_folder 
    local lib_folder
    local object_folder
    local bin_folder
    local history_folder
    local is_commits_enabled 
    local compiler_version
    local enable_warnings

    if conf_file then
        for line in conf_file:lines() do
            local value
            if line then
                if string.find(line, "=") then
                    value = string.sub(line, string.find(line, "=") + 2, #line)
                end
            end

            if string.find(line, "project_name") then
                project_name = value
            elseif string.find(line, "project_type") then
                project_type = value
            elseif string.find(line, "root_path") then
                root_folder = value
            elseif string.find(line, "include_path") then
                include_folder = value
            elseif string.find(line, "src_path") then
                src_folder = value
            elseif string.find(line, "lib_path") then
                lib_folder = value
            elseif string.find(line, "object_path") then
                object_folder = value
            elseif string.find(line, "history_path") then
                history_folder = value
            elseif string.find(line, "commits_enabled") then
                is_commits_enabled = value
            elseif string.find(line, "compiler_version") then
                compiler_version = value
            elseif string.find(line, "enable_warnings") then
                enable_warnings = value
            end
        end
    else
        print(red .. "Configuration file doesn't exists" .. reset)
    end

    local warning_code = ""

    if enable_warnings == "true" then
        warning_code = " -Wall"
    else
        warning_code = ""
    end

    print(cyan .. "\nCompilation of " .. project_name .. " is started...\n" .. reset)

    local source_file_names = {}
    local get_files_in_dir_command 

    if is_unix then
        get_files_in_dir_command = 'ls -p "' .. src_folder .. '" | grep -v /'
    else
        get_files_in_dir_command = 'dir "' .. src_folder .. '" /b'
    end

    local files = io.popen(get_files_in_dir_command)
    if files then
        for file_name in files:lines() do
            table.insert(source_file_names, file_name)
        end
        files:close() 
    end

    local current_time = os.time()
    os.execute(create_dir_command .. "\"" .. history_folder .. seperator .. current_time .. "\"")

    for i, v in ipairs(source_file_names) do
        os.execute(copy_file_command .. src_folder .. seperator .. v .. " " .. history_folder .. seperator .. current_time .. seperator .. v)
    end

    if is_commits_enabled == "true" then
        os.execute(create_dir_command .. "\"" .. history_folder .. seperator .. current_time .. "-commit" .. "\"")
        if is_unix then
            os.execute("rsync -av --exclude=" .. "\'" .. history_folder .. "\' " .. root_folder .. " " .. history_folder .. seperator .. current_time .. "-commit")
        else 
            os.execute("robocopy " .. root_folder .. " " .. history_folder .. seperator .. current_time .. "-commit" .. " /E /XD " .. history_folder)
        end
    end
    conf_file = io.open(current_path .. seperator .. "moony.conf", "r")

    local compile_command = "g++ -c " .. "--std=c++" .. compiler_version .. " " .. warning_code .. " " 
    if project_type == "shared" then
        compile_command = "g++ -c -fPIC " .. "--std=c++" .. compiler_version .. " " .. warning_code .. " " 
    end
    conf_file = io.open(current_path .. seperator .. "moony.conf", "r")

    local last_commit_number_
    if conf_file then
        for line in conf_file:lines() do
            if string.find(line, "last_commit_number") then
                last_commit_number_ = string.sub(line, string.find(line, "=") + 2, #line)
                print(cyan .. "\nHistory number -> " .. last_commit_number_ .. "\n" .. reset)
            end
        end
    end

    local different_source_file_count = 0

    for i, v in ipairs(source_file_names) do
        local file = io.open(src_folder .. seperator .. v)

        if last_commit_number_ ~= 0 then
            local hist_file = io.open(history_folder .. seperator .. last_commit_number_ .. seperator .. v)
            if file and hist_file then
                if file:read("a") == hist_file:read("a") then
                else 
                    different_source_file_count = different_source_file_count + 1
                    compile_command = compile_command .. src_folder .. seperator .. v .. " "
                end
            elseif not hist_file then
                different_source_file_count = different_source_file_count + 1
                compile_command = compile_command .. src_folder .. seperator .. v .. " "
            end
        else
            different_source_file_count = different_source_file_count + 1
            compile_command = compile_command .. src_folder .. seperator .. v .. " "
        end
    end

    if conf_file then
        conf_file:close()
    end
    
    conf_file = io.open(current_path .. seperator .. "moony.conf", "r")

    compile_command = compile_command .. " -I" .. include_folder 

    local lines = {}

    if conf_file then
        for line in conf_file:lines() do
            table.insert(lines, line)
        end
        conf_file:close()
    end

    for i, line in ipairs(lines) do
        if string.find(line, "last_commit_number") then
            local equal_index = string.find(line, "=")
            if equal_index then
                local new_line = "last_commit_number = " .. os.time()
                lines[i] = new_line
                break 
            end
        end
    end

    conf_file = io.open(current_path .. seperator .. "moony.conf", "w")
    if not conf_file then
        print("Error: Could not open the file for writing.")
        return
    end

    for _, line in ipairs(lines) do
        conf_file:write(line .. "\n")
    end
    conf_file:close()

    local succes

    if different_source_file_count > 0 then
        succes = os.execute(compile_command)
    else
        print(red .. "\nAll source files are same with last compile !\n" .. reset)
        succes = false
    end

    if succes then
        print(green .. "Compilation of " .. project_name .. " is succesfully completed\n" .. reset)
    else 
        print(red .. "Compilation of " .. project_name .. " is failed !\n" .. reset)
    end

    local object_file_names = {}
    
    if is_unix then
        get_files_in_dir_command = 'ls -p "' .. root_folder .. '" | grep -v /'
    else
        get_files_in_dir_command = 'dir "' .. root_folder .. '" /b'
    end

    local files = io.popen(get_files_in_dir_command)

    if files then
        for file_name in files:lines() do
            if file_name:sub(-1) == "o" then
                table.insert(object_file_names, file_name)
            end
        end
        files:close() 
    end

    for i, v in ipairs(object_file_names) do
        succes = os.execute(move_file_command .. root_folder .. seperator .. v .. " " .. object_folder .. seperator .. v)
    end

    if succes then
        print(cyan .. "\nMoved object files of " .. project_name .. " to object directory (check in configuration file)\n" .. reset)
    else 
        print(red .. "\nCannot moved object files of " .. project_name .. " to object directory (check in configuration file)\n" .. reset)
    end

    if is_info_enabled then
        if is_info_enabled == "-i" then
            for i, v in ipairs(object_file_names) do
                print("-> " .. yellow .. object_folder .. seperator .. v .. reset)
            end
        end
    else 
        print(magenta .. "To see which object files created use -i flag\n" .. reset)
    end
end

function Link_Project(is_info_enabled)
    local current_path = io.popen(get_current_path_command):read("l")
    local conf_file = io.open(current_path .. seperator .. "moony.conf", "r")
    
    local project_name
    local project_type
    local binary_name
    local root_folder
    local include_folder 
    local src_folder 
    local lib_folder
    local object_folder
    local bin_folder
    local history_folder 
    local compiler_version
    local enable_warnings

    if conf_file then
        for line in conf_file:lines() do
            local value
            if line then
                if string.find(line, "=") then
                    value = string.sub(line, string.find(line, "=") + 2, #line)
                end
            end

            if string.find(line, "project_name") then
                project_name = value
            elseif string.find(line, "project_type") then
                project_type = value
            elseif string.find(line, "binary_name") then
                binary_name = value
            elseif string.find(line, "root_path") then
                root_folder = value
            elseif string.find(line, "include_path") then
                include_folder = value
            elseif string.find(line, "src_path") then
                src_folder = value
            elseif string.find(line, "lib_path") then
                lib_folder = value
            elseif string.find(line, "object_path") then
                object_folder = value
            elseif string.find(line, "history_path") then
                history_folder = value
            elseif string.find(line, "compiler_version") then
                compiler_version = value
            elseif string.find(line, "enable_warnings") then
                enable_warnings = value
            end
        end
    else
        print("Conf file doesnt exists")
    end

    local warning_code = ""

    if enable_warnings == "true" then
        warning_code = " -Wall"
    else
        warning_code = ""
    end

    print(cyan .. "\nLinking of " .. project_name .. " is started...\n" .. reset)

    local get_files_in_dir_command
    local object_files = {}
    
    if is_unix then
        get_files_in_dir_command = 'ls -p "' .. object_folder .. '" | grep -v /'
    else
        get_files_in_dir_command = 'dir "' .. object_folder .. '" /b'
    end

    local files = io.popen(get_files_in_dir_command)

    if files then
        for file_name in files:lines() do
            if file_name:sub(-1) == "o" then
                table.insert(object_files, object_folder .. seperator .. file_name)
            end
        end
        files:close() 
    end

    local link_command
    local succes
    local libraries = {}

    if project_type == "executable" then
        link_command = "g++ " .. "--std=c++" .. compiler_version .. " " .. warning_code .. " " 

        for i, v in ipairs(object_files) do
            link_command = link_command .. v .. " "
        end
    
        if is_unix then
            link_command = link_command .. " -o " .. binary_name
        else
            link_command = link_command .. " -o " .. binary_name .. ".exe"
        end
    
        link_command = link_command .. " -I" .. include_folder .. " -L" .. lib_folder

        local conf_file = io.open(current_path .. seperator .. "moony.conf", "r")

        if conf_file then
            for line in conf_file:lines("l") do
                if string.find(line, "library") then
                    table.insert(libraries, string.sub(line, string.find(line, "=") + 2, #line)) 
                end 
            end
        end

        for i, v in ipairs(libraries) do
            link_command = link_command .. " -l" .. v
        end
        succes = os.execute(link_command)
    elseif project_type == "static" then
        link_command = "ar rcs lib" .. binary_name .. ".a" 

        for i, v in ipairs(object_files) do
            link_command = link_command .. " " .. v .. " "
        end
        succes = os.execute(link_command)
    elseif project_type == "shared" then
        link_command = "g++ -shared " .. "--std=c++" .. compiler_version .. " " .. warning_code .. " " 

        for i, v in ipairs(object_files) do
            link_command = link_command .. " " .. object_folder .. seperator .. v
        end

        if is_unix then
           link_command = link_command .. " -o lib" .. binary_name 
        else 
            link_command = link_command .. " -o lib" .. binary_name .. ".dll"
        end

        succes = os.execute(link_command)
    elseif project_type == "include" then
        print("There is no need to compile for header libraries :)")
    else 
        print("project type is incorrect")
    end

    if succes then
        print(green .. "Linking of " .. project_name .. " is succesfully completed\n" .. reset)
    else 
        print(red .. "Linking of " .. project_name .. " is failed !\n" .. reset)
    end
    
    if is_info_enabled then
        if is_info_enabled == "-i" then
            for i, v in ipairs(libraries) do
                print("-> " .. yellow .. v .. reset)
            end
            for i, v in ipairs(object_files) do
                print("-> " .. yellow .. v .. reset)
            end
        end
    else 
        print(magenta .. "To see which object files linked and libraries used, use -i flag\n" .. reset)
    end
end

function Clean_Project(is_info_enabled)
    local current_path = io.popen(get_current_path_command):read("l")
    local conf_file = io.open(current_path .. seperator .. "moony.conf")
    
    local project_name
    local project_type
    local binary_name
    local root_folder
    local include_folder 
    local src_folder 
    local lib_folder
    local object_folder
    local bin_folder
    local history_folder 

    if conf_file then
        for line in conf_file:lines() do
            local value = string.sub(line, string.find(line, "=") + 2, #line)

            if string.find(line, "project_name") then
                project_name = value
            elseif string.find(line, "project_type") then
                project_type = value
            elseif string.find(line, "binary_name") then
                binary_name = value
            elseif string.find(line, "root_path") then
                root_folder = value
            elseif string.find(line, "include_path") then
                include_folder = value
            elseif string.find(line, "src_path") then
                src_folder = value
            elseif string.find(line, "lib_path") then
                lib_folder = value
            elseif string.find(line, "object_path") then
                object_folder = value
            elseif string.find(line, "history_path") then
                history_folder = value
            end
        end
    else
        print("Conf file doesnt exists")
    end

    print(cyan .. "\nCleaning of " .. project_name .. " is started..." .. reset)

    local get_files_in_dir_command
    local object_files = {}
    
    if is_unix then
        get_files_in_dir_command = 'ls -p "' .. object_folder .. '" | grep -v /'
    else
        get_files_in_dir_command = 'dir "' .. object_folder .. '" /b'
    end

    local files = io.popen(get_files_in_dir_command)

    if files then
        for file_name in files:lines() do
            if file_name:sub(-1) == "o" then
                table.insert(object_files, object_folder .. seperator .. file_name)
            end
        end
        files:close() 
    end

    local clean_command = delete_file_command
    local succes_obj

    if #object_files > 0 then
        for i, v in ipairs(object_files) do
            clean_command = clean_command .. v .. " "
        end
        succes_obj = os.execute(clean_command)
    end

    local succes_bin

    if project_type == "executable" then
        if io.open(root_folder .. seperator .. binary_name .. executable_extension) then
            succes_bin = os.execute(delete_file_command .. " " .. binary_name .. executable_extension)
        end
    elseif project_type == "static" then
        if io.open(root_folder .. seperator .. "lib" .. binary_name .. static_extension) then
            succes_bin = os.execute(delete_file_command .. "lib" .. binary_name .. static_extension)
        end
    elseif project_type == "shared" then
        if io.open(root_folder .. seperator .. "lib" .. binary_name .. shared_extension) then
            succes_bin = os.execute(delete_file_command .. "lib" .. binary_name .. shared_extension)
        end
    elseif project_type == "header" then
        print("There is no need for clean project")
    end

    if succes_obj and succes_bin then
        print(green .. "\nCleaning of " .. project_name .. " is succesfully completed\n" .. reset)
    else 
        print(red .. "\nCleaning of " .. project_name .. " is failed !\n" .. reset)
    end

    if is_info_enabled then
        if is_info_enabled == "-i" then
            for i, v in ipairs(object_files) do
                print("-> " .. yellow .. v .. reset)
            end
            if project_type == "executable" and binary_name then
                print("-> " .. yellow .. binary_name .. executable_extension .. reset)
            elseif project_type == "static" and binary_name then
                print("-> " .. yellow .. "lib" .. binary_name .. static_extension .. reset)
            elseif project_type == "shared" and binary_name then
                print("-> " .. yellow .. "lib" .. binary_name .. shared_extension .. reset)
            elseif project_type == "header" then
                print("There is no need for clean project")
            end
        end
    else 
        print(magenta .. "To see which object files and binary cleaned, use -i flag\n" .. reset)
    end
end

function Run_Project() 
    local current_path = io.popen(get_current_path_command):read("l")
    local conf_file = io.open(current_path .. seperator .. "moony.conf")
    
    local project_name
    local project_type
    local binary_name
    local root_folder
    local include_folder 
    local src_folder 
    local lib_folder
    local object_folder
    local bin_folder
    local history_folder 

    if conf_file then
        for line in conf_file:lines() do
            local value = string.sub(line, string.find(line, "=") + 2, #line)

            if string.find(line, "project_name") then
                project_name = value
            elseif string.find(line, "project_type") then
                project_type = value
            elseif string.find(line, "binary_name") then
                binary_name = value
            elseif string.find(line, "root_path") then
                root_folder = value
            elseif string.find(line, "include_path") then
                include_folder = value
            elseif string.find(line, "src_path") then
                src_folder = value
            elseif string.find(line, "lib_path") then
                lib_folder = value
            elseif string.find(line, "object_path") then
                object_folder = value
            elseif string.find(line, "history_path") then
                history_folder = value
            end
        end
    else
        print("Conf file doesnt exists")
    end
    print(run_command .. binary_name)
    os.execute(run_command .. binary_name)
end

function Add_Library(type, library_folder)
    local current_path = io.popen(get_current_path_command):read("l")
    local conf_file = io.open(current_path .. seperator .. "moony.conf", "r")
    
    local project_name
    local project_type
    local binary_name
    local root_folder
    local include_folder 
    local src_folder 
    local lib_folder
    local object_folder
    local bin_folder
    local history_folder 

    if conf_file then
        for line in conf_file:lines() do
            local value
            if line then
                if string.find(line, "=") then
                    value = string.sub(line, string.find(line, "=") + 2, #line)
                end
            end

            if string.find(line, "project_name") then
                project_name = value
            elseif string.find(line, "project_type") then
                project_type = value
            elseif string.find(line, "binary_name") then
                binary_name = value
            elseif string.find(line, "root_path") then
                root_folder = value
            elseif string.find(line, "include_path") then
                include_folder = value
            elseif string.find(line, "src_path") then
                src_folder = value
            elseif string.find(line, "lib_path") then
                lib_folder = value
            elseif string.find(line, "object_path") then
                object_folder = value
            elseif string.find(line, "history_path") then
                history_folder = value
            end
        end
    else
        print("Conf file doesnt exists")
    end

    local library_root_folder = root_folder .. seperator .. library_folder

    local get_files_in_dir_command

    local library_include_folder = library_root_folder .. seperator .. "include" .. seperator
    local library_lib_folder = library_root_folder .. seperator .. "lib" .. seperator

    if is_unix then
        get_files_in_dir_command = 'ls -p "' .. library_include_folder .. '" | grep -v /'
    else
        get_files_in_dir_command = 'dir "' .. library_include_folder .. '" /b'
    end

    local include_files = {}

    local files = io.popen(get_files_in_dir_command)

    if files then
        for file_name in files:lines() do
            table.insert(include_files, file_name)
            print(file_name)
        end
        files:close() 
    end

    for i, v in ipairs(include_files) do
        os.execute(copy_file_command .. library_include_folder .. seperator .. v .. " " .. include_folder .. seperator .. v)
    end

    if is_unix then
        get_files_in_dir_command = 'ls -p "' .. library_lib_folder .. '" | grep -v /'
    else
        get_files_in_dir_command = 'dir "' .. library_lib_folder .. '" /b'
    end

    local lib_files = {}

    local files = io.popen(get_files_in_dir_command)

    if files then
        for file_name in files:lines() do
            table.insert(lib_files, file_name)
            print(file_name)
        end
        files:close() 
    end

    if type == "static" then
        local conf_file = io.open(current_path .. seperator .. "moony.conf", "a")

        for i, v in ipairs(lib_files) do
            os.execute(copy_file_command .. library_lib_folder .. seperator .. v .. " " .. lib_folder .. seperator .. v)
            v = string.sub(v, 1, #v - 2)
            if conf_file then
                conf_file:write(string.format("library = %s\n", v))
            else
                print("Conf file doesnt exists")
            end
        end
    elseif type == "shared" then
        local conf_file = io.open(current_path .. seperator .. "moony.conf", "a")

        for i, v in ipairs(lib_files) do
            os.execute(copy_file_command .. library_lib_folder .. seperator .. v .. " " .. lib_folder .. seperator .. v)
            v = string.sub(v, 1, #v - 2)
            if conf_file then
                conf_file:write("library = " .. v .. "\n")
            else
                print("Conf file doesnt exists")
            end
            os.execute(copy_file_command .. library_lib_folder .. seperator .. v .. " " .. root_folder .. seperator .. v)
        end
    elseif type == "header" then
        
    end
end

if arg[1] == "create" then
    local project_type = arg[2]
    local project_name = arg[3]

    Create_Folder_Structure(is_unix, project_type, project_name)

elseif arg[1] == "compile" then
    Compile_Project(arg[2])
elseif arg[1] == "link" then
    Link_Project(arg[2])
elseif arg[1] == "build" then
    Compile_Project(arg[2])
    print("\n------------------------------------------")
    Link_Project(arg[3])
elseif arg[1] == "clean" then
    Clean_Project(arg[2])
elseif arg[1] == "run" then
    Run_Project()
elseif arg[1] == "add" then
    Add_Library(arg[2], arg[3])
end
