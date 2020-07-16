robot.logger:register_module("nodes.search_block")

package.loaded['nodes.search_block'] = function(data, rule_node)
   -- create a search node based on rule_node
   return {
      type = "sequence",
      children = {
         -- prepare, lift to highest
         {
            type = "selector",
            children = {
               -- if lift reach position(0.07), return true, stop selector
               function()
                  if (robot.lift_system.position > robot.api.constants.lift_system_upper_limit - 
                        robot.api.parameters.lift_system_position_tolerance) and
                     (robot.lift_system.position < robot.api.constants.lift_system_upper_limit +
                        robot.api.parameters.lift_system_position_tolerance) then
                     return false, true
                  else
                     return false, false
                  end
               end,
               -- set position(0.07)
               function()
                  robot.lift_system.set_position(robot.api.constants.lift_system_upper_limit)
                  return true -- always running
               end,
            },
         },
         -- search
         -- TODO: maybe it will be better if the robot keeps looking for blocks during obstacle avoidance
         {
            type = "sequence",
            children = {
               -- check obstacle and randomwalk
               {
                  type = "sequence",
                  children = {
                     -- if obstacle and avoid
                     robot.nodes.create_obstacle_avoidance_node(data),
                     -- obstacle clear, random walk
                     function()
                        local random_angle =
                           robot.random.uniform(-robot.api.parameters.search_random_range,
                                                 robot.api.parameters.search_random_range)
                        robot.api.move.with_bearing(robot.api.parameters.default_speed,
                                                    random_angle)
                        return false, true
                     end,
                  },
               },
               -- choose a block,
               -- if got one, return true, stop sequence
               {
                  type = "selector", 
                  children = {
                     rule_node, -- return true or false
                     function() return true end, -- if rule-node doesn't find a match, return running
                  },
               },
               function()
                  robot.log_info("rule node matched, target acquired")
                  return false, true
               end
            },
         },
      },
   }
end
