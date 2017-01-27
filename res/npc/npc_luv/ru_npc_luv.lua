-- Routine for NPC "Luv"

velocity = 40

loadRoutine = function(R, W)

	if (W:isConditionFulfilled("npc_leeluv","gone")) then
		R:setDisposed()
	else
		R:setTilePosition(68,58)
		
		R:goToTile(66,58)
		R:setFacingLeft()
		R:wait(5000)

		R:goToTile(68,58)
		R:goToTile(68,52)
		R:goToTile(73,52)
		R:goToTile(73,56)
		R:setFacingDown()
		R:wait(8000)
		
		R:goToTile(73,53)
		R:goToTile(68,53)
		R:goToTile(68,60)
		R:setFacingDown()
		R:wait(3000)
		
		R:goToTile(68,58)
	end

end	