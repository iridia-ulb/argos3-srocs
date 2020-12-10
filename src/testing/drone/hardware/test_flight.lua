function init()
   state = 'preflight'
   setpoint = 'none'
end

function step()
   print('pose = ' .. tostring(robot.flight_system.position))
   print('state = ' .. state)
   print('setpoint = ' .. setpoint)
   if robot.flight_system.ready() then
       if state == 'preflight' then
           print('arming drone')
           robot.flight_system.set_armed(true, false)
           robot.flight_system.set_offboard_mode(true)
           robot.flight_system.set_target_pose(vector3(0,0,1), 0)
           print('enabling off-board mode')
           state = 'take_off'
           setpoint = 'first'
       elseif state == 'take_off' then
           if setpoint == 'first' then
               robot.flight_system.set_target_pose(vector3(0,0,1), 0)
               if math.abs(robot.flight_system.position.z + 1) < 0.025 then
                   robot.flight_system.set_target_pose(vector3(1,0,1), 0)
                   setpoint = 'second'
               end
           elseif setpoint == 'second' then
               robot.flight_system.set_target_pose(vector3(1,0,1), 0)
               if math.abs(robot.flight_system.position.x - 1) < 0.025 then
                   robot.flight_system.set_target_pose(vector3(1,1,1), 0)
                   setpoint = 'third'
               end
           elseif setpoint == 'third' then
               robot.flight_system.set_target_pose(vector3(1,1,1), 0)
               if math.abs(robot.flight_system.position.y - 1) < 0.025 then
                   robot.flight_system.set_target_pose(vector3(1,1,0), 0)
                   setpoint = 'fourth'
               end
           elseif setpoint == 'fourth' then
               robot.flight_system.set_target_pose(vector3(1,1,0), 0)
               if math.abs(robot.flight_system.position.z + 0) < 0.025 then
                   state = 'landing'
                   setpoint = 'none'
               end
           end
       elseif state == 'landing' then
           robot.flight_system.set_offboard_mode(false)
           robot.flight_system.set_armed(false, false)
           state = 'postflight'
       end
   end
end


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