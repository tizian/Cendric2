-- Dialogue for NPC "npc_inina2"
loadDialogue = function(DL) 

	if (not DL:isConditionFulfilled("npc_inina2", "talked")) then 
		DL:setRoot(1) 
	else 
		DL:setRoot(2) 
	end 

	if (not DL:isConditionFulfilled("npc_inina2", "talked")) then 

		DL:createNPCNode(1, 3, "DL_Inina_StrangePassage") -- Startling, isn't it? Those caverns led directly into the sewers of Gandria, without us noticing it.
		DL:addNode()


		DL:createNPCNode(3, 4, "DL_Inina_StrangePassage2") -- I've tried to follow the passage over there, but it has been blocked somehow.
		DL:addNode()


		DL:createNPCNode(4, -1, "DL_Inina_StrangePassage3") -- I don't know a spell that could push through this blockade. Maybe you could succeed?
		DL:addConditionProgress("npc_inina2", "talked")
		DL:addQuestDescription("further_investigation", 1)
		DL:addNode()

	end


	DL:createChoiceNode(2)
	DL:addChoice(-1, "") -- 
	DL:addNode()

end