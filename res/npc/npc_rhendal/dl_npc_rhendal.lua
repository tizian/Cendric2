-- Dialogue for NPC "npc_rhendal"
loadDialogue = function(DL) 

	if (not DL:isConditionFulfilled("npc_rhendal", "talked")) then 
		DL:setRoot(1) 
	elseif (DL:isQuestState("silkweed_potion", "void")) then 
		DL:setRoot(5) 
	elseif (not DL:isQuestState("silkweed_potion", "completed")) then 
		DL:setRoot(12) 
	elseif (not DL:isConditionFulfilled("npc_rhendal", "vision")) then 
		DL:setRoot(24) 
	elseif (not DL:isConditionFulfilled("npc_rhendal", "mark")) then 
		DL:setRoot(25) 
	else 
		DL:setRoot(35) 
	end 

	if (not DL:isConditionFulfilled("npc_rhendal", "talked")) then 

		DL:createNPCNode(1, 2, "DL_Rhendal_Hello") -- Hello, young man. What brings you into my humble home?
		DL:addConditionProgress("npc_rhendal", "talked")
		DL:addNode()


		DL:createChoiceNode(2)
		DL:addChoice(-2, "DL_Choice_IGotProblem") -- I have a problem.
		if (DL:isConditionFulfilled("npc_edmond", "who_am_i")) then 
			DL:addChoice(4, "DL_Choice_EdmondSentMe") -- The hunter Edmond sent me to you.
		end
		if (DL:isConditionFulfilled("npc_ingrid", "who_am_i")) then 
			DL:addChoice(7, "DL_Choice_IngridSentMe") -- Ingrid told me you can help me.
		end
		DL:addNode()

		if (DL:isConditionFulfilled("npc_edmond", "who_am_i")) then 

			DL:createNPCNode(4, -2, "DL_Rhendal_EdmondSentYou") -- Ah, Edmond. Our talented hunter.
			DL:addNode()

		end

		if (DL:isConditionFulfilled("npc_ingrid", "who_am_i")) then 

			DL:createNPCNode(7, -2, "DL_Rhendal_IngridSentYou") -- Ah, Ingrid. Our talented cook.
			DL:addNode()

		end

	end

	if (DL:isQuestState("silkweed_potion", "void")) then 

		DL:createNPCNode(5, 6, "DL_Rhendal_WhatIsProblem") -- So, what exactly is your problem?
		DL:addNode()


		DL:createCendricNode(6, 3, "DL_Cendric_Problem") -- I've awoken in some forest and I can't remember anything. I have no clue who I am and how I got there.
		DL:addNode()


		DL:createNPCNode(3, 8, "DL_Rhendal_Problem") -- Hm. That doesn't sound good.
		DL:addNode()


		DL:createNPCNode(8, 9, "DL_Rhendal_Problem2") -- But you're in luck, I happen to be initiated in the art of alchemy and I know just the right potion that may help you.
		DL:addNode()


		DL:createNPCNode(9, 10, "DL_Rhendal_BringMe") -- I have most ingredients for it right here.
		DL:addNode()


		DL:createNPCNode(10, 11, "DL_Rhendal_BringMe2") -- I still need some fresh leaves from a silkweed and a magical stone from a cairn wraith though.
		DL:addNode()


		DL:createNPCNode(11, -2, "DL_Rhendal_BringMe3") -- If you could bring me those, I will brew the potion for you.
		DL:changeQuestState("silkweed_potion", "started")
		DL:addNode()

	end

	if (not DL:isQuestState("silkweed_potion", "completed")) then 

		DL:createChoiceNode(12)
		if (DL:isQuestComplete("silkweed_potion")) then 
			DL:addChoice(15, "DL_Choice_FoundIngredients") -- I found the ingredients for the potion.
		end
		DL:addChoice(13, "DL_Choice_WhereSilkweed") -- Where can I find the silkweed?
		DL:addChoice(14, "DL_Choice_WhereCairn") -- Where do cairn wraiths live?
		if (not DL:isConditionFulfilled("npc_rhendal", "trust_you")) then 
			DL:addChoice(18, "DL_Choice_TrustYou") -- Why should I trust you? What if that potion is going to poison me?
		end
		DL:addChoice(-1, "") -- 
		DL:addNode()

		if (DL:isQuestComplete("silkweed_potion")) then 

			DL:createNPCNode(15, 16, "DL_Rhendal_FoundIngredients") -- Very good. I just completed the other preparations. Now let's add the silkweed and the stone...
			DL:removeItem("qe_silkweed", 1)
			DL:removeItem("mi_corrupt_stone_ice", 1)
			DL:setWeather("res/map/elderhome/elderhome.tmx", "snow", 30)
			DL:addNode()


			DL:createNPCNode(16, 17, "DL_Rhendal_FoundIngredients2") -- ... and it's done. You can drink it now.
			DL:changeQuestState("silkweed_potion", "completed")
			DL:addNode()


			DL:createChoiceNode(17)
			DL:addChoice(20, "DL_Choice_DrinkOkay") -- Okay...
			DL:addChoice(21, "DL_Choice_DrinkBut") -- But...
			DL:addNode()


			DL:createNPCNode(20, -1, "") -- 
			DL:startCutscene("silkweed_potion")
			DL:addNode()


			DL:createNPCNode(21, 22, "DL_Rhendal_DrinkBut") -- No "but"! Drink it.
			DL:addNode()


			DL:createCendricNode(22, 23, "DL_Choice_DrinkOkay") -- 
			DL:addNode()


			DL:createNPCNode(23, -2, "") -- 
			DL:startCutscene("silkweed_potion")
			DL:addNode()

		end


		DL:createNPCNode(13, -2, "DL_Rhendal_WhereSilkweed") -- I grow some silkweed in my garden, just in front of my house.
		DL:addNode()


		DL:createNPCNode(14, -2, "DL_Rhendal_WhereCairn") -- Usually in the mountains. I think I've seen some on the windy plateau, right behind the crypt of our village.
		DL:addNode()

		if (not DL:isConditionFulfilled("npc_rhendal", "trust_you")) then 

			DL:createNPCNode(18, 19, "DL_Rhendal_TrustYou") -- Do you have another choice?
			DL:addConditionProgress("npc_rhendal", "trust_you")
			DL:addNode()


			DL:createCendricNode(19, -2, "DL_Cendric_TrustYou") -- ...
			DL:addNode()

		end

	end

	if (not DL:isConditionFulfilled("npc_rhendal", "vision")) then 

		DL:createNPCNode(24, 26, "DL_Rhendal_Snowing") -- (Shivers) What... it's snowing? 
		DL:learnSpell(2)
		DL:addQuestProgress("who_am_i", "information_gathered")
		DL:changeQuestState("who_am_i", "completed")
		DL:setWeather("res/map/elderhome/elderhome.tmx", "", 30)
		DL:changeQuestState("the_mark", "started")
		DL:addNode()


		DL:createNPCNode(26, 27, "DL_Rhendal_Snowing2") -- So you're an elementalist? A mage that controls the elements?
		DL:addNode()


		DL:createNPCNode(27, 28, "DL_Rhendal_Snowing3") -- Fascinating! What did you see in your vision?
		DL:addNode()


		DL:createChoiceNode(28)
		if (not DL:isConditionFulfilled("npc_rhendal", "vision_dungeon")) then 
			DL:addChoice(29, "DL_Choice_VisionDungeon") -- I was imprisoned in some dungeon.
		end
		if (not DL:isConditionFulfilled("npc_rhendal", "vision_flames")) then 
			DL:addChoice(30, "DL_Choice_VisionFlames") -- There were... purple flames.
		end
		DL:addChoice(31, "DL_Choice_VisionMark") -- This mark appeared on my wrist. What does it mean?
		DL:addNode()

		if (not DL:isConditionFulfilled("npc_rhendal", "vision_dungeon")) then 

			DL:createNPCNode(29, -2, "DL_Rhendal_VisionDungeon") -- Hm. Not everything that appears in your vision has to be true. What else did you see?
			DL:addConditionProgress("npc_rhendal", "vision_dungeon")
			DL:gotoNode(28)
			DL:addNode()

		end

		if (not DL:isConditionFulfilled("npc_rhendal", "vision_flames")) then 

			DL:createNPCNode(30, -2, "DL_Rhendal_VisionFlames") -- Maybe some kind of magic. I've never heard of purple magic, though.
			DL:addConditionProgress("npc_rhendal", "vision_flames")
			DL:gotoNode(28)
			DL:addNode()

		end


		DL:createNPCNode(31, 32, "DL_Rhendal_VisionMark") -- Hmm, let me see... Interesting.
		DL:addConditionProgress("npc_rhendal", "vision")
		DL:addNode()


		DL:createNPCNode(32, 33, "DL_Rhendal_VisionMark2") -- It seems like the potion has not only brought you back some memories, but has also revealed this mark.
		DL:addNode()


		DL:createChoiceNode(33)
		DL:addChoice(34, "DL_Choice_SideEffect") -- Could it be some side-effect of the potion?
		DL:addChoice(-2, "DL_Choice_KnowAnythingMark") -- Do you know anything about this mark?
		DL:addNode()


		DL:createNPCNode(34, 36, "DL_Rhendal_SideEffect") -- No, the silkweed potion doesn't create this kind of marks. It only reveals what's hidden.
		DL:addNode()


		DL:createChoiceNode(36)
		DL:addChoice(-2, "DL_Choice_KnowAnythingMark") -- 
		DL:addNode()

	end

	if (not DL:isConditionFulfilled("npc_rhendal", "mark")) then 

		DL:createNPCNode(25, 42, "DL_Rhendal_KnowAnythingMark") -- I don't recognize the symbol... it's definitely magic but doesn't have a friendly aura, no, not at all...
		DL:addConditionProgress("npc_rhendal", "mark")
		DL:addNode()


		DL:createNPCNode(42, 43, "DL_Rhendal_KnowAnythingMark2") -- I think it's some kind of curse, and it's getting stronger.
		DL:addNode()


		DL:createNPCNode(43, -2, "DL_Rhendal_KnowAnythingMark3") -- You'd be better off with finding someone who knows more about it than me and can break it.
		DL:addQuestDescription("the_mark", 1)
		DL:addNode()

	end


	DL:createChoiceNode(35)
	if (not DL:isConditionFulfilled("npc_rhendal", "mage")) then 
		DL:addChoice(37, "DL_Choice_Mage") -- What exactly is a mage?
	end
	if (not DL:isConditionFulfilled("npc_rhendal", "you_mage")) then 
		DL:addChoice(41, "DL_Choice_YouMage") -- Are you a mage?
	end
	if (not DL:isConditionFulfilled("npc_rhendal", "magic") and not DL:isSpellEquipped(2)) then 
		DL:addChoice(38, "DL_Choice_Magic") -- How can I use my magic?
	end
	if (not DL:isConditionFulfilled("npc_rhendal", "job")) then 
		DL:addChoice(39, "DL_Choice_Job") -- Do you have a job for me?
	end
	if (DL:isConditionFulfilled("npc_rhendal", "job") and DL:isQuestState("broken_bridge", "void") and DL:isSpellEquipped(2)) then 
		DL:addChoice(47, "DL_Choice_BrokenBridge") -- I'm ready for your job.
	end
	if (DL:isQuestState("broken_bridge", "started") and DL:isQuestComplete("broken_bridge")) then 
		DL:addChoice(53, "DL_Choice_GotSchnapps") -- I got the schnapps for you.
	end
	if (DL:hasItem("qe_spoiledfeudalfire", 1)) then 
		DL:addChoice(57, "DL_Choice_GotSchnapps") -- 
	end
	if (DL:isQuestState("broken_bridge", "started") and not (DL:isQuestComplete("broken_bridge") or DL:hasItem("qe_spoiledfeudalfire", 1)) and DL:isConditionFulfilled("npc_innkeeper", "bought_feudal_fire")) then 
		DL:addChoice(60, "DL_Choice_IDrankSchnaps") -- I got the schnapps for you... and it was delicious.
	end
	DL:addChoice(-1, "DL_Choice_Bye") -- That's everything I needed to know. Bye.
	DL:addNode()

	if (not DL:isConditionFulfilled("npc_rhendal", "mage")) then 

		DL:createNPCNode(37, 40, "DL_Rhendal_Mage") -- Mages are able to control the magic energy that flows through this world.
		DL:addConditionProgress("npc_rhendal", "mage")
		DL:addNode()


		DL:createNPCNode(40, -2, "DL_Rhendal_Mage2") -- It's a special gift - not all people in Admantris have it.
		DL:addNode()

	end

	if (not DL:isConditionFulfilled("npc_rhendal", "you_mage")) then 

		DL:createNPCNode(41, -2, "DL_Rhendal_YouMage") -- No. I'm "only" an alchemist. I use the magical energy contained in some plants, but I can't control it myself.
		DL:addConditionProgress("npc_rhendal", "you_mage")
		DL:addNode()

	end

	if (not DL:isConditionFulfilled("npc_rhendal", "magic") and not DL:isSpellEquipped(2)) then 

		DL:createNPCNode(38, 45, "DL_Rhendal_Magic") -- First, you need a staff that is able to focus magical energy.
		DL:addNode()


		DL:createNPCNode(45, 46, "DL_Rhendal_Magic2") -- I grow a magical Hawthorn tree in my garden. Just take a branch from there and you will have what you need.
		DL:addNode()


		DL:createNPCNode(46, -1, "DL_Rhendal_Magic3") -- Equip the staff and open your spellbook to equip your iceball spell.
		DL:addHint("Spellbook")
		DL:addNode()

	end

	if (not DL:isConditionFulfilled("npc_rhendal", "job")) then 

		DL:createNPCNode(39, 44, "DL_Rhendal_Job") -- Yes, I would have a job for an ice mage.
		DL:addConditionProgress("npc_rhendal", "job")
		DL:addNode()


		DL:createNPCNode(44, -2, "DL_Rhendal_Job2") -- I will tell you more about it, after you've equipped a staff with an iceball spell.
		DL:addNode()

	end

	if (DL:isConditionFulfilled("npc_rhendal", "job") and DL:isQuestState("broken_bridge", "void") and DL:isSpellEquipped(2)) then 

		DL:createNPCNode(47, 48, "DL_Rhendal_BrokenBridge") -- Perfect, the staff suits you well.
		DL:addConditionProgress("npc_rhendal", "magic")
		DL:addNode()


		DL:createNPCNode(48, 49, "DL_Rhendal_BrokenBridge2") -- Well. I need some special schnapps for my potions. The only one who brews it is the innkeeper of the tavern in the West.
		DL:addNode()


		DL:createNPCNode(49, 50, "DL_Rhendal_BrokenBridge3") -- The problem is now, that the bridge that connects this village and the tavern, is broken.
		DL:addNode()


		DL:createNPCNode(50, 51, "DL_Rhendal_BrokenBridge4") -- We would need some time to rebuild it, but I need the schnapps as soon as possible.
		DL:addNode()


		DL:createNPCNode(51, 52, "DL_Rhendal_BrokenBridge5") -- Use your ice magic to freeze the river and get to the other side.
		DL:changeQuestState("broken_bridge", "started")
		DL:addNode()


		DL:createNPCNode(52, -1, "DL_Rhendal_BrokenBridge6") -- Ask the innkeeper for a "Feudal Fire". Here, this gold should be enough. Hurry!
		DL:addGold(20)
		DL:addConditionProgress("npc_rhendal", "talked_about_schnapps")
		DL:addNode()

	end

	if (DL:isQuestState("broken_bridge", "started") and DL:isQuestComplete("broken_bridge")) then 

		DL:createNPCNode(53, 54, "DL_Rhendal_GotSchnapps") -- Oh, perfect, thank you. So you managed to cross the river. Hmm, a truly special drink. Do you want some?
		DL:removeItem("pe_feudalfire", 1)
		DL:changeQuestState("broken_bridge", "completed")
		DL:addNode()


		DL:createChoiceNode(54)
		DL:addChoice(55, "DL_Choice_IWantDrink") -- Yes, why not.
		DL:addChoice(56, "DL_Choice_IDontWantDrink") -- No, thank you.
		DL:addNode()


		DL:createNPCNode(55, -2, "DL_Rhendal_GivesSchnapps") -- Yes, that's the right decision. Here, take a glass.
		DL:addItem("pe_glassoffeudalfire", 1)
		DL:addNode()


		DL:createNPCNode(56, -2, "DL_Rhendal_NoSchnappsPity") -- Oh, what a pity. You don't know what you're missing.
		DL:addNode()

	end

	if (DL:hasItem("qe_spoiledfeudalfire", 1)) then 

		DL:createNPCNode(57, 58, "DL_Rhendal_GotSchnapps") -- 
		DL:removeItem("qe_spoiledfeudalfire", 1)
		DL:addConditionProgress("npc_rhendal", "spoiled_schnapps")
		DL:addNode()


		DL:createChoiceNode(58)
		DL:addChoice(59, "DL_Choice_IDontWantDrink") -- 
		DL:addNode()


		DL:createNPCNode(59, -2, "DL_Rhendal_NoSchnappsPity") -- 
		DL:addNode()

	end

	if (DL:isQuestState("broken_bridge", "started") and not (DL:isQuestComplete("broken_bridge") or DL:hasItem("qe_spoiledfeudalfire", 1)) and DL:isConditionFulfilled("npc_innkeeper", "bought_feudal_fire")) then 

		DL:createNPCNode(60, -1, "DL_Rhendal_YouDrankSchnaps") -- What? Oh you silly boy! You drank it yourself? You're not the sharpest knife in the drawer, are you?!
		DL:changeQuestState("broken_bridge", "failed")
		DL:addNode()

	end

end