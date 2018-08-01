state = "charging"

--[[ This function is executed every time you press the 'execute' button ]]
function init()
   robot.electromagnet_system.set_discharge_mode("disabled")
end



--[[ This function is executed at each time step
     It must contain the logic of your controller ]]
function step()
   log("electromagnet_system.voltage: " .. robot.electromagnet_system.voltage .. " (" .. state .. ")")
   if state == "charging" and robot.electromagnet_system.voltage > 22 then
      robot.electromagnet_system.set_discharge_mode("destructive")
      state = "discharging"
   elseif state == "discharging" and robot.electromagnet_system.voltage < 10 then
      robot.electromagnet_system.set_discharge_mode("disabled")
      state = "charging"
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
