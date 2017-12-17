-- Dialogue for NPC "npc_robert4"
loadDialogue = function(DL) 

	if (not DL:isConditionFulfilled("npc_robert4", "talked")) then 
		DL:setRoot(1) 
	else 
		DL:setRoot(2) 
	end 

	if (not DL:isConditionFulfilled("npc_robert4", "talked")) then 

		DL:createNPCNode(1, 3, "DL_Robert_YouDidIt") -- (Robert is still shivering) You... you did it!
		DL:addConditionProgress("npc_robert4", "talked")
		DL:addNode()


		DL:createNPCNode(3, -2, "DL_Robert_YouDidIt2") -- Maybe I underestimated you. Thank you...
		DL:addNode()

	end


	DL:createChoiceNode(2)
	DL:addChoice(-1, "") -- 
	DL:addNode()

end