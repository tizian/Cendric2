-- Routine for NPC "Inina"

velocity = 40

loadRoutine = function(R, W)

	if (W:isConditionFulfilled("default","gandria_open")) then
		R:setDisposed()
		return
	end

	R:setTilePosition(44,52)

	R:wait(4000)	
	R:goToTile(43,52)
	R:goToTile(44,52)
	R:goToTile(44,51)
	R:wait(4000)
	R:goToTile(44,54)
	R:wait(4000)
	R:goToTile(44,52)
	R:goToTile(46,52)

end	