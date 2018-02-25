-- Dialogue for NPC "npc_steven"
loadDialogue = function(DL) 

	if (not DL:isConditionFulfilled("npc_steven", "talked")) then 
		DL:setRoot(1) 
	elseif (not DL:isConditionFulfilled("npc_steven", "lloyd")) then 
		DL:setRoot(2) 
	else 
		DL:setRoot(18) 
	end 

	if (not DL:isConditionFulfilled("npc_steven", "talked")) then 

		DL:createNPCNode(1, 3, "DL_Steven_ANewFace") -- And again, a new face in the city. As if we hadn't enough troublemakers here already. 
		DL:addConditionProgress("npc_steven", "talked")
		DL:addNode()


		DL:createNPCNode(3, -2, "DL_Steven_YouShouldKnowRules") -- You should know the rules of the city, if you don't want to end up jailed.
		DL:addNode()

	end

	if (not DL:isConditionFulfilled("npc_steven", "lloyd")) then 

		DL:createChoiceNode(2)
		if (not DL:isConditionFulfilled("npc_steven", "rules")) then 
			DL:addChoice(5, "DL_Choice_ExplainRules") -- Explain the rules of the city to me.
		end
		if (not DL:isConditionFulfilled("npc_steven", "who_are_you")) then 
			DL:addChoice(6, "DL_Choice_WhoAreYou") -- Who are you?
		end
		if (not DL:isConditionFulfilled("npc_steven", "rules")) then 
			DL:addChoice(4, "DL_Choice_NoTime") -- I don't have time for this.
		end
		if (DL:isConditionFulfilled("npc_steven", "who_are_you") and DL:hasItem("qe_recommendationletter", 1)) then 
			DL:addChoice(12, "DL_Choice_Lloyd") -- I need to talk to some "Lloyd" here... (Show the letter)
		end
		if (DL:isConditionFulfilled("npc_steven", "rules")) then 
			DL:addChoice(-1, "DL_Choice_CU") -- See you.
		end
		DL:addNode()

		if (not DL:isConditionFulfilled("npc_steven", "rules")) then 

			DL:createNPCNode(5, 9, "DL_Steven_Rules1") -- Well, first, you should know that the Order of the Eternal Light is in charge here. Therefore; respect us and follow our orders, or you won't have a very pleasant stay in Gandria.
			DL:addConditionProgress("npc_steven", "rules")
			DL:addNode()


			DL:createNPCNode(9, 10, "DL_Steven_Rules2") -- Behave well if you go into a stranger's house - we don't like thieves here. Most citizens of Gandria use observer spells to avoid being robbed.
			DL:addNode()


			DL:createNPCNode(10, 11, "DL_Steven_Rules3") -- And, last but not least: We don't like to see other spells than those which originate from the high divine art.
			DL:addNode()


			DL:createNPCNode(11, 64, "DL_Steven_Rules4") -- You will hang for practicing Necromancy.
			DL:addNode()


			DL:createCendricNode(64, -2, "DL_Cendric_Okay") -- Okay.
			DL:addNode()

		end

		if (not DL:isConditionFulfilled("npc_steven", "who_are_you")) then 

			DL:createNPCNode(6, 7, "DL_Steven_WhoAreYou") -- I'm Steven, paladin and vigilante of Gandria. I am, so to speak, the right hand of our Commander, Lloyd. Don't mess with me or my city, understood?
			DL:addConditionProgress("npc_steven", "who_are_you")
			DL:addNode()


			DL:createChoiceNode(7)
			DL:addChoice(-2, "DL_Choice_Understood") -- Understood.
			DL:addChoice(-2, "DL_Choice_MaybeUnderstood") -- I'll try.
			DL:addChoice(8, "DL_Choice_NotUnderstood") -- ... (Mischievous grin)
			DL:addNode()


			DL:createNPCNode(8, -1, "DL_Steven_IKeepEyeOnYou") -- I'll keep an eye on you, boy.
			DL:addNode()

		end

		if (not DL:isConditionFulfilled("npc_steven", "rules")) then 

			DL:createNPCNode(4, -1, "DL_Steven_DontTrouble") -- Well then... Don't make trouble!
			DL:addNode()

		end

		if (DL:isConditionFulfilled("npc_steven", "who_are_you") and DL:hasItem("qe_recommendationletter", 1)) then 

			DL:createNPCNode(12, 13, "DL_Steven_Laughter") -- (Steven takes the letter and then starts laughing uncontrollably)
			DL:removeItem("qe_recommendationletter", 1)
			DL:addConditionProgress("npc_steven", "lloyd")
			DL:addNode()


			DL:createNPCNode(13, 14, "DL_Steven_PieceOfPaper") -- And you thought this piece of paper will bring you to him? That's truly ridiculous.
			DL:addNode()


			DL:createChoiceNode(14)
			DL:addChoice(15, "DL_Choice_Urgent") -- But it's urgent!
			DL:addChoice(16, "DL_Choice_Inina") -- But this letter is from the High Priestess Inina!
			DL:addChoice(17, "DL_Choice_Intimidate") -- Just let me to him and no one gets hurt.
			DL:addNode()


			DL:createNPCNode(15, -2, "DL_Steven_Urgent") -- Sure it is. But there are other urgent things too. Like what you'll have to do to get your letter back.
			DL:addNode()


			DL:createNPCNode(16, -2, "DL_Steven_Inina") -- Good for you. But you can't talk to her either until you've taken care of my little problem. Then you'll get your letter back.
			DL:addNode()


			DL:createNPCNode(17, -2, "DL_Steven_Intimidate") -- Ah, you're a tough boy aren't you. But I think I'll have to keep your letter for a while - until you've taken care of some problem of mine...
			DL:addNode()

		end

		if (DL:isConditionFulfilled("npc_steven", "rules")) then 
		end

	end


	DL:createChoiceNode(18)
	if (DL:isQuestState("stevens_helmet", "void")) then 
		DL:addChoice(19, "DL_Choice_TellmeQuest") -- (Sigh) Okay, what should I do for you?
	end
	if (DL:isQuestState("stevens_helmet", "started") and DL:isQuestComplete("stevens_helmet")) then 
		DL:addChoice(21, "DL_Choice_GotHelmet") -- I got your helmet, now give me that letter back.
	end
	if (not DL:isQuestState("antimagic_crystal", "void") and not DL:isConditionFulfilled("npc_steven", "crystal_buy_unlock") and not DL:isConditionFulfilled("npc_steven", "crystal_buy_lock")) then 
		DL:addChoice(30, "DL_Choice_UnlockKeyBuy") -- I'm looking for an anti-magic crystal...
	end
	if (not DL:isQuestState("antimagic_crystal", "void") and DL:isConditionFulfilled("npc_steven", "crystal_buy_unlock") and not DL:isConditionFulfilled("npc_steven", "crystal_bought") and DL:hasItem("gold", 100)) then 
		DL:addItemChoice(35, "DL_Choice_BuyCrystal", "gold", 100) -- Sell me that crystal.
	end
	if (not DL:isQuestState("antimagic_crystal", "void") and not DL:isConditionFulfilled("npc_steven", "key_stolen")) then 
		DL:addChoice(29, "DL_Choice_StartKey") -- (Start an attempt to steal Steven's tower key)
	end
	DL:addChoice(-1, "") -- 
	DL:addNode()

	if (DL:isQuestState("stevens_helmet", "void")) then 

		DL:createNPCNode(19, 20, "DL_Steven_HelmetQuest") -- I got a little bit drunk last night and lost my helmet to some shady guys in the tavern.
		DL:changeQuestState("stevens_helmet", "started")
		DL:addNode()


		DL:createNPCNode(20, 65, "DL_Steven_HelmetQuest2") -- I'm pretty sure that they belong to the dirty thief guild.
		DL:addNode()


		DL:createNPCNode(65, 66, "DL_Steven_HelmetQuest3") -- I need someone that doesn't look like they belongs to the order, someone like you, to get my helmet back.
		DL:addNode()


		DL:createNPCNode(66, -2, "DL_Steven_HelmetQuest4") -- If you succeed, I'll give you your piece of paper back.
		DL:addNode()

	end

	if (DL:isQuestState("stevens_helmet", "started") and DL:isQuestComplete("stevens_helmet")) then 

		DL:createNPCNode(21, 22, "DL_Steven_GotHelmet") -- Very good! Give it to me!
		DL:addNode()


		DL:createChoiceNode(22)
		DL:addChoice(23, "DL_Choice_FirstLetter") -- Give me the letter first.
		DL:addChoice(27, "DL_Choice_GiveHelmet") -- Okay, here you go.
		DL:addNode()


		DL:createNPCNode(23, 24, "DL_Steven_GivesLetter") -- Okay okay, here you go. You can speak with Commander Lloyd. And now give me my helmet!
		DL:addItem("qe_recommendationletter", 1)
		DL:addConditionProgress("default", "barracks_open")
		DL:changeQuestState("stevens_helmet", "completed")
		DL:addNode()


		DL:createChoiceNode(24)
		DL:addChoice(28, "DL_Choice_GiveHelmet") -- 
		DL:addChoice(25, "DL_Choice_NoHelmet") -- You wish.
		DL:addNode()


		DL:createNPCNode(28, -1, "DL_Steven_ThanksHelmet") -- Hehe. Thank you.
		DL:addReputationProgress("cleric", 5)
		DL:removeItem("eq_stevenhelmet", 1)
		DL:addNode()


		DL:createNPCNode(25, 26, "DL_Steven_Angry") -- Grr, you little... You'll pay for this! If Lloyd hears about this...
		DL:addReputationProgress("thief", 5)
		DL:addNode()


		DL:createChoiceNode(26)
		DL:addChoice(-1, "DL_Choice_CommanderLloyd") -- It's "Commander" Lloyd.
		DL:addChoice(-1, "DL_Choice_DontDrinkMuch") -- Well, it's your fault if you drink too much.
		DL:addNode()


		DL:createNPCNode(27, -1, "DL_Steven_GotoLloyd") -- Perfect. Here is your letter. You can now go and see Commander Lloyd.
		DL:addItem("qe_recommendationletter", 1)
		DL:addConditionProgress("default", "barracks_open")
		DL:removeItem("eq_stevenhelmet", 1)
		DL:addReputationProgress("cleric", 5)
		DL:changeQuestState("stevens_helmet", "completed")
		DL:addNode()

	end

	if (not DL:isQuestState("antimagic_crystal", "void") and not DL:isConditionFulfilled("npc_steven", "crystal_buy_unlock") and not DL:isConditionFulfilled("npc_steven", "crystal_buy_lock")) then 

		DL:createNPCNode(30, 31, "DL_Steven_UnlockKeyBuy") -- You're lucky. Actually, I do have one here... but it's not cheap. Why do you need one anyway?
		DL:addNode()


		DL:createChoiceNode(31)
		DL:addChoice(32, "DL_Choice_CrystalForCurse") -- I want to break a curse with it.
		DL:addChoice(33, "DL_Choice_CrystalProtection") -- I need protection against necromancy.
		DL:addChoice(34, "DL_Choice_CrystalNoneOf") -- That's none of your business.
		DL:addNode()


		DL:createNPCNode(32, -2, "DL_Steven_CrystalBuyOk") -- 
		DL:addConditionProgress("npc_steven", "crystal_buy_unlock")
		DL:addNode()


		DL:createNPCNode(33, -2, "DL_Steven_CrystalBuyOk") -- That's a good reason, indeed. If you want, I can sell one to you for 100 gold.
		DL:addConditionProgress("npc_steven", "crystal_buy_unlock")
		DL:addNode()


		DL:createNPCNode(34, -2, "DL_Steven_CrystalBuyNOk") -- Hmpf! I don't think I can sell you one then. Go ask someone else.
		DL:addConditionProgress("npc_steven", "crystal_buy_lock")
		DL:addNode()

	end

	if (not DL:isQuestState("antimagic_crystal", "void") and DL:isConditionFulfilled("npc_steven", "crystal_buy_unlock") and not DL:isConditionFulfilled("npc_steven", "crystal_bought") and DL:hasItem("gold", 100)) then 

		DL:createNPCNode(35, -2, "DL_Steven_BuyCrystal") -- Hehehe. You made a really good deal today!
		DL:removeGold(100)
		DL:addItem("qe_antimagiccrystal", 1)
		DL:addConditionProgress("npc_steven", "crystal_bought")
		DL:addReputationProgress("cleric", 5)
		DL:addNode()

	end

	if (not DL:isQuestState("antimagic_crystal", "void") and not DL:isConditionFulfilled("npc_steven", "key_stolen")) then 

		DL:createChoiceNode(29)
		DL:addChoice(36, "DL_Choice_StealBird") -- Hey, look! A bird!
		DL:addChoice(38, "DL_Choice_StealTellMe") -- Tell me something about yourself.
		DL:addChoice(39, "DL_Choice_StealEternal") -- So, what exactly is that "Eternal Light"?
		DL:addChoice(-1, "") -- 
		DL:addNode()


		DL:createNPCNode(36, 37, "DL_Steven_StealBird") -- Ehh... What? We're in the barracks here!
		DL:addNode()


		DL:createChoiceNode(37)
		DL:addChoice(40, "DL_Choice_TryStealBird2") -- But it just flew through that door!
		DL:addChoice(41, "DL_Choice_TrySteal") -- (Try to steal Steven's tower key)
		DL:addNode()


		DL:createNPCNode(40, 42, "DL_Steven_TryStealBird2") -- Dude... are you kidding me?
		DL:addNode()


		DL:createCendricNode(42, -2, "DL_Cendric_FailSteal") -- 
		DL:addNode()


		DL:createCendricNode(41, -2, "DL_Cendric_FailSteal") -- (Your attempt to steal the tower key failed)
		DL:addNode()


		DL:createNPCNode(38, 43, "DL_Steven_StealTellMe") -- Why would I... ah, never mind. You must be an admirer of mine.
		DL:addNode()


		DL:createChoiceNode(43)
		DL:addChoice(45, "DL_Choice_OfCourseYouAre") -- Yes, of course!
		DL:addChoice(44, "DL_Choice_TrySteal") -- 
		DL:addNode()


		DL:createNPCNode(45, 46, "DL_Steven_StealTellMe2") -- Hehe. Now, listen. I'm currently one of the mightiest paladins in Gandria. I'm sure, Lloyd will chose me as his successor!
		DL:addNode()


		DL:createChoiceNode(46)
		DL:addChoice(47, "DL_Choice_IThinkYouAre") -- Yes, I fully understand.
		DL:addChoice(49, "DL_Choice_IThinkYouNot") -- You wish.
		DL:addChoice(48, "DL_Choice_TrySteal") -- 
		DL:addNode()


		DL:createNPCNode(47, 50, "DL_Steven_IThinkYouAre") -- Yes... The whole city will be under my command! (Laughs)
		DL:addNode()


		DL:createChoiceNode(50)
		DL:addChoice(51, "DL_Choice_WholeCityWill") -- Oh yes, it will!
		DL:addChoice(52, "DL_Choice_TrySteal") -- 
		DL:addNode()


		DL:createNPCNode(51, 53, "DL_Steven_WholeCityWill") -- Wait... you've never been that nice before...
		DL:addNode()


		DL:createCendricNode(53, -2, "DL_Cendric_FailSteal") -- 
		DL:addNode()


		DL:createCendricNode(52, -2, "DL_Cendric_SucceedSteal") -- (Your attempt to steal the tower key succeeded!)
		DL:addConditionProgress("npc_steven", "key_stolen")
		DL:addItem("ke_tower", 1)
		DL:addReputationProgress("thief", 5)
		DL:addNode()


		DL:createNPCNode(49, -1, "DL_Steven_IThinkYouNot") -- Hah! You'll see.
		DL:addNode()


		DL:createCendricNode(48, -2, "DL_Cendric_FailSteal") -- 
		DL:addNode()


		DL:createCendricNode(44, -2, "DL_Cendric_FailSteal") -- 
		DL:addNode()


		DL:createNPCNode(39, 54, "DL_Steven_StealEternal") -- It's the purest form of energy - and our source of magic.
		DL:addNode()


		DL:createChoiceNode(54)
		DL:addChoice(56, "DL_Choice_GoOn") -- (Go on...)
		DL:addChoice(55, "DL_Choice_TrySteal") -- 
		DL:addNode()


		DL:createNPCNode(56, 57, "DL_Steven_StealEternal2") -- It's the only magic that has it's own will. It chooses its mages.
		DL:addNode()


		DL:createChoiceNode(57)
		DL:addChoice(59, "DL_Choice_Unlucky") -- And the ones who are not chosen, are just unlucky?
		DL:addChoice(58, "DL_Choice_TrySteal") -- 
		DL:addNode()


		DL:createNPCNode(59, 60, "DL_Steven_Unlucky") -- They're unworthy, yes. But as long as you're a mage, you can still come back to the pure divine magic.
		DL:addNode()


		DL:createChoiceNode(60)
		DL:addChoice(62, "DL_Choice_HowDivine") -- And how does that work?
		DL:addChoice(61, "DL_Choice_TrySteal") -- 
		DL:addNode()


		DL:createNPCNode(62, 63, "DL_Steven_HowDivine") -- Hm... so many questions. Are you trying to distract me? 
		DL:addNode()


		DL:createCendricNode(63, -2, "DL_Cendric_FailSteal") -- 
		DL:addNode()


		DL:createCendricNode(61, -2, "DL_Cendric_SucceedSteal") -- 
		DL:addConditionProgress("npc_steven", "key_stolen")
		DL:addItem("ke_tower", 1)
		DL:addReputationProgress("thief", 5)
		DL:addNode()


		DL:createCendricNode(58, -2, "DL_Cendric_FailSteal") -- 
		DL:addNode()


		DL:createCendricNode(55, -2, "DL_Cendric_FailSteal") -- 
		DL:addNode()

	end

end