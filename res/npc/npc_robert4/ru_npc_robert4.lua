-- Routine for NPC "Robert" in Velius' Dungeon after Velius is dead.

velocity = 50

loadRoutine = function(R, W)

    if (not W:isConditionFulfilled("boss", "BossVelius") or W:isConditionFulfilled("npc_robert4", "dead")) then
        R:setDisposed()
        return
    end
	
	R:setTilePosition(13.5,10)
    R:setFacingLeft()
	R:setReloadEnabled(false)
    R:setLooped(false)
	
end	