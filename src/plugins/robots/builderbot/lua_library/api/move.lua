package.preload['api_move'] = function()
   -- register module with logger
   robot.logger:register_module('api_move')

   return {
      with_velocity = function(left, right)
         robot.differential_drive.set_target_velocity(left, -right)
      end,

      -- move with bearing:  
      -- move v m/s forward, with th degree/s to the left
      -- TODO: needs validation on real robot
      with_bearing = function(velocity, angle)
         local interwheel_dist = 0.1225
         local diff = interwheel_dist * math.pi * (angle / 360)
         local left = velocity - diff
         local right = velocity + diff
         robot.differential_drive.set_target_velocity(left, -right)
      end,

      -- move following vector3
      -- x front, y left
      with_vector3 = function(vec)
         local v = vec:length()
         if vec.x < 0 then
            v = -v
         end
         local diff = math.atan(vec.y / vec.x) * 180 / math.pi
         robot.api.move.with_bearing(v, diff)
      end,
   }
end
