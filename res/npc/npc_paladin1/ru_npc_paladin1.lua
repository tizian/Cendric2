-- Routine for NPC "Paladin1"

velocity = 40

loadRoutine = function(R, W)

	if (W:isConditionFulfilled("default","gandria_open")) then
		R:setDisposed()
	else
		R:setTilePosition(45,54)
		R:setReloadEnabled(false)
		
		R:wait(5000)	
		R:goToTile(45,54)
		R:goToTile(46,55)
	end

end	