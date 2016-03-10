-- Dialogue for NPC "npc_ingrid"
loadDialogue = function(DL)

	DL:createChoiceNode(0)
			
	if (DL:isQuestState("meat_delivery", "started") and DL:isQuestComplete("meat_delivery")) then
		DL:addChoice(10, "DL_Choice_IGotMeat") --  I got some meat for you with the best regards from Edmond.
	end
	if (DL:isQuestState("roast_meat_for_ingrid", "started") and DL:isQuestComplete("roast_meat_for_ingrid")) then
		DL:addChoice(20, "DL_Choice_IGotRoastedMeat") -- Here is your roasted meat.  
	end
	if (not DL:isConditionFulfilled("npc_ingrid", "meat_given")) then
		DL:addChoice(32, "DL_Choice_ImHungry") --  I'm hungry.
	end
	if (not DL:isConditionFulfilled("npc_ingrid", "who_are_you")) then
		DL:addChoice(30, "DL_Choice_WhoAreYou") -- Who are you? 
	end
			
	DL:addChoice(-1, "DL_Choice_CU") --  See you later
	DL:addNode()
	
	DL:setRoot(0)
	
	DL:createNPCNode(10, 11, "DL_Ingrid_TakesRawMeat") -- How nice of you! But, I'm pretty busy at the moment.
	DL:changeQuestState("meat_delivery", "completed")
	DL:removeItem("fo_rawmeat", 5)
	DL:addNode()
	
	DL:createNPCNode(11, -1, "DL_Ingrid_GoRoastMeat") -- I could need your help. Take the meat, roast it over the fire and bring it back to me when you're finished.
	DL:changeQuestState("roast_meat_for_ingrid", "started")
	DL:addItem("fo_rawmeat", 5)
	DL:addNode()
	
	DL:createNPCNode(20, -1, "DL_Ingrid_TakesRoastedMeat") -- Perfect, thanks a lot. Boy, you look hungry. Take this stew as a reward, it will strengthen you.
	DL:changeQuestState("roast_meat_for_ingrid", "completed")
	DL:removeItem("fo_roastedmeat", 5)
	DL:addItem("pe_ingridstew", 1)
	DL:addNode()
	
	DL:createNPCNode(30, 31, "DL_Ingrid_IAmIngrid") -- I'm Ingrid, the cook of this humble village, responsible to feed all the hungry mouths here. And what brings you here? 
	DL:addConditionProgress("npc_ingrid", "who_are_you")
	DL:addNode()
	
	DL:createChoiceNode(31)
	if (not DL:isConditionFulfilled("npc_ingrid", "meat_given")) then
		DL:addChoice(32, "DL_Choice_ImHungry") --  I'm hungry.
	end
	DL:addChoice(33, "DL_Choice_ImLost") -- I'm lost.
	DL:addChoice(34, "DL_Choice_ImJustHavingALook") -- I'm just having a look around.
	if (DL:isQuestState("meat_delivery", "started") and DL:isQuestComplete("meat_delivery")) then
		DL:addChoice(10, "DL_Choice_IGotMeat") --  I got some meat for you with the best regards from Edmond.
	end
	if (DL:isQuestState("roast_meat_for_ingrid", "started") and DL:isQuestComplete("roast_meat_for_ingrid")) then
		DL:addChoice(20, "DL_Choice_IGotRoastedMeat") -- Here is your roasted meat.  
	end
	DL:addNode()
	
	DL:createNPCNode(32, -2, "DL_Ingrid_GivesMeat") -- Oh, you poor, poor boy. Here, take this piece of meat, this should help for now.
	DL:addItem("fo_roastedmeat", 1)
	DL:addConditionProgress("npc_ingrid", "meat_given")
	DL:addNode()
	
	DL:createNPCNode(33, -2, "DL_Ingrid_Location") -- You're lost? You're in the middle of the meadows, in the lands of Admantris. Why don't you just take a look on your map you got there?
	DL:addHint("Map")
	DL:addNode()
	
	DL:createNPCNode(34, -2, "DL_Ingrid_DontTrouble") -- Don't make trouble.
	DL:addNode()

end	
	