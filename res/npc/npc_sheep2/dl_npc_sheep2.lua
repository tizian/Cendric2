-- Dialogue for NPC "npc_sheep2"
loadDialogue = function(DL)

	DL:createNPCNode(0, 1, "DL_Sheep_Bah") -- Baaah!
	DL:addNode()
	
	DL:createCendricNode(1, -1, "DL_Cendric_Nothing") -- ...
	DL:addNode()
	
	DL:setRoot(0)
	
end	
	