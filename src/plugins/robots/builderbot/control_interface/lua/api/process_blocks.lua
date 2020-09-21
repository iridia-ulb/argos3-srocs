-- register module with logger
robot.logger:register_module("api_process_blocks")

return function(blocks)
   -- calculate the camera position
   local camera_position =
      robot.api.constants.end_effector_position_offset +
      robot.camera_system.transform.position +
      vector3(0, 0, robot.lift_system.position)
   -- track blocks
   robot.api.track_blocks(blocks, robot.camera_system.tags)
   -- cache the block's position and orientation in the robot's coordinate system
   for i, block in pairs(blocks) do
      block.position_robot =
         vector3(block.position):rotate(robot.camera_system.transform.orientation) + camera_position
      block.orientation_robot = robot.camera_system.transform.orientation * block.orientation
   end
end
