--[[ This function is executed every time you press the 'execute' button ]]
function init()
   count = 1
end


--[[ This function is executed at each time step
     It must contain the logic of your controller ]]
function step()
   -- turn everything off
   robot.leds.set_body_led(false)
   robot.leds.set_front_led(false)
   robot.leds.set_ring_leds(false)
   -- switch one led on according to the count
   if count == 1 then
      robot.leds.set_body_led(true)
   elseif count == 2 then
      robot.leds.set_front_led(true)
   else
      local index = count - 2
      robot.leds.set_ring_led_index(index, true)
   end
   -- increment/wrap the count
   count = count + 1
   if count > 10 then
      count = 1
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
end
