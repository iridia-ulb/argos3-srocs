--[[ This function is executed every time you press the 'execute' button ]]
function init()
   settings = {
      {'forward', 100, 100},
      {'backwards', -100, -100},
      {'clockwise', 100, -100},
      {'counter-clockwise', -100, 100},
      {'stop', 0, 0}
   }
   count = 1
end


--[[ This function is executed at each time step
     It must contain the logic of your controller ]]
function step()
   -- set velocity
   local index = math.ceil(count / 10)
   local desc, left, right = table.unpack(settings[index])
   robot.differential_drive.set_target_velocity(left, right)
   print(string.format('--- step = %d, setting = %s ---', count, desc))
   print(string.format('left:  target_vel = %d, delta_pos = %d', left, robot.differential_drive.encoders.left))
   print(string.format('right: target_vel = %d, delta_pos = %d', right, robot.differential_drive.encoders.right))
   -- increment the config variable
   if count < 50 then
      count = count + 1
   end
end

--[[ This function is executed every time you press the 'reset'
     button in the GUI. It is supposed to restore the state
     of the controller to whatever it was right after init() was
     called. The state of sensors and actuators is reset
     automatically by ARGoS. ]]
function reset()
   count = 1
end

--[[ This function is executed only once, when the robot is removed
     from the simulation ]]
function destroy()
end
