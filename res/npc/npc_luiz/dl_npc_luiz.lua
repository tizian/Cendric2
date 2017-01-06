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
			DL:addNode()

		end

	end

	if (DL:getGuild() == "necromancer") then 

		DL:createChoiceNode(19)
		if (not DL:isConditionFulfilled("npc_luiz", "first_spell")) then 
			DL:addChoice(21, "DL_Choice_TeachSpells") -- Can you teach me some spells?
		end
		if (DL:isConditionFulfilled("npc_luiz", "first_spell") and not DL:isConditionFulfilled("npc_luiz", "second_spell") and DL:getReputation("necromancer") < 50) then 
			DL:addChoice(22, "DL_Choice_TeachMore") -- Can you teach me more spells?
		end
		if (DL:isConditionFulfilled("npc_luiz", "first_spell") and not DL:isConditionFulfilled("npc_luiz", "second_spell") and DL:getReputation("necromancer") >= 50) then 
			DL:addChoice(23, "DL_Choice_TeachMore") -- 
		end
		DL:addChoice(-1, "") -- 
		DL:addNode()

		if (not DL:isConditionFulfilled("npc_luiz", "first_spell")) then 

			DL:createNPCNode(21, -2, "DL_Luiz_FirstSpell") -- Yes, you may learn how to drain the life energy from your enemies, novice. 
			DL:addConditionProgress("npc_luiz", "first_spell")
			DL:addItem("sp_leech", 1)
			DL:addNode()

		end

		if (DL:isConditionFulfilled("npc_luiz", "first_spell") and not DL:isConditionFulfilled("npc_luiz", "second_spell") and DL:getReputation("necromancer") < 50) then 

			DL:createNPCNode(22, -2, "DL_Luiz_NoSpellReputation") -- You're not ready for that. Help us with our studies, and you will get the experience needed for more powerful spells.
			DL:addNode()

		end

		if (DL:isConditionFulfilled("npc_luiz", "first_spell") and not DL:isConditionFulfilled("npc_luiz", "second_spell") and DL:getReputation("necromancer") >= 50) then 

			DL:createNPCNode(23, -2, "DL_Luiz_SecondSpell") -- Yes, you've earned the experience to learn how to raise the dead...
			DL:addItem("sp_raisethedead", 1)
			DL:addConditionProgress("npc_luiz", "second_spell")
			DL:addNode()

		end

	end


	DL:createChoiceNode(20)
	DL:addChoice(-1, "") -- 
	DL:addNode()

end