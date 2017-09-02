-- Dialogue for NPC "npc_lola"
loadDialogue = function(DL) 

	if (not DL:isConditionFulfilled("npc_mona", "lola")) then 
		DL:setRoot(1) 
	else 
		DL:setRoot(2) 
	end 

	if (not DL:isConditionFulfilled("npc_mona", "lola")) then 

		DL:createNPCNode(1, -1, "DL_Lola_NotBought") -- Talk to Mona first if you want to have some fun, cutie.
		DL:addNode()

	end


	DL:createChoiceNode(2)
	DL:addChoice(-1, "DL_Choice_End") -- See you.
	DL:addNode()

end