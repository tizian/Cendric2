-- Dialogue for NPC "npc_jonathan"
loadDialogue = function(DL) 

	if (not DL:isConditionFulfilled("npc_jonathan", "talked")) then 
		DL:setRoot(1) 
	elseif (not DL:isQuestState("a_new_trace", "completed")) then 
		DL:setRoot(5) 
	elseif (DL:isQuestState("essence_of_fire", "void")) then 
		DL:setRoot(11) 
	elseif (not DL:isConditionFulfilled("npc_jonathan", "decipher_map")) then 
		DL:setRoot(17) 
	elseif (DL:isQuestState("join_a_guild", "void")) then 
		DL:setRoot(27) 
	elseif (DL:getGuild() == "void") then 
		DL:setRoot(36) 
	elseif (not DL:isConditionFulfilled("default", "chapter3")) then 
		DL:setRoot(41) 
	elseif (DL:isQuestState("element_master", "void")) then 
		DL:setRoot(45) 
	else 
		DL:setRoot(57) 
	end 

	if (not DL:isConditionFulfilled("npc_jonathan", "talked")) then 

		DL:createNPCNode(1, 2, "DL_Jonathan_WalkIntoHouse") -- Hey, what are you doing here? You can't just walk into other people's houses.
		DL:addConditionProgress("npc_jonathan", "talked")
		DL:addNode()


		DL:createChoiceNode(2)
		DL:addChoice(3, "DL_Choice_DoorWasOpen") -- But... the door was open.
		DL:addChoice(4, "DL_Choice_LookingForYou") -- I was looking for you.
		DL:addNode()


		DL:createNPCNode(3, -2, "DL_Jonathan_NoExcuNdse") -- Well, that's not really an excuse. Why are you here?
		DL:addNode()


		DL:createNPCNode(4, -2, "DL_Jonathan_WhatDoYouWant") -- What do you want from me then?
		DL:addNode()

	end

	if (not DL:isQuestState("a_new_trace", "completed")) then 

		DL:createChoiceNode(5)
		if (DL:isQuestDescriptionUnlocked("a_new_trace",1) and DL:hasItem("do_eleletter", 1) and DL:hasItem("do_elemap", 1)) then 
			DL:addChoice(6, "DL_Choice_Letter") -- Someone wants me to meet you. (Show the letter and give the map)
		end
		DL:addChoice(-1, "DL_Choice_Leave") -- I was just about to leave.
		DL:addNode()

		if (DL:isQuestDescriptionUnlocked("a_new_trace",1) and DL:hasItem("do_eleletter", 1) and DL:hasItem("do_elemap", 1)) then 

			DL:createNPCNode(6, 7, "DL_Jonathan_Letter") -- (Reads the letter carefully) Cyrus! So good to hear from him. So, you're from the academy? How are things going there? And what did Cyrus find out?
			DL:removeItem("do_elemap", 1)
			DL:addNode()


			DL:createChoiceNode(7)
			DL:addChoice(8, "DL_Choice_Disappoint") -- I'm sorry to disappoint you...
			DL:addNode()


			DL:createCendricNode(8, 9, "DL_Cendric_TellsTheStory") -- (Cendric tells the story about losing his memory and getting here)
			DL:addNode()


			DL:createNPCNode(9, 10, "DL_Jonathan_Wow") -- Wow... Losing the your whole memory is pretty unusual, even for a mage.
			DL:addNode()


			DL:createNPCNode(10, -2, "DL_Jonathan_Wow2") -- And summoning an iceball is the only spell you remember, right? It's good you came here. We will solve this together.
			DL:changeQuestState("a_new_trace", "completed")
			DL:addNode()

		end

	end

	if (DL:isQuestState("essence_of_fire", "void")) then 

		DL:createChoiceNode(11)
		if (not DL:isConditionFulfilled("npc_jonathan", "academy")) then 
			DL:addChoice(12, "DL_Choice_Academy") -- What academy are you talking about?
		end
		if (DL:isConditionFulfilled("npc_jonathan", "academy") and not DL:isConditionFulfilled("npc_jonathan", "elementalist")) then 
			DL:addChoice(13, "DL_Choice_Elementalists") -- What are elementalists?
		end
		if (DL:isConditionFulfilled("npc_jonathan", "elementalist") and not DL:isConditionFulfilled("npc_jonathan", "affinity")) then 
			DL:addChoice(14, "DL_Choice_Affinity") -- What does "affinity" mean?
		end
		if (DL:isConditionFulfilled("npc_jonathan", "elementalist") and not DL:isConditionFulfilled("npc_jonathan", "prove_yourself")) then 
			DL:addChoice(15, "DL_Choice_WhatShouldWeDo") -- And what should we do now?
		end
		if (DL:isConditionFulfilled("npc_jonathan", "prove_yourself")) then 
			DL:addChoice(18, "DL_Choice_ProveMyself") -- How can I prove myself?
		end
		DL:addChoice(-1, "") -- 
		DL:addNode()

		if (not DL:isConditionFulfilled("npc_jonathan", "academy")) then 

			DL:createNPCNode(12, -2, "DL_Jonathan_Academy") -- The only academy in Admantris where elementalists are educated. It is situated on the other side of the sea. You have obviously studied there.
			DL:addConditionProgress("npc_jonathan", "academy")
			DL:addNode()

		end

		if (DL:isConditionFulfilled("npc_jonathan", "academy") and not DL:isConditionFulfilled("npc_jonathan", "elementalist")) then 

			DL:createNPCNode(13, -2, "DL_Jonathan_Elementalists") -- Mages with an affinity to elemental magic. There are not too much left in Admantris, but I'm one of them... and you are - probably - too.
			DL:addConditionProgress("npc_jonathan", "elementalist")
			DL:addNode()

		end

		if (DL:isConditionFulfilled("npc_jonathan", "elementalist") and not DL:isConditionFulfilled("npc_jonathan", "affinity")) then 

			DL:createNPCNode(14, -2, "DL_Jonathan_Affinity") -- Each mage is born with a certain affinity to a type of magic. This affinity determines which spells he can learn. We elementalists are lucky - we are able to understand the whole spectrum of magic.
			DL:addConditionProgress("npc_jonathan", "affinity")
			DL:addNode()

		end

		if (DL:isConditionFulfilled("npc_jonathan", "elementalist") and not DL:isConditionFulfilled("npc_jonathan", "prove_yourself")) then 

			DL:createNPCNode(15, 16, "DL_Jonathan_NextStep") -- Hm. I knew that Cyrus was going to send me one of his students to support my research here. But to be honest, I hoped for someone who is a bit more...
			DL:addNode()


			DL:createNPCNode(16, -2, "DL_Jonathan_NextStep2") -- ...qualified. Also, someone who lost his memory is suspicious. Before I let you in on my studies, you should first prove that you're a worthy elementalist.
			DL:addConditionProgress("npc_jonathan", "prove_yourself")
			DL:addNode()

		end

		if (DL:isConditionFulfilled("npc_jonathan", "prove_yourself")) then 

			DL:createNPCNode(18, 19, "DL_Jonathan_FireballQuest") -- Every mage is able to learn how to summon an iceball, it's an easy spell. Controlling fire, on the other hand, is considered nearly impossible for mages without an elemental affinity.
			DL:addNode()


			DL:createNPCNode(19, -1, "DL_Jonathan_FireballQuest2") -- Some fire rats from the sewers recently invaded the laboratory in my basement. Find a way to lure them and collect the essence of fire from their lungs. Come back to me when you're done and I'll see if you're able to learn how to summon a fireball.
			DL:addItem("ke_jonathanbasement", 1)
			DL:changeQuestState("essence_of_fire", "started")
			DL:addNode()

		end

	end

	if (not DL:isConditionFulfilled("npc_jonathan", "decipher_map")) then 

		DL:createChoiceNode(17)
		if (DL:isQuestState("essence_of_fire", "started") and DL:isQuestComplete("essence_of_fire")) then 
			DL:addChoice(20, "DL_Choice_GotFireEssence") -- I got the essences of fire for you.
		end
		if (DL:isQuestState("essence_of_fire", "completed") and DL:isSpellLearned(3)) then 
			DL:addChoice(22, "DL_Choice_LearntFireball") -- I've read the scroll.
		end
		DL:addChoice(-1, "DL_Choice_CU") -- See you later.
		DL:addNode()

		if (DL:isQuestState("essence_of_fire", "started") and DL:isQuestComplete("essence_of_fire")) then 

			DL:createNPCNode(20, 21, "DL_Jonathan_EssenceComplete") -- Very good. Maybe you can still become a decent elementalist.
			DL:changeQuestState("essence_of_fire", "completed")
			DL:removeItem("qe_fireessence", 10)
			DL:addNode()


			DL:createNPCNode(21, -1, "DL_Jonathan_EssenceComplete2") -- Here, take this scroll and read it. Talk to me when you've learned the spell.
			DL:addItem("sp_fireball", 1)
			DL:addNode()

		end

		if (DL:isQuestState("essence_of_fire", "completed") and DL:isSpellLearned(3)) then 

			DL:createNPCNode(22, 23, "DL_Jonathan_LearnedFireball") -- And you've already acquired its knowledge. I think we're ready to get started then.
			DL:addNode()


			DL:createNPCNode(23, 24, "DL_Jonathan_LearnedFireball2") -- My research focuses on the old, forgotten knowledge of the first elementalists. I already know that they must have worked on spells that are way more powerful than any of the elemental magic that is known today. 
			DL:addNode()


			DL:createNPCNode(24, 25, "DL_Jonathan_LearnedFireball3") -- I've found evidence that there's an abandoned site of the first Elementalists somewhere near Gandria, that's why I'm working here.
			DL:addNode()


			DL:createNPCNode(25, 26, "DL_Jonathan_LearnedFireball4") -- I'm pretty sure that the map that Cyrus sent me is the key to find that location and solve the mysteries that lay there. But there's only one small problem with that map...
			DL:addNode()


			DL:createNPCNode(26, -2, "DL_Jonathan_LearnedFireball5") -- To decipher it, elemental magic is not sufficient. But using elemental magic combined with the power of another type of magic, we should be able to break the protection spell.
			DL:addConditionProgress("npc_jonathan", "decipher_map")
			DL:addNode()

		end

	end

	if (DL:isQuestState("join_a_guild", "void")) then 

		DL:createChoiceNode(27)
		if (not DL:isConditionFulfilled("npc_jonathan", "another_mage")) then 
			DL:addChoice(28, "DL_Choice_AnotherMage") -- So, should we ask another mage to help us?
		end
		if (not DL:isConditionFulfilled("npc_jonathan", "what_about_you")) then 
			DL:addChoice(29, "DL_Choice_WhatAboutYou") -- What about you? Don't you know some other spells?
		end
		if (not DL:isConditionFulfilled("npc_jonathan", "break_protection")) then 
			DL:addChoice(31, "DL_Choice_BreakingProtection") -- Do you think breaking a protection spell is a good idea?
		end
		if (not DL:isConditionFulfilled("npc_jonathan", "why_risky") and DL:isConditionFulfilled("npc_jonathan", "what_about_you")) then 
			DL:addChoice(32, "DL_Choice_WhyRisky") -- Why is that risky for you?
		end
		if (DL:isConditionFulfilled("npc_jonathan", "what_about_you")) then 
			DL:addChoice(34, "DL_Choice_WhatShouldIDo") -- I see. So, what should I do?
		end
		DL:addNode()

		if (not DL:isConditionFulfilled("npc_jonathan", "another_mage")) then 

			DL:createNPCNode(28, -2, "DL_Jonathan_AnotherMage") -- To be honest, I don't really trust any other mage in the city. The less people know about this map, the better.
			DL:addConditionProgress("npc_jonathan", "another_mage")
			DL:addNode()

		end

		if (not DL:isConditionFulfilled("npc_jonathan", "what_about_you")) then 

			DL:createNPCNode(29, 30, "DL_Jonathan_WhatAboutYou") -- Although we elementalists can learn every type of magic, I've always focused my studies on elemental magic only. 
			DL:addConditionProgress("npc_jonathan", "what_about_you")
			DL:addNode()


			DL:createNPCNode(30, -2, "DL_Jonathan_WhatAboutYou2") -- And now, it's too risky for me to go and learn something new. No, I need someone with a face that is not yet known to the citizens of Gandria. Someone like you.
			DL:addNode()

		end

		if (not DL:isConditionFulfilled("npc_jonathan", "break_protection")) then 

			DL:createNPCNode(31, -2, "DL_Jonathan_BreakingProtection") -- You mean, there's a good reason for this spell? Well, if there is, we'll find out soon enough.
			DL:addConditionProgress("npc_jonathan", "break_protection")
			DL:addNode()

		end

		if (not DL:isConditionFulfilled("npc_jonathan", "why_risky") and DL:isConditionFulfilled("npc_jonathan", "what_about_you")) then 

			DL:createNPCNode(32, 33, "DL_Jonathan_WhyRisky") -- It's just because lately, I get the impression that someone tries to mess with my work. Research notes and people disappear, I don't think you're an exception.
			DL:addConditionProgress("npc_jonathan", "why_risky")
			DL:addNode()


			DL:createNPCNode(33, -2, "DL_Jonathan_WhyRisky2") -- It doesn't seem to be a good idea for me to go outside until this city has gone back to normal.
			DL:addNode()

		end

		if (DL:isConditionFulfilled("npc_jonathan", "what_about_you")) then 

			DL:createNPCNode(34, 35, "DL_Jonathan_WhatShouldIDo") -- You'll have to learn another type of magic. There are - as far as I know - three mage guilds in Gandria, each focusing on another domain of magic.
			DL:addNode()


			DL:createNPCNode(35, -2, "DL_Jonathan_WhatShouldIDo2") -- I guess you already know one of the guilds: The clerics, which currently rule this city. But there are mages who practice necromancy or twilight magic in this city, you just have to find them.
			DL:changeQuestState("join_a_guild", "started")
			DL:addNode()

		end

	end

	if (DL:getGuild() == "void") then 

		DL:createChoiceNode(36)
		if (not DL:isConditionFulfilled("npc_jonathan", "light") and DL:isSpellLearned(17)) then 
			DL:addChoice(38, "DL_Choice_WhyGuild") -- I already know some divine magic. (Summon a light)
		end
		if (not DL:isConditionFulfilled("npc_jonathan", "where_necromancers")) then 
			DL:addChoice(37, "DL_Choice_WhereNecromancers") -- How can I find the necromancers?
		end
		if (not DL:isConditionFulfilled("npc_jonathan", "where_thieves")) then 
			DL:addChoice(39, "DL_Choice_WhereThieves") -- Where can I learn some twilight magic?
		end
		if (not DL:isConditionFulfilled("npc_jonathan", "where_clerics")) then 
			DL:addChoice(40, "DL_Choice_WhereClerics") -- What if I wanted to join the clerics?
		end
		DL:addChoice(-1, "") -- 
		DL:addNode()

		if (not DL:isConditionFulfilled("npc_jonathan", "light") and DL:isSpellLearned(17)) then 

			DL:createNPCNode(38, -2, "DL_Jonathan_Light") -- We can't decipher the map using some simple magic. You'll have to gain the trust of one of the three guilds in order to learn more advanced magic.
			DL:addConditionProgress("npc_jonathan", "light")
			DL:addNode()

		end

		if (not DL:isConditionFulfilled("npc_jonathan", "where_necromancers")) then 

			DL:createNPCNode(37, -2, "DL_Jonathan_WhereNecromancers") -- They need to stay hidden because their magic is not allowed in Gandria. But if I were you, I'd ask the alchemist Syrah, she seems to know some people.
			DL:addConditionProgress("npc_jonathan", "where_necromancers")
			DL:addQuestDescription("join_a_guild", 1)
			DL:addNode()

		end

		if (not DL:isConditionFulfilled("npc_jonathan", "where_thieves")) then 

			DL:createNPCNode(39, -2, "DL_Jonathan_WhereThieves") -- Mages who practice twilight magic are mostly considered as thieves. I'd start searching in the sewers of Gandria.
			DL:addConditionProgress("npc_jonathan", "where_thieves")
			DL:addQuestDescription("join_a_guild", 2)
			DL:addNode()

		end

		if (not DL:isConditionFulfilled("npc_jonathan", "where_clerics")) then 

			DL:createNPCNode(40, -2, "DL_Jonathan_WhereClerics") -- Try to gain the trust of their leader and convince them of your skills.
			DL:addConditionProgress("npc_jonathan", "where_clerics")
			DL:addQuestDescription("join_a_guild", 3)
			DL:addNode()

		end

	end

	if (not DL:isConditionFulfilled("default", "chapter3")) then 

		DL:createChoiceNode(41)
		if (DL:getGuild() == "cleric") then 
			DL:addChoice(42, "DL_Choice_JoinedClerics") -- I'm now a member of the Order of the Eternal Light.
		end
		if (DL:getGuild() == "thief") then 
			DL:addChoice(43, "DL_Choice_JoinedThieves") -- I now belong to the Shadow Stalkers.
		end
		if (DL:getGuild() == "necromancer") then 
			DL:addChoice(44, "DL_Choice_JoinedNecromancers") -- I'm now a member of the necromancers.
		end
		DL:addChoice(-1, "") -- 
		DL:addNode()

		if (DL:getGuild() == "cleric") then 

			DL:createNPCNode(42, 46, "DL_Jonathan_JoinedClerics") -- Maybe the most reasonable choice...
			DL:changeQuestState("join_a_guild", "completed")
			DL:addNode()


			DL:createNPCNode(46, 49, "DL_Jonathan_GoodJobGuild") -- Well, good job on joining a mage guid. I wasn't able to decrypt your map with just elemental magic.
			DL:addNode()


			DL:createNPCNode(49, 52, "DL_Jonathan_CombinedSkillsDivine") -- But combined with your divine magic, it could work.
			DL:addNode()


			DL:createNPCNode(52, -1, "") -- 
			DL:startCutscene("map_decrypt/map_decrypt_divine")
			DL:addConditionProgress("default", "chapter3")
			DL:addNode()

		end

		if (DL:getGuild() == "thief") then 

			DL:createNPCNode(43, 47, "DL_Jonathan_JoinedThieves") -- The underground of Gandria, eh. Just don't try to steal MY things.
			DL:changeQuestState("join_a_guild", "completed")
			DL:addNode()


			DL:createNPCNode(47, 50, "DL_Jonathan_GoodJobGuild") -- 
			DL:addNode()


			DL:createNPCNode(50, 53, "DL_Jonathan_CombinedSkillsTwilight") -- But combined with your twilight magic, it could work.
			DL:addNode()


			DL:createNPCNode(53, -1, "") -- 
			DL:startCutscene("map_decrypt/map_decrypt_twilight")
			DL:addConditionProgress("default", "chapter3")
			DL:addNode()

		end

		if (DL:getGuild() == "necromancer") then 

			DL:createNPCNode(44, 48, "DL_Jonathan_JoinedNecromancers") -- I didn't expect you'd go there. An interesting choice, indeed.
			DL:changeQuestState("join_a_guild", "completed")
			DL:addNode()


			DL:createNPCNode(48, 51, "DL_Jonathan_GoodJobGuild") -- 
			DL:addNode()


			DL:createNPCNode(51, 54, "DL_Jonathan_CombinedSkillsNecro") -- But combined with your necromancy, it could work.
			DL:addNode()


			DL:createNPCNode(54, -1, "") -- 
			DL:addConditionProgress("default", "chapter3")
			DL:startCutscene("map_decrypt/map_decrypt_necro")
			DL:addNode()

		end

	end

	if (DL:isQuestState("element_master", "void")) then 

		DL:createNPCNode(45, 55, "DL_Jonathan_StartElementMaster") -- I knew they would not make it easy for us. Like they really don't want to be found.
		DL:addNode()


		DL:createNPCNode(55, 56, "DL_Jonathan_StartElementMaster2") -- But we need to discover the secret they're hiding. Fire, Water, Air, Earth... You already know about the first two.
		DL:addNode()


		DL:createNPCNode(56, -2, "DL_Jonathan_StartElementMaster3") -- You need to learn how to control air and earth then. Together, we will find them.
		DL:changeQuestState("element_master", "started")
		DL:addNode()

	end


	DL:createChoiceNode(57)
	if (not DL:isConditionFulfilled("npc_jonathan", "where_spells")) then 
		DL:addChoice(58, "DL_Choice_WhereSpells") -- Can you teach me those spells?
	end
	if (not DL:isConditionFulfilled("npc_jonathan", "you_learn")) then 
		DL:addChoice(59, "DL_Choice_YouLearn") -- Why don't you learn those spells yourself?
	end
	DL:addChoice(-1, "") -- 
	DL:addNode()

	if (not DL:isConditionFulfilled("npc_jonathan", "where_spells")) then 

		DL:createNPCNode(58, 62, "DL_Jonathan_WhereSpells") -- No, I don't know much about air or earth spells. You need to find them yourself.
		DL:addNode()


		DL:createNPCNode(62, 63, "DL_Jonathan_WhereSpells2") -- I've heard that the lighthouse keeper Jack has a problem with a wind-monster. That may help you with learning some wind magic.
		DL:addQuestDescription("element_master", 1)
		DL:addConditionProgress("npc_jonathan", "where_spells")
		DL:addNode()


		DL:createNPCNode(63, -2, "DL_Jonathan_WhereSpells3") -- I assume you can find Jack in the brothel...
		DL:addNode()

	end

	if (not DL:isConditionFulfilled("npc_jonathan", "you_learn")) then 

		DL:createNPCNode(59, 60, "DL_Jonathan_YouLearn") -- My focus are only fire spells. Also, I don't feel safe going out of my house anymore.
		DL:addNode()


		DL:createNPCNode(60, 61, "DL_Jonathan_YouLearn2") -- I'm pretty sure I'm being watched - Someone followed me lately on my way home. 
		DL:addNode()


		DL:createNPCNode(61, -2, "DL_Jonathan_YouLearn3") -- And with all the people disappearing... No, no, I leave this to you.
		DL:addConditionProgress("npc_jonathan", "you_learn")
		DL:addNode()

	end

end