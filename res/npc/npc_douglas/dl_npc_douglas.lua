-- Dialogue for NPC "npc_douglas"
loadDialogue = function(DL) 

	if (not DL:isConditionFulfilled("npc_douglas", "talked")) then 
		DL:setRoot(1) 
	else 
		DL:setRoot(2) 
	end 

	if (not DL:isConditionFulfilled("npc_douglas", "talked")) then 

		DL:createNPCNode(1, -2, "DL_Douglas_Hi") -- Hey you! You look like you could use some new armour! Come and take a look!
		DL:addConditionProgress("npc_douglas", "talked")
		DL:addNode()

	end


	DL:createChoiceNode(2)
	DL:addChoice(3, "DL_Choice_ShowYourWares") -- Show me your armour.
	if (DL:isQuestState("receiver", "started")) then 
		DL:addChoice(4, "DL_Choice_Receiver") -- How much is your monocle?
	end
	DL:addChoice(-1, "") -- 
	DL:addNode()


	DL:createTradeNode(3, -2)
	DL:addNode()

	if (DL:isQuestState("receiver", "started")) then 

		DL:createNPCNode(4, -2, "DL_Douglas_Receiver") -- (Grins) That's priceless.
		DL:changeQuestState("receiver", "completed")
		DL:addConditionProgress("default", "receiver_douglas")
		DL:addReputationProgress("thief", 5)
		DL:addNode()

	end

end