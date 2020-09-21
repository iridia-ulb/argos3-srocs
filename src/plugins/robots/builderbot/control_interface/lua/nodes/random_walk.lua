-- register module with logger
robot.logger:register_module('nodes_random_walk')

-- return node generator
return function()
   return {
      type = 'sequence',
      children = {
         function()
            local random_angle = robot.random.uniform(-robot.api.parameters.search_random_range, robot.api.parameters.search_random_range)
            robot.api.move.with_bearing(robot.api.parameters.default_speed, random_angle)
            return false, true
         end
      }
   }
end
