-- Dialogue for NPC "npc_adelya"
loadDialogue = function(DL) 

	DL:setRoot(1) 


	DL:createNPCNode(1, 2, "DL_Adelya_BeGone") -- Don't talk to me! Can't you see I'm busy?
	DL:addNode()


	DL:createChoiceNode(2)
	if (not DL:isConditionFulfilled("npc_adelya", "money") and DL:hasItem("gold", 10)) then 
		DL:addItemChoice(3, "DL_Choice_Money", "gold", 10) -- (Slip her some gold)
	end
	DL:addChoice(-1, "") -- 
	DL:addNode()

	if (not DL:isConditionFulfilled("npc_adelya", "money") and DL:hasItem("gold", 10)) then 

		DL:createNPCNode(3, -1, "DL_Adelya_Money") -- (Adelya blows you a kiss)
		DL:removeGold(10)
		DL:addConditionProgress("npc_adelya", "mone")
		DL:addNode()

	end

end