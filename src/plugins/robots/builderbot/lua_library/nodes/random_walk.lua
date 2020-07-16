robot.logger:register_module("nodes.random_walk")
-- if there are obstacles avoid it and return running
-- if there no obstacles, return true
package.loaded['nodes.random_walk'] = function()
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
