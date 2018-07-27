--[[ This function is executed every time you press the 'execute' button ]]
vel = 0;

function init()
end


--[[ This function is executed at each time step
     It must contain the logic of your controller ]]
function step()
	if vel < 5 then
		robot.differential_drive.set_target_velocity(vel,-vel);
		vel = vel + 1
	end
end

--[[ This function is executed every time you press the 'reset'
     button in the GUI. It is supposed to restore the state
     of the controller to whatever it was right after init() was
     called. The state of sensors and actuators is reset
     automatically by ARGoS. ]]
function reset()
   count = 0
end

--[[ This function is executed only once, when the robot is removed
     from the simulation ]]
function destroy()
   -- put your code here
end
