-- Dialogue for NPC "npc_luiz"
loadDialogue = function(DL) 

	if (not DL:isConditionFulfilled("npc_luiz", "talked")) then 
		DL:setRoot(1) 
	elseif (DL:isConditionFulfilled("npc_luiz", "learn") and DL:getGuild() ~= "void" and not DL:isConditionFulfilled("npc_luiz", "guild")) then 
		DL:setRoot(15) 
	elseif (not DL:isConditionFulfilled("npc_luiz", "guild")) then 
		DL:setRoot(8) 
	elseif (DL:getGuild() == "necromancer") then 
		DL:setRoot(19) 
	else 
		DL:setRoot(20) 
	end 

	if (not DL:isConditionFulfilled("npc_luiz", "talked")) then 

		DL:createNPCNode(1, 2, "DL_Luiz_FirstTalk") -- Syrah was right about sending you here. 
		DL:addConditionProgress("npc_luiz", "talked")
		DL:addNode()


		DL:createNPCNode(2, 3, "DL_Luiz_FirstTalk2") -- Nothing kept you from following your way - not even using a forbidden spell.
		DL:addNode()


		DL:createNPCNode(3, 4, "DL_Luiz_FirstTalk3") -- So, if you're eager to learn more, you're welcome in these halls. 
		DL:addNode()


		DL:createChoiceNode(4)
		DL:addChoice(6, "DL_Choice_Forbidden") -- I didn't know this was forbidden.
		DL:addChoice(7, "DL_Choice_Test") -- Was this some kind of test?
		DL:addNode()


		DL:createNPCNode(6, -2, "DL_Luiz_Forbidden") -- Well, it shouldn't be. Still, mages hang when they are caught using necromancy.
		DL:addNode()


		DL:createNPCNode(7, -2, "DL_Luiz_Test") -- Yes - and you passed. Necromancy is not trivial, I'm impressed.
		DL:addNode()

	end

	if (DL:isConditionFulfilled("npc_luiz", "learn") and DL:getGuild() ~= "void" and not DL:isConditionFulfilled("npc_luiz", "guild")) then 

		DL:createNPCNode(15, -2, "DL_Luiz_YouJoinedOtherGuild") -- I see, you've already made your choice. Suit yourself.
		DL:addConditionProgress("npc_luiz", "guild")
		DL:addNode()

	end

	if (not DL:isConditionFulfilled("npc_luiz", "guild")) then 

		DL:createChoiceNode(8)
		if (not DL:isConditionFulfilled("npc_luiz", "forbidden")) then 
			DL:addChoice(9, "DL_Choice_ForbiddenMagic") -- Why is it forbidden to use necromancy?
		end
		if (not DL:isConditionFulfilled("npc_luiz", "who_are_you")) then 
			DL:addChoice(5, "DL_Choice_WhoAreYou") -- Are you the leader here?
		end
		if (not DL:isConditionFulfilled("npc_luiz", "learn")) then 
			DL:addChoice(11, "DL_Choice_Learn") -- Where can I learn more?
		end
		if (DL:isConditionFulfilled("npc_luiz", "learn") and not DL:isConditionFulfilled("npc_luiz", "what_if_join")) then 
			DL:addChoice(13, "DL_Choice_WhatIfJoin") -- Why should I join you?
		end
		if (DL:isConditionFulfilled("npc_luiz", "learn") and not DL:isConditionFulfilled("npc_luiz", "other_guild")) then 
			DL:addChoice(14, "DL_Choice_OtherGuild") -- What if I want to join another guild?
		end
		if (DL:isConditionFulfilled("npc_luiz", "learn")) then 
			DL:addChoice(16, "DL_Choice_WantToJoin") -- I want to join you.
		end
		DL:addChoice(-1, "") -- 
		DL:addNode()

		if (not DL:isConditionFulfilled("npc_luiz", "forbidden")) then 

			DL:createNPCNode(9, 10, "DL_Luiz_ForbiddenMagic") -- People are afraid of things they don't know - it's just like being afraid of the dark.
			DL:addNode()


			DL:createNPCNode(10, -2, "DL_Luiz_ForbiddenMagic2") -- But where there is light, there is also shadow. One cannot just ignore them. But we can explore and study them until they are no longer alien.
			DL:addConditionProgress("npc_luiz", "forbidden")
			DL:addNode()

		end

		if (not DL:isConditionFulfilled("npc_luiz", "who_are_you")) then 

			DL:createNPCNode(5, -2, "DL_Luiz_WhoAreYou") -- Yes. I'm Luiz, the leader of the circle. We make sure that this banned magic won't be forgotten.
			DL:addConditionProgress("npc_luiz", "who_are_you")
			DL:addNode()

		end

		if (not DL:isConditionFulfilled("npc_luiz", "learn")) then 

			DL:createNPCNode(11, 12, "DL_Luiz_Learn") -- I'm pleased to hear that you want to learn more.
			DL:addNode()


			DL:createNPCNode(12, -2, "DL_Luiz_Learn2") -- Everyone who makes it to these halls should be able to learn. You may use the library and if you want to learn more spells, you should consider joining us.
			DL:addConditionProgress("npc_luiz", "learn")
			DL:addNode()

		end

		if (DL:isConditionFulfilled("npc_luiz", "learn") and not DL:isConditionFulfilled("npc_luiz", "what_if_join")) then 

			DL:createNPCNode(13, -2, "DL_Luiz_WhatIfJoin") -- You will learn what necromancy has to offer. We can teach you powerful spells if you help us with our research.
			DL:addConditionProgress("npc_luiz", "what_if_join")
			DL:addNode()

		end

		if (DL:isConditionFulfilled("npc_luiz", "learn") and not DL:isConditionFulfilled("npc_luiz", "other_guild")) then 

			DL:createNPCNode(14, -2, "DL_Luiz_OtherGuild") -- I won't object. Everybody should be able to learn whatever they want to learn.
			DL:addConditionProgress("npc_luiz", "other_guild")
			DL:addNode()

		end

		if (DL:isConditionFulfilled("npc_luiz", "learn")) then 

			DL:createNPCNode(16, 17, "DL_Luiz_WantToJoin") -- You want to learn the forbidden magic and devote yourself to the mysteries of necromancy?
			DL:addNode()


			DL:createChoiceNode(17)
			DL:addChoice(18, "DL_Choice_JoinNecromancers") -- Yes. [JOIN THE NECROMANCERS]
			DL:addChoice(-2, "DL_Choice_INeedToThink") -- I still need time to think about this.
			DL:addNode()


			DL:createNPCNode(18, -2, "DL_Luiz_JoinNecromancers") -- Very well. Welcome to the circle, novice. May your knowledge always grow.
			DL:addConditionProgress("npc_luiz", "guild")
			DL:addConditionProgress("default", "necromancer")
			DL:addReputationProgress("necromancer", 10)
			DL:setGuild("necromancer")
			DL:addItem("eq_body_necrot1", 1)
			DL:equipItem("eq_body_necrot1")
			DL:addQuestDescription("join_a_guild", 4)
			DL:addNode()

		end

	end

	if (DL:getGuild() == "necromancer") then 

		DL:createChoiceNode(19)
		if (not DL:isConditionFulfilled("npc_luiz", "first_spell")) then 
			DL:addChoice(21, "DL_Choice_TeachSpells") -- Can you teach me some spells?
		end
		if (DL:isConditionFulfilled("npc_luiz", "first_spell") and not DL:isSpellLearned("15")) then 
			DL:addChoice(22, "DL_Choice_TeachMore") -- Can you teach me more spells?
		end
		if (DL:isQuestState("lloyds_plan", "void")) then 
			DL:addChoice(24, "DL_Choice_HowCanIHelp") -- How can I help you?
		end
		if (not DL:isQuestState("lloyds_plan", "void") and DL:isQuestState("cathedral_necro", "void")) then 
			DL:addChoice(26, "DL_Choice_WhatElseHelp") -- What else can I do for you?
		end
		if (DL:isQuestState("cathedral_necro", "started") and DL:isQuestComplete("cathedral_necro")) then 
			DL:addChoice(30, "DL_Choice_FinishedCathedral") -- I got the necrotic grimoire.
		end
		if (DL:isQuestState("lloyds_plan", "started") and DL:isQuestComplete("lloyds_plan")) then 
			DL:addChoice(23, "DL_Choice_CompleteLloydsPlan") -- I've talked to Lloyd...
		end
		if (DL:isSpellLearned("15") and not DL:isConditionFulfilled("npc_luiz", "no_more_spells")) then 
			DL:addChoice(35, "DL_Choice_TeachMore") -- 
		end
		if (DL:isQuestState("cathedral_necro", "completed") and DL:isQuestState("yasha_necro", "void")) then 
			DL:addChoice(37, "DL_Choice_WhatElseHelp") -- 
		end
		if (not DL:isConditionFulfilled("npc_luiz", "kill_yasha") and DL:isQuestState("yasha_necro", "started")) then 
			DL:addChoice(40, "DL_Choice_KillYasha") -- Do I have to kill the demon to get the mask?
		end
		if (not DL:isConditionFulfilled("npc_luiz", "where_yasha") and DL:isQuestState("yasha_necro", "started")) then 
			DL:addChoice(41, "DL_Choice_WhereYasha") -- How can I get to the Highland of Admantris?
		end
		if (DL:isQuestState("yasha_necro", "started") and DL:isQuestComplete("yasha_necro")) then 
			DL:addChoice(42, "DL_Choice_YashaComplete") -- I found Yaslaw's mask.
		end
		if (DL:isQuestState("missing_bob", "started") and DL:isQuestDescriptionUnlocked("find_velius",3) and DL:isQuestDescriptionUnlocked("missing_bob",1) and not DL:isConditionFulfilled("npc_luiz", "velius_found")) then 
			DL:addChoice(46, "DL_Choice_FoundVelius") -- I found this letter from Bob... (show letter)
		end
		DL:addChoice(-1, "") -- 
		DL:addNode()

		if (not DL:isConditionFulfilled("npc_luiz", "first_spell")) then 

			DL:createNPCNode(21, -2, "DL_Luiz_FirstSpell") -- Yes, you may learn how to drain the life energy from your enemies, novice. 
			DL:addConditionProgress("npc_luiz", "first_spell")
			DL:addItem("sp_leech", 1)
			DL:addNode()

		end

		if (DL:isConditionFulfilled("npc_luiz", "first_spell") and not DL:isSpellLearned("15")) then 

			DL:createNPCNode(22, -2, "DL_Luiz_NoSpellReputation") -- You're not ready for that. Help us with our studies, and you will get the experience needed for more powerful spells.
			DL:addNode()

		end

		if (DL:isQuestState("lloyds_plan", "void")) then 

			DL:createNPCNode(24, 25, "DL_Luiz_LloydsPlan") -- You know the leader of the clerics, Lloyd, don't you? He was acting strangely lately, it seems like he's planning something.
			DL:addNode()


			DL:createNPCNode(25, -2, "DL_Luiz_LloydsPlan2") -- We need someone who can talk to him and maybe even work for him. If you find out what he's up to, come back and report.
			DL:changeQuestState("lloyds_plan", "started")
			DL:addNode()

		end

		if (not DL:isQuestState("lloyds_plan", "void") and DL:isQuestState("cathedral_necro", "void")) then 

			DL:createNPCNode(26, 27, "DL_Luiz_CathedralNecro") -- Although the clerics do not know our hideout, they managed to get their hands on one of our grimoires about necromancy.
			DL:addNode()


			DL:createNPCNode(27, 28, "DL_Luiz_CathedralNecro2") -- The problem is, it was a particulary dangerous and headstrong book about summoning creatures from the underworld. 
			DL:addNode()


			DL:createNPCNode(28, 29, "DL_Luiz_CathedralNecro3") -- They took it into their cathedral to purify it, but this would do even more harm.
			DL:addNode()


			DL:createNPCNode(29, -2, "DL_Luiz_CathedralNecro4") -- Go and bring that book back to our library. I just hope it's not already too late.
			DL:changeQuestState("cathedral_necro", "started")
			DL:addNode()

		end

		if (DL:isQuestState("cathedral_necro", "started") and DL:isQuestComplete("cathedral_necro")) then 

			DL:createNPCNode(30, 31, "DL_Luiz_FinishedCathedral") -- Good job. I'll put it back into our library where it belongs.
			DL:removeItem("qe_necrobook", 1)
			DL:addNode()


			DL:createNPCNode(31, -2, "DL_Luiz_FinishedCathedral2") -- I'm slowly starting to trust you. I think you've gained the experience to learn how to raise the dead...
			DL:addItem("sp_raisethedead", 1)
			DL:addReputationProgress("necromancer", 10)
			DL:changeQuestState("cathedral_necro", "completed")
			DL:addNode()

		end

		if (DL:isQuestState("lloyds_plan", "started") and DL:isQuestComplete("lloyds_plan")) then 

			DL:createCendricNode(23, 32, "DL_Cendric_CompleteLloydsPlan") -- ... Some "V." is abducting mages. Lloyd is trying to find out who that is.
			DL:addNode()


			DL:createNPCNode(32, 33, "DL_Luiz_CompleteLloydsPlan") -- That's interesting, thank you for the report.
			DL:changeQuestState("lloyds_plan", "completed")
			DL:addReputationProgress("necromancer", 5)
			DL:addNode()


			DL:createNPCNode(33, 34, "DL_Luiz_CompleteLloydsPlan2") -- But now I'm a bit worried about our Robert. He didn't return from his last trip to the marshlands.
			DL:addNode()


			DL:createNPCNode(34, -2, "DL_Luiz_CompleteLloydsPlan3") -- Keep an eye open and tell me if you see him.
			DL:changeQuestState("missing_bob", "started")
			DL:addNode()

		end

		if (DL:isSpellLearned("15") and not DL:isConditionFulfilled("npc_luiz", "no_more_spells")) then 

			DL:createNPCNode(35, 36, "DL_Luiz_NoMoreSpells") -- No, I'm sorry, I taught you everything I know.
			DL:addConditionProgress("npc_luiz", "no_more_spells")
			DL:addNode()


			DL:createNPCNode(36, -2, "DL_Luiz_NoMoreSpells2") -- But a good necromancer is always searching for new spells. There is more out there than I know.
			DL:addNode()

		end

		if (DL:isQuestState("cathedral_necro", "completed") and DL:isQuestState("yasha_necro", "void")) then 

			DL:createNPCNode(37, 38, "DL_Luiz_YashaStart") -- Hm. Indeed, I do have something for you.
			DL:addNode()


			DL:createNPCNode(38, 39, "DL_Luiz_YashaStart2") -- There's a cursed temple in the Highlands of Admantris. A necromancer called Yaslaw once summoned a powerful demon in there.
			DL:addNode()


			DL:createNPCNode(39, 43, "DL_Luiz_YashaStart3") -- Unfortunately, he got captured by his own creature and never returned. 
			DL:addNode()


			DL:createNPCNode(43, 44, "DL_Luiz_YashaStart4") -- But as far as we know from the old writings, he carried a magical mask with him that granted him great powers.
			DL:addNode()


			DL:createNPCNode(44, -2, "DL_Luiz_YashaStart5") -- He has been dead long enough. Find his remains in the temple, let him lead you to his mask and bring it to us.
			DL:changeQuestState("yasha_necro", "started")
			DL:addNode()

		end

		if (not DL:isConditionFulfilled("npc_luiz", "kill_yasha") and DL:isQuestState("yasha_necro", "started")) then 

			DL:createNPCNode(40, -2, "DL_Luiz_KillYasha") -- You'll have to find out for yourself.
			DL:addConditionProgress("npc_luiz", "kill_yasha")
			DL:addNode()

		end

		if (not DL:isConditionFulfilled("npc_luiz", "where_yasha") and DL:isQuestState("yasha_necro", "started")) then 

			DL:createNPCNode(41, -2, "DL_Luiz_WhereYasha") -- As far as I know, there's a way through a volcano in the Marshlands, to the South of Gandria.
			DL:addConditionProgress("npc_luiz", "where_yasha")
			DL:addNode()

		end

		if (DL:isQuestState("yasha_necro", "started") and DL:isQuestComplete("yasha_necro")) then 

			DL:createNPCNode(42, 45, "DL_Luiz_YashaComplete") -- And you got his powers too as I can see. I'm really impressed, well done.
			DL:removeItem("qe_yashamask", 1)
			DL:changeQuestState("yasha_necro", "completed")
			DL:addReputationProgress("necromancer", 10)
			DL:addNode()


			DL:createNPCNode(45, -2, "DL_Luiz_YashaComplete2") -- Use its powers wisely, necromancer.
			DL:addNode()

		end

		if (DL:isQuestState("missing_bob", "started") and DL:isQuestDescriptionUnlocked("find_velius",3) and DL:isQuestDescriptionUnlocked("missing_bob",1) and not DL:isConditionFulfilled("npc_luiz", "velius_found")) then 

			DL:createCendricNode(46, 47, "DL_Cendric_FoundVelius") -- A mage named Velius abducted him along with other mages. I need to go to the crypt of Gandria and find out more.
			DL:addNode()


			DL:createNPCNode(47, 48, "DL_Luiz_FoundVelius") -- Hm. Very interesting. I know a mage named Velius, he studied with me a long time ago.
			DL:addConditionProgress("npc_luiz", "velius_found")
			DL:addNode()


			DL:createNPCNode(48, -2, "DL_Luiz_FoundVelius2") -- He, abducting mages... I don't know. That doesn't fit him. But you should go into the crypt and find out more.
			DL:addNode()

		end

	end


	DL:createChoiceNode(20)
	DL:addChoice(-1, "") -- 
	DL:addNode()

end