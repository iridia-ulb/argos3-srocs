-- register module with logger
robot.logger:register_module('nodes_timer')

-- return node generator
return function(time, method)
   -- count a period of time (time parameter)
   -- each step do method()
   local end_time
   return {
      type = 'sequence*',
      children = {
         function()
            if type(time) == 'function' then
               end_time = robot.system.time + time()
            else
               end_time = robot.system.time + time
            end
            return false, true
         end,
         function()
            if robot.system.time > end_time then
               return false, true
            else
               if type(method) == 'function' then
                  local running, result = method()
                  if running ~= nil then
                     return running, result
                  end
               end
               return true
            end
         end,
      },
   }
end
