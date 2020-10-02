-- register module with logger
robot.logger:register_module('nodes_aim_block')

-- return node generator
return function(data, aim_point)
   -- aim block, put the block into the center of the image
   return function()
      -- find the left/right/up/down corners of a tag of the target block
      if data.target == nil or data.target.id == nil or data.blocks[data.target.id] == nil then
         robot.logger:log_info("Target block is nil, abort approach")
         return false, false
      end
      local target_block = data.blocks[data.target.id]
      local target_tag = 
         target_block.tags.up or
         target_block.tags.front or
         target_block.tags.left or
         target_block.tags.right
      target_tag.corners.up = robot.camera_system.resolution.y
      target_tag.corners.down = 0
      target_tag.corners.left = robot.camera_system.resolution.x
      target_tag.corners.right = 0
      for i, v in ipairs(target_tag.corners) do
         if v.x < target_tag.corners.left then target_tag.corners.left = v.x end
         if v.x > target_tag.corners.right then target_tag.corners.right = v.x end
         if v.y < target_tag.corners.up then target_tag.corners.up = v.y end
         if v.y > target_tag.corners.down then target_tag.corners.down = v.y end
      end

      -- adjust manipulator height 
      local flag_camera
      local tolerance = robot.api.parameters.lift_system_position_tolerance
      local target_height = robot.lift_system.position - target_tag.position.y
      local upper_limit = robot.api.constants.lift_system_upper_limit
      local lower_limit = robot.api.constants.lift_system_lower_limit + target_block.position_robot.z - 0.02
      if target_height < lower_limit then target_height = lower_limit end
      if target_height > upper_limit then target_height = upper_limit end
      if robot.lift_system.position > target_height - tolerance and 
         robot.lift_system.position < target_height + tolerance then
         robot.lift_system.set_position(robot.lift_system.position)
         flag_camera = true
      else
         robot.lift_system.set_position(target_height)
         flag_camera = false
      end

      -- adjust direction
      local flag_orientation
      local err -- err = -1 to 1, positive means turn left, negtive means turn right
      local left_target_pixel = robot.camera_system.resolution.x * 4 / 32 
      local right_target_pixel = robot.camera_system.resolution.x * 28 / 32 
      local middle_target_pixel = robot.camera_system.resolution.x * 16 / 32 
      local pixel_tolerance = robot.camera_system.resolution.x * 1 / 32
      local current_pixel, target_pixel

      if aim_point ~= nil and aim_point.case == "left" then
         current_pixel = target_tag.corners.left
         target_pixel = left_target_pixel
      elseif aim_point ~= nil and aim_point.case == "right" then
         current_pixel = target_tag.corners.right
         target_pixel = right_target_pixel
      else
         current_pixel = target_tag.center.x
         target_pixel = middle_target_pixel
      end

      local err = (target_pixel - current_pixel) / pixel_tolerance
      if err > 1 then err = 1 end
      if err < -1 then err = -1 end

      local base_speed = 0
      if aim_point.forward_backup == "forward" then
         base_speed = robot.api.parameters.default_speed 
      elseif aim_point.forward_backup == "backup" then
         base_speed = -robot.api.parameters.default_speed 
      end

      robot.api.move.with_bearing(base_speed, robot.api.parameters.default_turn_speed * err)

      if current_pixel < target_pixel - pixel_tolerance or
         current_pixel > target_pixel + pixel_tolerance then
         flag_orientation = false
      else
         flag_orientation = true
      end

      -- return bt node value
      if flag_orientation == true and flag_camera == true then
         return false, true
      else
         return true
      end
   end
end
