-- Dialogue for NPC "npc_velius"
loadDialogue = function(DL) 

	DL:setRoot(1) 


	DL:createChoiceNode(1)
	DL:addChoice(2, "DL_Choice_TryOut") -- Let's fight, shall we?
	DL:addChoice(-1, "") -- 
	DL:addNode()


	DL:createNPCNode(2, 3, "DL_Velius_Finally") -- Finally.
	DL:addNode()


	DL:createNPCNode(3, -1, "") -- 
	DL:startLevel("res/level/boss_velius/boss_velius.tmx", 100, 200)
	DL:addNode()

end