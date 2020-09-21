-- register module with logger
robot.logger:register_module('api_process_structures')

-- for now we use distances to tell if blocks are connected or not,
-- this is not acurate, it could be done by figuring out if two blocks
-- share at least one edge, but it would require transforming one block position to the other block
local function check_connected(block_1, block_2)
   local result = false
   local orientation_tolerance = 0.0174533 -- one degree
   local distance_tolerance = 0.08
   local orientation_diff = math.sqrt(
      (block_1.orientation_robot.x - block_2.orientation_robot.x) ^ 2 +
      (block_1.orientation_robot.y - block_2.orientation_robot.y) ^ 2
   )

   local position_diff = (block_1.position_robot - block_2.position_robot):length()

   if orientation_diff > orientation_tolerance then
      result = false
   else
      if position_diff < distance_tolerance then
         result = true
      else
         result = false
      end
   end
   return result
end

local function add_block_to_group(structures, new_block)
   -- search existing groups, check each block to see if connected with the new one
   for i, group in ipairs(structures) do
      for j, block in ipairs(group) do
         if check_connected(block, new_block) then
            table.insert(group, new_block)
            return
         end
      end
   end
   -- the new_block doesn't belong to any existing group, create a new group
   table.insert(structures, {new_block})
end

return function(structures, blocks)
   -- clear structures
   for i, v in pairs(structures) do
      structures[i] = nil
   end
   -- cluster blocks into structures
   for i, block in pairs(blocks) do
      add_block_to_group(structures, block)
   end
end
