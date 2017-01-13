-- Dialogue for NPC "npc_citizen1"
loadDialogue = function(DL) 

	DL:setRoot(1) 


	DL:createChoiceNode(1)
	DL:addChoice(3, "DL_Choice_Where") -- Excuse me, where is...
	DL:addChoice(2, "DL_Choice_About") -- Could you tell me something about...
	DL:addChoice(-2, "") -- 
	DL:addNode()


	DL:createChoiceNode(3)
	DL:addChoice(-1, "") -- 
	DL:addNode()


	DL:createChoiceNode(2)
	DL:addChoice(-1, "") -- 
	DL:addNode()

end