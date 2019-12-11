--[[ This function is executed every time you press the 'execute' button ]]
function init()
   for i, camera in ipairs(robot.cameras_system) do    
      camera.enable()
   end
end

--[[ This function is executed at each time step
     It must contain the logic of your controller ]]
function step()
   print("time = " .. tostring(robot.system.time))
   local str = "temperatures = "
   for i, temp in ipairs(robot.system.temperatures) do
      str = str .. tostring(temp) .. " "
   end
   print(str)
   for i, camera in ipairs(robot.cameras_system) do 
      print("camera " .. i)
      for j, tag in ipairs(camera.tags) do
         print("  tag " .. j .. ": " .. tostring(tag.center))
      end
   end
end

--[[ This function is executed every time you press the 'reset'
     button in the GUI. It is supposed to restore the state
     of the controller to whatever it was right after init() was
     called. The state of sensors and actuators is reset
     automatically by ARGoS. ]]
function reset()
end

--[[ This function is executed only once, when the robot is removed
     from the simulation ]]
function destroy()
   for i, camera in ipairs(robot.cameras_system) do    
      camera.disable()
   end
end
