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
		DL:addChoice(10, "DL_Choice_LeeroyChicken") -- I could get you some chicken.
	end
	if (DL:isConditionFulfilled("npc_leeroy", "why_stop") and not DL:isConditionFulfilled("npc_leeroy", "not_bright")) then 
		DL:addChoice(13, "DL_Choice_NotBright") -- You're not the sharpest knife in the drawer, are you.
	end
	if (DL:isQuestState("leeroy_chicken", "started") and DL:isQuestComplete("leeroy_chicken")) then 
		DL:addChoice(14, "DL_Choice_ChickenDone") -- I got your chicken.
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


		DL:createNPCNode(9, 12, "DL_Leeroy_CrossBridge2") -- But I can't fight with an empty stomach - I need some tasty chicken first.
		DL:addNode()


		DL:createNPCNode(12, -2, "DL_Leeroy_CrossBridge3") -- Unfortunately, I need to hold the line here. I can't let some other guy steal my DRAGONS.
		DL:addConditionProgress("npc_leeroy", "cross_bridge")
		DL:addNode()

	end

	if (DL:isConditionFulfilled("npc_leeroy", "cross_bridge") and DL:isQuestState("leeroy_chicken", "void")) then 

		DL:createNPCNode(10, 11, "DL_Leeroy_LeeroyChicken") -- That would be great. But I only eat the finest chicken.
		DL:addNode()


		DL:createNPCNode(11, -2, "DL_Leeroy_LeeroyChicken2") -- You know the innkeeper Helena in Gandria? Her special grilled chicken is second to none. Bring me one of those and I'll slay those DRAGONS for you.
		DL:changeQuestState("leeroy_chicken", "started")
		DL:addNode()

	end

	if (DL:isConditionFulfilled("npc_leeroy", "why_stop") and not DL:isConditionFulfilled("npc_leeroy", "not_bright")) then 

		DL:createNPCNode(13, -2, "DL_Leeroy_NotBright") -- I prefer swords over knives, you know.
		DL:addConditionProgress("npc_leeroy", "not_bright")
		DL:addNode()

	end

	if (DL:isQuestState("leeroy_chicken", "started") and DL:isQuestComplete("leeroy_chicken")) then 

		DL:createNPCNode(14, 15, "DL_Leeroy_ChickenDone") -- And why did it take you so long? I nearly starved! (Leeroy takes the chicken and devours it hastily)
		DL:removeItem("mi_leeroychicken", 1)
		DL:changeQuestState("leeroy_chicken", "completed")
		DL:addNode()


		DL:createNPCNode(15, 16, "DL_Leeroy_ChickenDone2") -- But now I'm ready. Let's do this!
		DL:addConditionProgress("default", "swampbridge_open")
		DL:addNode()


		DL:createNPCNode(16, -1, "") -- 
		DL:startLevel("res/level/swampbridge/swampbridgte.tmx", 1, 1)
		DL:addNode()

	end

end