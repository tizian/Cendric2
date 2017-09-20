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
	if (not DL:isConditionFulfilled("npc_zeff3", "thea") and DL:isQuestState("theas_dream", "started")) then 
		DL:addChoice(11, "DL_Choice_Thea") -- Do you happen to need an apprentice?
	end
	DL:addChoice(7, "DL_Choice_Trade") -- What do you have to offer?
	DL:addChoice(4, "DL_Choice_Leatherworking") -- Could you craft something for me?
	DL:addChoice(-1, "DL_Choice_End") -- See you later.
	DL:addNode()

	if (not DL:isConditionFulfilled("npc_zeff3", "thea") and DL:isQuestState("theas_dream", "started")) then 

		DL:createNPCNode(11, -2, "DL_Zeff_Thea") -- Hrr. No, I'm sorry, I prefer to work alone.
		DL:addConditionProgress("npc_zeff3", "thea")
		DL:addNode()

	end


	DL:createTradeNode(7, -2)
	DL:addNode()


	DL:createNPCNode(4, 5, "DL_Zeff_Leatherworking") -- Sure, if you got a fur for me?
	DL:addNode()


	DL:createChoiceNode(5)
	if (DL:hasItem("mi_firerat_fur", 1)) then 
		DL:addCraftingChoice(6, "DL_Choice_FireRatFur") -- eq_fireratscarf,mi_firerat_fur,1,gold,20
	end
	if (DL:hasItem("mi_neko_teeth", 1) or DL:hasItem("mi_neko_fur", 1)) then 
		DL:addCraftingChoice(8, "DL_Choice_NekomataCloak") -- eq_nekomataback,mi_neko_fur,3,mi_neko_teeth,2,gold,70
	end
	if (DL:hasItem("mi_neko_teeth", 1) or DL:hasItem("mi_neko_fur", 1)) then 
		DL:addCraftingChoice(9, "DL_Choice_NekomataHead") -- eq_nekomatahead,mi_neko_fur,1,mi_neko_teeth,6,gold,100
	end
	if (DL:hasItem("mi_wolf_fur", 1)) then 
		DL:addCraftingChoice(10, "DL_Choice_WolfBonnet") -- eq_wolfbonnet,mi_wolf_fur,3,gold,40
	end
	DL:addChoice(-2, "DL_Choice_NothingToCraft") -- I'll come back later. [BACK]
	DL:addNode()

	if (DL:hasItem("mi_firerat_fur", 1)) then 

		DL:createNPCNode(6, -2, "DL_Zeff_FireRatFur") -- The fur of a fire rat, how nice. It has the ability to protect you against fire.
		DL:gotoNode(5)
		DL:addNode()

	end

	if (DL:hasItem("mi_neko_teeth", 1) or DL:hasItem("mi_neko_fur", 1)) then 

		DL:createNPCNode(8, -2, "DL_Zeff_NekomataCloak") -- A fine cloak for a skilled hunter. It's not easy to slay those beasts.
		DL:gotoNode(5)
		DL:addNode()

	end

	if (DL:hasItem("mi_neko_teeth", 1) or DL:hasItem("mi_neko_fur", 1)) then 

		DL:createNPCNode(9, -2, "DL_Zeff_NekomataHead") -- There you go. A very special headpiece, indeed.
		DL:gotoNode(5)
		DL:addNode()

	end

	if (DL:hasItem("mi_wolf_fur", 1)) then 

		DL:createNPCNode(10, -2, "DL_Zeff_Wolfbonnet") -- Wolf fur? I hope this isn't some kind of sick joke. But here, take this bonnet.
		DL:gotoNode(5)
		DL:addNode()

	end

end