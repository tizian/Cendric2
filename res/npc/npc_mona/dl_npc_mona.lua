-- Dialogue for NPC "npc_mona"
loadDialogue = function(DL) 

	if (not DL:isConditionFulfilled("npc_mona", "talked")) then 
		DL:setRoot(1) 
	else 
		DL:setRoot(2) 
	end 

	if (not DL:isConditionFulfilled("npc_mona", "talked")) then 

		DL:createNPCNode(1, 3, "DL_Mona_Hi") -- Oh, a new guest. Welcome to the Red Pearl.
		DL:addNode()


		DL:createNPCNode(3, -2, "DL_Mona_Hi2") -- If you got gold and respect my girls, you can have an extraordinary time here. (Smiles)
		DL:addConditionProgress("npc_mona", "talked")
		DL:addNode()

	end


	DL:createChoiceNode(2)
	if (not DL:isConditionFulfilled("npc_mona", "upper_floor")) then 
		DL:addChoice(5, "DL_Choice_UpperFloor") -- How can I get to the upper floor?
	end
	if (DL:isConditionFulfilled("npc_mona", "upper_floor") and not DL:isConditionFulfilled("npc_mona", "gate_open") and not DL:hasItem("gold", 100)) then 
		DL:addChoice(6, "DL_Choice_OpenGate") -- Let me go to the upper floor. (100 $gold$)
	end
	if (DL:isConditionFulfilled("npc_mona", "upper_floor") and not DL:isConditionFulfilled("npc_mona", "gate_open") and DL:hasItem("gold", 100)) then 
		DL:addChoice(8, "DL_Choice_OpenGate") -- 
	end
	if (DL:isConditionFulfilled("npc_mona", "upper_floor") and not DL:isConditionFulfilled("npc_mona", "only_meet") and not DL:isConditionFulfilled("npc_mona", "gate_open")) then 
		DL:addChoice(7, "DL_Choice_OnlyMeet") -- I just need to have a quick chat with someone.
	end
	if (DL:isConditionFulfilled("npc_mona", "gate_open") and not DL:isConditionFulfilled("npc_mona", "fun_unlock") and not DL:isConditionFulfilled("npc_mona", "fun_lock")) then 
		DL:addChoice(11, "DL_Choice_FunUnlock") -- What if I... you know... wanna have some fun?
	end
	if (DL:isConditionFulfilled("npc_mona", "fun_unlock") and DL:hasItem("gold", 100)) then 
		DL:addChoice(9, "DL_Choice_Fun") -- I wanna have some fun.
	end
	if (DL:isConditionFulfilled("npc_mona", "fun_unlock") and not DL:hasItem("gold", 100)) then 
		DL:addChoice(10, "DL_Choice_FunNoGold") -- I wanna have some fun. (100 Gold)
	end
	DL:addChoice(4, "DL_Choice_Trade") -- Can I get a drink here?
	DL:addChoice(-1, "") -- 
	DL:addNode()

	if (not DL:isConditionFulfilled("npc_mona", "upper_floor")) then 

		DL:createNPCNode(5, -2, "DL_Mona_UpperFloor") -- I'll open the gate for you - for a small compensation - so you can enjoy the show.
		DL:addConditionProgress("npc_mona", "upper_floor")
		DL:addNode()

	end

	if (DL:isConditionFulfilled("npc_mona", "upper_floor") and not DL:isConditionFulfilled("npc_mona", "gate_open") and not DL:hasItem("gold", 100)) then 

		DL:createNPCNode(6, -2, "DL_Mona_OpenGateNot") -- You don't have enough gold.
		DL:addNode()

	end

	if (DL:isConditionFulfilled("npc_mona", "upper_floor") and not DL:isConditionFulfilled("npc_mona", "gate_open") and DL:hasItem("gold", 100)) then 

		DL:createNPCNode(8, -2, "DL_Mona_OpenGate") -- Sure, have fun.
		DL:removeItem("gold", 100)
		DL:addConditionProgress("npc_mona", "gate_open")
		DL:addNode()

	end

	if (DL:isConditionFulfilled("npc_mona", "upper_floor") and not DL:isConditionFulfilled("npc_mona", "only_meet") and not DL:isConditionFulfilled("npc_mona", "gate_open")) then 

		DL:createNPCNode(7, -2, "DL_Mona_OnlyMeet") -- Then you can wait until they leave again. If you go up there, you'll have to pay.
		DL:addConditionProgress("npc_mona", "only_meet")
		DL:addNode()

	end

	if (DL:isConditionFulfilled("npc_mona", "gate_open") and not DL:isConditionFulfilled("npc_mona", "fun_unlock") and not DL:isConditionFulfilled("npc_mona", "fun_lock")) then 

		DL:createNPCNode(11, 12, "DL_Mona_FunUnlock") -- Hmm... (Mona eyeballs you) How old are you?
		DL:addNode()


		DL:createChoiceNode(12)
		DL:addChoice(13, "DL_Choice_AgeDontKnow") -- I don't know. I lost my memory. Old enough?
		DL:addChoice(14, "DL_Choice_AgeEighteen") -- Eighteen.
		DL:addChoice(15, "DL_Choice_TwentyFive") -- Twenty-five. (Lie)
		DL:addNode()


		DL:createNPCNode(13, -2, "DL_Mona_AgeDontKnow") -- Hm. I guess you may nearly be of age. I'll tell my girls to be gentle with you. (Smiles)
		DL:addConditionProgress("npc_mona", "fun_unlock")
		DL:addNode()


		DL:createNPCNode(14, -2, "DL_Mona_AgeEighteen") -- Hm. Alright, that may be true. It's okay then. (Smiles)
		DL:addConditionProgress("npc_mona", "fun_unlock")
		DL:addNode()


		DL:createNPCNode(15, -1, "DL_Mona_TwentyFive") -- No way! If you're not old enough to tell the truth, you're not old enough for my girls. Get out!
		DL:addConditionProgress("npc_mona", "fun_lock")
		DL:addNode()

	end

	if (DL:isConditionFulfilled("npc_mona", "fun_unlock") and DL:hasItem("gold", 100)) then 

		DL:createNPCNode(9, 16, "DL_Mona_WhatLookingFor") -- Very well. What are you looking for?
		DL:addNode()


		DL:createChoiceNode(16)
		if (not DL:isConditionFulfilled("npc_mona", "sex_romantic")) then 
			DL:addItemChoice(17, "DL_Choice_SexRomantic", "gold", 100) -- Something romantic.
		end
		if (not DL:isConditionFulfilled("npc_mona", "sex_exotic")) then 
			DL:addItemChoice(18, "DL_Choice_SexExotic", "gold", 100) -- Something exotic.
		end
		if (not DL:isConditionFulfilled("npc_mona", "sex_wild")) then 
			DL:addItemChoice(19, "DL_Choice_SexWild", "gold", 100) -- Something wild.
		end
		DL:addChoice(-2, "DL_Choice_DontKnowLater") -- I don't know. I'll come back later. [BACK]
		DL:addNode()

		if (not DL:isConditionFulfilled("npc_mona", "sex_romantic")) then 

			DL:createNPCNode(17, -2, "DL_Mona_SexRomantic") -- Alright. Then go to Lola, she's already waiting for you. (Winks)
			DL:addConditionProgress("npc_mona", "sex_romantic")
			DL:removeGold(100)
			DL:addNode()

		end

		if (not DL:isConditionFulfilled("npc_mona", "sex_exotic")) then 

			DL:createNPCNode(18, -2, "DL_Mona_SexExotic") -- Ah, then I have something for you. Go to Karma and she will show you.
			DL:addConditionProgress("npc_mona", "sex_exotic")
			DL:removeGold(100)
			DL:addNode()

		end

		if (not DL:isConditionFulfilled("npc_mona", "sex_wild")) then 

			DL:createNPCNode(19, -2, "DL_Mona_SexWild") -- I'm sorry, but Adelya - our wild cat - is currently busy.
			DL:gotoNode(16)
			DL:addConditionProgress("npc_mona", "sex_wild")
			DL:addNode()

		end

	end

	if (DL:isConditionFulfilled("npc_mona", "fun_unlock") and not DL:hasItem("gold", 100)) then 

		DL:createNPCNode(10, -2, "DL_Mona_OpenGateNot") -- 
		DL:addNode()

	end


	DL:createTradeNode(4, -2)
	DL:addNode()

end