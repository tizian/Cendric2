-- Dialogue for NPC "npc_nuray"
loadDialogue = function(DL) 

	if (DL:getGuild() == "thief" and not DL:isConditionFulfilled("npc_nuray", "congrats")) then 
		DL:setRoot(1) 
	elseif (not DL:isConditionFulfilled("npc_nuray", "who_are_you")) then 
		DL:setRoot(2) 
	else 
		DL:setRoot(4) 
	end 

	if (DL:getGuild() == "thief" and not DL:isConditionFulfilled("npc_nuray", "congrats")) then 

		DL:createNPCNode(1, -2, "DL_Nuray_Congratulations") -- So you're new here. Well done. Vincent won't let everyone join us.
		DL:addConditionProgress("npc_nuray", "congrats")
		DL:addNode()

	end

	if (not DL:isConditionFulfilled("npc_nuray", "who_are_you")) then 

		DL:createChoiceNode(2)
		DL:addChoice(3, "DL_Choice_WhoAreYou") -- Wer bist du?
		DL:addChoice(-1, "") -- 
		DL:addNode()


		DL:createNPCNode(3, 5, "DL_Nuray_WhoAreYou") -- 
		DL:addNode()


		DL:createNPCNode(5, -2, "DL_Nuray_WhoAreYou2") -- 
		DL:addConditionProgress("npc_nuray", "who_are_you")
		DL:addNode()

	end


	DL:createChoiceNode(4)
	DL:addChoice(-1, "") -- 
	DL:addNode()

end