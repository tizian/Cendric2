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


		DL:createNPCNode(3, -2, "DL_Mona_Hi2") -- If you got gold and are respectful to my girls, you can have an extraordinary time here. (Smiles)
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
		DL:addChoice(7, "DL_Choice_OnlyMeet") -- I only need to talk to someone briefly.
	end
	DL:addChoice(4, "DL_Choice_Trade") -- Can I get a drink here?
	DL:addChoice(-1, "") -- 
	DL:addNode()

	if (not DL:isConditionFulfilled("npc_mona", "upper_floor")) then 

		DL:createNPCNode(5, -2, "DL_Mona_UpperFloor") -- I'll open the gate for you for a small compensation so you can enjoy the show.
		DL:addConditionProgress("npc_mona", "upper_floor")
		DL:addNode()

	end

	if (DL:isConditionFulfilled("npc_mona", "upper_floor") and not DL:isConditionFulfilled("npc_mona", "gate_open") and not DL:hasItem("gold", 100)) then 

		DL:createNPCNode(6, -2, "DL_Mona_OpenGate") -- You don't have enough gold.
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


	DL:createTradeNode(4, -2)
	DL:addNode()

end