-- Dialogue for NPC "npc_ivo"
loadDialogue = function(DL)

	DL:createChoiceNode(0)
	if (not DL:isConditionFulfilled("npc_ivo", "who_are_you")) then
		DL:addChoice(2, "DL_Choice_Farmer") -- Is this your farm?
	end
	if (not DL:isConditionFulfilled("npc_ivo", "merchant")) then
		DL:addChoice(20, "DL_Choice_Merchant") -- Could you sell me some goods?
	end 
	if (DL:isConditionFulfilled("npc_ivo", "who_are_you") and not DL:isConditionFulfilled("npc_ivo", "gandria")) then
		DL:addChoice(30, "DL_Choice_Gandria") -- What can you tell me about Gandria?
	end 
	if (DL:isQuestState("monster_problem", "void") and DL:isConditionFulfilled("npc_ivo", "who_are_you")) then
		DL:addChoice(6, "DL_Choice_Job") -- What job can you offer?
	end 
	if (DL:isConditionFulfilled("npc_ivo", "who_are_you") and not DL:isConditionFulfilled("npc_ivo", "sheep_asked")) then
		DL:addChoice(4, "DL_Choice_AskSheep") -- I heard that your sheep are disappearing?
	end
	if (DL:isConditionFulfilled("npc_ivo", "sheep_asked") and not DL:isQuestState("monster_problem", "completed")) then
		DL:addChoice(10, "DL_Choice_Sheep") -- About your sheep...
	end
	DL:addChoice(-1, "DL_Choice_Bye") -- Bye.  
	DL:addNode()
	
	DL:setRoot(0)
	
	if (not DL:isConditionFulfilled("npc_ivo", "who_are_you")) then
	
		DL:createNPCNode(2, 3, "DL_Ivo_Farmer") -- Yes, I'm the farmer here. Are you looking for a job?
		DL:addConditionProgress("npc_ivo", "who_are_you")
		DL:addNode()
		
		DL:createChoiceNode(3)
		DL:addChoice(-2, "DL_Choice_NoJob") -- No, not really.
		if (DL:isQuestState("monster_problem", "started")) then
			DL:addChoice(4, "DL_Choice_AskSheep") -- I heard that your sheep are disappearing?
		end
		if (DL:isQuestState("monster_problem", "void")) then
			DL:addChoice(5, "DL_Choice_SheepQuest") -- Yes, can I do something for you?
		end 
		DL:addNode()
	
	end
	
	if (DL:isConditionFulfilled("npc_ivo", "who_are_you") and not DL:isConditionFulfilled("npc_ivo", "gandria")) then
		
		DL:createNPCNode(30, 31, "DL_Ivo_Gandria") -- Gandria is under the tightly-controlled jurisdiction of the Clerics and their paladins.
		DL:addConditionProgress("npc_ivo", "gandria")
		DL:addNode()
		
		DL:createNPCNode(31, 32, "DL_Ivo_Paladins") -- Useless mages with shiny armour and that's about it. They don't lift a finger for people like us - still, we work for them.
		DL:addNode()
		
		DL:createChoiceNode(32)
		DL:addChoice(33, "DL_Choice_FightBack") -- Why don't you do something about it?
		DL:addChoice(34, "DL_Choice_TalkToThem") -- I could try and talk to them...
		DL:addChoice(35, "DL_Choice_YourProblem") -- Well, that's your problem.
		DL:addNode()
		
		DL:createNPCNode(33, -2, "DL_Ivo_FightBack") -- It wouldn't be a good idea to rebel against the Clerics. As long as they leave us alone, we're good.
		DL:addNode()
		
		DL:createNPCNode(34, -2, "DL_Ivo_TalkToThem") -- Like we've never tried reasoning with them before... No. Talking is no use. Commoners like us are only scum to them.
		DL:addNode()
		
		DL:createNPCNode(35, -2, "DL_Ivo_MyProblem") -- I guess it is.
		DL:addNode()
		
	end 
	
	if (not DL:isConditionFulfilled("npc_ivo", "merchant")) then
		
		DL:createNPCNode(20, -2, "DL_Ivo_Merchant") -- My son Tristan is responsible for selling our goods. Go and talk to him if you want to buy something.
		DL:addConditionProgress("npc_ivo", "merchant")
		DL:addNode()
		
	end 
	
	if (not DL:isConditionFulfilled("npc_ivo", "sheep_asked")) then
	
		DL:createNPCNode(4, 5, "DL_Ivo_SheepDisappearing") -- That's true, unfortunately.
		DL:addConditionProgress("npc_ivo", "sheep_asked")
		DL:addNode()
		
		DL:createNPCNode(5, -2, "DL_Ivo_Grateful") -- I would be very grateful if you could take care of the thief.
		DL:addNode()
		
	end
	
	if (DL:isQuestState("monster_problem", "void")) then
		
		DL:createNPCNode(6, 7, "DL_Ivo_SheepQuest") -- Lately, some of my sheep went missing. Could be a wolf, could be some bandits. 
		DL:addConditionProgress("npc_ivo", "sheep_asked")
		DL:changeQuestState("monster_problem", "started")
		DL:addNode()
		
		DL:createNPCNode(7, -2, "DL_Ivo_Grateful") -- I would be very grateful if you could take care of the thief.
		DL:addNode()
		
	end 
	
	if (DL:isConditionFulfilled("npc_ivo", "sheep_asked") and not DL:isQuestState("monster_problem", "completed")) then
	
		DL:createChoiceNode(10)
		if (not DL:isConditionFulfilled("npc_ivo", "ask_tristan")) then
			DL:addChoice(11, "DL_Choice_WhereStart") -- Where should I start searching for the thief?
		end
		if (DL:isConditionFulfilled("npc_zeff", "sheep") and not DL:isConditionFulfilled("npc_ivo", "zeff_is_it")) then
			DL:addChoice(12, "DL_Choice_ZeffIsIt") -- I know who the thief is...
		end
		if (DL:isQuestComplete("monster_problem")) then
			DL:addChoice(14, "DL_Choice_MonsterBanished") -- The sheep thief won't bother you anymore.
		end
		DL:addChoice(-2, "DL_Choice_Back") -- [BACK]
		DL:addNode()
		
		DL:createNPCNode(11, -2, "DL_Ivo_AskTristan") -- Hm. You could ask my son, Tristan, he's in charge of tending the sheep.
		DL:addConditionProgress("npc_ivo", "ask_tristan")
		DL:gotoNode(10)
		DL:addNode()
		
		DL:createCendricNode(12, 13, "DL_Cendric_ZeffIsIt") -- A wolf-monster named Zeff is responsible for your missing sheep!
		DL:addConditionProgress("npc_ivo", "zeff_is_it")
		DL:addNode()
		
		DL:createNPCNode(13, -2, "DL_Ivo_GoAndKill") -- Interesting. And now go and make sure that he won't steal any more sheep. How you do it is for you to decide.
		DL:gotoNode(10)
		DL:addNode()
		
		DL:createNPCNode(14, -2, "DL_Ivo_ThankYou") -- Really? The thief won't come again? Thank you so much for helping us! Go to Tristan and tell him to give you a reward.
		DL:changeQuestState("monster_problem", "completed")
		DL:addNode()
		
	end
end	

	