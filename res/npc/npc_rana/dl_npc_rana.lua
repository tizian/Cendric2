-- Dialogue for NPC "npc_rana"
loadDialogue = function(DL) 

	if (not DL:isConditionFulfilled("npc_rana", "talked")) then 
		DL:setRoot(1) 
	else 
		DL:setRoot(2) 
	end 

	if (not DL:isConditionFulfilled("npc_rana", "talked")) then 

		DL:createNPCNode(1, 5, "DL_Rana_Hello") -- Hello there! You look like you could use some warm fire to warm you up.
		DL:addConditionProgress("npc_rana", "talked")
		DL:addNode()


		DL:createNPCNode(5, -2, "DL_Rana_Hello2") -- It's freezing here in the Highlands.
		DL:addNode()

	end


	DL:createChoiceNode(2)
	DL:addChoice(4, "DL_Choice_UnlockTrade") -- Do you sell something?
	if (DL:isConditionFulfilled("npc_rana", "trade")) then 
		DL:addChoice(3, "DL_Choice_Trade") -- Show me your wares.
	end
	DL:addChoice(-1, "") -- 
	DL:addNode()


	DL:createNPCNode(4, -2, "DL_Rana_UnlockTrade") -- Yes, we can trade.
	DL:addConditionProgress("npc_rana", "trade")
	DL:addNode()

	if (DL:isConditionFulfilled("npc_rana", "trade")) then 

		DL:createTradeNode(3, -2)
		DL:addNode()

	end

end