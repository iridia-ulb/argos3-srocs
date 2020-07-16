-- the logger must be loaded first since the other modules register against it
robot.logger = {
   verbosity = 0,
   modules = {},
   enabled = {
      ["nil"] = false
   },
   -- log an error
   log_err = function(self, first, ...)
      local info = debug.getinfo(2)
      local src = info.short_src
      local module_name = self.modules[src]
      if self.enabled[module_name] then
         self:log('err@' .. module_name .. ':', first, ...)
      end
   end,
   -- log a warning
   log_warn = function(self, first, ...)
      local info = debug.getinfo(2)
      local src = info.short_src
      local module_name = self.modules[src]
      if self.enabled[module_name] and self.verbosity > 0 then
         self:log('warn@' .. module_name .. ':', first, ...)
      end
   end,
   -- log info
   log_info = function(self, first, ...)
      local info = debug.getinfo(2)
      local src = info.short_src
      local module_name = self.modules[src]
      if self.enabled[module_name] and self.verbosity > 1 then
         self:log('info@' .. module_name .. ':', first, ...)
      end
   end,
   -- writes a message to the log
   log = function(self, prefix, first, ...)
      if type(first) == "table" then
         robot.utils.display.table(first)
      else
         print(prefix, first, ...)
      end
   end,
   -- sets the verbosity of the logger
   set_verbosity = function(self, verbosity)
      if type(verbosity) == 'number' then
         self.verbosity = verbosity
      else
         error("Invalid value passed to set_verbosity")
      end
   end,
   -- registers a module
   register_module = function(self, module_name)
      local info = debug.getinfo(2)
      local src = info.short_src
      self.modules[src] = module_name
      self.enabled[module_name] = true
   end,
   -- disables a module
   disable = function(self, module_name)
      self.enabled[module_name] = false
   end,
   -- enables a module
   enable = function(self, module_name)
      self.enabled[module_name] = true
   end,
}

function init_lua_library()
   -- functions for moving, detecting and tracking leds, blocks, tags etc
   robot.api = {
      constants =          require('api.constants'),
      parameters =         require('api.parameters'),
      match_rules =        require('api.match_rules'),
      move =               require('api.move'),
      process_blocks =     require('api.process_blocks'),
      process_leds =       require('api.process_leds'),
      process_obstacles =  require('api.process_obstacles'),
      process_structures = require('api.process_structures'),
      track_blocks =       require('api.track_blocks'),
   }
   -- functions for creating behavior tree nodes
   robot.nodes = {
      create_search_block_node =          require('nodes.search_block'),
      create_approach_block_node =        require("nodes.approach_block"),
      create_z_approach_block_node =      require('nodes.z_approach_block'),
      create_curved_approach_block_node = require('nodes.curved_approach_block'),
      create_pick_up_block_node =         require('nodes.pick_up_block'),
      create_place_block_node =           require('nodes.place_block'),
      create_reach_block_node =           require("nodes.reach_block"),
      create_aim_block_node =             require('nodes.aim_block'),
      create_timer_node =                 require('nodes.timer'),
      create_obstacle_avoidance_node =    require('nodes.obstacle_avoidance'),
      create_random_walk_node =           require("nodes.random_walk"),
      create_process_rules_node =         require("nodes.process_rules"),
      create_move_to_location_node =      require("nodes.move_to_location"),
      create_pick_up_behavior_node =      require('nodes.pick_up_behavior'),
      create_place_behavior_node =        require('nodes.place_behavior'),
   }
   -- utility functions
   robot.utils = {
      behavior_tree =   require('utils.behavior_tree'),
      draw =            require('utils.draw'),    
      hungarian =       require('utils.hungarian'),
      queue =           require('utils.queue'),
   }
end
