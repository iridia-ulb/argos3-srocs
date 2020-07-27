package.preload['nodes_process_rules'] = function()
    -- register module with logger
    robot.logger:register_module('nodes_process_rules')
 
    -- return node generator
    return function(data, rules, rule_type)
        return function()
            local target = robot.api.match_rules(data.blocks, rules, rule_type, data.sensor_condition)
            if target == nil then
                data.target.id = nil
                data.target.offset = vector3()
                return false, false
            else
                data.target.id = target.reference_id
                data.target.offset = target.offset
                data.target.type = target.type
                return false, true
            end
        end
    end
end
