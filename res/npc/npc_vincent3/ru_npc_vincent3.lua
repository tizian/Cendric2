-- Routine for NPC "Vincent" in the Sewers of Gandria

loadRoutine = function(R, W)

	if (not W:isConditionFulfilled("npc_vincent3", "talked")) then 
		R:setTalkingActive(true) 
		R:setTilePosition(2,3)
		R:setFacingUp()
		return
	end 
	
	R:setTilePosition(6,3)
	R:setFacingDown()
	
	R:wait(5000)
	R:goToTile(12,3)
	R:goToTile(12,5)
	R:goToTile(15,5)
	R:goToTile(15,3)
	R:setFacingLeft()
	R:wait(10000)
	R:goToTile(15,5)
	R:goToTile(12,5)
	R:goToTile(12,3)
	R:goToTile(6,3)

end	