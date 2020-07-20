package.preload['nodes_curved_approach_block'] = function()
   -- register module with logger
   robot.logger:register_module('nodes_curved_approach_block')

   -- return node generator
   return function(data, target_distance)
      local case = {
         left_right_case = 0,
         forward_backup_case = 1,
      }
      local aim = {}
      return {
         type = "sequence",
         children = {
            -- obstacle_avoidance
            robot.nodes.create_obstacle_avoidance_node(data),
            -- check the target block is still there 
            function()
               if data.target.id == nil or
                  data.blocks[data.target.id] == nil then
                  robot.logger:log_warn("target block is nil, abort approach")
                  robot.api.move.with_velocity(0,0)
                  return false, false
               else
                  return false, true
               end
            end,
            -- analyze block angle
            function()
               local target_block = data.blocks[data.target.id]
               local robot_to_block = 
                  vector3(-target_block.position_robot) : 
                  rotate(target_block.orientation_robot:inverse())
               local angle = 
                  math.atan(robot_to_block.y / robot_to_block.x) * 180 / math.pi
               local tolerance = 
                  robot.api.parameters.aim_block_angle_tolerance * 3
               -- TODO : find a better solution
               if angle > 40 or angle < -40 then 
                  robot.logger:log_info("too close to 45 degrees, approach aborted")
                  return false, false 
               end
               if target_block.position_robot.x > target_distance + 0.05 then
                  if case.left_right_case == 0 and
                     angle > tolerance / 2 then
                     case.left_right_case = -1 -- right
                  elseif case.left_right_case == 0 and
                         angle < -tolerance / 2 then
                     case.left_right_case = 1 -- left
                  elseif case.left_right_case == 1 and
                         angle > -tolerance / 4 then
                     case.left_right_case = 0
                  elseif case.left_right_case == -1 and
                         angle < tolerance / 4 then
                     case.left_right_case = 0
                  end
                  case.done = false
               else
                  if case.done == false and
                         angle > tolerance / 2 then 
                     case.left_right_case = -1 -- right
                  elseif case.done == false and
                         angle < -tolerance / 2 then
                     case.left_right_case = 1 -- left
                  elseif case.left_right_case == 1 and
                         angle > tolerance / 6 and
                         angle < tolerance / 2 then
                     case.left_right_case = 0
                     case.done = true
                  elseif case.left_right_case == -1 and
                         angle < -tolerance / 6 and
                         angle > -tolerance / 2 then
                     case.left_right_case = 0
                     case.done = true
                  end
               end
               return false, true
            end,
            -- prepare aim
            function()
               if case.forward_backup_case == 1 and
                  case.left_right_case == 1 or
                  case.forward_backup_case == -1 and
                  case.left_right_case == -1 then
                  aim.case = "left"
               elseif case.forward_backup_case == 1 and
                      case.left_right_case == -1 or
                      case.forward_backup_case == -1 and
                      case.left_right_case == 1 then
                  aim.case = "right"
               elseif case.left_right_case == 0 then
                  aim.case = nil
               end
               return false, true
            end,
            -- aim
            robot.nodes.create_aim_block_node(data, aim),
            -- forward or backup
            function()
               local target_block = data.blocks[data.target.id]
               local tolerence = robot.api.parameters.block_position_tolerance
               local default_speed = robot.api.parameters.default_speed
               if case.forward_backup_case == 1 then
                  -- forward case
                  --if target_block.position_robot.x > target_distance - tolerence then
                  if target_block.position_robot.x > target_distance then
                     -- still too far away, move forward
                     robot.api.move.with_velocity(default_speed, default_speed)
                     return true
                  else
                     -- close enough, check angle
                     if case.left_right_case == 0 then
                        -- success
                        robot.logger:log_info("approach succeeded")
                        return false, true
                     else
                        -- close enough, but wrong angle, switch to backup
                        case.forward_backup_case = -1
                        return true
                     end
                  end
               elseif case.forward_backup_case == -1 then
                  -- backup case
                  --if target_block.position_robot.x < target_distance + 0.03 + tolerence then
                  if target_block.position_robot.x < target_distance + 0.04 then
                     -- too close, keep move backward
                     robot.api.move.with_velocity(-default_speed,
                                                  -default_speed)
                     return true
                  else
                     -- far enough, forward again
                     case.forward_backup_case = 1
                     return true
                  end
               end
               robot.logger:log_info("unable to reverse")
            end,
         },
      }
   end
end
