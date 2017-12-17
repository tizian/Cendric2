-- Routine for NPC "King Logan III" in Velius' Dungeon, when he's back again..

loadRoutine = function(R, W)

    if (not W:isConditionFulfilled("boss", "BossVelius")) then
        R:setDisposed()
        return
    end

    if (not W:isConditionFulfilled("npc_loganthird4", "talked")) then
        R:setTalkingActive(true)
    end
    
    R:setReloadEnabled(false)
    R:setFacingDown()
    R:setLooped(false)
    R:setTilePosition(12.5, 9)

end	