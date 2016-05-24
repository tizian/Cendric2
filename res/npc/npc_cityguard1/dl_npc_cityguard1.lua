-- Dialogue for the two NPCs "City Guard"
loadDialogue = function(DL)
	
	
	

	if (not DL:isConditionFulfilled("npc_cityguard1", "talked")) then
		
		DL:createNPCNode(0, 1, "DL_Citguard_Stop") -- Stop! The gates of Gandria are closed for strangers!
		DL:addNode()
	
		DL:setRoot(0)
		
	else
	
		DL:setRoot(1)
		
	end
	
end	
	