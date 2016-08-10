-- Routine for NPC "Paladin2"

velocity = 40

loadRoutine = function(R, W)

	if (W:isConditionFulfilled("default","gandria_open")) then
		R:setDisposed()
	end

	R:setTilePosition(44,54)

	R:wait(5000)	
	R:goToTile(44,54)
	R:goToTile(43,55)

end	