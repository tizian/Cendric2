-- Dialogue for NPC "npc_paladin1"
loadDialogue = function(DL)

	DL:createNPCNode(0, 1, "DL_Paladin2_WhatIsIt") -- What is it?
	DL:addNode()

	DL:setRoot(0)
	
	DL:createChoiceNode(1)
	if (not DL:isConditionFulfilled("npc_paladin2", "what_doing")) then
		DL:addChoice(2, "DL_Choice_WhatAreYouDoing") -- What are you doing here?
	end
	if (not DL:isConditionFulfilled("npc_paladin2", "mage")) then
		DL:addChoice(10, "DL_Choice_Mage") -- Are you a mage? 
	end
	DL:addChoice(-1, "DL_Choice_Bye") -- See you. 
	DL:addNode()
	
	DL:createNPCNode(2, -2, "DL_Paladin2_GuardingInina") -- We're making sure that our High Priestess can fulfill her duty in peace.
	DL:addConditionProgress("npc_paladin2", "what_doing")
	DL:addNode()
	
	DL:createNPCNode(10, -2, "DL_Paladin1_Mage") -- Of course. You can't get the rank of a paladin while being a mere commoner.
	DL:addConditionProgress("npc_paladin2", "mage")
	DL:addNode()
end	
	