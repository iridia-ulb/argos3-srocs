function step()
   if count < 10 then
      count = count + 1
   else
      count = 1
   end
   robot.directional_leds.set_all_colors('black')
   robot.directional_leds.set_single_color(count, 'white')   
   
   -- defaults
   local left = 0.005
   local right = 0.005
   -- override defaults if the line was detected
   if line_detected(robot.ground.right) then
      if line_detected(robot.ground.center) then
         left = 0.01
         right = 0.00
         mode = "right"
      else
         left = 0.01
         right = -0.01
         mode = "hard right"
      end
   elseif line_detected(robot.ground.left) then
      if line_detected(robot.ground.center) then
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
   return sensor.reading < 0.6
end

function init()
   reset()
end

function reset() 
   count = 1
end

function destroy()
end

