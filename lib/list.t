local starter = [[#include <stdio.h>
#include <stdlib.h>    
#include <stdbool.h>
]]

local c = terralib.includecstring(starter)

local current_type_id = 0

function init(type)

    local listType = struct{
        data:&type;
        size:int;
    }

    listType.methods.new = terralib.overloadedfunction("list_new_" .. current_type_id, {
        terra()
            var result:listType;
            result.size = 0;
            result.data = [&type](c.malloc(0));
            return result;
        end,
        terra(size:int)
            var result:listType;
            result.size = size;
            result.data = [&type](c.malloc(size * terralib.sizeof(type)));
            return result;
        end,
        terra(size:int, value:type)
            var result:listType;
            result.size = size;
            result.data = [&type](c.malloc(size * terralib.sizeof(type)));
            for i = 0, size do
                result.data[i] = value;
            end
            return result;
        end
    })

    listType.methods.fromArray = terra(data:&type, size:int)
        var result:listType;
        result.size = size;
        result.data = [&type](c.malloc(size * terralib.sizeof(type)));
        for i = 0, size do
            result.data[i] = data[i];
        end
        return result;
    end

    terra listType:push(value:type)
        self.size = self.size + 1;
        self.data = [&type](c.realloc(self.data, self.size * terralib.sizeof(type)));
        self.data[self.size - 1] = value;
    end

    terra listType:insert(value:type, index:int)
        self.size = self.size + 1;
        self.data = [&type](c.realloc(self.data, self.size * terralib.sizeof(type)));
        for i = self.size - 1, index + 1, -1 do
            self.data[i] = self.data[i - 1];
        end
        self.data[index] = value;
    end

    terra listType:set(value:type, index:int)
        self.data[index] = value;
    end

    terra listType:get(index:int)
        return self.data[index];
    end

    terra listType:size()
        return self.size;
    end

    terra listType:pop()
        self.size = self.size - 1;
        self.data = [&type](c.realloc(self.data, self.size * terralib.sizeof(type)));
    end

    terra listType:concat(other:listType)
        for i = 0, other.size do
            self:push(other:get(i));
        end
    end

    terra listType:remove(index:int)
        for i = index, self.size - 1 do
            self.data[i] = self.data[i + 1];
        end
        self.size = self.size - 1;
        self.data = [&type](c.realloc(self.data, self.size * terralib.sizeof(type)));
    end

    listType.methods.clear = terralib.overloadedfunction("list_clear_" .. current_type_id, {
        terra(self:listType)
            self.size = 0;
            self.data = [&type](c.realloc(self.data, 0));
        end,
        terra(self:listType, value:type)
            for i = 0, self.size do
                self.data[i] = value;
            end
        end,
        terra(self:listType, size:int, value:type)
            self.size = size;
            self.data = [&type](c.realloc(self.data, size * terralib.sizeof(type)));
            for i = 0, size do
                self.data[i] = value;
            end
        end
    })

    terra listType:resize(size:int)
        self.size = size;
        self.data = [&type](c.realloc(self.data, size * terralib.sizeof(type)));
    end

    terra listType:free()
        c.free(self.data);
    end

    current_type_id = current_type_id + 1

    return listType
end

return init