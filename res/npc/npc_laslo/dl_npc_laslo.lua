-- Dialogue for NPC "npc_laslo"
loadDialogue = function(DL) 

	DL:setRoot(1) 


	DL:createChoiceNode(1)
	DL:addChoice(2, "DL_Choice_intro") -- [intro cutscene]
	DL:addChoice(3, "DL_Choice_silkweed_potion") -- [silkweed_potion cutscene]
	DL:addChoice(4, "DL_Choice_gandria") -- [gandria cutscene]
	DL:addChoice(5, "DL_Choice_map_decrypt") -- [map_decrypt cutscene]
	DL:addChoice(6, "DL_Choice_chapter_4") -- [chapter_4 cutscene]
	DL:addChoice(-1, "") -- 
	DL:addNode()


	DL:createNPCNode(2, -1, "") -- 
	DL:startCutscene("intro")
	DL:addNode()


	DL:createNPCNode(3, -1, "") -- 
	DL:startCutscene("silkweed_potion")
	DL:addNode()


	DL:createNPCNode(4, -1, "") -- 
	DL:startCutscene("gandria")
	DL:addNode()


	DL:createNPCNode(5, -1, "") -- 
	DL:startCutscene("map_decrypt/map_decrypt_divine")
	DL:addNode()


	DL:createNPCNode(6, -1, "") -- 
	DL:startCutscene("chapter_4")
	DL:addNode()

end