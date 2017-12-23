-- Dialogue for NPC "npc_inina4"
loadDialogue = function(DL) 

	DL:setRoot(1) 


	DL:createChoiceNode(1)
	if (not DL:isConditionFulfilled("npc_inina4", "ok")) then 
		DL:addChoice(2, "DL_Choice_Ok") -- Is everything alright?
	end
	DL:addChoice(-1, "") -- 
	DL:addNode()

	if (not DL:isConditionFulfilled("npc_inina4", "ok")) then 

		DL:createNPCNode(2, 3, "DL_Inina_Ok") -- Yes. We're no longer under his spell.
		DL:addConditionProgress("npc_inina4", "ok")
		DL:addNode()


		DL:createNPCNode(3, -2, "DL_Inina_Ok2") -- Thank you so much for saving me. I'll never forget this.
		DL:changeQuestState("missing_inina", "completed")
		DL:addNode()

	end

end