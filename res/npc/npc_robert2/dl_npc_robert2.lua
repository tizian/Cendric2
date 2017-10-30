-- Dialogue for NPC "npc_robert2"
loadDialogue = function(DL) 

	if (not DL:isConditionFulfilled("npc_robert2", "talked")) then 
		DL:setRoot(1) 
	else 
		DL:setRoot(2) 
	end 

	if (not DL:isConditionFulfilled("npc_robert2", "talked")) then 

		DL:createNPCNode(1, -2, "DL_Robert_Hello") -- There's nothing to see here, just walk on.
		DL:addConditionProgress("npc_robert2", "talked")
		DL:addNode()

	end


	DL:createChoiceNode(2)
	if (not DL:isConditionFulfilled("npc_robert2", "cemetery")) then 
		DL:addChoice(3, "DL_Choice_WhatDoing") -- What are you doing on the cemetery?
	end
	if (not DL:isConditionFulfilled("npc_robert2", "who_are_you")) then 
		DL:addChoice(4, "DL_Choice_WhoAreYou") -- Who are you?
	end
	if (not DL:isConditionFulfilled("npc_robert2", "from_here")) then 
		DL:addChoice(5, "DL_Choice_FromHere") -- Are you living here in this village?
	end
	if (not DL:isConditionFulfilled("npc_robert2", "crypt") and DL:isConditionFulfilled("npc_robert2", "cemetery")) then 
		DL:addChoice(6, "DL_Choice_Crypt") -- Have you been inside the crypt then?
	end
	if (DL:isConditionFulfilled("npc_robert2", "crypt") and not DL:isConditionFulfilled("npc_robert2", "teleport_quest")) then 
		DL:addChoice(11, "DL_Choice_TeleportQuest") -- What's in for me if I help you get your stone back?
	end
	if (DL:isConditionFulfilled("npc_robert2", "teleport_quest") and not DL:isConditionFulfilled("npc_robert2", "afraid") and not DL:isQuestState("teleport_robert", "completed")) then 
		DL:addChoice(15, "DL_Choice_Afraid") -- Why are you so afraid of that skeleton?
	end
	if (DL:isConditionFulfilled("npc_robert2", "teleport_quest") and DL:isQuestState("teleport_robert", "void")) then 
		DL:addChoice(14, "DL_Choice_StartRobertQuest") -- I'll get your stone from the crypt.
	end
	if (DL:isQuestState("teleport_robert", "started") and DL:isQuestComplete("teleport_robert")) then 
		DL:addChoice(16, "DL_Choice_FinishRobertQuest") -- I got your stone.
	end
	if (DL:isQuestState("silkweed_potion", "started") and not DL:isConditionFulfilled("npc_robert2", "cairn") and not DL:isConditionFulfilled("default", "plateau_open")) then 
		DL:addChoice(21, "DL_Choice_Cairn") -- Have you seen any cairn wraiths around here?
	end
	if (not DL:isQuestState("teleport_robert", "void")) then 
		DL:addChoice(-1, "DL_Choice_CU") -- See you.
	end
	DL:addNode()

	if (not DL:isConditionFulfilled("npc_robert2", "cemetery")) then 

		DL:createNPCNode(3, -2, "DL_Robert_WhatDoing") -- Hmm, nothing. But I strongly advise you not to go down into the crypt. It's spooky there. 
		DL:addConditionProgress("npc_robert2", "cemetery")
		DL:addNode()

	end

	if (not DL:isConditionFulfilled("npc_robert2", "who_are_you")) then 

		DL:createNPCNode(4, 7, "DL_Robert_WhoAreYou") -- I'm Bob and I possess magical powers! Impressive, ain't it.
		DL:addConditionProgress("npc_robert2", "who_are_you")
		DL:addNode()


		DL:createChoiceNode(7)
		DL:addChoice(-2, "DL_Choice_VeryImpressive") -- Very impressive, indeed.
		DL:addChoice(8, "DL_Choice_NotVeryImpressed") -- Hm. How's that special?
		DL:addNode()


		DL:createNPCNode(8, -2, "DL_Robert_NotVeryImpressed") -- Not many are able to wield magic here. You'd better show some respect.
		DL:addNode()

	end

	if (not DL:isConditionFulfilled("npc_robert2", "from_here")) then 

		DL:createNPCNode(5, -2, "DL_Robert_LivingHere") -- No, I'm not from here. I'm just passing through.
		DL:addConditionProgress("npc_robert2", "from_here")
		DL:addNode()

	end

	if (not DL:isConditionFulfilled("npc_robert2", "crypt") and DL:isConditionFulfilled("npc_robert2", "cemetery")) then 

		DL:createNPCNode(6, 9, "DL_Robert_Crypt") -- Hm. Yes. Listen, I was just trying out my powers... and then a little mistake happened. 
		DL:addConditionProgress("npc_robert2", "crypt")
		DL:addNode()


		DL:createNPCNode(9, 10, "DL_Robert_Crypt2") -- A skeleton in the crypt suddenly started to move! I was so shocked that I ran out of the crypt and then I noticed that my teleport stone was missing. 
		DL:addNode()


		DL:createNPCNode(10, -2, "DL_Robert_Crypt3") -- But I really need this stone! I can't get away without it.
		DL:addNode()

	end

	if (DL:isConditionFulfilled("npc_robert2", "crypt") and not DL:isConditionFulfilled("npc_robert2", "teleport_quest")) then 

		DL:createNPCNode(11, 12, "DL_Robert_TeleportQuest") -- You? You look like a mere commoner. I don't think you can help me.
		DL:addConditionProgress("npc_robert2", "teleport_quest")
		DL:addNode()


		DL:createNPCNode(12, 13, "DL_Robert_TeleportQuest2") -- But if you manage to get my stone back... Well, I have an inactive teleport stone here.
		DL:addNode()


		DL:createNPCNode(13, -2, "DL_Robert_TeleportQuest3") -- You could have it. It's worth a lot to mages, so you could sell it.
		DL:addNode()

	end

	if (DL:isConditionFulfilled("npc_robert2", "teleport_quest") and not DL:isConditionFulfilled("npc_robert2", "afraid") and not DL:isQuestState("teleport_robert", "completed")) then 

		DL:createNPCNode(15, -2, "DL_Robert_Afraid") -- I'm not afraid. Only cautious.
		DL:addConditionProgress("npc_robert2", "afraid")
		DL:addNode()

	end

	if (DL:isConditionFulfilled("npc_robert2", "teleport_quest") and DL:isQuestState("teleport_robert", "void")) then 

		DL:createNPCNode(14, -2, "DL_Robert_StartRobertQuest") -- Maybe you're lucky. I'll waith here.
		DL:changeQuestState("teleport_robert", "started")
		DL:addNode()

	end

	if (DL:isQuestState("teleport_robert", "started") and DL:isQuestComplete("teleport_robert")) then 

		DL:createNPCNode(16, 17, "DL_Robert_FinishRobertQuest") -- Wow. How did you... Well, that doesn't matter now. Thanks.
		DL:removeItem("qe_portstone_bob", 1)
		DL:changeQuestState("teleport_robert", "completed")
		DL:addNode()


		DL:createNPCNode(17, 18, "DL_Robert_FinishRobertQuest2") -- As promised, you get the inactive stone.
		DL:addItem("qe_portstone_inactive", 1)
		DL:addNode()


		DL:createNPCNode(18, 19, "DL_Robert_FinishRobertQuest3") -- If you'd be a mage, you'd be able to infuse it with magic from a cairn wraith.
		DL:addNode()


		DL:createNPCNode(19, 20, "DL_Robert_FinishRobertQuest4") -- I think I've just seen one coming to the windy plateau behind the crypt. 
		DL:changeQuestState("teleport_own", "started")
		DL:addNode()


		DL:createNPCNode(20, -1, "DL_Robert_FinishRobertQuest5") -- Good luck with it. I'm off then. (Robert takes the stone and disappears)
		DL:addConditionProgress("npc_robert2", "cairn_active")
		DL:addNode()

	end

	if (DL:isQuestState("silkweed_potion", "started") and not DL:isConditionFulfilled("npc_robert2", "cairn") and not DL:isConditionFulfilled("default", "plateau_open")) then 

		DL:createNPCNode(21, 22, "DL_Robert_Cairn") -- Hm... Maybe on the plateau behind the crypt.
		DL:addConditionProgress("npc_robert2", "cairn")
		DL:addNode()


		DL:createNPCNode(22, -2, "DL_Robert_Cairn2") -- But you'd need to go through the crypt for that... (shivers)
		DL:addNode()

	end

	if (not DL:isQuestState("teleport_robert", "void")) then 
	end

end