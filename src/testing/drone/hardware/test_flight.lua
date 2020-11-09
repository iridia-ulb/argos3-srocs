--[[ This function is executed every time you press the 'execute' button ]]
function init()
   state = 'preflight'
end

--[[ This function is executed at each time step
     It must contain the logic of your controller ]]
function step()
   print('time = ' .. tostring(robot.system.time))
   print('pose = ' .. tostring(robot.flight_system.position) .. 
         ' / ' .. tostring(robot.flight_system.orientation.z))
   print('state = ' .. state)
   if robot.flight_system.ready() then
      if state == 'preflight' then
         print('enabling off-board mode')
         robot.flight_system.set_offboard_mode(true)
         -- WARNING this should make the drone take off
         print('arming drone')
         robot.flight_system.set_armed(true, false)
         -- fly up one meter
         robot.flight_system.set_target_pose(vector3(0,0,1), 0)
         state = 'taking_off'
      elseif state == 'taking_off' then
         if math.abs(robot.flight_system.position.z - 8.5) < 0.025 then
            robot.flight_system.set_target_pose(vector3(0,0,0), 0)
            state = 'landing'
         end
      elseif state == 'landing' then
         if math.abs(robot.flight_system.position.z) < 0.025 then
            robot.flight_system.set_armed(false, false)
            robot.flight_system.set_offboard_mode(false)
            state = 'postflight'
         end
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
   print('disarming drone')
   robot.flight_system.set_armed(false, false)
   print('disabling off-board mode')
   robot.flight_system.set_offboard_mode(false)
end
