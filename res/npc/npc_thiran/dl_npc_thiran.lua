-- Dialogue for NPC "npc_thiran"
loadDialogue = function(DL) 

	if (not DL:isConditionFulfilled("npc_thiran", "talked")) then 
		DL:setRoot(1) 
	else 
		DL:setRoot(3) 
	end 

	if (not DL:isConditionFulfilled("npc_thiran", "talked")) then 

		DL:createChoiceNode(1)
		DL:addChoice(2, "DL_Choice_WhoAreYou") -- Who are you?
		DL:addChoice(-1, "") -- 
		DL:addNode()


		DL:createNPCNode(2, -2, "DL_Thiran_WhoAreYou") -- I'm Thiran, quartermaster of the clerics. What can I do for you?
		DL:addConditionProgress("npc_thiran", "talked")
		DL:addNode()

	end


	DL:createChoiceNode(3)
	DL:addChoice(4, "DL_Choice_Wares") -- Show me your wares.
	if (DL:getGuild() ~= "cleric") then 
		DL:addChoice(5, "DL_Choice_Learn") -- Can you teach me some spells?
	end
	if (DL:getGuild() == "cleric" and not DL:isConditionFulfilled("npc_thiran", "first_spell")) then 
		DL:addChoice(6, "DL_Choice_Learn") -- 
	end
	if (DL:isConditionFulfilled("npc_thiran", "first_spell") and DL:getReputation("cleric") < 50 and not DL:isConditionFulfilled("npc_thiran", "second_spell")) then 
		DL:addChoice(7, "DL_Choice_SecondSpell") -- Can you teach me more spells?
	end
	if (DL:isConditionFulfilled("npc_thiran", "first_spell") and DL:getReputation("cleric") >= 50 and not DL:isConditionFulfilled("npc_thiran", "second_spell")) then 
		DL:addChoice(8, "DL_Choice_SecondSpell") -- 
	end
	DL:addChoice(-1, "") -- 
	DL:addNode()


	DL:createTradeNode(4, -2)
	DL:addNode()

	if (DL:getGuild() ~= "cleric") then 

		DL:createNPCNode(5, -2, "DL_Thiran_NoTeaching") -- No, I only share my knowledge with members of the order.
		DL:addNode()

	end

	if (DL:getGuild() == "cleric" and not DL:isConditionFulfilled("npc_thiran", "first_spell")) then 

		DL:createNPCNode(6, -2, "DL_Thiran_YesTeachingFirst") -- Yes, as you've chosen the way of the Eternal Light, I can grant you access to its magic. Here, take this scroll and learn how to unleash the wrath of light.
		DL:addConditionProgress("npc_thiran", "first_spell")
		DL:addItem("sp_holyfire", 1)
		DL:addNode()

	end

	if (DL:isConditionFulfilled("npc_thiran", "first_spell") and DL:getReputation("cleric") < 50 and not DL:isConditionFulfilled("npc_thiran", "second_spell")) then 

		DL:createNPCNode(7, -2, "DL_Thiran_NoFurtherSpell") -- Not yet. I'm going to teach you another spell when you've worked for us.
		DL:addNode()

	end

	if (DL:isConditionFulfilled("npc_thiran", "first_spell") and DL:getReputation("cleric") >= 50 and not DL:isConditionFulfilled("npc_thiran", "second_spell")) then 

		DL:createNPCNode(8, -2, "DL_Thiran_SecondSpell") -- Yes, you've proven yourself worthy to learn more divine magic. Here you go.
		DL:addItem("sp_divineshield", 1)
		DL:addConditionProgress("npc_thiran", "second_spell")
		DL:addNode()

	end

end