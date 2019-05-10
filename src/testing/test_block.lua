count = 1

function init()
   reset()
end

function step()
   -- update count
   count = count + 1
   if count > 24 then
      count = 1
   end  
   robot.directional_leds.set_all_colors("black")
   robot.directional_leds.set_single_color(count, "red")
end

function reset()
   count = 1
   robot.directional_leds.set_all_colors("black")
   robot.directional_leds.set_single_color(count, "red")
end

function destroy()
end
