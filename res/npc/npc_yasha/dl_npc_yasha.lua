-- Dialogue for NPC "npc_yasha"
loadDialogue = function(DL) 

	if (not DL:isConditionFulfilled("npc_yasha", "talked")) then 
		DL:setRoot(1) 
	else 
		DL:setRoot(2) 
	end 

	if (not DL:isConditionFulfilled("npc_yasha", "talked")) then 

		DL:createNPCNode(1, -2, "DL_Yasha_Hi") -- (Growls) Another mortal dares to trespass into my temple. Any last words?
		DL:addConditionProgress("npc_yasha", "talked")
		DL:addNode()

	end


	DL:createChoiceNode(2)
	DL:addChoice(3, "Choice_Yasha_Attack") -- [ATTACK]
	DL:addChoice(-1, "") -- 
	DL:addNode()


	DL:createNPCNode(3, -1, "") -- 
	DL:startLevel("res/level/boss_yasha/boss_yasha.tmx", 1, 1)
	DL:addNode()

end