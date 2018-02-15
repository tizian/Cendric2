-- Dialogue for NPC "npc_clementia"
loadDialogue = function(DL) 

	if (not DL:isConditionFulfilled("npc_clementia", "talked")) then 
		DL:setRoot(1) 
	else 
		DL:setRoot(2) 
	end 

	if (not DL:isConditionFulfilled("npc_clementia", "talked")) then 

		DL:createNPCNode(1, -2, "DL_Clementia_Hi") -- Oh, hello there! If you're looking for the finest weapons in Gandria, you're lucky; here they are!
		DL:addConditionProgress("npc_clementia", "talked")
		DL:addNode()

	end


	DL:createChoiceNode(2)
	DL:addChoice(3, "DL_Choice_ShowYourWeapons") -- Show me your weapons.
	DL:addChoice(-1, "") -- 
	DL:addNode()


	DL:createTradeNode(3, -2)
	DL:addNode()

end