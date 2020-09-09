function step()
   if count < 8 then
      count = count + 1
   else
      count = 1
   end
   robot.directional_leds.set_all_colors('black')
   robot.directional_leds.set_single_color(count, 'white')
   log(robot.id .. '.imu.magnetometer = ' .. tostring(robot.imu.magnetometer))
end

function init()
   count = 1
end

function reset() 
   count = 1
end

function destroy()
end

