robot.logger:register_module("nodes.place_block")

-- assume I am forward_distance away from the block
-- shameful move blindly for that far (use reach_block node)
-- anti release the electomagnet to drop the block
package.loaded['nodes.place_block'] = function(data, forward_distance)
   return {
      type = "sequence*",
      children = {
         -- recharge
         function()
            robot.electromagnet_system.set_discharge_mode("disable")
            return false, true
         end,
         -- reach the block
         robot.nodes.create_reach_block_node(data, forward_distance),
         -- change color
         function()
            if data.target.type ~= nil then
               robot.nfc.write(tostring(data.target.type)) -- TODO: check whether nfc is still used
            end
            return false, true
         end,
         -- release block
         function()
            robot.electromagnet_system.set_discharge_mode("destructive")
            return false, true
         end,
         -- wait for 2 sec
         robot.nodes.create_timer_node(2),
         -- recharge magnet
         function()
            robot.electromagnet_system.set_discharge_mode("disable")
         end,
      },
   }
end
