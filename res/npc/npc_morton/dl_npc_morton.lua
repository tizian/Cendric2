-- Dialogue for NPC "npc_morton"
loadDialogue = function(DL) 

	if (not DL:isConditionFulfilled("npc_morton", "talked")) then 
		DL:setRoot(1) 
	else 
		DL:setRoot(2) 
	end 

	if (not DL:isConditionFulfilled("npc_morton", "talked")) then 

		DL:createNPCNode(1, -2, "DL_Morton_BeGone") -- What are you doing here? You're disturbing the peace of the dead. Be gone!
		DL:addConditionProgress("npc_morton", "talked")
		DL:addNode()

	end


	DL:createChoiceNode(2)
	if (DL:isQuestState("favor_for_a_favor", "started") and not DL:isConditionFulfilled("npc_morton", "crypt_open")) then 
		DL:addChoice(3, "DL_Choice_Syrah") -- Syrah sent me. I need to go into that crypt.
	end
	DL:addChoice(-1, "") -- 
	DL:addNode()

	if (DL:isQuestState("favor_for_a_favor", "started") and not DL:isConditionFulfilled("npc_morton", "crypt_open")) then 

		DL:createNPCNode(3, 4, "DL_Morton_Syrah") -- Ah, Syrah. (Morton stares at you) Why does she always hire guys like you. 
		DL:addNode()


		DL:createNPCNode(4, -2, "DL_Morton_Syrah2") -- (Sighs) You can go in. But hurry up and don't touch anything you don't need.
		DL:addConditionProgress("npc_morton", "crypt_open")
		DL:addNode()

	end

end