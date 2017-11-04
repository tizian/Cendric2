-- Routine for NPC "King Logan III" in Velius' Dungeon, when he's dead..

loadRoutine = function(R, W)

    if (not W:isConditionFulfilled("npc_loganthird2","dead")) then
        R:setDisposed()
        return
    end

    R:setTalkingEnabled(false)
    R:setReloadEnabled(false)
    R:setLooped(false)
    R:setTilePosition(12.5, 9)

end	