-- Routine for NPC "King Logan III" in Velius' Dungeon

velocity = 150

loadRoutine = function(R, W)

    if (W:isConditionFulfilled("npc_loganthird2","dead")) then
        R:setDisposed()
        return
    end

    
    R:setTalkingEnabled(false)
    R:setReloadEnabled(false)
    R:setLooped(false)
    R:setTilePosition(12.5, 15.5)
    R:goToTile(12.5, 9)
    W:addConditionProgress("npc_loganthird2", "dead")

end	