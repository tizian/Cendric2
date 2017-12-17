-- Routine for NPC "Inina" in Velius' Dungeon after Velius is dead.

velocity = 50

loadRoutine = function(R, W)

    if (not W:isConditionFulfilled("boss", "BossVelius") or W:isConditionFulfilled("npc_inina4", "dead")) then
        R:setDisposed()
        return
    end
	
	R:setTilePosition(11.5,10)
    R:setFacingRight()
	R:setReloadEnabled(false)
    R:setLooped(false)
	
end	