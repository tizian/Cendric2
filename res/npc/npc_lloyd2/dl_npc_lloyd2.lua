-- Dialogue for NPC "npc_lloyd2"
loadDialogue = function(DL) 

	if (DL:isQuestState("further_investigation", "void") and not DL:isConditionFulfilled("npc_lloyd2", "talked")) then 
		DL:setRoot(1) 
	elseif (not DL:isConditionFulfilled("npc_lloyd2", "talked")) then 
		DL:setRoot(2) 
	elseif (not DL:isConditionFulfilled("npc_lloyd2", "going_on")) then 
		DL:setRoot(8) 
	elseif (not DL:isConditionFulfilled("npc_lloyd2", "hurry_up")) then 
		DL:setRoot(12) 
	else 
		DL:setRoot(13) 
	end 

	if (DL:isQuestState("further_investigation", "void") and not DL:isConditionFulfilled("npc_lloyd2", "talked")) then 

		DL:createNPCNode(1, 3, "DL_Lloyd_WhatsGoingOn") -- By the Eternal Light! What's going on here?
		DL:addNode()


		DL:createCendricNode(3, 4, "DL_Cendric_WasLookingForYou") -- Commander Lloyd, what a coincidence! I was looking for you to give you this letter from Inina.
		DL:removeItem("qe_recommendationletter", 1)
		DL:addNode()


		DL:createNPCNode(4, 5, "DL_Lloyd_ReadLetter") -- (Lloyd reads the letter) Hm, interesting. Thank you for helping us with that problem.
		DL:changeQuestState("clerics_recommendation", "completed")
		DL:addReputationProgress("cleric", 5)
		DL:addNode()


		DL:createNPCNode(5, 6, "DL_Lloyd_ReadLetter2") -- Inina wanted you to join the Clerics, but you've already made another decision.
		DL:addConditionProgress("npc_lloyd", "guild")
		DL:addNode()


		DL:createNPCNode(6, 7, "DL_Lloyd_ReadLetter3") -- She also wrote that she needs your help again, looking for hints that could help us find out where that beast came from.
		DL:changeQuestState("further_investigation", "started")
		DL:addConditionProgress("default", "second_boss_open")
		DL:addNode()


		DL:createNPCNode(7, -2, "DL_Lloyd_ReadLetter4") -- But first, back to that lighthouse; what was going on there?
		DL:addConditionProgress("npc_lloyd2", "talked")
		DL:addNode()

	end

	if (not DL:isConditionFulfilled("npc_lloyd2", "talked")) then 

		DL:createNPCNode(2, -2, "DL_Lloyd_WhatsGoingOn") -- 
		DL:addConditionProgress("npc_lloyd2", "talked")
		DL:addNode()

	end

	if (not DL:isConditionFulfilled("npc_lloyd2", "going_on")) then 

		DL:createChoiceNode(8)
		DL:addChoice(9, "DL_Choice_KillBird") -- I had to slay a monstrous bird.
		DL:addChoice(10, "DL_Choice_NothingImportant") -- Nothing of interest. I just enjoyed the thunderstorm.
		if (DL:isConditionFulfilled("npc_jack", "why_not_clerics")) then 
			DL:addChoice(11, "DL_Choice_YourJob") -- Technically, I only did what you were supposed to do.
		end
		DL:addNode()


		DL:createNPCNode(9, -2, "DL_Lloyd_KillBird") -- Hm, Jack told me about that monster, but we've never seen it. Good job on hunting it down.
		DL:addConditionProgress("npc_lloyd2", "going_on")
		DL:addNode()


		DL:createNPCNode(10, -2, "DL_Lloyd_NothingImportant") -- Hmm. It seemed to be a quite extraordinary thunderstorm. But if you say so...
		DL:addConditionProgress("npc_lloyd2", "going_on")
		DL:addNode()

		if (DL:isConditionFulfilled("npc_jack", "why_not_clerics")) then 

			DL:createNPCNode(11, -2, "DL_Lloyd_YourJob") -- Which means that you've slain that bird Jack told us about? Good job on hunting it down.
			DL:addConditionProgress("npc_lloyd2", "going_on")
			DL:addNode()

		end

	end

	if (not DL:isConditionFulfilled("npc_lloyd2", "hurry_up")) then 

		DL:createNPCNode(12, 14, "DL_Lloyd_HurryUp") -- Nevertheless, Inina still needs your help exploring the strange caverns. She's been away for too long now.
		DL:addNode()


		DL:createNPCNode(14, -2, "DL_Lloyd_HurryUp2") -- I want you to immediately go back to the caverns and support her. That's an order, not a request!
		DL:addConditionProgress("npc_lloyd2", "hurry_up")
		DL:addNode()

	end


	DL:createChoiceNode(13)
	DL:addChoice(-1, "") -- 
	DL:addNode()

end