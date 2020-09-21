-- register module with logger
robot.logger:register_module('api_process_obstacles')

return function(obstacles, blocks)
   local end_effector_position =
      robot.api.constants.end_effector_position_offset + vector3(0, 0, robot.lift_system.position)

   -- clear obstacles table
   for i, v in pairs(obstacles) do obstacles[i] = nil end

   -- what rangefinders sense
   for i, rf in pairs(robot.rangefinders) do
      if rf.proximity < robot.api.parameters.proximity_maximum_distance then
         local obstacle_position_robot =
            vector3(0, 0, rf.proximity):rotate(rf.transform.orientation) + rf.transform.position
         if rf.transform.anchor == 'end_effector' then
            obstacle_position_robot = obstacle_position_robot + end_effector_position
         end
         obstacles[#obstacles + 1] = {
            position = obstacle_position_robot,
            source = tostring(i)
         }
      end
   end

   -- every block that the camera sees is considered an obstacle
   if blocks ~= nil then
      for i, block in pairs(blocks) do
         obstacles[#obstacles + 1] = {
            position = block.position_robot,
            source = 'camera'
         }
      end
   end
end
