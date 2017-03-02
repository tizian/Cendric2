-- Routine for NPC "Inina" in the sewers

velocity = 40

loadRoutine = function(R, W)
	
	if (not W:isConditionFulfilled("npc_inina2","talked")) then
		R:setTilePosition(6,17)
		R:setFacingDown()
		R:setLooped(false)
		R:setReloadEnabled(true)
		return
	end

	R:setTilePosition(3,15)
	R:setReloadEnabled(false)
	
	R:setFacingUp()
	R:wait(4000)	
	R:goToTile(6,15)
	R:goToTile(6,25)
	R:goToTile(2,25)
	R:setFacingUp()
	R:wait(4000)	
	R:goToTile(6,25)
	R:goToTile(6,15)
	R:goToTile(7,15)
	R:goToTile(7,10)
	R:setFacingLeft()
	R:wait(6000)
	R:goToTile(7,15)
	R:goToTile(3,15)
	
end	