-- Dialogue for NPC "npc_thea"
loadDialogue = function(DL)

	DL:createChoiceNode(0)
	DL:addChoice(-1, "DL_Choice_WhoAreYou") -- Who are you?
	DL:addChoice(-1, "DL_Choice_WhatAreYouDoing") -- What are you doing here?
	DL:addChoice(-1, "") --  ""
	DL:addNode()
	
	DL:setRoot(0)
	
end	
	