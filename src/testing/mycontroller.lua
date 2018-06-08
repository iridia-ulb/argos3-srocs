--[[ This function is executed every time you press the 'execute' button ]]
function init()
end

--[[ This function is executed at each time step
     It must contain the logic of your controller ]]
function step()
   local pixels = robot.camera.get_pixels(1,2,3,4)
   for index,pixel in ipairs(pixels) do 
      log(index .. ": " .. pixel.y .. " " .. pixel.u .. " " .. pixel.v)
   end
   log("tags = " .. #robot.camera.tags)
--	robot.dds.set_target_velocity(robot.random.uniform() * 10 - 5, robot.random.uniform() * 10 - 5)
end

--[[ This function is executed every time you press the 'reset'
     button in the GUI. It is supposed to restore the state
     of the controller to whatever it was right after init() was
     called. The state of sensors and actuators is reset
     automatically by ARGoS. ]]
function reset()
   -- put your code here
end

--[[ This function is executed only once, when the robot is removed
     from the simulation ]]
function destroy()
   -- put your code here
end
