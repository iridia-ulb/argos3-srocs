-- just as a reminder {black = 0, pink = 1, orange = 2, green = 3, blue = 4}

local rules = {}
rules.list = {
   {
      rule_type = 'pickup',
      structure = {
         {
            index = vector3(0, 0, 0),
            type = 0
         },
      },
      target = {
         reference_index = vector3(0, 0, 0),
         offset_from_reference = vector3(0, 0, 0),
      },
      generate_orientations = false
   },
   --[[
   {
      rule_type = 'place',
      structure = {
         {
            index = vector3(0, 0, 0),
            type = 1
         },
      },
      target = {
         reference_index = vector3(0, 0, 0),
         offset_from_reference = vector3(1, 0, 0),
      },
      generate_orientations = false
   },
   ]]
   {
      rule_type = 'place',
      structure = {
         {
            index = vector3(0, 0, 0),
            type = 5,
         },
      },
      target = {
         reference_index = vector3(0, 0, 0),
         offset_from_reference = vector3(1, 0, 0),
         type = 1
      },
      generate_orientations = false
   }
}
rules.selection_method = 'nearest_win'
return rules
