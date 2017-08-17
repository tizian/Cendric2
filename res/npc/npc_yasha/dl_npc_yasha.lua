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
	if (DL:isQuestState("yasha_sanctuary", "completed")) then 
		DL:addChoice(4, "DL_Choice_YashaSanctuaryDone") -- Can we have a peaceful talk, demon?
	end
	DL:addChoice(3, "Choice_Yasha_Attack") -- [ATTACK]
	DL:addChoice(-1, "") -- 
	DL:addNode()

	if (DL:isQuestState("yasha_sanctuary", "completed")) then 

		DL:createNPCNode(4, 5, "DL_Yasha_YashaSanctuaryDone") -- (Yasha stares at you) Hrr. You did traverse my temple without touching my sacred urns. No human did overcome the temptation to touching them before. 
		DL:addConditionProgress("npc_yasha", "friendly")
		DL:addNode()


		DL:createNPCNode(5, -2, "DL_Yasha_YashaSanctuaryDone2") -- I will answer all of your questions, human.
		DL:changeQuestState("yasha_sanctuary", "completed")
		DL:addNode()

	end


	DL:createNPCNode(3, -1, "") -- 
	DL:startLevel("res/level/boss_yasha/boss_yasha.tmx", 1, 1)
	DL:addNode()

end