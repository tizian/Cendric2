-- Dialogue for NPC "npc_nuray"
loadDialogue = function(DL) 

	if (DL:getGuild() == "thief" and not DL:isConditionFulfilled("npc_nuray", "congrats")) then 
		DL:setRoot(1) 
	elseif (not DL:isConditionFulfilled("npc_nuray", "who_are_you")) then 
		DL:setRoot(2) 
	else 
		DL:setRoot(4) 
	end 

	if (DL:getGuild() == "thief" and not DL:isConditionFulfilled("npc_nuray", "congrats")) then 

		DL:createNPCNode(1, -2, "DL_Nuray_Congratulations") -- So you're new here. Well done. Vincent won't let everyone join us.
		DL:addConditionProgress("npc_nuray", "congrats")
		DL:addNode()

	end

	if (not DL:isConditionFulfilled("npc_nuray", "who_are_you")) then 

		DL:createChoiceNode(2)
		DL:addChoice(3, "DL_Choice_WhoAreYou") -- Who are you?
		DL:addChoice(-1, "") -- 
		DL:addNode()


		DL:createNPCNode(3, -2, "DL_Nuray_WhoAreYou") -- I'm Nuray and I can sell you armour if you're interested.
		DL:addConditionProgress("npc_nuray", "who_are_you")
		DL:addNode()

	end


	DL:createChoiceNode(4)
	if (not DL:isConditionFulfilled("npc_nuray", "spells")) then 
		DL:addChoice(7, "DL_Choice_Spells") -- Can you teach me some spells?
	end
	DL:addChoice(6, "DL_Choice_Trade") -- Show me your wares.
	if (not DL:isConditionFulfilled("npc_nuray", "leader")) then 
		DL:addChoice(5, "DL_Choice_Leader") -- Is Vincent the leader of the guild?
	end
	if (DL:getGuild() == "thief" and DL:isQuestState("receiver", "void")) then 
		DL:addChoice(9, "DL_Choice_Receiver") -- Can I sell you my valuable stuff?
	end
	if (DL:isQuestState("missing_koray", "started") and DL:isQuestDescriptionUnlocked("find_velius",3) and DL:isQuestDescriptionUnlocked("missing_koray",1) and not DL:isConditionFulfilled("npc_nuray", "velius_found")) then 
		DL:addChoice(11, "DL_Choice_VeliusFound") -- Your brother was abducted by some mage named Velius.
	end
	DL:addChoice(-1, "") -- 
	DL:addNode()

	if (not DL:isConditionFulfilled("npc_nuray", "spells")) then 

		DL:createNPCNode(7, 8, "DL_Nuray_Spells") -- No, I'm no mage. I'm the only one in my family born without this... curse.
		DL:addNode()


		DL:createNPCNode(8, -2, "DL_Nuray_Spells2") -- But I'm quite okay without it. (Points at her daggers)
		DL:addConditionProgress("npc_nuray", "spells")
		DL:addNode()

	end


	DL:createTradeNode(6, -2)
	DL:addNode()

	if (not DL:isConditionFulfilled("npc_nuray", "leader")) then 

		DL:createNPCNode(5, -2, "DL_Nuray_Leader") -- (Smiles) No, we don't really have a "leader". Just people with more prestige than others. 
		DL:addConditionProgress("npc_nuray", "leader")
		DL:addNode()

	end

	if (DL:getGuild() == "thief" and DL:isQuestState("receiver", "void")) then 

		DL:createNPCNode(9, 10, "DL_Nuray_Receiver") -- You're looking for a receiver, eh. I'm not your girl then, but talk to Douglas on the market square.
		DL:addNode()


		DL:createNPCNode(10, -2, "DL_Nuray_Receiver2") -- Just ask him how much his monocle costs and he'll know what's up.
		DL:changeQuestState("receiver", "started")
		DL:addNode()

	end

	if (DL:isQuestState("missing_koray", "started") and DL:isQuestDescriptionUnlocked("find_velius",3) and DL:isQuestDescriptionUnlocked("missing_koray",1) and not DL:isConditionFulfilled("npc_nuray", "velius_found")) then 

		DL:createNPCNode(11, -2, "DL_Nuray_VeliusFound") -- What?! Damn mages! Go and find him!
		DL:addConditionProgress("npc_nuray","velius_found")
		DL:addNode()

	end

end