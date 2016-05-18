-- Dialogue for NPC "Zeff"
loadDialogue = function(DL)
	
	DL:createChoiceNode(0)
	if (not DL:isConditionFulfilled("npc_zeff", "who_are_you")) then 
		DL:addChoice(-1, "DL_Choice_WhoAreYou") -- Who are you?
	end
	if (not DL:isConditionFulfilled("npc_zeff", "what_are_you_doing")) then
		DL:addChoice(-1, "DL_Choice_WhatAreYouDoing") -- What are you doing here?
	end

	DL:addChoice(-1, "DL_Choice_CU") --  See you later.
	DL:addNode()
	
	DL:setRoot(0)
end	
	