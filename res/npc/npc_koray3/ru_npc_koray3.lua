-- Routine for NPC "Koray" in Velius' Dungeon after Velius is dead.

velocity = 50

loadRoutine = function(R, W)

    if (not W:isConditionFulfilled("boss", "BossVelius") or W:isConditionFulfilled("npc_koray3", "dead")) then
        R:setDisposed()
        return
    end
	
	R:setTilePosition(12.5,11)
    R:setFacingUp()
	R:setReloadEnabled(false)
    R:setLooped(false)
	
end	