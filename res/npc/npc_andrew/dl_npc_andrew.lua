-- Dialogue for NPC "npc_andrew"
loadDialogue = function(DL)

	DL:createChoiceNode(0)
	if (not DL:isConditionFulfilled("npc_andrew", "who_are_you")) then 
		DL:addChoice(1, "DL_Choice_WhoAreYou") -- Who are you?
	end
	if (not DL:isConditionFulfilled("npc_andrew", "what_are_you_doing")) then
		DL:addChoice(2, "DL_Choice_WhatAreYouDoing") -- What are you doing here?
	end
	if (not DL:isConditionFulfilled("npc_andrew", "mages")) then
		DL:addChoice(20, "DL_Choice_Mages") -- What do you think about mages?
	end
	if (DL:isConditionFulfilled("npc_andrew", "what_are_you_doing") and not DL:isConditionFulfilled("npc_andrew", "bridge")) then
		DL:addChoice(10, "DL_Choice_TheBridgeIsOk") -- The river is frozen, you can go home.
	end
	DL:addChoice(-1, "") --  ""
	DL:addNode()
	
	DL:createNPCNode(1, -2, "DL_Andrew_IAm") -- I'm Andrew... hic! Nice to meet ya. 
	DL:addConditionProgress("npc_andrew", "who_are_you")
	DL:addNode()
	
	DL:createNPCNode(20, -2, "DL_Andrew_Mages") -- A bunch of freaks, if y'ask me... hic!... I'm pretty glad that none of them livesh in our village.
	DL:addConditionProgress("npc_andrew", "mages")
	DL:addNode()
	
	DL:createNPCNode(2, 3, "DL_Andrew_IAmDrinking") -- Hehe... shilly question... Having some beers, enjoying my time here... I can't go back anyway.
	DL:addConditionProgress("npc_andrew", "what_are_you_doing")
	DL:addNode()
	
	DL:createChoiceNode(3)
	DL:addChoice(4, "DL_Choice_WhyCantYouGoBack") -- Why can't you go back?
	DL:addChoice(5, "DL_Choice_BackToWhere") -- Back to where?
	DL:addNode()
	
	DL:createNPCNode(4, -2, "DL_Andrew_BrokenBridge") -- The bridsh is broken, you know. The bridge to the village, where my wife lives. But that ...hic!... doeshnt bother me at all.
	DL:addNode()
	
	DL:createNPCNode(5, -2, "DL_Andrew_ToVillage") -- To the village... and my wife, Ingrid. The bridsh is broken, you know. But that ...hic!... doeshnt bother me at all.
	DL:addNode()
	
	DL:createNPCNode(10, -1, "DL_Andrew_IWontGoBack") -- Yeah, really... but itsh summer, that makes no sense. And... to be honest... hic... I really like it here.
	DL:addConditionProgress("npc_andrew", "bridge")
	DL:addNode()
	
	DL:setRoot(0)
	
end	
	