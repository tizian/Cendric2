-- Dialogue for NPC "npc_jason"
loadDialogue = function(DL) 

	if (not DL:isConditionFulfilled("npc_jason", "talked")) then 
		DL:setRoot(1) 
	else 
		DL:setRoot(2) 
	end 

	if (not DL:isConditionFulfilled("npc_jason", "talked")) then 

		DL:createNPCNode(1, -2, "DL_Jason_Hello") -- Hm? Just because Rana loves to talk does not mean I do.
		DL:addConditionProgress("npc_jason", "talked")
		DL:addNode()

	end


	DL:createChoiceNode(2)
	DL:addChoice(-1, "") -- 
	DL:addNode()

end