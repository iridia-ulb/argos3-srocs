
function init()                                                                                                                                                         
    state = 'preflight'                                                                                                                                                 
 end             


function step()                                                                                                                                                        
    print('pose = ' .. tostring(robot.flight_system.position))                                                                                                                                                                                                                                                                                                                                                        
    if robot.flight_system.ready() then                                                                                                                                 
      if state == 'preflight' then 
         print('state = ' .. state)                                                                                                                                       
         print('arming drone')                                                                                                                                       
         robot.flight_system.set_armed(true, false)
         robot.flight_system.set_offboard_mode(true) 
         print('enabling off-board mode')
         robot.flight_system.set_target_pose(vector3(0,0,1), 0)                                                                                                                                                                                                                                                                                                                    
         state = 'flight'   
      elseif state == 'flight' then      
         print('state = ' .. state)    
      end 
   end                                                                                                                                                                
end                                                                                                                                                                     


function reset()
end

function destroy()
   print('disarming drone')
   robot.flight_system.set_armed(false, false)                     
   print('disabling off-board mode')
   robot.flight_system.set_offboard_mode(false)
end 


