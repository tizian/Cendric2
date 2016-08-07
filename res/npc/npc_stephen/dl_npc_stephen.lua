-- Dialogue for NPC "npc_stephen"
loadDialogue = function(DL) 

	if (not DL:isConditionFulfilled("npc_stephen", "talked")) then 
		DL:setRoot(1) 
	else 
		DL:setRoot(2) 
	end 

	if (not DL:isConditionFulfilled("npc_stephen", "talked")) then 

		DL:createNPCNode(1, 3, "DL_NPC_ANewFace") -- And again, a new face in the city. As if we hadn't already enough troublemakers here. 
		DL:addConditionProgress("npc_stephen", "talked")
		DL:addNode()


		DL:createNPCNode(3, -2, "DL_NPC_YouShouldKnowRules") -- You should know the rules of the city, if you don't want to end up jailed.
		DL:addNode()

	end


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
	if (DL:isConditionFulfilled("npc_stephen", "rules")) then 
		DL:addChoice(-1, "DL_Choice_CU") -- See you.
	end
	DL:addNode()

	if (not DL:isConditionFulfilled("npc_stephen", "rules")) then 

		DL:createNPCNode(5, 9, "DL_NPC_Rules1") -- Well, first, you should know that the Order of the Eternal Light is in charge here. Therefore, respect us and follow our orders as long as you're here, or it won't be a very pleasant stay in Gandria.
		DL:addNode()


		DL:createNPCNode(9, 10, "DL_NPC_Rules2") -- Behave well if you go into a stranger's house - we don't like thieves here. Most citicens of Gandria use observer spells to avoid being robbed.
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

	if (DL:isConditionFulfilled("npc_stephen", "rules")) then 
	end

end