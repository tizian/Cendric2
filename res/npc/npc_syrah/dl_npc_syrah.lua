-- Dialogue for NPC "npc_syrah"
loadDialogue = function(DL) 

	if (not DL:isConditionFulfilled("npc_syrah", "talked")) then 
		DL:setRoot(1) 
	else 
		DL:setRoot(2) 
	end 

	if (not DL:isConditionFulfilled("npc_syrah", "talked")) then 

		DL:createNPCNode(1, -2, "DL_Syrah_Greeting") -- Well hello there! You look like you could use some potions made by the infamous alchemist Syrah. A love potion or a hair dye - there's nearly nothing I'm not capable of brewing.
		DL:addConditionProgress("npc_syrah", "talked")
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
	DL:addChoice(8, "DL_Choice_CanYouBrew") -- Can you brew something for me?
	if (not DL:isConditionFulfilled("npc_syrah", "Choice-1")) then 
		DL:addChoice(11, "DL_Choice_Trade") -- Show me your potions.
	end
	DL:addChoice(-1, "") -- 
	DL:addNode()

	if (DL:isQuestState("the_mark", "started") and DL:isQuestState("questionable_method", "void")) then 

		DL:createNPCNode(3, 4, "DL_Syrah_Mark") -- (Syrah eyes the mark carefully) Hm. I've seen this mark before. Definitely a tracking curse. But it's one of the strongest I've ever seen.
		DL:addNode()


		DL:createNPCNode(4, 5, "DL_Syrah_Mark2") -- That's not going to be easy. But if you help me to gather some ingredients, I think I can try to remove or at least weaken the curse.
		DL:addNode()


		DL:createNPCNode(5, -1, "DL_Syrah_Mark3") -- (Syrah pulls a green book out of her shelf and browses in it) I only need some bones, some blood of a bat and the heart of a fire rat. I think you can find all of this in the sewers.
		DL:changeQuestState("questionable_method", "started")
		DL:addNode()

	end

	if (DL:isQuestState("questionable_method", "started") and not DL:isConditionFulfilled("npc_syrah", "sewers")) then 

		DL:createNPCNode(6, -1, "DL_Syrah_Sewers") -- Some houses have a basement with a connection to the sewers. I think there's also a door near the harbour that leads down.
		DL:addConditionProgress("npc_syrah", "sewers")
		DL:addNode()

	end

	if (not DL:isQuestState("questionable_method", "void") and not DL:isConditionFulfilled("npc_syrah", "mark_seen")) then 

		DL:createNPCNode(7, -1, "DL_Syrah_MarkSeen") -- Hm, let me think. There was this strange guy in the Basilisk Inn, he had the same or a similar mark. But no one could understand what he wanted to say, he only talked rubbish.
		DL:addQuestDescription("the_mark", 3)
		DL:addConditionProgress("npc_syrah", "mark_seen")
		DL:addNode()

	end


	DL:createNPCNode(8, 9, "DL_Syrah_BrewSomething") -- Sure, if you got the ingredients?
	DL:addNode()


	DL:createChoiceNode(9)
	if (DL:hasItem("fo_healingherb", 1) and DL:hasItem("fo_water", 1)) then 
		DL:addChoice(10, "DL_Choice_LesserHealingPotion") -- $fo_lesserhealingpotion$ (1 $fo_healingherb$, 1 $fo_water$)
	end
	DL:addChoice(-2, "DL_Choice_ComeBackLater") -- I'll come back later [BACK]
	DL:addNode()

	if (DL:hasItem("fo_healingherb", 1) and DL:hasItem("fo_water", 1)) then 

		DL:createNPCNode(10, -2, "DL_Syrah_HereYouGo") -- Thanks. Here you go.
		DL:removeItem("fo_water", 1)
		DL:removeItem("fo_healingherb", 1)
		DL:addItem("fo_lesserhealingpotion", 1)
		DL:addNode()

	end

	if (not DL:isConditionFulfilled("npc_syrah", "Choice-1")) then 

		DL:createTradeNode(11, -1)
		DL:addNode()

	end

end