-- Dialogue for NPC "npc_etoz"
loadDialogue = function(DL) 

	if (not DL:isConditionFulfilled("npc_etoz", "talked")) then 
		DL:setRoot(1) 
	else 
		DL:setRoot(2) 
	end 

	if (not DL:isConditionFulfilled("npc_etoz", "talked")) then 

		DL:createNPCNode(1, -2, "DL_Etoz_Hello") -- Hello
		DL:addConditionProgress("npc_etoz", "talked")
		DL:addNode()

	end


	DL:createChoiceNode(2)
	DL:addChoice(-1, "") -- 
	DL:addNode()

end