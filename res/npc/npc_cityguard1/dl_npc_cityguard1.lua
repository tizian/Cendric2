-- Dialogue for the two NPCs "City Guard"
loadDialogue = function(DL)
	
	DL:createChoiceNode(1)
	if (not DL:isConditionFulfilled("npc_cityguard", "gates_closed")) then
		DL:addChoice(10, "DL_Choice_WhyGatesClosed") -- Why are the gates closed?
	end
	if (not DL:isConditionFulfilled("npc_cityguard", "stranger")) then
		DL:addChoice(2, "DL_Choice_Stranger") -- Do I look like a stranger to you?
	end
	if (DL:isConditionFulfilled("npc_cityguard", "gates_closed")) then
	
		if (DL:isConditionFulfilled("npc_zeff", "curse_talked") and not DL:isConditionFulfilled("npc_cityguard", "zeff")) then
			DL:addChoice(20, "DL_Choice_Zeff") -- Do you know a guy called "Zeff"?
		end

		if (DL:isConditionFulfilled("npc_cityguard", "zeff") and not DL:isConditionFulfilled("npc_cityguard", "letin_zeff") and DL:isQuestState("zeff_returns", "started")) then
			DL:addChoice(30, "DL_Choice_AboutZeff") -- About Zeff...
		end
		
		if (DL:isQuestState("gates_of_gandria", "started") and DL:hasItem("qe_recommendationletter", 1)) then
			DL:addChoice(40, "DL_Choice_MysterySolved") -- I know why the people from Gandria went missing.
		end
		
		if (DL:isQuestState("gates_of_gandria", "completed") and not DL:isConditionFulfilled("default", "gandria_open")) then
			DL:addChoice(50, "DL_Choice_LetMeIn") -- Let me in.
		end
		
	end
	DL:addChoice(-1, "") -- ...  
	DL:addNode()

	if (not DL:isConditionFulfilled("npc_cityguard", "talked")) then
		
		DL:createNPCNode(0, 1, "DL_Citguard_Stop") -- STOP! The gates of Gandria are closed for strangers!
		DL:addConditionProgress("npc_cityguard", "talked")
		DL:addNode()
	
		DL:setRoot(0)
		
	else
	
		DL:setRoot(1)
		
	end
	
	if (not DL:isConditionFulfilled("npc_cityguard", "stranger")) then
		
		DL:createNPCNode(2, 3, "DL_Citguard_IDontKnowYou") -- I don't know you. And we can't let anyone in the city we don't know.
		DL:addConditionProgress("npc_cityguard", "stranger")
		DL:addNode()
		
		DL:createChoiceNode(3)
		DL:addChoice(-2, "DL_Choice_Okay") -- Okay...
		DL:addChoice(4, "DL_Choice_AreYouSure") -- Are you entirely sure that you don't recognize me?
		DL:addNode()
		
		DL:createNPCNode(4, 5, "DL_Citguard_Eyeballing") -- (Eyeballs you) Yes. Maybe I don't know every filthy vagabond that lurks around at the waterfront...
		DL:addNode()
		
		DL:createNPCNode(5, -2, "DL_Citguard_Eyeballing2") -- But I would certainly recognize anyone who has left the city in the last few days. And you're not one of them. 
		DL:addNode()
		
	end
	
	if (not DL:isConditionFulfilled("npc_cityguard", "gates_closed")) then
		
		DL:createNPCNode(10, 11, "DL_Citguard_PeopleDisappearing") -- There were a few alarming cases of people leaving the city and not returning, lately.  
		DL:addConditionProgress("npc_cityguard", "gates_closed")
		DL:addNode()
		
		DL:createNPCNode(11, -2, "DL_Citguard_GatesClosed") -- We have to assume that there is still something or someone out there that is responsible for this. The gates stay closed until this mystery is solved.
		DL:changeQuestState("gates_of_gandria","started")
		DL:addNode()
		
	end
	
	if (DL:isConditionFulfilled("npc_cityguard", "gates_closed")) then
	
		if (DL:isConditionFulfilled("npc_zeff", "curse_talked") and not DL:isConditionFulfilled("npc_cityguard", "zeff")) then
			
			DL:createNPCNode(20, 21, "DL_Citguard_Zeff") -- Hmm, let me think... Yes, if I recall correctly, there's a leatherworker Zeff who lives in Gandria. But he's actually one of the people who are missing. Why do you ask?
			DL:addConditionProgress("npc_cityguard", "zeff")
			DL:addNode()
			
			DL:createChoiceNode(21)
			DL:addChoice(-2, "DL_Choice_Nevermind") -- Nevermind...
			DL:addChoice(22, "DL_Choice_IveSeenZeff") -- I've seen him! He was turned into a monster!
			DL:addNode()
			
			DL:createNPCNode(22, -1, "DL_Citguard_Laughs") -- (Laughs) Yes, yes, of course, a monster. And now tell this bullshit someone who actually believes it. Off you go.
			DL:addNode()
			
		end
		
		if (DL:isConditionFulfilled("npc_cityguard", "zeff") and not DL:isConditionFulfilled("npc_cityguard", "letin_zeff") and DL:isQuestState("zeff_returns", "started")) then
	
			DL:createChoiceNode(30)
			DL:addChoice(31, "DL_Choice_YouHaveToBelieve") -- You have to believe me, he was cursed.
			DL:addChoice(-2, "DL_Choice_Nevermind") -- Nevermind...
			DL:addNode()
			
			DL:createCendricNode(31, 32, "DL_Cendric_YouHaveToBelieve") -- He now looks like a wolf-man but he is still himself. You have to let him return to Gandria.
			DL:addNode()
			
			DL:createNPCNode(32, 33, "DL_Citguard_WhatCurse") -- Hrrm... And what will you do to convince me? I don't know you. This could be a trap.
			DL:addNode()
			
			DL:createChoiceNode(33)
			if (DL:hasItem("gold", 50)) then
				DL:addChoice(34, "DL_Choice_TryGold") -- Maybe this will change your mind? (50 Gold)
			end
			if (not DL:isConditionFulfilled("npc_cityguard", "try_charm")) then
				DL:addChoice(35, "DL_Choice_TryCharm") -- I give you my word.
			end
			if (not DL:isConditionFulfilled("npc_cityguard", "try_intimidate")) then
				DL:addChoice(36, "DL_Choice_TryIntimidate") -- Just let him in and I don't have to hurt you.
			end
			if (DL:hasItem("qe_recommendationletter", 1)) then
				DL:addChoice(37, "DL_Choice_TryLetter") -- You know, Inina trusts me. (Show letter)
			end
            if (DL:isQuestComplete("clerics_recommendation")) then
				DL:addChoice(60, "DL_Choice_TryLloyd") -- Lloyd trusts me. You better do, too.
			end

			DL:addChoice(-2, "DL_Choice_Nevermind") -- Nevermind...
			DL:addNode()
			
			DL:createNPCNode(34, 38, "DL_Citguard_TryGold") -- (Smiles) You know how this works, don't you.
			DL:removeItem("gold", 50)
			DL:addNode()
			
			DL:createNPCNode(35, -2, "DL_Citguard_TryCharm") -- Your word means nothing to me, stranger.
			DL:addConditionProgress("npc_cityguard", "try_charm")
			DL:gotoNode(33)
			DL:addNode()
			
			DL:createNPCNode(36, -2, "DL_Citguard_Intimidate") -- Oh, I've had lots of people trying to intimidate me. But, you know, I couldn't care less. 
			DL:addConditionProgress("npc_cityguard", "try_intimidate")
			DL:gotoNode(33)
			DL:addNode()
			
			DL:createNPCNode(37, 38, "DL_Citguard_TryLetter") -- Hm, the seal of the High Priestess Inina. I'll have to believe you, then.
			DL:addNode()
            
            DL:createNPCNode(60, 38, "DL_Citguard_TryLloyd") -- You've already talked to Commander Lloyd?
			DL:addNode()
            
			DL:createNPCNode(38, -2, "DL_Citguard_LetZeffIn") -- Alright, I will inform the other guards. You can tell him it is safe to return to Gandria.
			DL:addConditionProgress("npc_cityguard", "letin_zeff")
			DL:addQuestDescription("zeff_returns", 1)
			DL:addQuestProgress("zeff_returns", "guards_convinced")
			DL:addNode()
			
		end
		
		if (DL:isQuestState("gates_of_gandria", "started") and DL:hasItem("qe_recommendationletter", 1)) then
		
			DL:createChoiceNode(40)
			DL:addChoice(41, "DL_Choice_OnlyLloyd") -- But I won't tell you, I'll only talk to Lloyd in person. (Show letter)
			DL:addChoice(42, "DL_Choice_WolfProblem") -- They were all transformed into wolves. (show letter)
			DL:addNode()
			
			DL:createNPCNode(41, 43, "DL_Citguard_OnlyLloyd") -- It's "Commander" Lloyd.
			DL:addNode()
			
			DL:createNPCNode(42, 43, "DL_Citguard_WolfProblem") -- I hope you're joking.
			DL:addNode()
			
			DL:createNPCNode(43, -2, "DL_Citguard_LetCendricIn") -- (Looks at the seal) But if Inina herself gave you that letter, We'll have to let you in.
			DL:changeQuestState("gates_of_gandria", "completed")
			DL:addNode()

		end
		
		if (DL:isQuestState("gates_of_gandria", "completed") and not DL:isConditionFulfilled("default", "gandria_open")) then

			DL:createNPCNode(50, -1, "") -- 
			DL:addConditionProgress("default", "gandria_open")
			DL:startCutscene("gandria")
			DL:addNode()

		end
		
	end
	
end	
	