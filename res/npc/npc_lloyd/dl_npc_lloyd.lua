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
	elseif (DL:getGuild() == "cleric") then 
		DL:setRoot(17) 
	else 
		DL:setRoot(11) 
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
			DL:removeItem("qe_recommendationletter",1)
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
		DL:addQuestDescription("join_a_guild", 4)
		DL:addNode()

	end

	if (DL:getGuild() == "cleric") then 

		DL:createChoiceNode(17)
		if (DL:isQuestState("cathedral_cleric", "void")) then 
			DL:addChoice(20, "DL_Choice_HowCanIHelp") -- What needs to be done?
		end
		if (DL:isQuestState("cathedral_cleric", "started") and DL:isQuestComplete("cathedral_cleric")) then 
			DL:addChoice(23, "DL_Choice_CathedralFinished") -- I've purified the necrotic grimoire.
		end
		if (DL:isQuestState("cathedral_cleric", "completed") and DL:isQuestState("yasha_cleric", "void")) then 
			DL:addChoice(25, "DL_Choice_YashaStart") -- Is there anything else I can do for you?
		end
		if (DL:isQuestState("yasha_cleric", "started") and not DL:isConditionFulfilled("npc_lloyd", "yasha_doubt")) then 
			DL:addChoice(32, "DL_Choice_YashaDoubt") -- What if I don't survive the leap?
		end
		if (DL:isQuestState("yasha_cleric", "started") and DL:isQuestComplete("yasha_cleric")) then 
			DL:addChoice(31, "DL_Choice_YashaComplete") -- I survived the leap of faith.
		end
		if (DL:isQuestState("find_velius", "started") and DL:isQuestDescriptionUnlocked("find_velius",3) and not DL:isConditionFulfilled("npc_lloyd", "velius_found")) then 
			DL:addChoice(39, "DL_Choice_VeliusFound") -- 
		end
		DL:addChoice(-1, "") -- 
		DL:addNode()

		if (DL:isQuestState("cathedral_cleric", "void")) then 

			DL:createNPCNode(20, 21, "DL_Lloyd_CathedralCleric") -- We have found a dangerous grimoire about necromancy and brought it into our cathedral.
			DL:addNode()


			DL:createNPCNode(21, 22, "DL_Lloyd_CathedralCleric2") -- But the book seems to have its own will - we couldn't destroy it yet.
			DL:addNode()


			DL:createNPCNode(22, -2, "DL_Lloyd_CathedralCleric3") -- I want you to go to our cathedral. Kindle the divine candles with your light magic and purify that deviant book. 
			DL:changeQuestState("cathedral_cleric", "started")
			DL:addNode()

		end

		if (DL:isQuestState("cathedral_cleric", "started") and DL:isQuestComplete("cathedral_cleric")) then 

			DL:createNPCNode(23, 24, "DL_Lloyd_CathedralFinished") -- Well done. It's twisted magic is no longer besmirching our cathedral.
			DL:addNode()


			DL:createNPCNode(24, -2, "DL_Lloyd_CathedralFinished2") -- I think you've gained enough experience to learn how to shield yourself against evil magic. Take this scroll as a reward for your help.
			DL:changeQuestState("cathedral_cleric", "completed")
			DL:addItem("sp_divineshield", 1)
			DL:addReputationProgress("cleric", 10)
			DL:addNode()

		end

		if (DL:isQuestState("cathedral_cleric", "completed") and DL:isQuestState("yasha_cleric", "void")) then 

			DL:createNPCNode(25, 26, "DL_Lloyd_YashaStart") -- You've already done a lot for us and showed your skill with staff and magic.
			DL:addNode()


			DL:createNPCNode(26, 27, "DL_Lloyd_YashaStart2") -- The time has come to prove your faith in the Eternal Light.
			DL:addNode()


			DL:createCendricNode(27, 28, "DL_Cendric_YashaStart3") -- How should I prove my faith?
			DL:addNode()


			DL:createNPCNode(28, 29, "DL_Lloyd_YashaStart4") -- There's an abadoned temple, guarded by a demon in the Highlands of Admantris. Find this temple and face its tests.
			DL:addNode()


			DL:createNPCNode(29, 30, "DL_Lloyd_YashaStart5") -- If you find a way through the temple, you'll discover a dark abyss. With the power of the Eternal Light, summon a holy shield and jump.
			DL:addNode()


			DL:createNPCNode(30, -2, "DL_Lloyd_YashaStart6") -- If your faith is strong, you won't only survive it, but also gain the power to never ever get stuck in a dark abyss again. 
			DL:changeQuestState("yasha_cleric", "started")
			DL:addNode()

		end

		if (DL:isQuestState("yasha_cleric", "started") and not DL:isConditionFulfilled("npc_lloyd", "yasha_doubt")) then 

			DL:createNPCNode(32, -2, "DL_Lloyd_YashaDoubt") -- That would mean you're unworthy. Nobody would miss you.
			DL:addConditionProgress("npc_lloyd", "yasha_doubt")
			DL:addNode()

		end

		if (DL:isQuestState("yasha_cleric", "started") and DL:isQuestComplete("yasha_cleric")) then 

			DL:createNPCNode(31, -2, "DL_Lloyd_YashaComplete") -- Yes, I can feel the light flow through you. May it always guide you.
			DL:changeQuestState("yasha_cleric", "completed")
			DL:addReputationProgress("cleric", 10)
			DL:addNode()

		end

		if (DL:isQuestState("find_velius", "started") and DL:isQuestDescriptionUnlocked("find_velius",3) and not DL:isConditionFulfilled("npc_lloyd", "velius_found")) then 

			DL:createCendricNode(39, 40, "DL_Cendric_VeliusFound") -- 
			DL:addNode()


			DL:createNPCNode(40, 41, "DL_Lloyd_VeliusFound") -- 
			DL:addConditionProgress("npc_lloyd", "velius_found")
			DL:addNode()


			DL:createNPCNode(41, 42, "DL_Lloyd_VeliusFound2") -- 
			DL:addQuestDescription("find_velius", 4)
			DL:addNode()


			DL:createNPCNode(42, 43, "DL_Lloyd_VeliusFound3") -- 
			DL:addNode()


			DL:createNPCNode(43, 44, "DL_Lloyd_VeliusFound4") -- 
			DL:addNode()


			DL:createNPCNode(44, -2, "DL_Lloyd_VeliusFound5") -- 
			DL:addNode()

		end

	end


	DL:createChoiceNode(11)
	if (DL:isQuestState("find_velius", "started") and DL:isQuestDescriptionUnlocked("find_velius",3) and not DL:isConditionFulfilled("npc_lloyd", "velius_found")) then 
		DL:addChoice(33, "DL_Choice_VeliusFound") -- I found a letter from Inina... (Show letter)
	end
	DL:addChoice(-1, "") -- 
	DL:addNode()

	if (DL:isQuestState("find_velius", "started") and DL:isQuestDescriptionUnlocked("find_velius",3) and not DL:isConditionFulfilled("npc_lloyd", "velius_found")) then 

		DL:createCendricNode(33, 34, "DL_Cendric_VeliusFound") -- That "V." is called Velius. He abducted some mages including Inina and I should go into the crypt of Gandria.
		DL:addNode()


		DL:createNPCNode(34, 35, "DL_Lloyd_VeliusFound") -- Velius is the one who abducted Inina? Interesting.
		DL:addConditionProgress("npc_lloyd", "velius_found")
		DL:addNode()


		DL:createNPCNode(35, 36, "DL_Lloyd_VeliusFound2") -- The only Velius I know is one of the royal mages. I would never dare to accuse him of something.
		DL:addQuestDescription("find_velius", 4)
		DL:addNode()


		DL:createNPCNode(36, 37, "DL_Lloyd_VeliusFound3") -- I'm a subordinate of the king and his mages. I'm not allowed to help you.
		DL:addNode()


		DL:createNPCNode(37, 38, "DL_Lloyd_VeliusFound4") -- This letter might be a trap as well...
		DL:addNode()


		DL:createNPCNode(38, -2, "DL_Lloyd_VeliusFound5") -- However, I won't forbid you to go into the crypt and look for yourself.
		DL:addNode()

	end

end