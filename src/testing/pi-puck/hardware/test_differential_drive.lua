--[[ This function is executed every time you press the 'execute' button ]]
function init()
   config = 1
   velocities = {
      {1, 1}, -- forward
      {-1, -1}, -- backwards
      {1, -1}, -- cw
      {-1, 1}, -- ccw
      {0, 0} -- stop
   }
end


--[[ This function is executed at each time step
     It must contain the logic of your controller ]]
function step()
   -- set velocity
   robot.differential_drive.set_target_velocity(unpack(velocities[config]))
   -- increment the config variable
   if config < 5 then
      config = config + 1
   end
end

--[[ This function is executed every time you press the 'reset'
     button in the GUI. It is supposed to restore the state
     of the controller to whatever it was right after init() was
     called. The state of sensors and actuators is reset
     automatically by ARGoS. ]]
function reset()
   config = 1
end

--[[ This function is executed only once, when the robot is removed
     from the simulation ]]
function destroy()
end
