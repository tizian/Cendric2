-- Dialogue for the two NPCs "City Guard"
loadDialogue = function(DL)
	
	
	

	if (not DL:isConditionFulfilled("npc_cityguard", "talked")) then
		
		DL:createNPCNode(0, 1, "DL_Citguard_Stop") -- STOP! The gates of Gandria are closed for strangers!
		DL:setConditionFulfilled("npc_cityguard", "talked")
		DL:addNode()
	
		DL:setRoot(0)
		
	else
	
		DL:setRoot(1)
		
	end
	
end	
	