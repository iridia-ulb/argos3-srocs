--[[ This function is executed every time you press the 'execute' button ]]
function init()
   robot.lift_system.calibrate();
end

--[[ This function is executed at each time step
     It must contain the logic of your controller ]]
function step()
   log("lift_system.state: " .. robot.lift_system.state)
   log("lift_system.limit_switches: " .. robot.lift_system.limit_switches.top .. " " .. robot.lift_system.limit_switches.bottom)
   log("lift_system.position: " .. robot.lift_system.position)
   if robot.lift_system.state == "inactive" then
      if robot.lift_system.position > 0.07 then
         robot.lift_system.set_position(0.01)
      elseif robot.lift_system.position < 0.07 then
         robot.lift_system.set_position(0.13)
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
   -- put your code here
end
