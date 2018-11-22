--[[ This function is executed every time you press the 'execute' button ]]
function init()
end

--[[ This function is executed at each time step
     It must contain the logic of your controller ]]
function step()
   print_tag_info()
end

function print_tag_info()
   log("#robot.camera_system.tags = ", #robot.camera_system.tags)
   for i,tag in ipairs(robot.camera_system.tags) do
      log("tag ", i, ":")
      for j,corner in ipairs(tag.corners) do
         log("  ", corner.x, ", ", corner.y)
      end
   end
end

--[[ This function is executed every time you press the 'reset'
     button in the GUI. It is supposed to restore the state
     of the controller to whatever it was right after init() was
     called. The state of sensors and actuators is reset
     automatically by ARGoS. ]]
function reset()
   count = 0
end

--[[ This function is executed only once, when the robot is removed
     from the simulation ]]
function destroy()
   -- put your code here
end
