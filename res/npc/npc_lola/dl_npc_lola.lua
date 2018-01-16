-- Dialogue for NPC "npc_lola"
loadDialogue = function(DL) 

	if (not DL:isConditionFulfilled("npc_mona", "sex_romantic")) then 
		DL:setRoot(1) 
	else 
		DL:setRoot(2) 
	end 

	if (not DL:isConditionFulfilled("npc_mona", "sex_romantic")) then 

		DL:createNPCNode(1, -1, "DL_Lola_NotBought") -- Talk to Mona first if you want to have some fun, cutie.
		DL:addNode()

	end


	DL:createChoiceNode(2)
	if (not DL:isConditionFulfilled("npc_lola", "sex_lola")) then 
		DL:addChoice(3, "DL_Choice_Hey") -- Hey...
	end
	DL:addChoice(-1, "DL_Choice_End") -- See you.
	DL:addNode()

	if (not DL:isConditionFulfilled("npc_lola", "sex_lola")) then 

		DL:createNPCNode(3, -2, "DL_Lola_SexyBoy") -- Hey, sexy boy.
		DL:addConditionProgress("npc_lola", "sex_lola")
		DL:addNode()

	end

end