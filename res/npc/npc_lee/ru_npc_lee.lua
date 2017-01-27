-- Routine for NPC "Lee"

velocity = 40

loadRoutine = function(R, W)

	if (W:isConditionFulfilled("npc_leeluv","gone")) then
		R:setDisposed()
	else
		R:setTilePosition(62,54)
		R:setFacingDown()
		R:wait(2000)
		
		R:goToTile(57,54)
		R:goToTile(57,60)
		R:setFacingDown()
		R:wait(4000)
		
		R:goToTile(57,58)
		R:goToTile(58,58)
		R:setFacingRight()
		R:wait(5000)
		
		R:goToTile(57,54)
		R:goToTile(50,54)
		R:goToTile(50,60)
		R:goToTile(51,60)
		R:setFacingDown()
		R:wait(8000)
		
		R:goToTile(50,54)
		R:goToTile(50,60)
		R:goToTile(57,54)
		R:goToTile(62,54)
	end

end	