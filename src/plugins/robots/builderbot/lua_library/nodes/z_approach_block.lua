package.preload['nodes_z_approach_block'] = function()
   -- register module with logger
   robot.logger:register_module('nodes_z_approach_block')

   -- return node generator
   return function(data, distance)
      -- approach the target reference block until distance away 
      local location = {}
      return {
         type = "sequence*",
         children = {
            -- calculate location
            function()
               local target_block = data.blocks[data.target.id]
               location.position = target_block.position_robot + 
                  vector3(1,0,0):rotate(target_block.orientation_robot) * distance
               location.orientation = target_block.orientation_robot * quaternion(math.pi, vector3(0,0,1))
               return false, true
            end,
            -- move to the location
            {
               type = "sequence",
               children = {
                  robot.nodes.create_obstacle_avoidance_node(data),
                  robot.nodes.create_move_to_location_node(location),
               }
            }
         }, -- end of the children of go to pre-position
      } -- end of go to pre-position
   end
end