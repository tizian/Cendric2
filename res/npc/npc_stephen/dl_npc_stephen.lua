-- Dialogue for NPC "npc_stephen"
loadDialogue = function(DL) 

	if (not DL:isConditionFulfilled("npc_stephen", "talked")) then 
		DL:setRoot(1) 
	elseif (not DL:isConditionFulfilled("npc_stephen", "lloyd")) then 
		DL:setRoot(2) 
	else 
		DL:setRoot(18) 
	end 

	if (not DL:isConditionFulfilled("npc_stephen", "talked")) then 

		DL:createNPCNode(1, 3, "DL_NPC_ANewFace") -- And again, a new face in the city. As if we hadn't already enough troublemakers here. 
		DL:addConditionProgress("npc_stephen", "talked")
		DL:addNode()


		DL:createNPCNode(3, -2, "DL_NPC_YouShouldKnowRules") -- You should know the rules of the city, if you don't want to end up jailed.
		DL:addNode()

	end

	if (not DL:isConditionFulfilled("npc_stephen", "lloyd")) then 

		DL:createChoiceNode(2)
		if (not DL:isConditionFulfilled("npc_stephen", "rules")) then 
			DL:addChoice(5, "DL_Choice_ExplainRules") -- Explain me the rules of the city.
		end
		if (not DL:isConditionFulfilled("npc_stephen", "who_are_you")) then 
			DL:addChoice(6, "DL_Choice_WhoAreYou") -- Who are you?
		end
		if (not DL:isConditionFulfilled("npc_stephen", "rules")) then 
			DL:addChoice(4, "DL_choice_NoTime") -- I don't have time for this.
		end
		if (DL:isConditionFulfilled("npc_stephen", "who_are_you") and DL:hasItem("qe_recommendationletter", 1)) then 
			DL:addChoice(12, "DL_Choice_Lloyd") -- I need to talk to some "Lloyd" here... (Show the letter)
		end
		if (DL:isConditionFulfilled("npc_stephen", "rules")) then 
			DL:addChoice(-1, "DL_Choice_CU") -- See you.
		end
		DL:addNode()

		if (not DL:isConditionFulfilled("npc_stephen", "rules")) then 

			DL:createNPCNode(5, 9, "DL_NPC_Rules1") -- Well, first, you should know that the Order of the Eternal Light is in charge here. Therefore, respect us and follow our orders as long as you're here, or it won't be a very pleasant stay in Gandria.
			DL:addNode()


			DL:createNPCNode(9, 10, "DL_NPC_Rules2") -- Behave well if you go into a stranger's house - we don't like thieves here. Most citizens of Gandria use observer spells to avoid being robbed.
			DL:addNode()


			DL:createNPCNode(10, 11, "DL_NPC_Rules3") -- And, last but not least, we don't really like seeing other spells than those which originate from the high divine art. You can hang for practicing Necromancy.
			DL:addNode()


			DL:createCendricNode(11, -2, "DL_Cendric_Okay") -- Okay.
			DL:addConditionProgress("npc_stephen", "rules")
			DL:addNode()

		end

		if (not DL:isConditionFulfilled("npc_stephen", "who_are_you")) then 

			DL:createNPCNode(6, 7, "DL_NPC_WhoAreYou") -- I'm Stephen, paladin and vigilante of Gandria and so to say directly the right hand of our Commander, Lloyd. Don't mess with me or my city, understood?
			DL:addConditionProgress("npc_stephen", "who_are_you")
			DL:addNode()


			DL:createChoiceNode(7)
			DL:addChoice(-2, "DL_choice_Understood") -- Understood.
			DL:addChoice(-2, "DL_Choice_MaybeUnderstood") -- I'll try.
			DL:addChoice(8, "DL_Choice_NotUnderstood") -- ... (Mischievous grin)
			DL:addNode()


			DL:createNPCNode(8, -1, "DL_NPC_IKeepEyeOnYou") -- I'll keep an eye on you, boy.
			DL:addNode()

		end

		if (not DL:isConditionFulfilled("npc_stephen", "rules")) then 

			DL:createNPCNode(4, -1, "DL_NPC_DontTrouble") -- Well then... Don't make trouble!
			DL:addNode()

		end

		if (DL:isConditionFulfilled("npc_stephen", "who_are_you") and DL:hasItem("qe_recommendationletter", 1)) then 

			DL:createNPCNode(12, 13, "DL_Stephen_Laughter") -- (Stephen takes the letter and then starts laughing uncontrollably)
			DL:removeItem("qe_recommendationletter", 1)
			DL:addConditionProgress("npc_stephen", "lloyd")
			DL:addNode()


			DL:createNPCNode(13, 14, "DL_Stephen_PieceOfPaper") -- And you thought this piece of paper will bring you to him? That's truly ridiculous.
			DL:addNode()


			DL:createChoiceNode(14)
			DL:addChoice(15, "DL_Choice_Urgent") -- But it's urgent!
			DL:addChoice(16, "DL_Choice_Inina") -- But this letter is from the High Priestess Inina!
			DL:addChoice(17, "DL_Choice_Intimidate") -- Just let me to him and no one gets hurt.
			DL:addNode()


			DL:createNPCNode(15, -2, "DL_Stephen_Urgent") -- Sure it is. But there are other urgent things too. Like what you'll have to do to get your letter back.
			DL:addNode()


			DL:createNPCNode(16, -2, "DL_Stephen_Inina") -- Good for you. But you can't talk to her either until you've taken care of my tiny problem. Then you'll get your letter back.
			DL:addNode()


			DL:createNPCNode(17, -2, "DL_Stephen_Intimidate") -- Ah, you're a tough boy aren't you. But I think I'll have to keep your letter for a while - until you've taken care of some problem of mine...
			DL:addNode()

		end

		if (DL:isConditionFulfilled("npc_stephen", "rules")) then 
		end

	end


	DL:createChoiceNode(18)
	DL:addChoice(19, "DL_Choice_OpenGate") -- Open the damn gate!
	DL:addChoice(-1, "") -- 
	DL:addNode()


	DL:createNPCNode(19, -1, "DL_Stephen_Sure") -- Sure.
	DL:addConditionProgress("default", "innerwall_open")
	DL:addNode()

end