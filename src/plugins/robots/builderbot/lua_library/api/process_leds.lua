robot.logger:register_module("api.process_leds")

package.loaded['api.process_leds'] = function(blocks, custom_type_function)
   -- takes tags in camera_frame_reference
   for i, block in ipairs(blocks) do
      for j, tag in pairs(block.tags) do
         tag.type = 0
         block.type = 0
         for j, led_location_on_tag in ipairs(robot.api.constants.block_led_offset_from_tag) do
            local led_location_on_camera = vector3(led_location_on_tag):rotate(tag.orientation) + tag.position
            local color_number = robot.camera_system.detect_led(led_location_on_camera)
            if color_number ~= tag.type and color_number ~= 0 then
               tag.type = color_number
               block.type = tag.type
            end
         end
      end
      if type(custom_type_function) == "function" then
         block.type = custom_type_function(block) or block.type
      end
   end
end


