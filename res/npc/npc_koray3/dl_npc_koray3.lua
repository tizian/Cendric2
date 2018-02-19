-- Dialogue for NPC "npc_koray3"
loadDialogue = function(DL) 

	if (not DL:isConditionFulfilled("npc_koray3", "talked")) then 
		DL:setRoot(1) 
	else 
		DL:setRoot(2) 
	end 

	if (not DL:isConditionFulfilled("npc_koray3", "talked")) then 

		DL:createNPCNode(1, 3, "DL_Koray_Fight") -- Well, what can I say.
		DL:addConditionProgress("npc_koray3", "talked")
		DL:addNode()


		DL:createNPCNode(3, 4, "DL_Koray_Fight2") -- This was a truly amazing fight.
		DL:addNode()


		DL:createNPCNode(4, -2, "DL_Koray_Fight3") -- Good job! We're free again.
		DL:changeQuestState("missing_koray", "completed")
		DL:addNode()

	end


	DL:createChoiceNode(2)
	DL:addChoice(-1, "") -- 
	DL:addNode()

end