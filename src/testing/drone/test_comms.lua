--[[ This function is executed every time you press the 'execute' button ]]
pid = 0
count = 0

function init()
   reset()
end

--[[ This function is executed at each time step
     It must contain the logic of your controller ]]
function step()
   count = count + 1
   print("step " .. count .. ":")
   if true then  
   robot.wifi.tx_data({count .. ": " .. pid})
   end
   for index, message in ipairs(robot.wifi.rx_data) do
      for k, v in pairs(message) do
         print(k,v)
      end
   end
end

--[[
function print_tag_info()
   log("#robot.camera_system.tags = ", #robot.camera_system.tags)
   for i,tag in ipairs(robot.camera_system.tags) do
      log("tag ", i, ":")
      for j,corner in ipairs(tag.corners) do
         log("  ", string.format("%.2f", corner.x), ", ", string.format("%.2f", corner.y))
      end
   end
end
]]--

--[[ This function is executed every time you press the 'reset'
     button in the GUI. It is supposed to restore the state
     of the controller to whatever it was right after init() was
     called. The state of sensors and actuators is reset
     automatically by ARGoS. ]]
function reset()
   count = 0
   pid = io.open('/proc/self/stat'):read("*number")
   print("PID:" .. pid)
--[[
   if robot.camera_system then
      robot.camera_system.enable()
   end
]]--
end

--[[ This function is executed only once, when the robot is removed
     from the simulation ]]
function destroy()
   -- put your code here
end
