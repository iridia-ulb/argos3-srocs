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

   -- defaults
   local left = 0.005
   local right = 0.005
   -- override defaults if the line was detected
   if line_detected(robot.ground_sensors[3]) then
      if line_detected(robot.ground_sensors[2]) then
         left = 0.01
         right = 0.00
         mode = "right"
      else
         left = 0.01
         right = -0.01
         mode = "hard right"
      end
   elseif line_detected(robot.ground_sensors[1]) then
      if line_detected(robot.ground_sensors[2]) then
         left = 0.00
         right = 0.01
         mode = "left"
      else
         left = -0.01
         right = 0.01
         mode = "hard left"
      end
   end
   --log(mode)
   robot.differential_drive.set_target_velocity(left, -right)
end

function line_detected(sensor) 
   return sensor.reflected < 0.6
end

function init()
   reset()
end

function reset() 
   count = 1
end

function destroy()
end

