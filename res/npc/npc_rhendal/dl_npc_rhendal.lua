-- Dialogue for NPC "npc_rhendal"
loadDialogue = function(DL)

	DL:createChoiceNode(10)
			
	if (DL:isQuestState("silkweed_potion", "started") and DL:isQuestComplete("silkweed_potion")) then
		DL:addChoice(30, "DL_Choice_IGotSilkweed") --  I got the Silkweed.
	end
	if (DL:isQuestState("silkweed_potion", "started")) then
		DL:addChoice(20, "DL_Choice_CanITrustYou") --  Can I trust you? What if that potion is going to poison me?
		DL:addChoice(22, "DL_Choice_WhereIsYourGarden") --  Where can I find the Silkweed?
	end
			
	DL:addChoice(-1, "DL_Choice_CU") --  See you later
	DL:addNode()
	
	if (DL:isQuestState("silkweed_potion", "started") and DL:isConditionFulfilled("npc_rhendal", "potion_drunk")) then
	
		DL:createNPCNode(0, 1, "DL_Rhendal_SoYoureAMage") -- So, you're a magician, boy. I've felt that there you have a strong magic aura the first time you walked through that door.
		DL:changeQuestState("silkweed_potion", "completed")
		DL:addNode()
		
		DL:createChoiceNode(1)
		DL:addChoice(2, "DL_Choice_WhatDoesThatMean") --  A magician... What does that mean?
		DL:addChoice(-1, "DL_Choice_WhatAboutTheMark") --  About that mark on my wrist...
		DL:addNode()
		
		DL:createNPCNode(2, 3, "DL_Rhendal_MageIs") -- It means that you are able to focus the magic energy that flows through this world and use it to your favour.
		DL:addNode()
		
		DL:createNPCNode(3, -1, "DL_Rhendal_MagiciansAre") -- It's a special gift - not all people in Admantris have it. 
		DL:addNode()
	
		DL:setRoot(0)
	
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
		
		DL:createNPCNode(6, 7, "DL_Rhendal_Eyeballing") -- That's interesting and worrying at the same time, boy. But you're lucky, I happen to be initiated in the art of alchemy and I know just the right potion that may help you.
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
		
		DL:createNPCNode(30, 31, "DL_Rhendal_DrinkThePotion") -- Very good. I just completed the other preparations. Now let's add that Silkweed ... Here, take it and drink it as fast as possible.
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
		DL:startCutscene("silkweed_potion")
		DL:addNode()
	end
end	
	