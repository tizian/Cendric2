-- Dialogue for NPC "npc_robert"
loadDialogue = function(DL) 

	if (DL:getGuild() == "necromancer" and not DL:isConditionFulfilled("npc_robert", "congrats")) then 
		DL:setRoot(1) 
	else 
		DL:setRoot(2) 
	end 

	if (DL:getGuild() == "necromancer" and not DL:isConditionFulfilled("npc_robert", "congrats")) then 

		DL:createNPCNode(1, -2, "DL_Robert_Congrats") -- Oh, so you're the new one here! I hope we can be friends then.
		DL:addConditionProgress("npc_robert", "congrats")
		DL:addNode()

	end


	DL:createChoiceNode(2)
	if (not DL:isConditionFulfilled("npc_robert", "who_are_you") and not DL:isConditionFulfilled("npc_robert2", "who_are_you")) then 
		DL:addChoice(3, "DL_Choice_WhoAreYou") -- Who are you?
	end
	if (not DL:isConditionFulfilled("npc_robert", "who_are_you") and DL:isConditionFulfilled("npc_robert2", "who_are_you")) then 
		DL:addChoice(8, "DL_Choice_HeyBob") -- Hey Bob!
	end
	DL:addChoice(-1, "") -- 
	DL:addNode()

	if (not DL:isConditionFulfilled("npc_robert", "who_are_you") and not DL:isConditionFulfilled("npc_robert2", "who_are_you")) then 

		DL:createNPCNode(3, 4, "DL_Robert_WhoAreYou") -- I'm Robert, you can call me Bob. 
		DL:addConditionProgress("npc_robert", "who_are_you")
		DL:addNode()


		DL:createNPCNode(4, 5, "DL_Robert_WhoAreYou2") -- I'm new here, but already working like crazy to become a good Necromancer. Luiz says I have talent.
		DL:addNode()


		DL:createChoiceNode(5)
		DL:addChoice(6, "DL_Choice_YouHaveTalent") -- I'm sure you'll do great.
		DL:addChoice(7, "DL_Choice_NoTalent") -- At least someone who thinks that way.
		DL:addNode()


		DL:createNPCNode(6, -2, "DL_Robert_YouHaveTalent") -- Thank you. I guess you could learn a lot too, maybe with a bit more time.
		DL:addNode()


		DL:createNPCNode(7, -1, "DL_Robert_NoTalent") -- Better than no one. Looking at you...
		DL:addNode()

	end

	if (not DL:isConditionFulfilled("npc_robert", "who_are_you") and DL:isConditionFulfilled("npc_robert2", "who_are_you")) then 

		DL:createNPCNode(8, 9, "DL_Robert_HeyBob") -- (Surprised) You, here? So you're a mage after all...
		DL:addConditionProgress("npc_robert", "who_are_you")
		DL:addNode()


		DL:createNPCNode(9, 10, "DL_Robert_WhoAreYou2") -- 
		DL:addNode()


		DL:createChoiceNode(10)
		DL:addChoice(11, "DL_Choice_YouHaveTalent") -- 
		DL:addChoice(12, "DL_Choice_NoTalent") -- 
		DL:addNode()


		DL:createNPCNode(11, -2, "DL_Robert_YouHaveTalent") -- 
		DL:addNode()


		DL:createNPCNode(12, -1, "DL_Robert_NoTalent") -- 
		DL:addNode()

	end

end