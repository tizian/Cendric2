-- Dialogue for NPC "npc_paladin2"
loadDialogue = function(DL)

	DL:createNPCNode(0, 1, "DL_Paladin2_WhatIsIt") -- What is it?
	DL:addNode()

	DL:setRoot(0)
	
	DL:createChoiceNode(1)
	if (not DL:isConditionFulfilled("default", "gandria_open") and not DL:isConditionFulfilled("npc_paladin2", "what_doing")) then
		DL:addChoice(2, "DL_Choice_WhatAreYouDoing") -- What are you doing here?
	end
	if (DL:isConditionFulfilled("default", "gandria_open") and not DL:isConditionFulfilled("npc_paladin2", "where_inina")) then
		DL:addChoice(20, "DL_Choice_WhereIsInina") -- Where is Inina?
	end
	if (not DL:isConditionFulfilled("npc_paladin2", "mage")) then
		DL:addChoice(10, "DL_Choice_Mage") -- Are you a mage? 
	end
	if (not DL:isConditionFulfilled("default", "gandria_open") and DL:isConditionFulfilled("npc_paladin2", "what_doing") and not DL:isConditionFulfilled("npc_paladin2", "duty")) then
		DL:addChoice(11, "DL_Choice_WhatIsDuty") -- What is the Priestess' duty?
	end
	DL:addChoice(-1, "DL_Choice_Bye") -- See you. 
	DL:addNode()
	
	DL:createNPCNode(11, -2, "DL_Paladin2_AskHer") -- I got no time to chit-chat. Go and ask her yourself. But you'd better show her some respect, we're watching you.
	DL:addConditionProgress("npc_paladin2", "duty")
	DL:gotoNode(1)
	DL:addNode()
	
	DL:createNPCNode(2, -2, "DL_Paladin2_GuardingInina") -- We're making sure that our High Priestess can fulfill her duty in peace.
	DL:addConditionProgress("npc_paladin2", "what_doing")
	DL:gotoNode(1)
	DL:addNode()
	
	DL:createNPCNode(10, -2, "DL_Paladin1_Mage") -- Of course. You can't get the rank of a paladin while being a mere commoner.
	DL:addConditionProgress("npc_paladin2", "mage")
	DL:gotoNode(1)
	DL:addNode()
	
	if (DL:isConditionFulfilled("default", "gandria_open") and not DL:isConditionFulfilled("npc_paladin2", "where_inina")) then
		
		DL:createNPCNode(20, -2, "DL_Paladin2_WhereInina") -- She's gone back to Gandria. Someone's got to find out how and why this beast came so close to the city.
		DL:addConditionProgress("npc_paladin2", "where_inina")
		DL:gotoNode(1)
		DL:addNode()
		
	end
	
end	
	