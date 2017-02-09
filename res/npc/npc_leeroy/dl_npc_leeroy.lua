-- Dialogue for NPC "npc_leeroy"
loadDialogue = function(DL) 

	if (not DL:isConditionFulfilled("npc_leeroy", "talked")) then 
		DL:setRoot(1) 
	else 
		DL:setRoot(2) 
	end 

	if (not DL:isConditionFulfilled("npc_leeroy", "talked")) then 

		DL:createNPCNode(1, -2, "DL_Leeroy_Stop") -- Hey you, stop right there! You can't cross that bridge.
		DL:addConditionProgress("npc_leeroy", "talked")
		DL:addNode()

	end


	DL:createChoiceNode(2)
	if (not DL:isConditionFulfilled("npc_leeroy", "why_stop")) then 
		DL:addChoice(3, "DL_Choice_WhyStop") -- Why can't I cross that bridge?
	end
	if (not DL:isConditionFulfilled("npc_leeroy", "who_are_you")) then 
		DL:addChoice(5, "DL_Choice_WhoAreYou") -- Who are you?
	end
	if (DL:isConditionFulfilled("npc_leeroy", "why_stop") and not DL:isConditionFulfilled("npc_leeroy", "no_dragons")) then 
		DL:addChoice(7, "DL_Choice_NoDragons") -- I don't see any dragons.
	end
	if (DL:isConditionFulfilled("npc_leeroy", "why_stop") and not DL:isConditionFulfilled("npc_leeroy", "cross_bridge")) then 
		DL:addChoice(8, "DL_Choice_CrossBridge") -- But I want to cross the bridge!
	end
	if (DL:isConditionFulfilled("npc_leeroy", "cross_bridge") and DL:isQuestState("leeroy_chicken", "void")) then 
		DL:addChoice(10, "DL_Choice_LeeroyChicken") -- I could easily kill a dragon...
	end
	if (DL:isConditionFulfilled("npc_leeroy", "why_stop") and not DL:isConditionFulfilled("npc_leeroy", "not_bright")) then 
		DL:addChoice(13, "DL_Choice_NotBright") -- You're not the sharpest knife in the drawer, are you.
	end
	DL:addChoice(-1, "") -- 
	DL:addNode()

	if (not DL:isConditionFulfilled("npc_leeroy", "why_stop")) then 

		DL:createNPCNode(3, 4, "DL_Leeroy_WhyStop") -- You won't believe it, but there are DRAGONS on this bridge.
		DL:addNode()


		DL:createNPCNode(4, -2, "DL_Leeroy_WhyStop2") -- I'm just making sure nobody gets mauled before I've slain them.
		DL:addConditionProgress("npc_leeroy", "why_stop")
		DL:addNode()

	end

	if (not DL:isConditionFulfilled("npc_leeroy", "who_are_you")) then 

		DL:createNPCNode(5, 6, "DL_Leeroy_WhoAreYou") -- I'm Leeroy, former paladin of the Order. Lloyd fired me because I didn't follow his stupid orders.
		DL:addNode()


		DL:createNPCNode(6, -2, "DL_Leeroy_WhoAreYou2") -- They think they don't need me but I'll show them! I'm gonna slay those DRAGONS!
		DL:addConditionProgress("npc_leeroy", "who_are_you")
		DL:addNode()

	end

	if (DL:isConditionFulfilled("npc_leeroy", "why_stop") and not DL:isConditionFulfilled("npc_leeroy", "no_dragons")) then 

		DL:createNPCNode(7, -2, "DL_Leeroy_NoDragons") -- D'uh, that's because they're hiding! But they are just waiting for someone like you to feast on.
		DL:addConditionProgress("npc_leeroy", "no_dragons")
		DL:addNode()

	end

	if (DL:isConditionFulfilled("npc_leeroy", "why_stop") and not DL:isConditionFulfilled("npc_leeroy", "cross_bridge")) then 

		DL:createNPCNode(8, 9, "DL_Leeroy_CrossBridge") -- You can't because if someone's going to slay those DRAGONS, that's gonna be me.
		DL:addNode()


		DL:createNPCNode(9, -2, "DL_Leeroy_CrossBridge2") -- And if you want to join me, you only can do so if you're able to kill a dragon.
		DL:addConditionProgress("npc_leeroy", "cross_bridge")
		DL:addNode()

	end

	if (DL:isConditionFulfilled("npc_leeroy", "cross_bridge") and DL:isQuestState("leeroy_chicken", "void")) then 

		DL:createNPCNode(10, 11, "DL_Leeroy_LeeroyChicken") -- Oh yeah? Well you have to prove it. Bring me a skull of a dragon and I'll let you on that bridge.
		DL:changeQuestState("leeroy_chicken", "started")
		DL:addNode()


		DL:createNPCNode(11, -2, "DL_Leeroy_LeeroyChicken2") -- Oh, and get me some roasted chicken, fighting with an empty stomach is never a good idea.
		DL:addNode()

	end

	if (DL:isConditionFulfilled("npc_leeroy", "why_stop") and not DL:isConditionFulfilled("npc_leeroy", "not_bright")) then 

		DL:createNPCNode(13, -2, "DL_Leeroy_NotBright") -- I prefer swords over knives, you know.
		DL:addConditionProgress("npc_leeroy", "not_bright")
		DL:addNode()

	end

end