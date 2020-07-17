package.preload['nodes_aim_block'] = function()
   -- register module with logger
   robot.logger:register_module('nodes_aim_block')

   -- return node generator
   return function(data, aim_point)
      -- aim block, put the block into the center of the image
      return function()
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
         local tolerance = robot.api.parameters.lift_system_position_tolerance
         local target_height = robot.lift_system.position - target_tag.position.y 
         local upper_limit = robot.api.constants.lift_system_upper_limit
         local lower_limit = robot.api.constants.lift_system_lower_limit + target_block.position_robot.z - 0.02
         if target_height < lower_limit then target_height = lower_limit end
         if target_height > upper_limit then target_height = upper_limit end
         if robot.lift_system.position > target_height - tolerance and 
            robot.lift_system.position < target_height + tolerance then
            flag_camera = true
         else
            robot.lift_system.set_position(target_height)
            flag_camera = false
         end
         local turn
         local err = 1
         if aim_point ~= nil and aim_point.case == "left" then
            if target_tag.corners.left < robot.camera_system.resolution.x * 3 / 32 then
               turn = "left"
               flag_orientation = false
            elseif target_tag.corners.left > robot.camera_system.resolution.x * 5 / 32 then
               turn = "right"
               flag_orientation = false
            else
               turn = "no"
               flag_orientation = true
            end
         elseif aim_point ~= nil and aim_point.case == "right" then
            if target_tag.corners.right < robot.camera_system.resolution.x * 27 / 32 then
               turn = "left"
               flag_orientation = false
            elseif target_tag.corners.right > robot.camera_system.resolution.x * 29 / 32 then
               turn = "right"
               flag_orientation = false
            else
               turn = "no"
               flag_orientation = true
            end
         else
            local tolerence = robot.api.parameters.aim_block_angle_tolerance 
            local angle = math.atan(target_block.position_robot.y / target_block.position_robot.x) * 180 / math.pi  -- x should always be positive
            if angle < -tolerence then
               turn = "right"
               err = -angle / 10
               flag_orientation = false
            elseif angle > tolerence then
               turn = "left"
               err = angle / 10
               flag_orientation = false
            else
               turn = "no"
               flag_orientation = true
            end
         end
         if turn == "left" then
            robot.api.move.with_velocity(-robot.api.parameters.default_speed * err -
                                          robot.api.parameters.default_speed / 2, 
                                       robot.api.parameters.default_speed * err +
                                          robot.api.parameters.default_speed / 2)
            flag_orientation = false
         elseif turn == "right" then
            robot.api.move.with_velocity(robot.api.parameters.default_speed * err +
                                          robot.api.parameters.default_speed / 2,
                                       -robot.api.parameters.default_speed * err -
                                          robot.api.parameters.default_speed / 2)
            flag_orientation = false
         elseif turn == "no" then
            robot.api.move.with_velocity(0, 0)
            flag_orientation = true
         end
         if flag_orientation == true and flag_camera == true then
            return false, true
         else
            return true
         end
      end
   end
end
