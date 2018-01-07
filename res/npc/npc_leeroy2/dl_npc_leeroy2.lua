-- Dialogue for NPC "npc_leeroy2"
loadDialogue = function(DL) 

	if (DL:isQuestState("save_leeroy", "completed")) then 
		DL:setRoot(1) 
	else 
		DL:setRoot(2) 
	end 

	if (DL:isQuestState("save_leeroy", "completed")) then 

		DL:createNPCNode(1, -1, "DL_Leeroy_Thanks") -- Thank you.
		DL:addNode()

	end


	DL:createNPCNode(2, 3, "DL_Leeroy_DidIt") -- I did it! See, I was not wrong about those DRAGONS after all.
	DL:changeQuestState("save_leeroy", "completed")
	DL:unlockAchievement("ACH_LEEROY")
	DL:addNode()


	DL:createNPCNode(3, 4, "DL_Leeroy_DidIt2") -- But what if they come back and you need to fight them all alone?
	DL:addNode()


	DL:createNPCNode(4, 5, "DL_Leeroy_DidIt3") -- Here, you'd better take my sword. I don't need it anymore.
	DL:addItem("we_leeroysword", 1)
	DL:addNode()


	DL:createNPCNode(5, -1, "DL_Leeroy_DidIt4") -- And now, I'm off to a new adventure! See ya.
	DL:addNode()

end