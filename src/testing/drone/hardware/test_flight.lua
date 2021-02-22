--[[ This function is executed every time you press the 'execute' button ]]
function init()
   streaming_dur = 200  
   stream_count = 0
   state = 'pre_flight'
end

--[[ This function is executed at each time step
    It must contain the logic of your controller ]]
   function step()
      print('time: ' .. tostring(robot.system.time))
      print('state = ' .. state)
      if robot.flight_system.ready() then
          if state == 'pre_flight' then
            -- publish the setpoint before entering the offboard mode for (streaming_dur/ticks_per_sec) seconds
            if stream_count <= streaming_dur then
                print('streaming setpoint...')
                robot.flight_system.set_target_pose(vector3(0,0,1), 0)
                stream_count = stream_count + 1
            else
               state = 'off_board'
            end
      elseif state == 'off_board' then
         robot.flight_system.set_offboard_mode(true)
         print('off_board mode enabled')
         state = 'armed'
      elseif state == 'armed'  then
         robot.flight_system.set_armed(true, false)
         print('vehicle armed')
         state = 'take_off'
         print('take_off')
      elseif state == 'take_off' then
         if math.abs(robot.flight_system.position.z  - robot.flight_system.target_position.z) < 0.025 then
            state = 'landing'
            robot.flight_system.set_target_pose(vector3(0,0,0), 0)
         else
            -- publish the setpoint during the flight
            robot.flight_system.set_target_pose(vector3(0,0,1), 0)
         end
      elseif state == 'landing' then
         if math.abs(robot.flight_system.position.z  - robot.flight_system.target_position.z) < 0.025 then
            robot.flight_system.set_armed(false, false)
            robot.flight_system.set_offboard_mode(false)
            state = 'postflight'
         else
            -- publish the setpoint during the flight
            robot.flight_system.set_target_pose(vector3(0,0,0), 0)
         end
--[[   
         if math.abs(robot.flight_system.position.z  - robot.flight_system.initial_position.z) < 0.025 then
            robot.flight_system.set_armed(false, false)
            robot.flight_system.set_offboard_mode(false)
            state = 'postflight'
         else
            --  auto_land
            robot.flight_system.auto_land(0.5, robot.flight_system.orientation.z, vector3(0,0,0))  -- (landing descend rate: m/s, desired yaw angle: rad, XYZ :m)
         end
]]
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
