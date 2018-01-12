-- Dialogue for NPC "npc_chickenchaser"
loadDialogue = function(DL) 

	DL:setRoot(1) 


	DL:createNPCNode(1, -1, "DL_Chickenchaser_Ah") -- Aaaaaaaaaah!
	DL:addItem("eq_chicken", 1)
	DL:unlockAchievement("ACH_CHICKEN_SHOCKER")
	DL:addConditionProgress("npc_chickenchaser", "dies")
	DL:addNode()

end