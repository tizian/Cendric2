-- Dialogue for NPC "npc_morton"
loadDialogue = function(DL) 

	if (not DL:isConditionFulfilled("npc_morton", "talked")) then 
		DL:setRoot(1) 
	else 
		DL:setRoot(2) 
	end 

	if (not DL:isConditionFulfilled("npc_morton", "talked")) then 

		DL:createNPCNode(1, -2, "DL_Morton_BeGone") -- What are you doing here? You're disturbing the peace of the dead. Be gone!
		DL:addConditionProgress("npc_morton", "talked")
		DL:addNode()

	end


	DL:createChoiceNode(2)
	DL:addChoice(-1, "") -- 
	DL:addNode()

end