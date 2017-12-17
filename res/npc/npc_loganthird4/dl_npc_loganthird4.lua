-- Dialogue for NPC "npc_loganthird4"
loadDialogue = function(DL) 

	if (not DL:isConditionFulfilled("npc_loganthird4", "talked")) then 
		DL:setRoot(1) 
	else 
		DL:setRoot(4) 
	end 

	if (not DL:isConditionFulfilled("npc_loganthird4", "talked")) then 

		DL:createNPCNode(1, 2, "DL_Logan_Over") -- Wh... What is this... The voice in my head... is gone?
		DL:addConditionProgress("npc_loganthird4", "talked")
		DL:addNode()


		DL:createChoiceNode(2)
		DL:addChoice(3, "DL_Choice_Over") -- It's over.
		DL:addNode()


		DL:createNPCNode(3, -2, "DL_Logan_Over2") -- I... can't believe it.
		DL:addNode()

	end


	DL:createChoiceNode(4)
	DL:addChoice(-1, "") -- 
	DL:addNode()

end