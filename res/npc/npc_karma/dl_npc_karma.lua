-- Dialogue for NPC "npc_karma"
loadDialogue = function(DL) 

	if (not DL:isConditionFulfilled("npc_mona", "sex_exotic")) then 
		DL:setRoot(1) 
	elseif (not DL:isConditionFulfilled("npc_karma", "kamasutra")) then 
		DL:setRoot(2) 
	elseif (not DL:isConditionFulfilled("npc_karma", "started")) then 
		DL:setRoot(7) 
	elseif (not DL:isConditionFulfilled("npc_karma", "sex")) then 
		DL:setRoot(4) 
	elseif (not DL:isConditionFulfilled("npc_karma", "after_sex")) then 
		DL:setRoot(34) 
	else 
		DL:setRoot(35) 
	end 

	if (not DL:isConditionFulfilled("npc_mona", "sex_exotic")) then 

		DL:createNPCNode(1, -1, "DL_Karma_NotBought") -- Go and ask Mona if you want to have an exciting time with me. (Winks)
		DL:addNode()

	end

	if (not DL:isConditionFulfilled("npc_karma", "kamasutra")) then 

		DL:createChoiceNode(2)
		if (not DL:isConditionFulfilled("npc_karma", "kamasutra")) then 
			DL:addChoice(3, "DL_Choice_Hey") -- Hey...
		end
		DL:addChoice(-1, "DL_Choice_End") -- See you.
		DL:addNode()

		if (not DL:isConditionFulfilled("npc_karma", "kamasutra")) then 

			DL:createNPCNode(3, -2, "DL_Karma_Kamasutra") -- Hello, cutie. You're here to learn something about the art of Kama Sutra, I suppose?
			DL:addConditionProgress("npc_karma", "kamasutra")
			DL:addNode()

		end

	end

	if (not DL:isConditionFulfilled("npc_karma", "started")) then 

		DL:createChoiceNode(7)
		DL:addChoice(8, "DL_Choice_KamasutraTellMe") -- Kama Sutra? Tell me more about it.
		DL:addChoice(9, "DL_Choice_NotLearnPractice") -- I'm not here to learn - I want to practice!
		DL:addChoice(-1, "DL_Choice_WrongRoom") -- Oh... I must be in the wrong room. 
		DL:addNode()


		DL:createNPCNode(8, -2, "DL_Karma_KamasutraTellMe") -- Of course. Listen carefully.
		DL:addConditionProgress("npc_karma", "started")
		DL:gotoNode(5)
		DL:addNode()


		DL:createNPCNode(9, 19, "DL_Karma_NotLearnPractice") -- Patience, young lad! You cannot just jump to the last chapter without knowing the others.
		DL:addConditionProgress("npc_karma", "started")
		DL:addNode()


		DL:createChoiceNode(19)
		DL:addChoice(20, "DL_Choice_IKnowEverything") -- I know the other chapters!
		DL:addChoice(21, "DL_Choice_IKnowNothing") -- Okay, tell me more about it, then.
		DL:addNode()


		DL:createNPCNode(20, -2, "DL_Karma_IKnowEverything") -- If you say so, you surely can answer me three questions to my satisfaction...
		DL:gotoNode(18)
		DL:addNode()


		DL:createNPCNode(21, -2, "DL_Karma_KamasutraTellMe") -- 
		DL:gotoNode(5)
		DL:addNode()

	end

	if (not DL:isConditionFulfilled("npc_karma", "sex")) then 

		DL:createChoiceNode(4)
		DL:addChoice(5, "DL_Choice_ExplainKamasutra") -- Explain the art of Kama Sutra to me.
		DL:addChoice(18, "DL_Choice_GoQuestions") -- I'm ready for your questions.
		DL:addChoice(-1, "") -- 
		DL:addNode()


		DL:createNPCNode(5, 6, "DL_Karma_ExplainKamasutra") -- Good. The Kamasutra consists of seven parts. It's a guide to the art of love.
		DL:addNode()


		DL:createChoiceNode(6)
		DL:addChoice(10, "DL_Choice_GoOn") -- (Go on...)
		DL:addChoice(-2, "DL_Choice_HeardEnough") -- I've heard enough.
		DL:addNode()


		DL:createNPCNode(10, 11, "DL_Karma_ExplainKamasutra2") -- The most famous chapter focuses on sexual union, but this is only a fifth of the book.
		DL:addNode()


		DL:createChoiceNode(11)
		DL:addChoice(12, "DL_Choice_GoOn") -- 
		DL:addChoice(-2, "DL_Choice_HeardEnough") -- 
		DL:addNode()


		DL:createNPCNode(12, 13, "DL_Karma_ExplainKamasutra3") -- The book also covers the theory of love and describes what triggers desire and how to improve one's own physical attraction.
		DL:addNode()


		DL:createChoiceNode(13)
		DL:addChoice(14, "DL_Choice_GoOn") -- 
		DL:addChoice(-2, "DL_Choice_HeardEnough") -- 
		DL:addNode()


		DL:createNPCNode(14, 15, "DL_Karma_ExplainKamasutra4") -- In addition to the common arts of dancing, drawing, singing, massaging and hairdressing; magical tricks or how to teach a parrot to speak also impress a vis-à-vis.
		DL:addNode()


		DL:createChoiceNode(15)
		DL:addChoice(16, "DL_Choice_GoOn") -- 
		DL:addChoice(-2, "DL_Choice_HeardEnough") -- 
		DL:addNode()


		DL:createNPCNode(16, 17, "DL_Karma_ExplainKamasutra5") -- There's also a short introduction on love practices like "Milk and Water" or "Sesame and Rice".
		DL:addNode()


		DL:createNPCNode(17, -2, "DL_Karma_ExplainKamasutra6") -- This was now a short summary. To check whether you were attentive, I got some questions for you.
		DL:gotoNode(18)
		DL:addNode()


		DL:createNPCNode(18, 22, "DL_Karma_FirstQuestion") -- What is "Milk and Water"?
		DL:addNode()


		DL:createChoiceNode(22)
		DL:addChoice(23, "DL_Choice_FirstQuestion1") -- Something to drink.
		DL:addChoice(24, "DL_Choice_FirstQuestion2") -- A love practice.
		DL:addChoice(25, "DL_Choice_FirstQuestion3") -- Ingredients for a love potion.
		DL:addNode()


		DL:createNPCNode(23, -1, "DL_Karma_WrongQuestion1") -- You must be tired. Come again when you're able to concentrate!
		DL:addNode()


		DL:createNPCNode(24, 26, "DL_Karma_CorrectQuestion1") -- Good. You passed the first test. Now on to the second question: What's the Kama Sutra about?
		DL:addNode()


		DL:createChoiceNode(26)
		DL:addChoice(27, "DL_Choice_SecondQuestion1") -- It is a book that describes sexual practices with nice pictures.
		DL:addChoice(28, "DL_Choice_SecondQuestion2") -- A seven-piece guide about the art of love.
		DL:addNode()


		DL:createNPCNode(27, -1, "DL_Karma_WrongQuestion2") -- No, even if that's what most people believe, it's not.
		DL:addNode()


		DL:createNPCNode(28, 29, "DL_Karma_CorrectQuestion2") -- Good. And now my last question: What can be mastered to improve one's sexual attractiveness?
		DL:addNode()


		DL:createChoiceNode(29)
		DL:addChoice(30, "DL_Choice_ThirdQuestion1") -- Magical tricks and how to teach a parrot to speak.
		DL:addChoice(31, "DL_Choice_ThirdQuestion2") -- Looking as beautiful as me with my blonde hair.
		DL:addChoice(32, "DL_Choice_ThirdQuestion3") -- Knowing different sex positions.
		DL:addNode()


		DL:createNPCNode(30, 33, "DL_Karma_ThirdQuestion1") -- Very good. As you now know some basics of the Kama Sutra, we can now try out "Milk and Water".
		DL:addNode()


		DL:createNPCNode(33, -1, "") -- 
		DL:startCutscene("sex_karma")
		DL:addConditionProgress("npc_karma", "sex")
		DL:addNode()


		DL:createNPCNode(31, -1, "DL_Karma_ThirdQuestion2") -- (Sighs) No... looks are not everything.
		DL:addNode()


		DL:createNPCNode(32, -1, "DL_Karma_ThirdQuestion3") -- You should have listened more carefully.
		DL:addNode()


		DL:createNPCNode(25, -1, "DL_Karma_WrongQuestion1") -- 
		DL:addNode()

	end

	if (not DL:isConditionFulfilled("npc_karma", "after_sex")) then 

		DL:createNPCNode(34, 36, "DL_Karma_SexDone") -- Wow. That was incredible. What's the theory without the practice, eh. (Smiles)
		DL:addConditionProgress("npc_karma", "after_sex")
		DL:addNode()


		DL:createNPCNode(36, -2, "DL_Karma_SexDone2") -- Here, take this book as a gift. It will show you how to get even better.
		DL:addItem("do_kamasutra", 1)
		DL:addNode()

	end


	DL:createNPCNode(35, -1, "DL_Karma_AfterSex") -- See you later, honey. 
	DL:addNode()

end