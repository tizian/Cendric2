-- Routine for NPC "Inina" in Velius' Dungeon

velocity = 40

loadRoutine = function(R, W)

    if (W:isConditionFulfilled("boss", "BossVelius")) then
        R:setDisposed()
        return
    end
	
	R:setTilePosition(5,5)
	R:setReloadEnabled(false)
	
	R:setFacingUp()
	R:wait(5000)	
	R:goToTile(6,5)
	R:setFacingDown()
	R:wait(2000)
    R:goToTile(4,5)
	R:setFacingLeft()
	R:wait(1400)	
    R:goToTile(5,5)	
	
end	