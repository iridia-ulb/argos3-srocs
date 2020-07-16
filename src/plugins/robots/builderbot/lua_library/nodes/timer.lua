robot.logger:register_module('nodes.timer')

package.loaded['nodes.timer'] = function(time, method)
   -- count a period of time (time parameter)
   -- each step do method()
   local end_time
   return {
      type = 'sequence*',
      children = {
         function()
            if type(time) == 'function' then
               time = time()
            end
            end_time = robot.system.time + time
            return false, true
         end,
         function()
            if robot.system.time > end_time then
               return false, true
            else
               if type(method) == 'function' then
                  method()
               end
               return true
            end
         end,
      },
   }
end