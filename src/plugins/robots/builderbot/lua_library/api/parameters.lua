package.preload['api_parameters'] = function()
    -- register module with logger
    robot.logger:register_module('api_parameters')

    return {
        default_speed = tonumber(robot.params.default_speed or 0.005),
        default_turn_speed = tonumber(robot.params.default_turn_speed or 5),
        search_random_range = tonumber(robot.params.search_random_range or 25),
        aim_block_angle_tolerance = tonumber(robot.params.aim_block_angle_tolerance or 0.5),
        block_position_tolerance = tonumber(robot.params.block_position_tolerance or 0.001),
        proximity_touch_tolerance = tonumber(robot.params.proximity_touch_tolerance or 0.003),
        proximity_detect_tolerance = tonumber(robot.params.proximity_detect_tolerance or 0.03),
        proximity_maximum_distance = tonumber(robot.params.proximity_maximum_distance or 0.05),
        lift_system_rf_cover_threshold = tonumber(robot.params.lift_system_rf_cover_threshold or 0.06),
        lift_system_position_tolerance = tonumber(robot.params.lift_system_position_tolerance or 0.001),
        obstacle_avoidance_backup = tonumber(robot.params.obstacle_avoidance_backup or 0.08),
        obstacle_avoidance_turn = tonumber(robot.params.obstacle_avoidance_turn or 60),
    }
end
