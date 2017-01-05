-- Dialogue for NPC "npc_lloyd"
loadDialogue = function(DL) 

	if (not DL:isConditionFulfilled("npc_lloyd", "talked")) then 
		DL:setRoot(3) 
	elseif (DL:isQuestState("further_investigation", "void")) then 
		DL:setRoot(1) 
	elseif (DL:getGuild() ~= "void" and not DL:isConditionFulfilled("npc_lloyd", "guild")) then 
		DL:setRoot(19) 
	elseif (not DL:isConditionFulfilled("npc_lloyd", "guild")) then 
		DL:setRoot(10) 
	else 
		DL:setRoot(17) 
	end 

	if (not DL:isConditionFulfilled("npc_lloyd", "talked")) then 

		DL:createChoiceNode(3)
		DL:addChoice(5, "DL_Choice_HelloUnpolite") -- Hi.
		DL:addChoice(6, "DL_Choice_HelloMediumPolite") -- Hello, Lloyd.
		DL:addChoice(4, "DL_Choice_HelloPolite") -- All hail, Commander Lloyd.
		DL:addChoice(-1, "") -- 
		DL:addNode()


		DL:createNPCNode(5, -2, "DL_Lloyd_HelloUnpolite") -- (Lloyd glares down at you, clearly displeased) What do you want? Keep it short.
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

	if (DL:isQuestState("further_investigation", "void")) then 

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


			DL:createNPCNode(8, 9, "DL_Lloyd_ReadLetter2") -- Inina seems to think a great deal of you. She wants you to join the clerics and help her find out where that monster came from.
			DL:addNode()


			DL:createNPCNode(9, -2, "DL_Lloyd_ReadLetter3") -- You two should go back to the place where you defeated that monster and look for hints.
			DL:changeQuestState("further_investigation", "started")
			DL:addConditionProgress("default", "second_boss_open")
			DL:addNode()

		end

		if (not DL:isConditionFulfilled("npc_lloyd", "want_cleric")) then 

			DL:createNPCNode(7, -2, "DL_Lloyd_NoCleric") -- Well, you may be a mage, but why should I trust you? We can't just take anyone.
			DL:addConditionProgress("npc_lloyd", "want_cleric")
			DL:addNode()

		end

	end

	if (DL:getGuild() ~= "void" and not DL:isConditionFulfilled("npc_lloyd", "guild")) then 

		DL:createNPCNode(19, -2, "DL_Lloyd_AnotherGuild") -- I'm sorry, but I see that you have already chosen another path. I still hope you'll use your magic for to do good.
		DL:addConditionProgress("npc_lloyd", "guild")
		DL:addNode()

	end

	if (not DL:isConditionFulfilled("npc_lloyd", "guild")) then 

		DL:createChoiceNode(10)
		if (not DL:isConditionFulfilled("npc_lloyd", "one_guild")) then 
			DL:addChoice(15, "DL_Choice_OneGuild") -- What if I want to join another guild?
		end
		if (not DL:isConditionFulfilled("npc_lloyd", "what_if_join")) then 
			DL:addChoice(16, "DL_Choice_WhatIfIJoin") -- What happens if I join you?
		end
		DL:addChoice(12, "DL_Choice_IWantToJoin") -- 
		DL:addChoice(-1, "") -- 
		DL:addNode()

		if (not DL:isConditionFulfilled("npc_lloyd", "one_guild")) then 

			DL:createNPCNode(15, -2, "DL_Lloyd_OneGuild") -- That's for you to decide. But make your decision carefully, as you may only join one guild.
			DL:addConditionProgress("npc_lloyd", "one_guild")
			DL:addNode()

		end

		if (not DL:isConditionFulfilled("npc_lloyd", "what_if_join")) then 

			DL:createNPCNode(16, -2, "DL_Lloyd_WhatIfIJoin") -- You'll get to learn divine magic. Also, we'll give you a job that will bring you gold and honour.
			DL:addConditionProgress("npc_lloyd", "what_if_join")
			DL:addNode()

		end


		DL:createNPCNode(12, 18, "DL_Lloyd_JoinQuestion") -- Alright, young mage. If you want to join the order of the clerics, you'll have to swear the oath of the Eternal Light.
		DL:addNode()


		DL:createNPCNode(18, 13, "DL_Lloyd_JoinQuestion2") -- Promise to use your magic to heal and protect the innocent and to punish the ones who do not follow the path of the light.
		DL:addNode()


		DL:createChoiceNode(13)
		DL:addChoice(14, "DL_Choice_Promise") -- I promise. [JOIN THE CLERICS]
		DL:addChoice(-2, "DL_Choice_Rethink") -- I need more time to think about this.
		DL:addNode()


		DL:createNPCNode(14, -1, "DL_Lloyd_JointClerics") -- I now declare you a mage of the Order of the Clerics. May the Eternal Light guide thee. 
		DL:addConditionProgress("npc_lloyd", "guild")
		DL:addConditionProgress("default", "cleric")
		DL:addReputationProgress("cleric", 10)
		DL:setGuild("cleric")
		DL:addItem("eq_body_divinet1", 1)
		DL:equipItem("eq_body_divinet1")
		DL:addNode()

	end


	DL:createChoiceNode(17)
	DL:addChoice(-1, "") -- 
	DL:addNode()

end