--[[ This function is executed every time you press the 'execute' button ]]
function init()
end

--[[ This function is executed at each time step
     It must contain the logic of your controller ]]
function step()
   log("#robot.camera.tags = ", #robot.camera.tags)

   for i,tag in ipairs(robot.camera.tags) do
      log("tag ", i, ":")
      local xmin = 4096
      local xmax = 0
      local ymin = 4096
      local ymax = 0
      for j,corner in ipairs(tag.corners) do
         log("  ", corner.x, ", ", corner.y)
         xmin = math.min(xmin, corner.x)
         xmax = math.max(xmax, corner.x)
         ymin = math.min(ymin, corner.y)
         ymax = math.max(ymax, corner.y)
      end
      robot.camera.get_pixels(xmin, ymin, xmax - xmin, ymax - ymin)
   end


--[[
   local pixels = robot.camera.get_pixels(1,2,3,4)
   for index,pixel in ipairs(pixels) do 
      log(index .. ": " .. pixel.y .. " " .. pixel.u .. " " .. pixel.v)
   end
   log("tags = " .. #robot.camera.tags)
]]--
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
