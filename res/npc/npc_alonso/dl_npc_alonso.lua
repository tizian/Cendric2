-- Dialogue for NPC "npc_alonso"
loadDialogue = function(DL) 

	if (not DL:isConditionFulfilled("npc_alonso", "talked")) then 
		DL:setRoot(1) 
	else 
		DL:setRoot(2) 
	end 

	if (not DL:isConditionFulfilled("npc_alonso", "talked")) then 

		DL:createNPCNode(1, -2, "DL_Alonso_Hi") -- Sausages! Cheese! Ham! Cheap and fresh! 
		DL:addConditionProgress("npc_alonso", "talked")
		DL:addNode()

	end


	DL:createChoiceNode(2)
	DL:addChoice(3, "DL_Choice_ShowYourWares") -- Show me your wares.
	DL:addChoice(-1, "") -- 
	DL:addNode()


	DL:createTradeNode(3, -2)
	DL:addNode()

end