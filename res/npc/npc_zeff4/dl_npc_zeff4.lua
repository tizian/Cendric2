-- Dialogue for NPC "npc_zeff4"
loadDialogue = function(DL) 

	if (not DL:isConditionFulfilled("npc_zeff4", "talked")) then 
		DL:setRoot(1) 
	else 
		DL:setRoot(2) 
	end 

	if (not DL:isConditionFulfilled("npc_zeff4", "talked")) then 

		DL:createNPCNode(1, -2, "DL_Zeff_Na") -- Hn.
		DL:addConditionProgress("npc_zeff4", "talked")
		DL:addNode()

	end


	DL:createChoiceNode(2)
	DL:addChoice(-1, "") -- 
	DL:addNode()

end