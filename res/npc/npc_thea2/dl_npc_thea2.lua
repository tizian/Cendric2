-- Dialogue for NPC "npc_thea2"
loadDialogue = function(DL) 

	if (not DL:isConditionFulfilled("npc_thea2", "talked")) then 
		DL:setRoot(1) 
	else 
		DL:setRoot(2) 
	end 

	if (not DL:isConditionFulfilled("npc_thea2", "talked")) then 

		DL:createNPCNode(1, 3, "DL_Thea_Hey") -- Oh, here you are! My apprenticeship is going really well so far.
		DL:addNode()


		DL:createNPCNode(3, -2, "DL_Thea_Hey2") -- I wouldn't be here without you. Here, take this herb, it's very rare and might be useful for you. (Winks)
		DL:addConditionProgress("npc_thea2", "talked")
		DL:addItem("fo_stormweed", 1)
		DL:addNode()

	end


	DL:createChoiceNode(2)
	DL:addChoice(-1, "") -- 
	DL:addNode()

end