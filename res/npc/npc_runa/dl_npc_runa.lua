-- Dialogue for NPC "npc_runa"
loadDialogue = function(DL) 

	if (not DL:isConditionFulfilled("npc_runa", "hi")) then 
		DL:setRoot(1) 
	elseif (not DL:isConditionFulfilled("npc_runa", "evil_mage")) then 
		DL:setRoot(3) 
	else 
		DL:setRoot(21) 
	end 

	if (not DL:isConditionFulfilled("npc_runa", "hi")) then 

		DL:createChoiceNode(1)
		if (not DL:isConditionFulfilled("npc_runa", "hi")) then 
			DL:addChoice(2, "DL_Choice_Hi") -- Hi.
		end
		DL:addChoice(-1, "") -- 
		DL:addNode()

		if (not DL:isConditionFulfilled("npc_runa", "hi")) then 

			DL:createNPCNode(2, -2, "DL_Runa_Hi") -- Oh, hello there! If you want to buy something from our farm, talk to my son Tristan.
			DL:addConditionProgress("npc_runa", "hi")
			DL:addNode()

		end

	end

	if (not DL:isConditionFulfilled("npc_runa", "evil_mage")) then 

		DL:createChoiceNode(3)
		if (not DL:isConditionFulfilled("npc_runa", "who_are_you")) then 
			DL:addChoice(4, "DL_Choice_WhoAreYou") -- Who are you?
		end
		if (not DL:isConditionFulfilled("npc_runa", "job")) then 
			DL:addChoice(10, "DL_Choice_Job") -- Do you have a job for me?
		end
		if (not DL:isConditionFulfilled("npc_runa", "sheep_done") and DL:isQuestState("monster_problem", "completed")) then 
			DL:addChoice(12, "DL_Choice_SheepDone") -- I took care of the sheep thief.
		end
		if (DL:isConditionFulfilled("npc_runa", "job") and not DL:isConditionFulfilled("npc_runa", "runas_deal")) then 
			DL:addChoice(13, "DL_Choice_Delivery") -- Tell me more about that delivery.
		end
		if (DL:isConditionFulfilled("npc_runa", "runas_deal") and DL:isQuestState("runas_deal", "void")) then 
			DL:addChoice(17, "DL_Choice_RunasDealStart") -- Okay, give me the Physalis, I'll take them to Bjarne.
		end
		if (DL:isConditionFulfilled("npc_runa", "runas_deal") and not DL:isConditionFulfilled("npc_runa", "who_bjarne")) then 
			DL:addChoice(16, "DL_Choice_WhoBjarne") -- Who is that "Bjarne"?
		end
		if (DL:isConditionFulfilled("npc_runa", "runas_deal") and not DL:isConditionFulfilled("npc_runa", "paladins")) then 
			DL:addChoice(30, "DL_Choice_Paladins") -- What is your problem with the paladins?
		end
		if (DL:isConditionFulfilled("npc_runa", "runas_deal") and not DL:isConditionFulfilled("npc_runa", "who_mage")) then 
			DL:addChoice(18, "DL_Choice_WhoMage") -- Is this Bjarne a mage?
		end
		if (DL:isConditionFulfilled("npc_runa", "runas_deal") and not DL:isConditionFulfilled("npc_runa", "pendant")) then 
			DL:addChoice(19, "DL_Choice_Pendant") -- What is this "precious pendant" exactly?
		end
		if (DL:isConditionFulfilled("npc_runa", "runas_deal") and not DL:isConditionFulfilled("npc_runa", "physalis")) then 
			DL:addChoice(20, "DL_Choice_Physalis") -- What exactly are "Physalis"?
		end
		if (DL:isConditionFulfilled("npc_runa", "pendant") and not DL:isConditionFulfilled("npc_runa", "why_protect")) then 
			DL:addChoice(22, "DL_Choice_WhyProtect") -- Why do you need protection against magic?
		end
		if (DL:isConditionFulfilled("npc_runa", "why_protect") and not DL:isConditionFulfilled("npc_runa", "paranoid")) then 
			DL:addChoice(26, "DL_Choice_Paranoid") -- I think, you're a bit too paranoid...
		end
		if (DL:isQuestState("runas_deal", "started") and DL:isQuestComplete("runas_deal")) then 
			DL:addChoice(27, "DL_Choice_GotPendant") -- I got your pendant. Here.
		end
		DL:addChoice(-1, "") -- 
		DL:addNode()

		if (not DL:isConditionFulfilled("npc_runa", "who_are_you")) then 

			DL:createNPCNode(4, 5, "DL_Runa_WhoAreYou") -- I'm Runa and I'm working as a farmer here. From time to time, I also help out in the River Inn.
			DL:addConditionProgress("npc_runa", "who_are_you")
			DL:addNode()


			DL:createNPCNode(5, 6, "DL_Runa_WhoAreYou2") -- And you? Are you just running around and ask everyone who they are?
			DL:addNode()


			DL:createChoiceNode(6)
			DL:addChoice(7, "DL_Choice_AskEveryoneYes") -- Yes...
			DL:addChoice(8, "DL_Choice_AskEveryoneNo") -- No.
			DL:addChoice(9, "DL_Choice_IJustWantedToTalk") -- I just wanted to have a little chat.
			DL:addNode()


			DL:createNPCNode(7, -2, "DL_Runa_AskEveryoneYes") -- Ah... the youth of today... as if there wasn't enough other work to do.
			DL:addNode()


			DL:createNPCNode(8, -2, "DL_Runa_AskEveryoneNo") -- You mean, I'm special? Hah, now I'm flattered.
			DL:addNode()


			DL:createNPCNode(9, -2, "DL_Runa_IJustWantedToTalk") -- Not everyone has time for a little chitchat. There's still so much work to be done!
			DL:addNode()

		end

		if (not DL:isConditionFulfilled("npc_runa", "job")) then 

			DL:createNPCNode(10, 11, "DL_Runa_Job") -- Hm. We do have a problem with disappearing sheep. If you want to help, talk to my husband, Ivo.
			DL:addConditionProgress("npc_runa", "job")
			DL:addNode()


			DL:createNPCNode(11, -2, "DL_Runa_Job2") -- Other than that, I could need some help with a delivery to Gandria. 
			DL:addNode()

		end

		if (not DL:isConditionFulfilled("npc_runa", "sheep_done") and DL:isQuestState("monster_problem", "completed")) then 

			DL:createNPCNode(12, -2, "DL_Runa_SheepDone") -- Well done. I hope you already got your reward from Tristan.
			DL:addConditionProgress("npc_runa", "sheep_done")
			DL:addNode()

		end

		if (DL:isConditionFulfilled("npc_runa", "job") and not DL:isConditionFulfilled("npc_runa", "runas_deal")) then 

			DL:createNPCNode(13, 14, "DL_Runa_Delivery") -- The last time I was in the market in Gandria, some guy called Bjarne bought all of my Physalis.
			DL:addNode()


			DL:createNPCNode(14, 15, "DL_Runa_Delivery2") -- He then asked me to bring him even more - in exchange, he would give me a very precious pendant.
			DL:addNode()


			DL:createNPCNode(15, -2, "DL_Runa_Delivery3") -- I would go myself... But at the moment, the situation with the paladins in Gandria is a bit tense.
			DL:addConditionProgress("npc_runa", "runas_deal")
			DL:addNode()

		end

		if (DL:isConditionFulfilled("npc_runa", "runas_deal") and DL:isQuestState("runas_deal", "void")) then 

			DL:createNPCNode(17, -2, "DL_Runa_RunasDealStart") -- Good! Here you go. Don't forget to bring me the pendant!
			DL:changeQuestState("runas_deal", "started")
			DL:addItem("fo_physalis", 10)
			DL:addNode()

		end

		if (DL:isConditionFulfilled("npc_runa", "runas_deal") and not DL:isConditionFulfilled("npc_runa", "who_bjarne")) then 

			DL:createNPCNode(16, -2, "DL_Runa_WhoBjarne") -- Hm. I don't really remember how he looked like. But he sure was handsome and likeable.
			DL:addConditionProgress("npc_runa", "who_bjarne")
			DL:addNode()

		end

		if (DL:isConditionFulfilled("npc_runa", "runas_deal") and not DL:isConditionFulfilled("npc_runa", "paladins")) then 

			DL:createNPCNode(30, 31, "DL_Runa_Paladins") -- They should be there to protect us, but they only serve their "god" and not the people of Admantris.
			DL:addConditionProgress("npc_runa", "paladins")
			DL:addNode()


			DL:createNPCNode(31, -2, "DL_Runa_Paladins2") -- I really don't know who thought it would be a good idea to put them in charge.
			DL:addNode()

		end

		if (DL:isConditionFulfilled("npc_runa", "runas_deal") and not DL:isConditionFulfilled("npc_runa", "who_mage")) then 

			DL:createNPCNode(18, 23, "DL_Runa_WhoMage") -- No, definitely not. I prefer not to talk to mages.
			DL:addConditionProgress("npc_runa", "who_mage")
			DL:addNode()


			DL:createChoiceNode(23)
			DL:addChoice(24, "DL_Choice_TalkingMage") -- But you're talking to one right now...
			DL:addChoice(25, "DL_Choice_WhyHate") -- What is your problem with mages?
			DL:addNode()


			DL:createNPCNode(24, -2, "DL_Runa_TalkingMage") -- Whaaat?
			DL:addConditionProgress("npc_runa", "evil_mage")
			DL:changeQuestState("runas_deal", "failed")
			DL:addNode()


			DL:createNPCNode(25, -2, "DL_Runa_WhyHate") -- Why would you even ask! They're elitist monsters, that's why.
			DL:addNode()

		end

		if (DL:isConditionFulfilled("npc_runa", "runas_deal") and not DL:isConditionFulfilled("npc_runa", "pendant")) then 

			DL:createNPCNode(19, -2, "DL_Runa_Pendant") -- It's an anti-magic pendant. It protects from curses and other magic attacks. Exactly what I need!
			DL:addConditionProgress("npc_runa", "pendant")
			DL:addNode()

		end

		if (DL:isConditionFulfilled("npc_runa", "runas_deal") and not DL:isConditionFulfilled("npc_runa", "physalis")) then 

			DL:createNPCNode(20, -2, "DL_Runa_Physalis") -- It's a nightshade plant which we grow on our farm. It's pretty sweet, but eating too much can be poisonous.
			DL:addConditionProgress("npc_runa", "physalis")
			DL:addNode()

		end

		if (DL:isConditionFulfilled("npc_runa", "pendant") and not DL:isConditionFulfilled("npc_runa", "why_protect")) then 

			DL:createNPCNode(22, -2, "DL_Runa_WhyProtect") -- Because that's the only way we can stand a chance when they attack... you should better get a pendant, too.
			DL:addConditionProgress("npc_runa", "why_protect")
			DL:addNode()

		end

		if (DL:isConditionFulfilled("npc_runa", "why_protect") and not DL:isConditionFulfilled("npc_runa", "paranoid")) then 

			DL:createNPCNode(26, -2, "DL_Runa_Paranoid") -- Paranoid? Me? Ha! Just wait, you'll see...
			DL:addConditionProgress("npc_runa", "paranoid")
			DL:addNode()

		end

		if (DL:isQuestState("runas_deal", "started") and DL:isQuestComplete("runas_deal")) then 

			DL:createNPCNode(27, 28, "DL_Runa_GotPendant") -- Oooh! (Runa seizes the pendant)
			DL:removeItem("eq_antimagicpendant", 1)
			DL:addNode()


			DL:createNPCNode(28, 29, "DL_Runa_GotPendant2") -- Thank you so much! Now, I'm finally safe from this magic.
			DL:changeQuestState("runas_deal", "completed")
			DL:addNode()


			DL:createNPCNode(29, -2, "DL_Runa_GotPendant3") -- Here, take these goods from our farm as your reward.
			DL:addItem("fo_physalis", 1)
			DL:addItem("fo_cheese", 1)
			DL:addItem("fo_milk", 1)
			DL:addItem("fo_rawmeat", 2)
			DL:addNode()

		end

	end


	DL:createNPCNode(21, -1, "DL_Runa_EvilMage") -- Be gone, filthy mage!
	DL:addNode()

end