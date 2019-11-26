function step()
   local left = 0.05
   local right = 0.05
   robot.differential_drive.set_target_velocity(left, -right)
end

function init()
end

function reset() 
end

function destroy()
end

