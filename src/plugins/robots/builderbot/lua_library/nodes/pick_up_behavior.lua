package.preload['nodes_pick_up_behavior'] = function()
   -- register module with logger
   robot.logger:register_module('nodes_pick_up_behavior')

   -- return node generator
   return function(data, rules)
      return {
         type = "selector*",
         children = {
            -- check if robot already has a block
            function()
               if robot.rangefinders['underneath'].proximity < 
                  robot.api.parameters.proximity_touch_tolerance then
                  return false, true
               else
                  return false, false
               end
            end,
            {
               -- search and pick up block
               type = "sequence*",
               children = {
                  robot.nodes.create_approach_block_node(data, 
                     robot.nodes.create_search_block_node(data,
                        robot.nodes.create_process_rules_node(data, rules, "pickup")
                     ),
                     0.20
                  ),
                  robot.nodes.create_pick_up_block_node(data, 0.20),
                  function()
                     robot.logger:log_info("pick up block complete")
                     return false, true
                  end,
               }
            }
         }
      }
   end
end
