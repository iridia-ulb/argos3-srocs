robot.api = {}
robot.api.set_face_color = function(face, color)
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

local color = {"magenta", "orange", "green", "blue"}
color[0] = "black"

local direction = {"top", "north", "west", "east", "south", "bottom"}
local direction_number = {
   top = 1, 
   north = 2, 
   west = 3,
   east = 4, 
   south = 5, 
   bottom = 6,
}

function init()
   reset()
end

function reset()
   if robot.id == "block1" then
      robot.api.set_face_color("top", color[1])
      for i = 2, 5 do
         robot.api.set_face_color(direction[i], color[2])
      end
   end

   if robot.id == "block6" then
      robot.directional_leds.set_all_colors(color[3])
   end
end

function step()
   for face, radio in pairs(robot.radios) do
      if #radio.rx_data > 0 then        
         local received = radio.rx_data[1][1]
         if received > 48 then
            received = received - 48
            robot.directional_leds.set_all_colors(color[received])
         else
            local opposite_face = direction[7 - direction_number[face]]
            if received > 0 then
               robot.api.set_face_color(opposite_face, color[2])
               robot.radios[opposite_face].tx_data{received - 1}
            else
               robot.api.set_face_color(opposite_face, color[3])
            end
         end
      end
   end 
   
   if robot.id == "block1" then
      for i = 2, 5 do
         robot.radios[direction[i]].tx_data({1})
      end
   end
end


function destroy()
end