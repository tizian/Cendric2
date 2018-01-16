-- Dialogue for NPC "npc_karma"
loadDialogue = function(DL) 

	if (not DL:isConditionFulfilled("npc_mona", "sex_exotic")) then 
		DL:setRoot(1) 
	else 
		DL:setRoot(2) 
	end 

	if (not DL:isConditionFulfilled("npc_mona", "sex_exotic")) then 

		DL:createNPCNode(1, -1, "DL_Karma_NotBought") -- Go and ask Mona if you want to have an exciting time with me. (Winks)
		DL:addNode()

	end


	DL:createChoiceNode(2)
	if (not DL:isConditionFulfilled("npc_karma", "sex_karma")) then 
		DL:addChoice(3, "DL_Choice_Hey") -- Hey...
	end
	DL:addChoice(-1, "DL_Choice_End") -- See you.
	DL:addNode()

	if (not DL:isConditionFulfilled("npc_karma", "sex_karma")) then 

		DL:createNPCNode(3, -2, "DL_Karma_SexyBoy") -- Hey, sexy boy.
		DL:addConditionProgress("npc_karma", "sex_karma")
		DL:addNode()

	end

end