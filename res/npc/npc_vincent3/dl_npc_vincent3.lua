-- Dialogue for NPC "npc_vincent3"
loadDialogue = function(DL) 

	if (not DL:isConditionFulfilled("npc_vincent3", "talked")) then 
		DL:setRoot(1) 
	elseif (DL:isConditionFulfilled("npc_vincent3", "shadowstalkers") and DL:getGuild() ~= "void" and not DL:isConditionFulfilled("npc_vincent3", "guild")) then 
		DL:setRoot(21) 
	elseif (not DL:isConditionFulfilled("npc_vincent3", "guild")) then 
		DL:setRoot(2) 
	elseif (DL:getGuild() == "thief") then 
		DL:setRoot(16) 
	else 
		DL:setRoot(17) 
	end 

	if (not DL:isConditionFulfilled("npc_vincent3", "talked")) then 

		DL:createNPCNode(1, -2, "DL_Vincent_Welcome") -- So you've finally found us. Welcome to the the hideout of the Shadow Stalkers.
		DL:addConditionProgress("npc_vincent3", "talked")
		DL:addNode()

	end

	if (DL:isConditionFulfilled("npc_vincent3", "shadowstalkers") and DL:getGuild() ~= "void" and not DL:isConditionFulfilled("npc_vincent3", "guild")) then 

		DL:createNPCNode(21, -2, "DL_Vincent_CantJoin") -- You've already made your decision. What a waste...
		DL:addConditionProgress("npc_vincent3", "guild")
		DL:addNode()

	end

	if (not DL:isConditionFulfilled("npc_vincent3", "guild")) then 

		DL:createChoiceNode(2)
		if (not DL:isConditionFulfilled("npc_vincent3", "shadowstalkers")) then 
			DL:addChoice(3, "DL_Choice_WhoAreYou") -- Who are the "Shadow Stalkers"?
		end
		if (not DL:isConditionFulfilled("npc_vincent3", "why_hiding")) then 
			DL:addChoice(4, "DL_Choice_WhyHiding") -- Why are you hiding?
		end
		if (DL:isConditionFulfilled("npc_vincent3", "why_hiding") and not DL:isConditionFulfilled("npc_vincent3", "inform_clerics")) then 
			DL:addChoice(8, "DL_Choice_InformClerics") -- What if I just inform the clerics about your little hideout?
		end
		if (DL:isConditionFulfilled("npc_vincent3", "shadowstalkers") and not DL:isConditionFulfilled("npc_vincent3", "who_guild")) then 
			DL:addChoice(9, "DL_Choice_WhoInGuild") -- Who belongs to this guild?
		end
		if (DL:isConditionFulfilled("npc_vincent3", "shadowstalkers") and not DL:isConditionFulfilled("npc_vincent3", "one_guild")) then 
			DL:addChoice(10, "DL_Choice_OneGuild") -- What if I want to join another guild?
		end
		if (DL:isConditionFulfilled("npc_vincent3", "shadowstalkers") and not DL:isConditionFulfilled("npc_vincent3", "what_if_join")) then 
			DL:addChoice(11, "DL_Choice_WhatIfJoin") -- Why should I join you?
		end
		if (DL:isConditionFulfilled("npc_vincent3", "shadowstalkers")) then 
			DL:addChoice(13, "DL_Choice_IWantJoin") -- I want to join you.
		end
		DL:addChoice(-1, "") -- 
		DL:addNode()

		if (not DL:isConditionFulfilled("npc_vincent3", "shadowstalkers")) then 

			DL:createNPCNode(3, 5, "DL_Vincent_WhoAreYou") -- We are a guild that focuses on twilight magic and the benefits it brings us.
			DL:addNode()


			DL:createNPCNode(5, 6, "DL_Vincent_WhoAreYou2") -- You seem to have an affinity to this type of magic, as you've mastered the art of summoning a picklock incredibly fast.
			DL:addNode()


			DL:createNPCNode(6, -2, "DL_Vincent_WhoAreYou3") -- Joining us could uncover your hidden talents...
			DL:addConditionProgress("npc_vincent3", "shadowstalkers")
			DL:addNode()

		end

		if (not DL:isConditionFulfilled("npc_vincent3", "why_hiding")) then 

			DL:createNPCNode(4, 7, "DL_Vincent_WhyHiding") -- You see what's going on in this city, right? Mages who don't study "divine" magic are frowned upon.
			DL:addNode()


			DL:createNPCNode(7, -2, "DL_Vincent_WhyHiding2") -- Twilight and elemental magic are tolerated though in contrast to necromancy - but still, the paladins don't like what we're doing with our magic.
			DL:addConditionProgress("npc_vincent3", "why_hiding")
			DL:addNode()

		end

		if (DL:isConditionFulfilled("npc_vincent3", "why_hiding") and not DL:isConditionFulfilled("npc_vincent3", "inform_clerics")) then 

			DL:createNPCNode(8, -2, "DL_Vincent_InformClerics") -- By the time the clerics would arrive here, we'd be gone and you - well, your body - would be floating around in the sewers.
			DL:addConditionProgress("npc_vincent3", "inform_clerics")
			DL:addNode()

		end

		if (DL:isConditionFulfilled("npc_vincent3", "shadowstalkers") and not DL:isConditionFulfilled("npc_vincent3", "who_guild")) then 

			DL:createNPCNode(9, -2, "DL_Vincent_WhoInGuild") -- Do I look like I'm going to tell you that? Maybe if you'd join us.
			DL:addConditionProgress("npc_vincent3", "who_guild")
			DL:addNode()

		end

		if (DL:isConditionFulfilled("npc_vincent3", "shadowstalkers") and not DL:isConditionFulfilled("npc_vincent3", "one_guild")) then 

			DL:createNPCNode(10, -2, "DL_Vincent_OneGuild") -- You'd be missing a great opportunity. But that's your choice.
			DL:addConditionProgress("npc_vincent3", "one_guild")
			DL:addNode()

		end

		if (DL:isConditionFulfilled("npc_vincent3", "shadowstalkers") and not DL:isConditionFulfilled("npc_vincent3", "what_if_join")) then 

			DL:createNPCNode(11, -2, "DL_Vincent_WhatIfJoin") -- You'll get to learn twilight magic and besides that, you're going to get rich.
			DL:addConditionProgress("npc_vincent3", "what_if_join")
			DL:addNode()

		end

		if (DL:isConditionFulfilled("npc_vincent3", "shadowstalkers")) then 

			DL:createNPCNode(13, 14, "DL_Vincent_Join") -- Good. You want to become a brother of the Shadow Stalkers, pledge your loyality to the guild and learn the ways of the shadows?
			DL:addNode()


			DL:createChoiceNode(14)
			DL:addChoice(15, "DL_Choice_IJoin") -- Yes. [JOIN THE SHADOW STALKERS]
			DL:addChoice(-2, "DL_Choice_Rethink") -- I need more time to think about this.
			DL:addNode()


			DL:createNPCNode(15, -2, "DL_Vincent_DeclareThief") -- You won't regret this. Welcome in our guild, shadow stalker.
			DL:addConditionProgress("npc_vincent3", "guild")
			DL:addConditionProgress("default", "thief")
			DL:addReputationProgress("thief", 10)
			DL:setGuild("thief")
			DL:addItem("eq_body_twilightt1", 1)
			DL:equipItem("eq_body_twilightt1")
			DL:addQuestDescription("join_a_guild", 4)
			DL:addNode()

		end

	end

	if (DL:getGuild() == "thief") then 

		DL:createChoiceNode(16)
		if (not DL:isConditionFulfilled("npc_vincent3", "first_spell")) then 
			DL:addChoice(18, "DL_Choice_TeachSpells") -- Can you teach me some spells?
		end
		if (DL:isConditionFulfilled("npc_vincent3", "first_spell") and DL:getReputation("thief") < 100) then 
			DL:addChoice(19, "DL_Choice_TeachMore") -- Can you teach me more spells?
		end
		if (DL:isQuestState("lloyds_plan", "void")) then 
			DL:addChoice(12, "DL_Choice_HowCanIHelp") -- How can I support you?
		end
		if (not DL:isQuestState("lloyds_plan", "void") and DL:isQuestState("cathedral_thief", "void")) then 
			DL:addChoice(22, "DL_Choice_HowElseHelp") -- What else can I do to demonstrate my skills?
		end
		if (DL:isQuestState("cathedral_thief", "started") and DL:isQuestComplete("cathedral_thief")) then 
			DL:addChoice(27, "DL_Choice_CathedralFinished") -- I got the candleholders.
		end
		if (DL:isQuestState("lloyds_plan", "started") and DL:isQuestComplete("lloyds_plan")) then 
			DL:addChoice(20, "DL_Choice_CompleteLloydsPlan") -- I've talked to Lloyd...
		end
		DL:addChoice(-1, "") -- 
		DL:addNode()

		if (not DL:isConditionFulfilled("npc_vincent3", "first_spell")) then 

			DL:createNPCNode(18, -2, "DL_Vincent_FirstSpell") -- Yes, I will teach you. Take this scroll and learn how to summon a deadly trap.
			DL:addConditionProgress("npc_vincent3", "first_spell")
			DL:addItem("sp_shadowtrap", 1)
			DL:addNode()

		end

		if (DL:isConditionFulfilled("npc_vincent3", "first_spell") and DL:getReputation("thief") < 100) then 

			DL:createNPCNode(19, -2, "DL_Vincent_NoSpellReputation") -- Maybe later. First, you need to show us that you can handle this kind of magic.
			DL:addNode()

		end

		if (DL:isQuestState("lloyds_plan", "void")) then 

			DL:createNPCNode(12, 26, "DL_Vincent_LloydsPlan") -- You know the leader of the clerics, Lloyd, don't you? He was acting strangely lately, it seems like he's planning something.
			DL:addNode()


			DL:createNPCNode(26, -2, "DL_Vincent_LloydsPlan2") -- We need someone who can talk to him and maybe even work for him. If you find out what he's up to, come back and report.
			DL:changeQuestState("lloyds_plan", "started")
			DL:addNode()

		end

		if (not DL:isQuestState("lloyds_plan", "void") and DL:isQuestState("cathedral_thief", "void")) then 

			DL:createNPCNode(22, 23, "DL_Vincent_CathedralThief") -- The clerics have made a big mistake when they tried to exorcise a dangerous grimoire about necromancy.
			DL:addNode()


			DL:createNPCNode(23, 24, "DL_Vincent_CathedralThief2") -- The book is running amok in their cathedral. A great opportunity for us though.
			DL:addNode()


			DL:createNPCNode(24, 25, "DL_Vincent_CathedralThief3") -- While they're distracted, it should be mere child's play to steal the precious candleholders from their cathedral.
			DL:addNode()


			DL:createNPCNode(25, -2, "DL_Vincent_CathedralThief4") -- Bring us those golden candleholders and I'll teach you more about our magic.
			DL:changeQuestState("cathedral_thief", "started")
			DL:addNode()

		end

		if (DL:isQuestState("cathedral_thief", "started") and DL:isQuestComplete("cathedral_thief")) then 

			DL:createNPCNode(27, 28, "DL_Vincent_CathedralFinished") -- Well done. Taking them when everyone else was distracted was a well-thought-out idea, wasn't it.
			DL:removeItem("qe_divinecandleholder", 3)
			DL:changeQuestState("cathedral_thief", "completed")
			DL:addNode()


			DL:createNPCNode(28, -2, "DL_Vincent_CathedralFinished2") -- You've shown me that you are able to use our magic. Take this scroll as a reward and learn how to be one with the shadows.
			DL:addReputationProgress("thief", 10)
			DL:addItem("sp_invisibility", 1)
			DL:addNode()

		end

		if (DL:isQuestState("lloyds_plan", "started") and DL:isQuestComplete("lloyds_plan")) then 

			DL:createCendricNode(20, 29, "DL_Choice_CompleteLloydsPlan2") -- ... Some "V." is abducting mages. Lloyd is trying to find out who that is.
			DL:addNode()


			DL:createNPCNode(29, 30, "DL_Vincent_CompleteLloydsPlan") -- Some "V.", eh. (Grins) Well, it's not me, I can assure you that. Thank you for the report.
			DL:changeQuestState("lloyds_plan", "completed")
			DL:addReputationProgress("thief", 5)
			DL:addNode()


			DL:createNPCNode(30, 31, "DL_Vincent_CompleteLloydsPlan2") -- We'll be a lot more vigilantly now. I'm only worried about Koray, Nuray's brother.
			DL:addNode()


			DL:createNPCNode(31, -2, "DL_Vincent_CompleteLloydsPlan3") -- He went to the marshlands recently. If you see him, tell Nuray.
			DL:changeQuestState("missing_koray", "started")
			DL:addNode()

		end

	end


	DL:createChoiceNode(17)
	DL:addChoice(-1, "") -- 
	DL:addNode()

end