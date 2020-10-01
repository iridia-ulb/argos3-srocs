-- register module with logger
robot.logger:register_module('api_track_blocks')

--    this function finds axis of a block :    
--         |Z           Z| /Y       the one pointing up is Z
--         |__ Y         |/         the nearest one pointing towards the camera is X
--        /               \         and then Y follows right hand coordinate system
--      X/                 \X
--
-- All vector in the system of the camera
--             /z
--            /
--            ------- x
--            |
--            |y     in the camera's eye
local function find_block_xyz(position, orientation) -- for camera
   local X, Y, Z -- vectors of XYZ axis of a block (in camera's coor system) 
   -- all the 6 dirs of a block
   local dirs = {}
   dirs[1] = vector3(1,0,0)
   dirs[2] = vector3(0,1,0)
   dirs[3] = vector3(0,0,1)
   dirs[1]:rotate(orientation)
   dirs[2]:rotate(orientation)
   dirs[3]:rotate(orientation)
   dirs[4] = -dirs[1]
   dirs[5] = -dirs[2]
   dirs[6] = -dirs[3]
   -- clear out 3 pointing far away
   for i, v in pairs(dirs) do
      if v.z > 0 then dirs[i] = nil end
   end
   -- choose the one pointing highest(min y) as Z 
   local highest_i 
   local highest_y = 0
   for i, v in pairs(dirs) do
      if v.y < highest_y then highest_y = v.y highest_i = i end
   end
   Z = dirs[highest_i]
   dirs[highest_i] = nil
   -- choose the one pointing nearest(min z) as X
   local nearest_i
   local nearest_z = math.huge
   for i, v in pairs(dirs) do
      if (position + v):length() < nearest_z then nearest_z = (position + v):length(); nearest_i = i end
   end
   X = dirs[nearest_i]
   dirs[nearest_i] = nil
   Y = vector3(Z):cross(X)
   return X, Y, Z  -- unit vectors
end

local function xy_to_quaternion(_orientation, _X, _Y)
   -- assume Z match
   -- from the XY to calculate the right quaternion
   local orientation = _orientation
   local x = vector3(1,0,0)
   x:rotate(orientation)
   if (x - _X):length() < 0.2 then     
      -- x match 
      return orientation
   elseif (x - _Y):length() < 0.2 then 
      -- x matches Y, rotate 90 clockwise
      return orientation * quaternion(-math.pi/2, vector3(0,0,1))
   elseif (x + _X):length() < 0.2 then 
      -- x matches -X, rotate 180 clockwise
      return orientation * quaternion(math.pi, vector3(0,0,1))
   elseif (x + _Y):length() < 0.2 then 
      -- x matches -Y, rotate 90 anti-clockwise
      return orientation * quaternion(math.pi/2, vector3(0,0,1))
   end
end

local function xyx_to_quaternion(_orientation, _X, _Y, _Z)
   -- from the XYZ to calculate the right quaternion
   local orientation = _orientation
   local x = vector3(1,0,0)
   local y = vector3(0,1,0)
   local z = vector3(0,0,1)
   x:rotate(orientation)
   y:rotate(orientation)
   z:rotate(orientation)
   if (z - _Z):length() < 0.2 then     
      -- z is up
      return xy_to_quaternion(orientation, _X, _Y)
   elseif (-z - _Z):length() < 0.2 then     
      -- -z is up, rotate 180 along x
      orientation = orientation * quaternion(math.pi, vector3(1,0,0))
      return xy_to_quaternion(orientation, _X, _Y)
   elseif (x - _Z):length() < 0.2 then     
      -- x is up, rotate a-clock 90 along y
      orientation = orientation * quaternion(math.pi/2, vector3(0,1,0))
      return xy_to_quaternion(orientation, _X, _Y)
   elseif (-x - _Z):length() < 0.2 then     
      -- -x is up, rotate clock 90 along y
      orientation = orientation * quaternion(-math.pi/2, vector3(0,1,0))
      return xy_to_quaternion(orientation, _X, _Y)
   elseif (y - _Z):length() < 0.2 then     
      -- y is up, rotate clock 90 along x
      orientation = orientation * quaternion(-math.pi/2, vector3(1,0,0))
      return xy_to_quaternion(orientation, _X, _Y)
   elseif (-y - _Z):length() < 0.2 then     
      -- y is up, rotate a-clock 90 along x
      orientation = orientation * quaternion(math.pi/2, vector3(1,0,0))
      return xy_to_quaternion(orientation, _X, _Y)
   end
end

local function update_block(old_block, new_block)
   old_block.position = new_block.position
   old_block.orientation = new_block.orientation
   old_block.X = new_block.X
   old_block.Y = new_block.Y
   old_block.Z = new_block.Z
   old_block.tags = new_block.tags
end

local function hungarian_match(_old_blocks, _new_blocks)
   -- the index of _old_blocks maybe not consistent, like 1, 2, 4, 6
   -- put it into oldBlockArray with 1,2,3,4
   local old_block_array = {}
   local count = 0
   for i, block in pairs(_old_blocks) do
      count = count + 1
      old_block_array[count] = block
      old_block_array[count].index = i
   end
   -- max size
   local n = #old_block_array
   if #_new_blocks > n then n = #_new_blocks end
   -- set penalty matrix
      -- fill n * n with 0
   local penalty_base = 0.1
   local penalty_matrix = {}
   for i = 1, n do 
      penalty_matrix[i] = {}
      for j = 1,n do 
         penalty_matrix[i][j] = penalty_base
      end
   end
   --                new blocks
   --             * * * * * * * *
   -- old blocks  *             *
   --             * * * * * * * *
   for i, old_block in ipairs(old_block_array) do
      for j, new_block in ipairs(_new_blocks) do
         local dis = (old_block.position - new_block.position):length()
         penalty_matrix[i][j] = dis + penalty_base + 0.01   -- 0.01 to make it not the base
      end
   end
   -- TODO: something is wrong with the hungarian algorithm, 
   --       when value is near 0, it doesn't give the right answer, 
   --       this penalty base is a bypass of this problem
   local hun = robot.utils.hungarian.create(penalty_matrix, false) -- false => min problem
   hun:aug()
   -- hun.match_of_X[i] is the index of match for old_block_array[i]
   for i, old_block in ipairs(old_block_array) do
      if penalty_matrix[i][hun.match_of_X[i]] == penalty_base then
         -- lost
         local index = old_block.index
         _old_blocks[index] = nil
      else
         -- tracking
         local index = old_block.index
         --_old_blocks[index] = _new_blocks[hun.match_of_X[i]]
         update_block(_old_blocks[index], _new_blocks[hun.match_of_X[i]])
      end
   end
   local index = 1
   for j, new_block in ipairs(_new_blocks) do
      if penalty_matrix[hun.match_of_Y[j]][j] == penalty_base then
         -- new blocks
         while _old_blocks[index] ~= nil do index = index + 1 end
         _old_blocks[index] = new_block
         _old_blocks[index].id = index
      end
   end
end

local function check_tag_direction(block)
   for i, tag in ipairs(block.tags) do
      local dif = (tag.position - block.position) * (1/robot.api.constants.block_side_length) * 2
      if (block.X - dif):length() < 0.5 then
         block.tags.front = tag
      elseif (block.Z - dif):length() < 0.5 then
         block.tags.up = tag
      elseif (block.Y - dif):length() < 0.5 then
         block.tags.right = tag
      elseif (-block.Y - dif):length() < 0.5 then
         block.tags.left = tag
      end
   end
   for i, tag in ipairs(block.tags) do
      block.tags[i] = nil
   end
end

return function(_blocks, _tags)
   local blocks = {}
   -- cluster tags into blocks
   local p = vector3(0, 0, robot.api.constants.block_side_length/2)
   for i, tag in ipairs(_tags) do
      local middle_point_v3 = vector3(p):rotate(tag.orientation) + tag.position
      -- find which block it belongs
      local flag = 0
      for j, block in ipairs(blocks) do
         if (middle_point_v3 - block.position):length() < robot.api.constants.block_side_length/3 then
            flag = 1
            block.tags[#block.tags + 1] = tag
            block.positionSum = block.positionSum + middle_point_v3
            break
         end
      end
      if flag == 0 then
         blocks[#blocks + 1] = {
            position = middle_point_v3, 
            positionSum = middle_point_v3,
            orientation = tag.orientation,
            tags = {
               tag
            },
         }
      end
   end
   -- average block position
   for i, block in ipairs(blocks) do
      block.position = block.positionSum * (1/#block.tags)
      block.positionSum = nil
   end
   -- adjust block orientation
   for i, block in ipairs(blocks) do
      block.X, block.Y, block.Z = find_block_xyz(block.position, block.orientation)
         -- X,Y,Z are unit vectors
      block.orientation = xyx_to_quaternion(block.orientation, block.X, block.Y, block.Z)
         -- to make orientation matches X,Y,Z
      check_tag_direction(block)
   end
   hungarian_match(_blocks, blocks)
end
