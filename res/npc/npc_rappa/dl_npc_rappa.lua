-- Dialogue for NPC "npc_rappa"
loadDialogue = function(DL) 

	if (not DL:isConditionFulfilled("npc_rappa", "stage1")) then 
		DL:setRoot(1) 
	elseif (not DL:isConditionFulfilled("npc_rappa", "rap_ok") and not DL:isConditionFulfilled("npc_rappa", "rap_nok")) then 
		DL:setRoot(3) 
	else 
		DL:setRoot(20) 
	end 

	if (not DL:isConditionFulfilled("npc_rappa", "stage1")) then 

		DL:createChoiceNode(1)
		DL:addChoice(2, "DL_Choice_Hey") -- Excuse me...
		DL:addChoice(-1, "") -- 
		DL:addNode()


		DL:createNPCNode(2, -2, "DL_Rappa_StopIt") -- Stop screaming, it hurts my ears, can't you see?
		DL:addConditionProgress("npc_rappa", "stage1")
		DL:addNode()

	end

	if (not DL:isConditionFulfilled("npc_rappa", "rap_ok") and not DL:isConditionFulfilled("npc_rappa", "rap_nok")) then 

		DL:createChoiceNode(3)
		DL:addChoice(5, "DL_Choice_ItWasLoud") -- It was loud, I agree.
		DL:addChoice(4, "DL_Choice_WhoAreYou") -- And you are ...?
		DL:addChoice(16, "DL_Choice_Bye") -- Bye, I'll leave you alone.
		DL:addNode()


		DL:createNPCNode(5, 6, "DL_Rappa_FinallyFree") -- You can be lucky you're finally free!
		DL:addNode()


		DL:createChoiceNode(6)
		DL:addChoice(7, "DL_Choice_YoureConfused") -- Ehm... I'll leave - you seem to be confused...
		DL:addChoice(8, "DL_Choice_ISee") -- You talk about screaming and being free, you're talking about...
		DL:addChoice(12, "DL_Choice_IKnowTheGame") -- As free as I can be.
		DL:addNode()


		DL:createNPCNode(7, -1, "DL_Rappa_YoureConfused") -- Then go! You see, I'm not amused.
		DL:addConditionProgress("npc_rappa", "rap_nok")
		DL:addNode()


		DL:createChoiceNode(8)
		DL:addChoice(9, "DL_Choice_APlay") -- ... a play!
		DL:addChoice(10, "DL_Choice_APrison") -- ... a prison?
		DL:addChoice(11, "DL_Choice_ABird") -- ... a bird?
		DL:addNode()


		DL:createNPCNode(9, -1, "DL_Rappa_APlay") -- Ah, you're too dumb. I'm not gonna stay!
		DL:addConditionProgress("npc_rappa", "rap_nok")
		DL:addNode()


		DL:createNPCNode(10, -1, "DL_NPC_ImAfraid") -- That reminds me of hell! I have to go - farewell!
		DL:addConditionProgress("npc_rappa", "rap_nok")
		DL:addNode()


		DL:createNPCNode(11, -1, "DL_Rappa_ABird") -- The bird is a word! The most stupid thing I've ever heard - that's why I leave, nerd!
		DL:addConditionProgress("npc_rappa", "rap_nok")
		DL:addNode()


		DL:createCendricNode(12, 13, "DL_Cendric_IKnowTheGame") -- See, I can talk in rhymes as well.
		DL:addNode()


		DL:createNPCNode(13, 14, "DL_Rappa_KnowTheGame") -- Yes, you're a smart one, I can tell. But you remind me of a place I don't wanna be.
		DL:addNode()


		DL:createNPCNode(14, -1, "DL_Rappa_KnowTheGame2") -- That scares me like hell - I'll leave, farewell!
		DL:addConditionProgress("npc_rappa", "rap_ok")
		DL:addNode()


		DL:createNPCNode(4, 15, "DL_Rappa_WhoAreYou") -- Carrots, celery, meat... no fish on this bar?
		DL:addNode()


		DL:createChoiceNode(15)
		DL:addChoice(17, "DL_Choice_RappaConfused") -- You seem to be a bit confused.
		DL:addChoice(18, "DL_Choice_CountStuff") -- You're counting the stuff in the shelf?
		DL:addChoice(19, "DL_Choice_Bizarre") -- Maybe there's something in that jar.
		DL:addNode()


		DL:createNPCNode(17, -1, "DL_Rappa_RappaConfused") -- I wanted to talk, but you refused.
		DL:addConditionProgress("npc_rappa", "rap_nok")
		DL:addNode()


		DL:createNPCNode(18, -1, "DL_Rappa_CountStuff") -- You're dumb, leave me to myself.
		DL:addConditionProgress("npc_rappa", "rap_nok")
		DL:addNode()


		DL:createNPCNode(19, -1, "DL_Rappa_Bizarre") -- I see, we're on par. Anyway, I miss some potato, I really really have to go.
		DL:addConditionProgress("npc_rappa", "rap_ok")
		DL:addNode()


		DL:createNPCNode(16, -1, "DL_Rappa_Bye") -- It's better like that, I should have known.
		DL:addConditionProgress("npc_rappa", "rap_nok")
		DL:addNode()

	end


	DL:createNPCNode(20, -1, "DL_Rappa_Shelf") -- (Rappa seems to be ordering the stuff in the shelf)
	DL:addNode()

end