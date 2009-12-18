--[[
    This section will most likely be written in C and simply exposed to the scripts
]]--
function inheritsFrom( baseClass )
    -- http://lua-users.org/wiki/InheritanceTutorial
    local new_class = {}
    local class_mt = { __index = new_class }

    function new_class:new()
        local newinst = {}
        setmetatable( newinst, class_mt )
        return newinst
    end

    if baseClass then
        setmetatable( new_class, { __index = baseClass } )
    end

    return new_class
end

Job = {}
Job.__index = Job

function Job:run(prog, args)
    args_str = ''
    for k, v in ipairs(args) do
        args_str = args_str .. v .. " "
    end
    print('running ' .. prog .. " " .. args_str)
end

--[[
    Archive Class
--]]

Archive = inheritsFrom(Job)

function Archive:execute(args)
     self:run('/usr/bin/tar', args)
end

Archive.new():execute({[1] = 44, [2] = 'masterbrew'})
