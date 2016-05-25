-- Dialogue for the two NPCs "City Guard"
loadDialogue = function(DL)
	
	DL:createChoiceNode(1)
	if (not DL:isConditionFulfilled("npc_cityguard", "gates_closed")) then
		DL:addChoice(10, "DL_Choice_WhyGatesClosed") -- Why are the gates closed?
	end
	if (not DL:isConditionFulfilled("npc_cityguard", "stranger")) then
		DL:addChoice(2, "DL_Choice_Stranger") -- Do I look like a stranger to you?
	end
	if (DL:isConditionFulfilled("npc_cityguard", "gates_closed") and DL:isConditionFulfilled("npc_zeff", "curse_talked") and not DL:isConditionFulfilled("npc_cityguard", "zeff")) then
		DL:addChoice(20, "DL_Choice_Zeff") -- Do you know a guy called "Zeff"?
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
		
		DL:createNPCNode(4, -2, "DL_Citguard_Eyeballing") -- (Eyeballs you) Yes. Maybe I don't know every filthy vagabond that lurks around at the waterfront, but I would certainly recognize anyone who has left the city in the last few days. And you're not one of them. 
		DL:addNode()
		
	end
	
	if (not DL:isConditionFulfilled("gates_closed", "gates_closed")) then
		
		DL:createNPCNode(10, 11, "DL_Citguard_PeopleDisappearing") -- There were a few alarming cases of people leaving the city and not returning, lately.  
		DL:addConditionProgress("npc_cityguard", "gates_closed")
		DL:addNode()
		
		DL:createNPCNode(11, -2, "DL_Citguard_GatesClosed") -- We have to assume that there is still something or someone out there that is responsible for this. The gates stay closed until this mystery is solved.
		DL:changeQuestState("gates_of_gandria","started")
		DL:addNode()
		
	end
	
	if (DL:isConditionFulfilled("npc_cityguard", "gates_closed") and DL:isConditionFulfilled("npc_zeff", "curse_talked") and not DL:isConditionFulfilled("npc_cityguard", "zeff")) then
		
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
	
end	
	