--[[ This function is executed every time you press the 'execute' button ]]
count = 0
vel = 0

function init()
end


--[[ This function is executed at each time step
     It must contain the logic of your controller ]]
function step()
   if(count < 100) then
      count = count + 1
      if count % 10 == 0 then
		   vel = vel + 5
         log("set velocity = " .. vel);
		   robot.differential_drive.set_target_velocity(vel,-vel);
	   end
      log("left:  " .. robot.differential_drive.encoders.left)
      log("right: " .. robot.differential_drive.encoders.right)
   end
end

--[[ This function is executed every time you press the 'reset'
     button in the GUI. It is supposed to restore the state
     of the controller to whatever it was right after init() was
     called. The state of sensors and actuators is reset
     automatically by ARGoS. ]]
function reset()
   count = 0
   vel = 0
end

--[[ This function is executed only once, when the robot is removed
     from the simulation ]]
function destroy()
   -- put your code here
end
