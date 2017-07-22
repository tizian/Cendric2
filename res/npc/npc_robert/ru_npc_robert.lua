-- Routine for NPC "Robert"

loadRoutine = function(R, W)

    if (W:isConditionFulfilled("boss", "BossMercenaries")) then
        R:setDisposed()
        return
    end

	R:setTilePosition(7,15)

	R:goToTile(8, 15)
	R:goToTile(8, 14.5)
	R:setFacingUp()
	R:wait(5000)
	
	R:goToTile(9,15)
	R:goToTile(9,16)
	R:goToTile(14,16)
	R:goToTile(14,15.5)
	R:setFacingUp()
	R:wait(10000)
	
	R:goToTile(14,16)
	R:goToTile(9,16)
	R:goToTile(9,15)
	
	R:goToTile(7,15)
	R:setFacingDown()
	R:wait(5000)
	
end	