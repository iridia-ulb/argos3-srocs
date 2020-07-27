-- just as a reminder {black = 0, pink(magenta) = 1, orange = 2, green = 3, blue = 4}
local color = {"magenta", "orange", "green", "blue"}
color[0] = "black"

local set_face_color = function(face, color)
   if face == "north" then
      robot.directional_leds.set_single_color(1, color)
      robot.directional_leds.set_single_color(2, color)
      robot.directional_leds.set_single_color(3, color)
      robot.directional_leds.set_single_color(4, color)
   elseif face == "east" then
      robot.directional_leds.set_single_color(5, color)
      robot.directional_leds.set_single_color(6, color)
      robot.directional_leds.set_single_color(7, color)
      robot.directional_leds.set_single_color(8, color)
   elseif face == "south" then
      robot.directional_leds.set_single_color(9, color)
      robot.directional_leds.set_single_color(10, color)
      robot.directional_leds.set_single_color(11, color)
      robot.directional_leds.set_single_color(12, color)
   elseif face == "west" then
      robot.directional_leds.set_single_color(13, color)
      robot.directional_leds.set_single_color(14, color)
      robot.directional_leds.set_single_color(15, color)
      robot.directional_leds.set_single_color(16, color)
   elseif face == "top" then
      robot.directional_leds.set_single_color(17, color)
      robot.directional_leds.set_single_color(18, color)
      robot.directional_leds.set_single_color(19, color)
      robot.directional_leds.set_single_color(20, color)
   elseif face == "bottom" then
      robot.directional_leds.set_single_color(21, color)
      robot.directional_leds.set_single_color(22, color)
      robot.directional_leds.set_single_color(23, color)
      robot.directional_leds.set_single_color(24, color)
   end
end

function init()
   reset()
end

function reset()
   if robot.id == "block6" then
      robot.directional_leds.set_all_colors("magenta")
   elseif robot.id == "block7" then
      robot.directional_leds.set_all_colors("green")
   elseif robot.id == "block11" or robot.id == "block12" then
      set_face_color("top", "magenta")
      set_face_color("north", "orange")
      set_face_color("south", "orange")
      set_face_color("west", "orange")
      set_face_color("east", "orange")
   else
      robot.directional_leds.set_all_colors("black")
   end
end

function step()
   for face, radio in pairs(robot.radios) do
      if #radio.rx_data > 0 then        
         local received = radio.rx_data[1][1] - 48
         robot.directional_leds.set_all_colors(color[received])
      end
   end 
end


function destroy()
end
