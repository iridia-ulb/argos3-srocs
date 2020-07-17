package.preload['nodes_place_behavior'] = function()
   -- register module with logger
   robot.logger:register_module('nodes_place_behavior')
   
   -- return node generator
   return function(data, rules)
      return {
         type = "selector*",
         children = {
            function()
               if robot.rangefinders['underneath'].proximity >
                  robot.api.parameters.proximity_touch_tolerance then
                  return false, true
               else
                  return false, false
               end
            end,
            -- place
            {
               type = "sequence*",
               children = {
                  robot.nodes.create_approach_block_node(data, 
                     robot.nodes.create_search_block_node(data, 
                        robot.nodes.create_process_rules_node(data, rules, "place")
                     ),
                     0.20
                  ),
                  robot.nodes.create_place_block_node(data, 0.20),
                  -- reverse
                  robot.nodes.create_timer_node(
                     0.08 / robot.api.parameters.default_speed,
                     function()
                        robot.api.move.with_velocity(-robot.api.parameters.default_speed, 
                                                     -robot.api.parameters.default_speed)
                     end
                  ),
                  function()
                     robot.logger:log_info("place block complete")
                     return false, true
                  end
               }
            }
         }
      }
   end
end
