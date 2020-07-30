package.preload['api_match_rules'] = function()
   -- register module with logger
   robot.logger:register_module('api_match_rules')

   local function check_position_in_safe_zone(position, safe_zone)
      if safe_zone == nil or safe_zone == 1 then return true end
      -- x right, y down
      local horizontal_fov = math.pi/3 -- TODO: ask argos to provide
      local vertical_fov = math.pi/3
      local y_max = math.tan(horizontal_fov / 2)
      local y_min = -math.tan(horizontal_fov / 2)
      local x_max = math.tan(vertical_fov / 2)
      local x_min = -math.tan(vertical_fov / 2)
      if type(safe_zone) == "number" then
         x_max = x_max * safe_zone
         x_min = x_min * safe_zone
         y_max = y_max * safe_zone
         y_min = y_min * safe_zone
      elseif type(safe_zone) == "table" then
         if safe_zone.up_margin ~= nil then
            y_min = y_min * (1 - safe_zone.up_margin * 2)
         end
         if safe_zone.down_margin ~= nil then
            y_max = y_max * (1 - safe_zone.down_margin * 2)
         end
         if safe_zone.left_margin ~= nil then
            x_min = x_min * (1 - safe_zone.left_margin * 2)
         end
         if safe_zone.right_margin ~= nil then
            x_max = x_max * (1 - safe_zone.right_margin * 2)
         end
      end
      local x = position.x / position.z
      local y = position.y / position.z
      if x_min <= x and x < x_max and
         y_min <= y and y < y_max then
            return true
      else
            return false
      end
   end

   local function round_by_digits(num, numDecimalPlaces)
      local mult = 10 ^ (numDecimalPlaces or 0)
      return math.floor(num * mult + 0.5) / mult
   end

   -- generate structure in indices frame from a visual structure or a rule
   local function generate_uniform_structure(structure, view_point)
      -- view point is the position and orientation relative to structure[1]
      view_point = view_point or {}
      view_point.position = view_point.position or vector3()
      view_point.orientation = view_point.orientation or quaternion()
      -- calculate relative position, camera in viewpoint
      local block1_in_camera = {
         position = structure[1].position or structure[1].index,
         orientation = structure[1].orientation or quaternion(),
      }
      local viewpoint_in_camera = {
         position = block1_in_camera.position + 
                  vector3(view_point.position):rotate(block1_in_camera.orientation),
         orientation = view_point.orientation * block1_in_camera.orientation,
      }
      local camera_in_viewpoint = {
         position = vector3(-viewpoint_in_camera.position):rotate(
                     viewpoint_in_camera.orientation:inverse()
                  ),
         orientation = viewpoint_in_camera.orientation:inverse(),
      }
      local side_length = robot.api.constants.block_side_length
      if structure[1].position == nil then side_length = 1 end
      -- rotate and uniform every block in the structure
      local uniform_structure = {}
      for i, block in ipairs(structure) do
         local block_in_camera = {
            position = block.position or block.index,
            orientation = block.orientation or quaternion(),
         }
         local uniform_block = {
            id = block.id,
            type = block.type,
            index = camera_in_viewpoint.position +
                  vector3(block_in_camera.position):rotate(camera_in_viewpoint.orientation),
         }
         uniform_block.index = uniform_block.index * (1/side_length)
         uniform_block.index.x = round_by_digits(uniform_block.index.x, 0)
         uniform_block.index.y = round_by_digits(uniform_block.index.y, 0)
         uniform_block.index.z = round_by_digits(uniform_block.index.z, 0)
         table.insert(uniform_structure, uniform_block)
      end
      -- offset indices based on the lowest x,y,z
      local lowest_x = math.huge
      local lowest_y = math.huge
      local lowest_z = math.huge
      for i, indexed_block in ipairs(uniform_structure) do
         -- Getting lowest x,y,z (should be seperated along with transform indexed blocks to unified origin)
         if indexed_block.index.x < lowest_x then
            lowest_x = indexed_block.index.x
         end
         if indexed_block.index.y < lowest_y then
            lowest_y = indexed_block.index.y
         end
         if indexed_block.index.z < lowest_z then
            lowest_z = indexed_block.index.z
         end
      end
      local origin = vector3(lowest_x, lowest_y, lowest_z)
      -- tranform indexed blocks to unified origin
      for i, block in pairs(uniform_structure) do
         block.index = block.index - origin
      end
      -- for a visual structure, we need to calculate the real location from index later
      uniform_structure.index_to_position = {
         offset = block1_in_camera.position +
                  vector3(origin * side_length):rotate(viewpoint_in_camera.orientation),
         orientation = viewpoint_in_camera.orientation,
      }
      return uniform_structure
   end

   local function generate_aligned_visual_structures(structures)
      local structures_in_index_frame = {}
      for i, group in ipairs(structures) do
         table.insert(structures_in_index_frame, 
            generate_uniform_structure(group)
         )
      end
      return structures_in_index_frame
   end

   local function generate_new_rule_by_rotating_90(rule)
      local new_rule = {
         rule_type = rule.rule_type,
         external_condition = rule.external_condition,
         generate_orientations = rule.generate_orientations, -- TODO: unused
         safe_zone = rule.safe_zone,                         -- TODO: unused
         structure = generate_uniform_structure(rule.structure, {
            orientation = quaternion(-math.pi/2, vector3(0,0,1))
         }),
         target = {}
      }
      new_rule.target.reference_index = 
         new_rule.structure[1].index + 
         vector3(rule.target.reference_index - rule.structure[1].index) :
         rotate(quaternion(math.pi/2, vector3(0,0,1)))
      new_rule.target.offset_from_reference = 
         vector3(rule.target.offset_from_reference) : 
         rotate(quaternion(math.pi/2, vector3(0,0,1)))
      return new_rule
   end

   local function generate_aligned_and_rotated_rules_structures(rules_list)
      local aligned_and_rotated_rules_list = {}
      for i, rule in ipairs(rules_list) do
         local new_rule = rule
         table.insert(aligned_and_rotated_rules_list, new_rule)
         new_rule = generate_new_rule_by_rotating_90(new_rule)
         table.insert(aligned_and_rotated_rules_list, new_rule)
         new_rule = generate_new_rule_by_rotating_90(new_rule)
         table.insert(aligned_and_rotated_rules_list, new_rule)
         new_rule = generate_new_rule_by_rotating_90(new_rule)
         table.insert(aligned_and_rotated_rules_list, new_rule)
      end
      return aligned_and_rotated_rules_list
   end

   local function match_structures(visual_structure, rule_structure, offset)
      local structure_matching_result = true
      for i, rule_block in ipairs(rule_structure) do
         local block_matched = false
         for j, visual_block in ipairs(visual_structure) do
            if visual_block.index == rule_block.index + offset then --found required index
               if (visual_block.type == rule_block.type) or (rule_block.type == nil) then -- found the same required type
                  block_matched = true
                  break
               end
            end
         end
         if block_matched == false then
            structure_matching_result = false
            break
         end
      end
      return structure_matching_result
   end

   local function get_block_id_from_index(index, visual_structure)
      for i, block in ipairs(visual_structure) do
         if block.index == index then
            return block.id
         end
      end
      return nil
   end

   local function generate_possible_targets(visual_structures, rule_list, rule_type, external_condition)
      -- generate possible targets
      local possible_targets = {}
      for i, visual_structure in ipairs(visual_structures) do
         for j, rule in ipairs(rule_list) do
            for k, reference_block_in_visual in ipairs(visual_structure) do
               -- position of the reference block:
               local reference_position = 
                  visual_structure.index_to_position.offset + 
                  vector3(reference_block_in_visual.index * robot.api.constants.block_side_length):rotate(
                     visual_structure.index_to_position.orientation
                  )
               local offset = reference_block_in_visual.index - rule.target.reference_index
               if (rule.external_condition == nil or rule.external_condition == external_condition) and
                  rule.rule_type == rule_type and
                  check_position_in_safe_zone(reference_position, rule.safe_zone) and
                  match_structures(visual_structure, rule.structure, offset) then

                  local new_target = {
                     reference_id =
                        get_block_id_from_index(rule.target.reference_index + offset, visual_structure),
                     offset = rule.target.offset_from_reference,
                     type = rule.target.type,
                     safe = true,
                  }
                  table.insert(possible_targets, new_target)
               end
            end
         end
      end
      return possible_targets
   end

   local function select_target(blocks, possible_targets, selection_method)
      local nearest_distance = math.huge
      local nearest_target = nil
      local furthest_distance = -math.huge
      local furthest_target = nil
      for i, target in ipairs(possible_targets) do
         local reference_block = blocks[target.reference_id]
         local target_position = 
            reference_block.position +
            vector3(target.offset * robot.api.constants.block_side_length) :
               rotate(reference_block.orientation)
         local distance = target_position:length()
         if distance < nearest_distance then
            nearest_distance = distance
            nearest_target = target
         end
         if distance > furthest_distance then
            furthest_distance = distance
            furthest_target = target
         end
      end
      if selection_method == "nearest_win" then
         return nearest_target
      elseif selection_method == "furthest_win" then
         return furthest_target
      end
   end

   -- return the module function
   return function(blocks, rules, rule_type, external_condition)
      local structures = {}
      robot.api.process_structures(structures, blocks)
      if #structures == 0 then
         return nil
      end
      -- align visual structures
      local structures_in_index_frame = 
         generate_aligned_visual_structures(structures)
      -- align rule structures
      if rules.aligned == nil then 
         rules.list = 
            generate_aligned_and_rotated_rules_structures(rules.list)
         rules.aligned = true
      end
      -- match rules
      local possible_targets = 
         generate_possible_targets(structures_in_index_frame, rules.list, rule_type, external_condition)
      local target = 
         select_target(blocks, possible_targets, rules.selection_method)
      return target
   end
end
