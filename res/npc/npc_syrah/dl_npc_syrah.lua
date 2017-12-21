-- Dialogue for NPC "npc_syrah"
loadDialogue = function(DL) 

	if (not DL:isConditionFulfilled("npc_syrah", "talked")) then 
		DL:setRoot(1) 
	elseif (DL:hasItem("qe_invisrecipe", 1) and not DL:isConditionFulfilled("npc_syrah", "invis_recipe")) then 
		DL:setRoot(18) 
	else 
		DL:setRoot(2) 
	end 

	if (not DL:isConditionFulfilled("npc_syrah", "talked")) then 

		DL:createNPCNode(1, -2, "DL_Syrah_Greeting") -- Well hello there! You look like you could use some potions made by the infamous alchemist Syrah. A love potion or a hair dye - there's nearly nothing I'm not capable of brewing.
		DL:addConditionProgress("npc_syrah", "talked")
		DL:addNode()

	end

	if (DL:hasItem("qe_invisrecipe", 1) and not DL:isConditionFulfilled("npc_syrah", "invis_recipe")) then 

		DL:createNPCNode(18, 19, "DL_Syrah_InvisRecipe") -- Oh, hello. There's a certain recipe missing from my basement. Do you know something about it?
		DL:addNode()


		DL:createChoiceNode(19)
		DL:addChoice(20, "DL_Choice_IBroughtItBack") -- Yes. I got it back for you. (Give the recipe)
		DL:addChoice(21, "DL_Choice_IKnowNothing") -- No, sorry.
		DL:addNode()


		DL:createNPCNode(20, -2, "DL_Syrah_IBroughtItBack") -- Wow, thank you. You know, that really means a lot to me. I won't ask where you got that though. (Winks)
		DL:removeItem("qe_invisrecipe", 1)
		DL:addReputationProgress("necromancer", 5)
		DL:changeQuestState("invis_recipe", "failed")
		DL:addConditionProgress("npc_syrah", "invis_recipe")
		DL:addNode()


		DL:createNPCNode(21, -2, "DL_Syrah_IKnowNothing") -- What a pity.
		DL:addConditionProgress("npc_syrah", "invis_recipe")
		DL:addNode()

	end


	DL:createChoiceNode(2)
	if (DL:isQuestState("the_mark", "started") and DL:isQuestState("questionable_method", "void")) then 
		DL:addChoice(3, "DL_Choice_Mark") -- Can you remove this curse? (Show the mark)
	end
	if (DL:isQuestState("questionable_method", "started") and not DL:isConditionFulfilled("npc_syrah", "sewers")) then 
		DL:addChoice(6, "DL_Choice_Sewers") -- How can I get to the sewers?
	end
	if (not DL:isQuestState("questionable_method", "void") and not DL:isConditionFulfilled("npc_syrah", "mark_seen")) then 
		DL:addChoice(7, "DL_Choice_MarkSeen") -- Where have you already seen this mark?
	end
	if ((DL:isQuestState("questionable_method", "started") and DL:isQuestComplete("questionable_method")) or (DL:isConditionFulfilled("npc_syrah", "helper1") and not DL:isConditionFulfilled("npc_syrah", "helper2"))) then 
		DL:addChoice(22, "DL_Choice_MarkQuestDone") -- I got what you wanted. (Give ingredients)
	end
	if (DL:isQuestState("questionable_method", "completed") and DL:isQuestState("favor_for_a_favor", "void")) then 
		DL:addChoice(31, "DL_Choice_FavourQuestStart") -- What kind of favour should that be?
	end
	if (DL:isQuestState("favor_for_a_favor", "started") and DL:isQuestComplete("favor_for_a_favor")) then 
		DL:addChoice(35, "DL_Choice_CompleteFavor") -- I got the cinderbloom for you.
	end
	if (DL:isConditionFulfilled("npc_luiz", "talked") and not DL:isConditionFulfilled("npc_syrah", "necromancers_found")) then 
		DL:addChoice(36, "DL_Choice_NecromancersFound") -- So you're a necromancer.
	end
	if (not DL:isConditionFulfilled("npc_syrah", "thea") and DL:isQuestState("theas_dream", "started")) then 
		DL:addChoice(37, "DL_Choice_Thea") -- Are you looking for an apprentice?
	end
	DL:addChoice(8, "DL_Choice_CanYouBrew") -- Can you brew something for me?
	DL:addChoice(11, "DL_Choice_Trade") -- Show me your potions.
	DL:addChoice(-1, "") -- 
	DL:addNode()

	if (DL:isQuestState("the_mark", "started") and DL:isQuestState("questionable_method", "void")) then 

		DL:createNPCNode(3, 4, "DL_Syrah_Mark") -- (Syrah eyes the mark carefully) Hm. I've seen this mark before. Definitely a tracking curse. But it's one of the strongest I've ever seen.
		DL:addNode()


		DL:createNPCNode(4, 5, "DL_Syrah_Mark2") -- That's not going to be easy. But if you help me to gather some ingredients, I think I can try to remove or at least weaken the curse.
		DL:addNode()


		DL:createNPCNode(5, -2, "DL_Syrah_Mark3") -- (Syrah pulls a green book out of her shelf and browses it) I only need some bones, some blood of a bat and the heart of a fire rat. I think you can gather all of this from the nasty creatures in the sewers.
		DL:changeQuestState("questionable_method", "started")
		DL:addNode()

	end

	if (DL:isQuestState("questionable_method", "started") and not DL:isConditionFulfilled("npc_syrah", "sewers")) then 

		DL:createNPCNode(6, -2, "DL_Syrah_Sewers") -- Some houses have a basement with a connection to the sewers. I think there's also a door near the harbour that leads down.
		DL:addConditionProgress("npc_syrah", "sewers")
		DL:addNode()

	end

	if (not DL:isQuestState("questionable_method", "void") and not DL:isConditionFulfilled("npc_syrah", "mark_seen")) then 

		DL:createNPCNode(7, -2, "DL_Syrah_MarkSeen") -- Hm, let me think. There was this strange guy in the Basilisk Inn, he had the same or a similar mark. But no one could understand what he wanted to say, he only talked rubbish.
		DL:addQuestDescription("the_mark", 3)
		DL:addConditionProgress("npc_syrah", "mark_seen")
		DL:addNode()

	end

	if ((DL:isQuestState("questionable_method", "started") and DL:isQuestComplete("questionable_method")) or (DL:isConditionFulfilled("npc_syrah", "helper1") and not DL:isConditionFulfilled("npc_syrah", "helper2"))) then 

		DL:createNPCNode(22, 23, "DL_Syrah_MarkQuestDone") -- Ah, perfect. Now let's get started.
		DL:changeQuestState("questionable_method", "completed")
		DL:addReputationProgress("necromancer", 10)
		DL:removeItem("qe_fireratheart", 1)
		DL:removeItem("qe_batblood", 2)
		DL:removeItem("qe_bone", 3)
		DL:addNode()


		DL:createNPCNode(23, 24, "DL_Syrah_MarkQuestDone2") -- (Syrah places the bones in a circle around the fire rat's heart.) Come.
		DL:addNode()


		DL:createChoiceNode(24)
		DL:addChoice(25, "DL_Choice_OkayMark") -- Let's get this over with.
		if (not DL:isConditionFulfilled("npc_syrah", "come_here")) then 
			DL:addChoice(26, "DL_Choice_NotOkayMark") -- I'm not sure whether this is a good idea.
		end
		DL:addNode()


		DL:createNPCNode(25, 27, "DL_Syrah_RelieveCurse") -- (Syrah grabs your arm and drags you into the circle.)
		DL:addConditionProgress("npc_syrah", "helper2")
		DL:addNode()


		DL:createNPCNode(27, 28, "DL_Syrah_RelieveCurse2") -- (She takes the blood and pours it over the mark, while murmuring some words.)
		DL:addNode()


		DL:createNPCNode(28, 29, "DL_Syrah_RelieveCurse3") -- See? I think it worked. I don't feel much of this strange magic anymore. 
		DL:addNode()


		DL:createNPCNode(29, 30, "DL_Syrah_RelieveCurse4") -- Hm. It's still visible. You'll have to seek out who did this to you. But now you can find him before he finds you. (Giggles)
		DL:addQuestDescription("the_mark", 4)
		DL:addNode()


		DL:createNPCNode(30, -2, "DL_Syrah_DoMeAFavour") -- But now, as I've done you a favour, you should do me one too, I think.
		DL:addNode()

		if (not DL:isConditionFulfilled("npc_syrah", "come_here")) then 

			DL:createNPCNode(26, -2, "DL_Syrah_NotOkayMark2") -- I'm only trying to help you! You want to be relieved from your curse, right? Now come here.
			DL:addConditionProgress("npc_syrah", "come_here")
			DL:gotoNode(24)
			DL:addConditionProgress("npc_syrah", "helper1")
			DL:addNode()

		end

	end

	if (DL:isQuestState("questionable_method", "completed") and DL:isQuestState("favor_for_a_favor", "void")) then 

		DL:createNPCNode(31, 32, "DL_Syrah_FavourQuestStart") -- There's a graveyard just outside the city, in the marshlands, where people are buried that were not welcome in the city - mostly criminals.
		DL:addNode()


		DL:createNPCNode(32, 33, "DL_Syrah_FavourQuestStart2") -- I need a special flower, the cinderbloom, for one of my potions. It only grows on graves of people who have died in a fire. 
		DL:addNode()


		DL:createNPCNode(33, 34, "DL_Syrah_FavourQuestStart3") -- I'm sure that you can find that flower there - mages who "misuse" magic are often burnt at the stake and whatever remains will be taken to this crypt.
		DL:addNode()


		DL:createNPCNode(34, -1, "DL_Syrah_FavourQuestStart4") -- Oh, and if you happen to meet the grave digger Morton, he's a good friend of mine. Just tell him I sent you, and he'll let you in.
		DL:changeQuestState("favor_for_a_favor", "started")
		DL:addNode()

	end

	if (DL:isQuestState("favor_for_a_favor", "started") and DL:isQuestComplete("favor_for_a_favor")) then 

		DL:createNPCNode(35, -2, "DL_Syrah_CompleteFavor") -- Thank you so much, that's very kind of you. Here, take a potion for your effort.
		DL:removeItem("qe_cinderbloom", 1)
		DL:addReputationProgress("necromancer", 10)
		DL:changeQuestState("favor_for_a_favor", "completed")
		DL:addItem("fo_greaterhealingpotion", 1)
		DL:addNode()

	end

	if (DL:isConditionFulfilled("npc_luiz", "talked") and not DL:isConditionFulfilled("npc_syrah", "necromancers_found")) then 

		DL:createNPCNode(36, -2, "DL_Syrah_NecromancersFound") -- Hehe. Glad to hear you've found Luiz and his crew. Keep it to yourself and you'll make some really good friends.
		DL:addConditionProgress("npc_syrah", "necromancers_found")
		DL:addReputationProgress("necromancer", 5)
		DL:addNode()

	end

	if (not DL:isConditionFulfilled("npc_syrah", "thea") and DL:isQuestState("theas_dream", "started")) then 

		DL:createNPCNode(37, 38, "DL_Syrah_Thea") -- Do you know something about herbs and potions then?
		DL:addConditionProgress("npc_syrah", "thea")
		DL:addNode()


		DL:createCendricNode(38, 39, "DL_Cendric_Thea") -- No, not me, I'm asking for a girl called Thea. She's looking for a job.
		DL:addNode()


		DL:createNPCNode(39, -2, "DL_Syrah_Thea2") -- Ah ok. Yes, you can tell her she can start here, as long as she's not totally clumsy.
		DL:addQuestDescription("theas_dream", 1)
		DL:addNode()

	end


	DL:createNPCNode(8, 9, "DL_Syrah_BrewSomething") -- Sure, if you got the ingredients?
	DL:addNode()


	DL:createChoiceNode(9)
	DL:addCraftingChoice(10, "DL_Choice_LesserHealingPotion") -- fo_lesserhealingpotion,fo_healingherb,1,fo_water,1
	DL:addCraftingChoice(12, "DL_Choice_MediumHealingPotion") -- fo_mediumhealingpotion,fo_healingherb,2,fo_water,1
	DL:addCraftingChoice(13, "DL_Choice_GreaterHealingPotion") -- fo_greaterhealingpotion,fo_healingherb,3,fo_water,1
	DL:addCraftingChoice(14, "DL_Choice_ElixirFire") -- pe_elixirfire,fo_redhat,1,fo_stormweed,1,fo_wine,1
	DL:addCraftingChoice(15, "DL_Choice_ElixirLight") -- pe_elixirlight,fo_glowingshroom,1,fo_stormweed,1,fo_wine,1
	DL:addCraftingChoice(16, "DL_Choice_ElixirShadow") -- pe_elixirshadow,fo_caveberry,1,fo_stormweed,1fo_wine,1
	DL:addCraftingChoice(17, "DL_Choice_ElixirIce") -- pe_elixirice,fo_windrose,1,fo_stormweed,1,fo_wine,1
	DL:addChoice(-2, "DL_Choice_ComeBackLater") -- I'll come back later [BACK]
	DL:addNode()


	DL:createNPCNode(10, -2, "DL_Syrah_HereYouGo") -- Thanks. Here you go.
	DL:gotoNode(9)
	DL:addNode()


	DL:createNPCNode(12, -2, "DL_Syrah_HereYouGo") -- 
	DL:gotoNode(9)
	DL:addNode()


	DL:createNPCNode(13, -2, "DL_Syrah_HereYouGo") -- 
	DL:gotoNode(9)
	DL:addNode()


	DL:createNPCNode(14, -2, "DL_Syrah_HereYouGo") -- 
	DL:gotoNode(9)
	DL:addNode()


	DL:createNPCNode(15, -2, "DL_Syrah_HereYouGo") -- 
	DL:gotoNode(9)
	DL:addNode()


	DL:createNPCNode(16, -2, "DL_Syrah_HereYouGo") -- 
	DL:gotoNode(9)
	DL:addNode()


	DL:createNPCNode(17, -2, "DL_Syrah_HereYouGo") -- 
	DL:gotoNode(9)
	DL:addNode()


	DL:createTradeNode(11, -2)
	DL:addNode()

end