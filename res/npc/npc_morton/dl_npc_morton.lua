-- Dialogue for NPC "npc_morton"
loadDialogue = function(DL) 

	if (not DL:isConditionFulfilled("npc_morton", "talked")) then 
		DL:setRoot(1) 
	elseif (DL:isSpellLearned(12) and not DL:isConditionFulfilled("npc_morton", "necromancers")) then 
		DL:setRoot(5) 
	else 
		DL:setRoot(2) 
	end 

	if (not DL:isConditionFulfilled("npc_morton", "talked")) then 

		DL:createNPCNode(1, -2, "DL_Morton_BeGone") -- What are you doing here? You're disturbing the peace of the dead. Be gone!
		DL:addConditionProgress("npc_morton", "talked")
		DL:addNode()

	end

	if (DL:isSpellLearned(12) and not DL:isConditionFulfilled("npc_morton", "necromancers")) then 

		DL:createNPCNode(5, -2, "DL_Morton_Necromancers") -- Be careful with this magic, boy...
		DL:addConditionProgress("npc_morton", "necromancers")
		DL:addNode()

	end


	DL:createChoiceNode(2)
	if (DL:isQuestState("favor_for_a_favor", "started") and not DL:isConditionFulfilled("npc_morton", "crypt_open")) then 
		DL:addChoice(3, "DL_Choice_Syrah") -- Syrah sent me. I need to go into that crypt.
	end
	if (not DL:isConditionFulfilled("npc_morton", "guys_like_me") and DL:isConditionFulfilled("npc_morton", "crypt_open")) then 
		DL:addChoice(6, "DL_Choice_GuysLikeMe") -- What do you mean with "guys like me"?
	end
	if (not DL:isConditionFulfilled("npc_morton", "other_guy") and DL:isConditionFulfilled("npc_morton", "guys_like_me")) then 
		DL:addChoice(7, "DL_Choice_OtherGuy") -- She's sent another guy?
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

	if (not DL:isConditionFulfilled("npc_morton", "guys_like_me") and DL:isConditionFulfilled("npc_morton", "crypt_open")) then 

		DL:createNPCNode(6, -2, "DL_Morton_GuysLikeMe") -- Green no-goods. The last one she sent to the crypt has never gotten out again.
		DL:addConditionProgress("npc_morton", "guys_like_me")
		DL:addNode()

	end

	if (not DL:isConditionFulfilled("npc_morton", "other_guy") and DL:isConditionFulfilled("npc_morton", "guys_like_me")) then 

		DL:createNPCNode(7, -1, "DL_Morton_OtherGuy") -- Yes, yes. Red hair, seemed to be as old as you and exactly as greenly. But no more chit-chat, I need to work.
		DL:addConditionProgress("npc_morton", "other_guy")
		DL:addNode()

	end

end