-- register module with logger
robot.logger:register_module('utils_draw')

local function arrow(color, from, to)
   if robot.debug then
      if robot.debug.draw then
         robot.debug.draw('arrow(' .. color .. ')(' .. from:__tostring() .. ')(' .. to:__tostring() .. ')')
      else
         robot.logger:log_warn("debug draw interface missing")
      end
   else
      robot.logger:log_warn("debug actuator missing")
   end
end

local function block_axes(block_position, block_orientation, color)
   local z = vector3(0, 0, 1)
   arrow(color, block_position, block_position + 0.1 * vector3(z):rotate(block_orientation))
end

local function line(color, from, to)
   function range(from, to, step)
      step = step or 1
      return function(_, lastvalue)
         local nextvalue = lastvalue + step
         if step > 0 and nextvalue <= to or step < 0 and nextvalue >= to or step == 0 then
            return nextvalue
         end
      end, nil, from - step
   end
   p0 = from
   p1 = to
   vdr = p1 - p0
   last_subpoint = p0
   for lambda in range(0, 1, 0.1) do
      x_ = p0.x + lambda * vdr.x
      y_ = p0.y + lambda * vdr.y
      z_ = p0.z + lambda * vdr.z
      current_subpoint = vector3(x_, y_, z_)
      arrow(color, last_subpoint, current_subpoint)
      last_subpoint = current_subpoint
   end
end

-- return the module table
return { arrow = arrow, block_axes = block_axes, line = line }
