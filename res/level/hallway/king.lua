-- Behavior for king logan III

isCheckpoint = false
state = 0

update = function(B, W)
    if (state == 0) then
        B:say("King1", 3)
        B:switchLever()
        B:setMovingTarget(710,200)
        B:wait(2)
        state = 1
        return
    end
    if (state == 1) then     
        B:executeFightAnimation()
        B:switchLever()
        B:wait(1)
        state = 2
        return
    end
    if (state == 2) then     
        B:setMovingTarget(950,280)
        B:wait(1)
        state = 3
        return
    end
    if (state == 3) then     
        B:executeFightAnimation()
        B:switchLever()
        B:wait(2)
        state = 4
        return
    end
    if (state == 4) then     
        B:setMovingTarget(1150,330)
        B:wait(1)
        state = 5
        return
    end
    if (state == 5) then     
        B:say("King2", 4)
        B:executeFightAnimation()
        B:switchLever()
        B:wait(4)
        state = 6
        return
    end
    if (state == 6) then     
        B:setMovingTarget(1760,580)
        B:wait(2)
        state = 7
        return
    end
    if (state == 7) then     
        B:setMovingTarget(1740,580)
        state = 8
        return
    end
    if (state == 8) then     
        B:say("King3", 4)
        B:executeFightAnimation()
        B:switchLever()
        B:wait(1)
        state = 9
        return
    end
    if (state == 9) then     
        B:setMovingTarget(1580,690)
        B:wait(3)
        state = 10
        return
    end
    if (state == 10) then     
        B:setMovingTarget(1800,800)
        B:wait(2)
        state = 11
        return
    end
    if (state == 11) then     
        B:say("King4", 3)
        B:executeFightAnimation()
        B:switchLever()
        B:wait(3)
        state = 12
        return
    end
    if (state == 12) then     
        B:setMovingTarget(2150,600)
        B:wait(4)
        state = 13
        return
    end
    if (state == 13) then     
        B:say("King5", 5)
        B:executeFightAnimation()
        B:switchLever()
        B:wait(8)
        state = 14
        return
    end
    if (state == 14) then     
        B:setMovingTarget(2950,500)
        B:wait(6)
        state = 15
        return
    end
    if (state == 15) then  
        B:say("King6", 10)
        B:wait(9)
        state = 16
        return
    end
    if (state == 16) then  
        B:say("King1", 6)
        B:setMovingTarget(3725,600)
        B:wait(6)
        state = 17
        return
    end
    if (state == 17) then  
        B:leaveLevel()
        state = 18
        return
    end
end
