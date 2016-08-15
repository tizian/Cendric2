-- Routine for NPC "Vincent" in Gandria

loadRoutine = function(R, W)

	if (not W:isConditionFulfilled("npc_vincent2", "talked") and not W:isQuestState("spoiled_fire", "started")) then 
		R:setTalkingActive(true) 
	end 
	
	R:setTilePosition(5,51.5)

	R:goToTile(15,51.5)
	R:wait(1000)
	R:goToTile(5,51.5)
	R:wait(1000)

end	