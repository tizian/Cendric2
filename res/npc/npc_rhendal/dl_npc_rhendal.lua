-- Dialogue for NPC "npc_rhendal"
loadDialogue = function(DL)

	DL:createChoiceNode(10)
			
	if (DL:isQuestState("silkweed_potion", "started") and DL:isQuestComplete("silkweed_potion")) then
		DL:addChoice(30, "DL_Choice_IGotSilkweed") --  I got the Silkweed.
	end
	if (DL:isQuestState("hawthorn_staff", "started") and DL:isQuestComplete("hawthorn_staff")) then
		DL:addChoice(70, "DL_Choice_IGotStaff") --  I got the Hawthorn staff.
	end
	if ((DL:isQuestState("broken_bridge", "started") and DL:isQuestComplete("broken_bridge")) or (DL:isQuestState("broken_bridge", "failed") and DL:hasItem("qe_spoiledfeudalfire", 1))) then
		DL:addChoice(90, "DL_Choice_IGotSchnapps") --  I got the schnapps for you.
	elseif (DL:hasItem("qe_spoiledfeudalfire", 1)) then
		DL:addChoice(90, "DL_Choice_IGotSchnappsAnyway") --  I've changed my decision and brought you some schnapps, for free.
	elseif (DL:isQuestState("broken_bridge", "started") and not (DL:isQuestComplete("broken_bridge") or DL:hasItem("qe_spoiledfeudalfire", 1)) and DL:isConditionFulfilled("npc_innkeeper", "bought_feudal_fire")) then
		DL:addChoice(90, "DL_Choice_IDrankSchnapps") --  I got the schnaps for you... and it was delicious.
	end
	if (DL:isQuestState("silkweed_potion", "started")) then
		DL:addChoice(20, "DL_Choice_CanITrustYou") --  Can I trust you? What if that potion is going to poison me?
		DL:addChoice(22, "DL_Choice_WhereIsYourGarden") --  Where can I find the Silkweed?
	end
	if (DL:isConditionFulfilled("npc_rhendal", "potion_drunk") and not DL:isConditionFulfilled("npc_rhendal", "what_is_a_mage")) then 
		DL:addChoice(40, "DL_Choice_WhatDoesThatMean") --  A mage... What does that mean?
	end
	if (DL:isConditionFulfilled("npc_rhendal", "what_is_a_mage") and not DL:isConditionFulfilled("npc_rhendal", "hawthorn_staff")) then
		DL:addChoice(60, "DL_Choice_WhatJob") -- What job can you offer?
	end
	if (DL:isConditionFulfilled("npc_rhendal", "hawthorn_staff") and DL:isQuestState("broken_bridge", "void") and not DL:isConditionFulfilled("npc_rhendal", "evil")) then
		DL:addChoice(80, "DL_Choice_HowCanIHelp") -- Okay, how can I help you?
	end
	if (DL:isConditionFulfilled("npc_rhendal", "potion_drunk") and not DL:isConditionFulfilled("npc_rhendal", "the_mark")) then 
		DL:addChoice(50, "DL_Choice_WhatAboutTheMark") --  About that mark on my wrist...
	end
	if (not DL:isConditionFulfilled("npc_rhendal", "potion_drunk") or DL:isConditionFulfilled("npc_rhendal", "hawthorn_staff")) then
		DL:addChoice(-1, "DL_Choice_CU") --  See you later
	end

	DL:addNode()
	
	if (DL:isQuestState("silkweed_potion", "started") and DL:isConditionFulfilled("npc_rhendal", "potion_drunk")) then

		DL:createNPCNode(0, 1, "DL_Rhendal_NiceVision") -- *Smiles* It seems that your vision was of use. I've never seen a guest of mine freeze my water before.  
		DL:learnSpell(3)
		DL:changeQuestState("the_mark", "started")
		DL:addQuestProgress("who_am_i", "information_gathered")
		DL:changeQuestState("who_am_i", "completed")
		DL:changeQuestState("silkweed_potion", "completed")
		DL:addHint("Spellbook")
		DL:addNode()

		DL:setRoot(0)
		
		DL:createNPCNode(1, -2, "DL_Rhendal_SoYoureAMage") -- So you're a mage. I've felt that you have a strong magic aura the first time you walked through that door.
		DL:addNode()
	
	elseif (DL:isConditionFulfilled("npc_rhendal", "talked")) then

		DL:setRoot(10)
	else
		DL:createNPCNode(0, 1, "DL_Rhendal_Hello") -- Hello, young man. What brings you into my humble home?
		DL:addConditionProgress("npc_rhendal", "talked")
		DL:addNode()
		
		DL:createChoiceNode(1)
		DL:addChoice(4, "DL_Choice_IGotProblem") --  I have a problem.
		if (DL:isConditionFulfilled("npc_edmond", "who_am_i")) then
			DL:addChoice(2, "DL_Choice_EdmondSentMe") -- The hunter Edmond sent me to you.
		end
		if (DL:isConditionFulfilled("npc_ingrid", "who_am_i")) then
			DL:addChoice(3, "DL_Choice_IngridSentMe") -- Ingrid told me you can help me.
		end
		DL:addNode()
	
		DL:setRoot(0)
		
		DL:createNPCNode(2, 4, "DL_Rhendal_EdmondSentYou") -- Ah, Edmond. Our talented hunter.  
		DL:addNode()
		
		DL:createNPCNode(3, 4, "DL_Rhendal_IngridSentYou") -- Ah, Ingrid. Our talented cook. 
		DL:addNode()
		
		DL:createNPCNode(4, 5, "DL_Rhendal_WhatIsProblem") -- So, what exactly is your problem?
		DL:addNode()
		
		DL:createCendricNode(5, 6, "DL_Cendric_Problem") -- I've awoken in the forest in the north and I can't remember anything. I have no clue who I am and how I got there. My name is Cendric, that's the only thing I know.
		DL:addNode()
		
		DL:createNPCNode(6, 7, "DL_Rhendal_Eyeballing") -- That's interesting and worrying at the same time. But you're lucky, I happen to be initiated in the art of alchemy and I know just the right potion that may help you.
		DL:addNode()
		
		DL:createNPCNode(7, -2, "DL_Rhendal_BringMe") -- I have most ingredients for it right here. If you could go pick the fresh leaves of a Silkweed I can start to prepare the potion for you. It grows in my garden, just outside.
		DL:changeQuestState("silkweed_potion", "started")
		DL:addNode()

	end
	
	if (DL:isQuestState("silkweed_potion", "started")) then
		DL:createNPCNode(20, 21, "DL_Rhendal_NoChoice") -- Do you have another choice?
		DL:addNode()
		
		DL:createCendricNode(21, -2, "DL_Cendric_NoChoice") -- ...
		DL:addNode()
		
		DL:createNPCNode(22, -1, "DL_Rhendal_SilkweedLocation") -- It grows in my garden, right in front of this house.
		DL:addNode()
		
		DL:createNPCNode(30, 31, "DL_Rhendal_DrinkThePotion") -- Very good. I just completed the other preparations. Now let's add that Silkweed ... And, it's done. You can drink it now.
		DL:addNode()
		
		DL:createChoiceNode(31)
		DL:addChoice(35, "DL_Choice_Okay") --  Okay...
		DL:addChoice(32, "DL_Choice_But") --  But...
		DL:addNode()
		
		DL:createNPCNode(32, 33, "DL_Rhendal_NoBut") -- No "but"!
		DL:addNode()
		
		DL:createCendricNode(33, 35, "DL_Cendric_OkayThen") -- ... Okay then.
		DL:addNode()
		
		DL:createNPCNode(35, -1, "") -- 
		DL:addConditionProgress("npc_rhendal", "potion_drunk")
		DL:removeItem("qe_silkweed", 1)
		DL:startCutscene("silkweed_potion")
		DL:addNode()
	end
	
	if (DL:isConditionFulfilled("npc_rhendal", "potion_drunk") and not DL:isConditionFulfilled("npc_rhendal", "what_is_a_mage")) then 
		
		DL:createNPCNode(40, 41, "DL_Rhendal_MageIs") -- It means that you are able to focus the magic energy that flows through this world and use it to your favour.
		DL:addConditionProgress("npc_rhendal", "what_is_a_mage")
		DL:addNode()
		
		DL:createNPCNode(41, 42, "DL_Rhendal_MagiciansAre") -- It's a special gift - not all people in Admantris have it. 
		DL:addNode()		
				
		DL:createNPCNode(42, 43, "DL_Rhendal_YouCanRead") -- Even if I'm not able to use that power myself, I still take an interest in the art of magic. You should definitely take a look into my book I wrote about it, its right next to the fireplace.
		DL:addNode()
		
		DL:createNPCNode(43, 44, "DL_Rhendal_YouHaveTo") -- But, what's more important at the moment, is that you have to get to know your powers. What insights did the vision bring you?
		DL:addNode()
		
		DL:createChoiceNode(44)
		DL:addChoice(45, "DL_Choice_IHadStaff") --  I once had a staff and a fancy hat.
		DL:addChoice(46, "DL_Choice_ICanFreeze") --  I can apparently freeze water.
		DL:addNode()
		
		DL:createNPCNode(45, 46, "DL_Rhendal_YouHadStaff") -- Yes, that's what some mages wear. But we saw you are a mage that is able to use ice magic.
		DL:addNode()
		
		DL:createNPCNode(46, -2, "DL_Rhendal_AndThisIsGood") -- And that's exactly what I was looking for. I can offer you a job, if you're interested.
		DL:addNode()
		
	end
	
	if (DL:isConditionFulfilled("npc_rhendal", "potion_drunk") and not DL:isConditionFulfilled("npc_rhendal", "the_mark")) then 
		
		DL:createNPCNode(50, 51, "DL_Rhendal_LetMeSee") -- Hmm, let me see... Interesting. It seems like the potion has not only brought you back some memories, but has also revealed this mark.
		DL:addNode()
		
		DL:createChoiceNode(51)
		DL:addChoice(52, "DL_Choice_SideEffect") --  It is not some side-effect of the potion?
		DL:addChoice(53, "DL_Choice_DoYouKnowAnything") -- Do you know anything about this mark?
		DL:addNode()
		
		DL:createNPCNode(52, 54, "DL_Rhendal_SideEffect") -- No, the Silkweed potion doesn't create this kind of marks. It only reveals what's hidden.
		DL:addNode()
		
		DL:createChoiceNode(54)
		DL:addChoice(53, "DL_Choice_DoYouKnowAnything") -- Do you know anything about this mark?
		DL:addNode()
		
		DL:createNPCNode(53, 55, "DL_Rhendal_TheMark") -- I don't recognize the symbol... it's definitely magic but doesn't have a friendly aura, no, not at all...
		DL:addNode()
		
		DL:createNPCNode(55, -2, "DL_Rhendal_Curse") -- I think it's some kind of curse, and it's getting stronger. You'd be better off with finding someone that can break it. I have no knowledge about this kind of magic, I'm sorry.
		DL:addQuestDescription("the_mark", 1)
		DL:addConditionProgress("npc_rhendal", "the_mark")
		DL:addNode()
		
	end
	
	if (DL:isConditionFulfilled("npc_rhendal", "what_is_a_mage") and not DL:isConditionFulfilled("npc_rhendal", "hawthorn_staff")) then
		
		if (DL:hasItem("we_hawthorn", 1)) then 
		
			DL:createNPCNode(60, 61, "DL_Rhendal_YouNeedAStaff") -- Before you can take this job, you need a staff to control and focus your magic. I grow a magical Hawthorn tree in my garden. Take a branch from this tree and you will have what you need.  
			DL:addConditionProgress("npc_rhendal", "hawthorn_staff")
			DL:addNode()
			
			DL:createCendricNode(61, 62, "DL_Cendric_IAlreadyHave") -- ... I think I ...
			DL:addNode()
			
			DL:createNPCNode(62, -2, "DL_Rhendal_YouAlreadyHave") -- Ah, I see, you already took a branch. Well, I won't be angry with you, but the next person from which you take things probably won't be that lenient... 
			DL:addNode()
		else
		
			DL:createNPCNode(60, -1, "DL_Rhendal_YouNeedAStaff") -- Before you can take this job, you need a staff to control and focus your magic. I grow a magical Hawthorn tree in my garden. Take a branch from this tree and you will have what you need.  
			DL:changeQuestState("hawthorn_staff", "started")
			DL:addConditionProgress("npc_rhendal", "hawthorn_staff")
			DL:addNode()
	
		end
		
	end
	
	if (DL:isQuestState("hawthorn_staff", "started") and DL:isQuestComplete("hawthorn_staff")) then
		
		DL:createNPCNode(70, -2, "DL_Rhendal_YouHaveStaff") -- It suits you well. 
		DL:changeQuestState("hawthorn_staff", "completed")
		DL:addNode()
			
	end
	
	if (DL:isConditionFulfilled("npc_rhendal", "hawthorn_staff") and DL:isQuestState("broken_bridge", "void") and not DL:isConditionFulfilled("npc_rhendal", "evil")) then
		
		DL:createNPCNode(80, 81, "DL_Rhendal_OurProblem") -- The only way between our village and the tavern leads over the bridge in the West. The innkeeper brews a special kind of schnapps which I need for one of my potions.
		DL:addNode()
		
		DL:createNPCNode(81, 82, "DL_Rhendal_BrokenBridge") -- Unfortunately, that bridge has broken lately and it will take us some days to rebuild it.
		DL:addNode()
		
		DL:createNPCNode(82, 83, "DL_Rhendal_YourJob") -- Your job would to get to the other side of the river by freezing a small passage, and get me a bottle of this schnapps from the tavern.
		DL:addConditionProgress("npc_rhendal", "talked_about_schnapps")
		DL:addNode()
		
		DL:createChoiceNode(83)
		DL:addChoice(84, "DL_Choice_IWillHelp") --  Okay, I'll see what I can do. 
		DL:addChoice(85, "DL_Choice_WhatsInIt") --  So, what's in it for me then?
		DL:addChoice(86, "DL_Choice_IWontHelp") --  I don't think I should waste my power like this...
		DL:addNode()
		
		DL:createNPCNode(84, -2, "DL_Rhendal_BrokenBridgeQuest") -- Great. Just ask the innkeeper for a "feudal fire", he'll know what you want. And take these coins, it should be enough.
		DL:addGold(20)
		DL:changeQuestState("broken_bridge", "started")
		DL:addNode()
		
		DL:createNPCNode(86, -1, "DL_Rhendal_YouAreEvil") -- And I don't think you'll get on the right way like this, boy.
		DL:addConditionProgress("npc_rhendal", "evil");
		DL:addNode()
		
		DL:createNPCNode(85, 87, "DL_Rhendal_WhyYouShould") -- Well, you'd prove that you plan to use your power for greater good. I'm sure that one or two people may be pleased to hear about your good deed.
		DL:addNode()
		
		DL:createChoiceNode(87)
		DL:addChoice(84, "DL_Choice_IWillHelp") --  Okay, I'll see what I can do.
		DL:addChoice(86, "DL_Choice_IWontHelp") --  I don't think I should waste my power like this...
		DL:addNode()
		
	end
	
	if (DL:isQuestState("broken_bridge", "started") and DL:isQuestComplete("broken_bridge")) then
		DL:createNPCNode(90, 91, "DL_Rhendal_ThankForSchnaps") -- Oh, perfect, thank you. So you managed to cross the river. Hmm, a truly special drink. Do you want some? 
		DL:changeQuestState("broken_bridge", "completed")
		DL:addReputationProgress("druid", 10)
		DL:removeItem("pe_feudalfire", 1)
		DL:addNode()
		
		DL:createChoiceNode(91)
		DL:addChoice(92, "DL_Choice_IWantDrink") --  Yes, why not.
		DL:addChoice(94, "DL_Choice_IDontWantDrink") --  No, thank you.
		DL:addNode()
		
		DL:createNPCNode(92, 93, "DL_Rhendal_GivesSchnapps") -- Yes, that's the right decision. Here, take a glass.
		DL:addItem("pe_glassoffeudalfire", 1)
		DL:addReputationProgress("druid", 5)
		DL:addNode()
		
		DL:createCendricNode(93, -2, "DL_Cendric_Thanks") -- Thanks.
		DL:addNode()
		
		DL:createNPCNode(94, -2, "DL_Rhendal_NoSchnappsPity") -- Oh, what a pity. You don't know what you're missing.
		DL:addNode()
		
	elseif (DL:hasItem("qe_spoiledfeudalfire", 1)) then
		DL:createNPCNode(90, 91, "DL_Rhendal_ThankForSchnaps") -- Oh, perfect, thank you. So you managed to cross the river. Hmm, a truly special drink. Do you want some? 
		DL:addReputationProgress("druid", 5)
		DL:addConditionProgress("npc_rhendal", "spoiled_schnapps")
		DL:removeItem("qe_spoiledfeudalfire", 1)
		DL:addNode()
		
		DL:createCendricNode(91, 94, "DL_Choice_IDontWantDrink") --  No, thank you.
		DL:addNode()
		
		DL:createNPCNode(94, -2, "DL_Rhendal_NoSchnappsPity") -- Oh, what a pity. You don't know what you're missing.
		DL:addNode()
	elseif (DL:isQuestState("broken_bridge", "started") and not (DL:isQuestComplete("broken_bridge") or DL:hasItem("qe_spoiledfeudalfire", 1)) and DL:isConditionFulfilled("npc_innkeeper", "bought_feudal_fire")) then
		DL:createNPCNode(90, -1, "DL_Rhendal_YouDrankSchnaps") -- What? Oh you silly boy! You drank it yourself? You're not the sharpest knife in the drawer, are you.
		DL:changeQuestState("broken_bridge", "failed")
		DL:addNode()
	end
	
end	
	