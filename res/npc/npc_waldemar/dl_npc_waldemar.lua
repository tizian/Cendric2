-- Dialogue for NPC "npc_waldemar"
loadDialogue = function(DL) 

	if (not DL:isConditionFulfilled("npc_waldemar", "talked")) then 
		DL:setRoot(1) 
	else 
		DL:setRoot(2) 
	end 

	if (not DL:isConditionFulfilled("npc_waldemar", "talked")) then 

		DL:createNPCNode(1, -2, "DL_Waldemar_ANewFace") -- Ah, a new face. Interested in buying some armour?
		DL:addConditionProgress("npc_waldemar", "talked")
		DL:addNode()

	end


	DL:createChoiceNode(2)
	if (not DL:isConditionFulfilled("npc_waldemar", "beak")) then 
		DL:addChoice(4, "DL_Choice_Mask") -- What's up with that... beak?
	end
	DL:addChoice(3, "DL_Choice_Trade") -- What do you have to offer?
	DL:addChoice(-1, "") -- 
	DL:addNode()

	if (not DL:isConditionFulfilled("npc_waldemar", "beak")) then 

		DL:createNPCNode(4, -2, "DL_Waldemar_Beak") -- A very useful mask when dealing with little pests. (Grins)
		DL:addConditionProgress("npc_waldemar", "beak")
		DL:addNode()

	end


	DL:createTradeNode(3, -2)
	DL:addNode()

end