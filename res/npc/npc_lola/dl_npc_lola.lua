-- Dialogue for NPC "npc_lola"
loadDialogue = function(DL) 

	if (not DL:isConditionFulfilled("npc_mona", "sex_romantic")) then 
		DL:setRoot(1) 
	elseif (not DL:isConditionFulfilled("npc_lola", "talk")) then 
		DL:setRoot(2) 
	elseif (not DL:isConditionFulfilled("npc_lola", "question_1")) then 
		DL:setRoot(4) 
	elseif (not DL:isConditionFulfilled("npc_lola", "question_2")) then 
		DL:setRoot(11) 
	elseif (not DL:isConditionFulfilled("npc_lola", "question_3")) then 
		DL:setRoot(17) 
	elseif (not DL:isConditionFulfilled("npc_lola", "sex")) then 
		DL:setRoot(25) 
	elseif (not DL:isConditionFulfilled("npc_lola", "after_sex")) then 
		DL:setRoot(29) 
	else 
		DL:setRoot(30) 
	end 

	if (not DL:isConditionFulfilled("npc_mona", "sex_romantic")) then 

		DL:createNPCNode(1, -1, "DL_Lola_NotBought") -- Talk to Mona first if you want to have some fun, cutie.
		DL:addNode()

	end

	if (not DL:isConditionFulfilled("npc_lola", "talk")) then 

		DL:createChoiceNode(2)
		if (not DL:isConditionFulfilled("npc_lola", "sex_lola")) then 
			DL:addChoice(3, "DL_Choice_Hey") -- Hey...
		end
		DL:addChoice(-1, "DL_Choice_End") -- See you.
		DL:addNode()

		if (not DL:isConditionFulfilled("npc_lola", "sex_lola")) then 

			DL:createNPCNode(3, -2, "DL_Lola_SexyBoy") -- Hey, handsome. Why don't you come in and we talk a bit?
			DL:addConditionProgress("npc_lola", "talk")
			DL:addNode()

		end

	end

	if (not DL:isConditionFulfilled("npc_lola", "question_1")) then 

		DL:createChoiceNode(4)
		if (not DL:isConditionFulfilled("npc_lola", "no_talk")) then 
			DL:addChoice(5, "DL_Choice_NoTalk") -- I'm not here to talk.
		end
		if (DL:isConditionFulfilled("npc_lola", "no_talk") and not DL:isConditionFulfilled("npc_lola", "angry") ) then 
			DL:addChoice(7, "DL_Choice_Angry") -- I thought this was a brothel, not a tea party.
		end
		DL:addChoice(6, "DL_Choice_OkTalk") -- What do you want to talk about?
		DL:addChoice(-1, "") -- 
		DL:addNode()

		if (not DL:isConditionFulfilled("npc_lola", "no_talk")) then 

			DL:createNPCNode(5, -1, "DL_Lola_NoTalk") -- You are in the wrong room then. Come back if you change your mind.
			DL:addConditionProgress("npc_lola", "no_talk")
			DL:addNode()

		end

		if (DL:isConditionFulfilled("npc_lola", "no_talk") and not DL:isConditionFulfilled("npc_lola", "angry") ) then 

			DL:createNPCNode(7, -2, "DL_Lola_Angry") -- Oh, it is. But love is not always about in-out-in-out, you know.
			DL:addConditionProgress("npc_lola", "angry")
			DL:addNode()

		end


		DL:createNPCNode(6, 8, "DL_Lola_OkTalk") -- Let's talk about you. Why are you here?
		DL:addConditionProgress("npc_lola", "question_1")
		DL:addNode()


		DL:createChoiceNode(8)
		DL:addChoice(9, "DL_Choice_Curious") -- I was just curious.
		DL:addChoice(10, "DL_Choice_IWasHorny") -- Well, to be honest, I'm a bit horny.
		DL:addChoice(12, "DL_Choice_LookingForLove") -- I'm looking for love.
		DL:addNode()


		DL:createNPCNode(9, -2, "DL_Lola_Curious") -- Most of you are. I hope you'll enjoy your stay here. (Smiles)
		DL:addNode()


		DL:createNPCNode(10, -2, "DL_Lola_IWasHorny") -- You'll get your money's worth, promised. (Winks)
		DL:addNode()


		DL:createNPCNode(12, -2, "DL_Lola_LookingForLove") -- Maybe you'll find it here. Depends on the kind of love you're looking for. (Smiles)
		DL:addNode()

	end

	if (not DL:isConditionFulfilled("npc_lola", "question_2")) then 

		DL:createNPCNode(11, 13, "DL_Lola_SecondQuestion") -- Now, another question. Who are you?
		DL:addConditionProgress("npc_lola", "question_2")
		DL:addNode()


		DL:createChoiceNode(13)
		DL:addChoice(14, "DL_Choice_NoneOfYourBusiness") -- That's none of your business.
		DL:addChoice(15, "DL_Choice_Adventurer") -- I'm a lonesome adventurer.
		DL:addChoice(16, "DL_Choice_NoOne") -- I don't really know that myself.
		DL:addNode()


		DL:createNPCNode(14, -2, "DL_Lola_NoneOfYourBusiness") -- Oh... okay.
		DL:addNode()


		DL:createNPCNode(15, -2, "DL_Lola_Adventurer") -- Hn. I love this kind of men.
		DL:addNode()


		DL:createNPCNode(16, -2, "DL_Lola_NoOne") -- Hm. I feel the same from time to time. But finding yourself is just part of life.
		DL:addNode()

	end

	if (not DL:isConditionFulfilled("npc_lola", "question_3")) then 

		DL:createNPCNode(17, 18, "DL_Lola_FavGirl") -- Okay, last question for you: What makes a girl attractive for you?
		DL:addConditionProgress("npc_lola", "question_3")
		DL:addNode()


		DL:createChoiceNode(18)
		DL:addChoice(19, "DL_Choice_GirlAttractive") -- A beautiful body. A nice butt, big boobs, a pretty face.
		DL:addChoice(21, "DL_Choice_GirlCook") -- I love it when a girl can cook.
		DL:addChoice(20, "DL_Choice_GirlSmart") -- A girl should be smart and speak her mind.
		DL:addChoice(22, "DL_Choice_GirlDontCare") -- I don't care. As long as it's a girl.
		DL:addChoice(23, "DL_Choice_GirlReallyDontCare") -- I really don't care. I don't think about such things.
		DL:addChoice(24, "DL_Choice_Gay") -- Well... actually... I think I'm gay.
		DL:addNode()


		DL:createNPCNode(19, -2, "DL_Lola_GirlAttractive") -- Mhm. Important, but not everything.
		DL:addNode()


		DL:createNPCNode(21, -2, "DL_Lola_GirlCook") -- The way to a man's heart is through his stomach.
		DL:addNode()


		DL:createNPCNode(20, -2, "DL_Lola_GirlSmart") -- Very important for a long-lasting relationship.
		DL:addNode()


		DL:createNPCNode(22, -2, "DL_Lola_GirlDontCare") -- (Lola giggles) Yes, yes. We don't have guys here, there's too little demand.
		DL:addNode()


		DL:createNPCNode(23, -2, "DL_Lola_GirlReallyDontCare") -- What a pity.
		DL:addNode()


		DL:createNPCNode(24, -2, "DL_Lola_Gay") -- Oh... really? Well, we can end this here then. I don't want to force you to anything.
		DL:addNode()

	end

	if (not DL:isConditionFulfilled("npc_lola", "sex")) then 

		DL:createChoiceNode(25)
		DL:addChoice(26, "DL_Choice_QuestionsForYou") -- Can *I* ask you some questions now?
		DL:addChoice(27, "DL_Choice_WantSex") -- I want to have some fun with you, now.
		DL:addChoice(-1, "DL_Choice_Enough") -- That's enough. I'm out.
		DL:addNode()


		DL:createNPCNode(26, 32, "DL_Lola_QuestionsForYou") -- Sure. What do you want to know?
		DL:addNode()


		DL:createChoiceNode(32)
		if (not DL:isConditionFulfilled("npc_lola", "room")) then 
			DL:addChoice(33, "DL_Choice_Room") -- Why is this room full of roses?
		end
		if (not DL:isConditionFulfilled("npc_lola", "job")) then 
			DL:addChoice(34, "DL_Choice_Job") -- Are you happy with your job?
		end
		if (not DL:isConditionFulfilled("npc_lola", "attractive")) then 
			DL:addChoice(35, "DL_Choice_Attractive") -- Do you think I'm attractive?
		end
		if (not DL:isConditionFulfilled("npc_lola", "turn_on")) then 
			DL:addChoice(36, "DL_Choice_TurnOn") -- What turns *you* on?
		end
		DL:addChoice(-2, "DL_Choice_Back") -- That's enough for now. [BACK]
		DL:addNode()

		if (not DL:isConditionFulfilled("npc_lola", "room")) then 

			DL:createNPCNode(33, -2, "DL_Lola_Room") -- It's the rose-room. Some guests always want to come here for a romantic time.
			DL:addConditionProgress("npc_lola", "room")
			DL:gotoNode(32)
			DL:addNode()

		end

		if (not DL:isConditionFulfilled("npc_lola", "job")) then 

			DL:createNPCNode(34, -2, "DL_Lola_Job") -- Most of the time, yes. But this is not something I'll do for my entire life.
			DL:addConditionProgress("npc_lola", "job")
			DL:addNode()

		end

		if (not DL:isConditionFulfilled("npc_lola", "attractive")) then 

			DL:createNPCNode(35, -2, "DL_Lola_Attractive") -- Yes, of course! I love your hair.
			DL:addConditionProgress("npc_lola", "attractive")
			DL:addNode()

		end

		if (not DL:isConditionFulfilled("npc_lola", "turn_on")) then 

			DL:createNPCNode(36, -2, "DL_Lola_TurnOn") -- Many things. I really love when the guy enjoys it and I can see it.
			DL:addConditionProgress("npc_lola", "turn_on")
			DL:addNode()

		end


		DL:createNPCNode(27, 28, "DL_Lola_WantSex") -- I won't make you wait any longer. (Lola pulls you closer...)
		DL:addNode()


		DL:createNPCNode(28, -1, "") -- 
		DL:startCutscene("sex_lola")
		DL:addConditionProgress("npc_lola", "sex")
		DL:addNode()

	end

	if (not DL:isConditionFulfilled("npc_lola", "after_sex")) then 

		DL:createNPCNode(29, 31, "DL_Lola_AfterSex") -- Mhm... that was good.
		DL:addConditionProgress("npc_lola", "after_sex")
		DL:addNode()


		DL:createNPCNode(31, -2, "DL_Lola_AfterSex2") -- Here, take this rose with you, as a souvenir. (Winks)
		DL:addItem("mi_rose", 1)
		DL:addNode()

	end


	DL:createNPCNode(30, -2, "DL_Lola_End") -- Be safe, cutie.
	DL:addNode()

end