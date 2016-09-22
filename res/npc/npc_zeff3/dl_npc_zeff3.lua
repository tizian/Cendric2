-- Dialogue for NPC "npc_zeff3"
loadDialogue = function(DL) 

	if (not DL:isConditionFulfilled("npc_zeff3", "talked")) then 
		DL:setRoot(1) 
	else 
		DL:setRoot(3) 
	end 

	if (not DL:isConditionFulfilled("npc_zeff3", "talked")) then 

		DL:createNPCNode(1, 2, "DL_Zeff_WelcomeBack") -- Ah, it's you. Welcome to Gandria. A beautiful city, isn't it.
		DL:addNode()


		DL:createNPCNode(2, -2, "DL_Zeff_IGotHat") -- You're lucky, I've just finished the hat I promised you. Thanks again for saving me.
		DL:addConditionProgress("npc_zeff3", "talked")
		DL:addItem("eq_icehat", 1)
		DL:addNode()

	end


	DL:createChoiceNode(3)
	DL:addChoice(7, "DL_Choice_Trade") -- What do you have to offer?
	DL:addChoice(4, "DL_Choice_Leatherworking") -- Could you craft something for me?
	DL:addChoice(-1, "DL_Choice_End") -- See you later.
	DL:addNode()


	DL:createTradeNode(7, -2)
	DL:addNode()


	DL:createNPCNode(4, 5, "DL_Zeff_Leatherworking") -- Sure, if you got a fur for me?
	DL:addNode()


	DL:createChoiceNode(5)
	if (DL:hasItem("mi_firerat_fur", 1)) then 
	DL:addItemChoice(6, "DL_Choice_FireRatFur", "mi_firerat_fur", 1) -- Scarf of the Fire Rat
	end
	DL:addChoice(-2, "DL_Choice_NothingToCraft") -- I'll come back later. [BACK]
	DL:addNode()

	if (DL:hasItem("mi_firerat_fur", 1)) then 

		DL:createNPCNode(6, -2, "DL_Zeff_FireRatFur") -- The fur of a fire rat, how nice. It has the ability to protect you against fire.
		DL:gotoNode(5)
		DL:removeItem("mi_firerat_fur", 1)
		DL:addItem("eq_fireratscarf", 1)
		DL:addNode()

	end

end