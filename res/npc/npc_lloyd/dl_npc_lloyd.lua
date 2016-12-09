-- Dialogue for NPC "npc_lloyd"
loadDialogue = function(DL) 

	if (not L:isConditionFulfilled("npc_lloyd", "talked")) then 
		DL:setRoot(3) 
	else 
		DL:setRoot(1) 
	end 

	if (not L:isConditionFulfilled("npc_lloyd", "talked")) then 

		DL:createChoiceNode(3)
		DL:addChoice(5, "DL_Choice_HelloUnpolite") -- Hi.
		DL:addChoice(6, "DL_Choice_HelloMediumPolite") -- Hello, Lloyd.
		DL:addChoice(4, "DL_Choice_HelloPolite") -- All hail, Commander Lloyd.
		DL:addChoice(-2, "") -- 
		DL:addNode()


		DL:createNPCNode(5, -2, "DL_Lloyd_HelloUnpolite") -- (Lloyd glares down at you, clearly not pleased) What do you want? Keep it short.
		DL:addConditionProgress("npc_lloyd", "talked")
		DL:addNode()


		DL:createNPCNode(6, -2, "DL_Lloyd_HelloUnpolite") -- 
		DL:addConditionProgress("npc_lloyd", "talked")
		DL:addNode()


		DL:createNPCNode(4, -2, "DL_Lloyd_HelloPolite") -- (Lloyd nods to you) Hello, young mage. What brings you to me?
		DL:addConditionProgress("npc_lloyd", "talked")
		DL:addReputationProgress("cleric", 5)
		DL:addNode()

	end


	DL:createChoiceNode(1)
	if (DL:isQuestState("clerics_recommendation", "started") and DL:isQuestComplete("clerics_recommendation")) then 
		DL:addChoice(2, "DL_Choice_RecommendationQuest") -- I got this from Inina for you. (Give letter)
	end
	if (not DL:isConditionFulfilled("npc_lloyd", "want_cleric")) then 
		DL:addChoice(7, "DL_Choice_IWantToJoin") -- I want to become a cleric.
	end
	DL:addChoice(-1, "") -- 
	DL:addNode()

	if (DL:isQuestState("clerics_recommendation", "started") and DL:isQuestComplete("clerics_recommendation")) then 

		DL:createNPCNode(2, 8, "DL_Lloyd_ReadLetter") -- (Lloyd reads the letter) Hm, interesting. Thank you for helping us with that problem.
		DL:changeQuestState("clerics_recommendation", "completed")
		DL:addReputationProgress("cleric", 5)
		DL:addNode()


		DL:createNPCNode(8, -2, "DL_Lloyd_ReadLetter2") -- Inina seems to think a great deal of you. She wants you to join the clerics and help her find out where that monster came from.
		DL:addNode()

	end

	if (not DL:isConditionFulfilled("npc_lloyd", "want_cleric")) then 

		DL:createNPCNode(7, -2, "DL_Lloyd_NoCleric") -- Well, you may be a mage, but why should I trust you? Also, we don't really need new recruits at the moment.
		DL:addConditionProgress("npc_lloyd", "want_cleric")
		DL:addNode()

	end

end