-- register module with logger
robot.logger:register_module('api_constants')

return {
    block_side_length = 0.055,
    -- from x+, counter-closewise
    block_led_offset_from_tag = {
    vector3(0.02,  0,    0),
    vector3(0,     0.02, 0),
    vector3(-0.02, 0,    0),
    vector3(0,    -0.02, 0)
    },
    -- TODO move into the SRoCS API
    lift_system_upper_limit = 0.135,
    lift_system_lower_limit = 0,
    end_effector_position_offset = vector3(0.09800875, 0, 0.055),
    end_effector_position_pickup_bias = 0.002, -- move less when picking up a block
    end_effector_position_place_bias = 0.002, -- secure distance above ground when placing a block
}
