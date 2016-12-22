-- Routine for NPC "Nuray" in the Sewers of Gandria

loadRoutine = function(R, W)

	if (W:getGuild() == "thief" and not W:isConditionFulfilled("npc_nuray", "congrats")) then 
		R:setTalkingActive(true) 
	end 
	
	R:setTilePosition(13,3)
	
	R:setFacingRight()
	R:wait(10000)
	R:goToTile(13,4)
	R:goToTile(15,4)
	R:goToTile(15,2)
	R:setFacingDown()
	R:wait(10000)
	R:goToTile(15,4)
	R:goToTile(13,4)
	R:goToTile(13,3)
	
end	