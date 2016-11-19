-- Dialogue for NPC "npc_yorick"
loadDialogue = function(DL) 

	DL:setRoot(1) 


	DL:createChoiceNode(1)
	if (not DL:isConditionFulfilled("npc_yorick", "alas")) then 
		DL:addChoice(2, "DL_Choice_Alas") -- Alas, poor Yorick!
	end
	DL:addChoice(-1, "") -- 
	DL:addNode()

	if (not DL:isConditionFulfilled("npc_yorick", "alas")) then 

		DL:createNPCNode(2, -1, "DL_Yorick_Alas") -- ...
		DL:addConditionProgress("npc_yorick", "alas")
		DL:addNode()

	end

end