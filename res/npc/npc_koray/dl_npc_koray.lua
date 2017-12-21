-- Dialogue for NPC "npc_koray"
loadDialogue = function(DL) 

	if (DL:isQuestState("help_koray", "void")) then 
		DL:setRoot(1) 
	elseif (DL:isQuestState("help_koray", "started")) then 
		DL:setRoot(22) 
	else 
		DL:setRoot(-1) 
	end 

	if (DL:isQuestState("help_koray", "void")) then 

		DL:createChoiceNode(1)
		if (not DL:isConditionFulfilled("npc_koray", "what_doing")) then 
			DL:addChoice(2, "DL_Choice_WhatDoing") -- What are you doing here?
		end
		if (not DL:isConditionFulfilled("npc_koray", "nuray") and DL:isQuestState("missing_koray", "started")) then 
			DL:addChoice(5, "DL_Choice_Nuray") -- Nuray is looking for you.
		end
		if (not DL:isConditionFulfilled("npc_koray", "earth")) then 
			DL:addChoice(7, "DL_Choice_Earth") -- Do you know something about earth magic?
		end
		if (DL:isConditionFulfilled("npc_koray", "what_doing") and not DL:isConditionFulfilled("npc_koray", "business")) then 
			DL:addChoice(8, "DL_Choice_Business") -- Waiting for someone like me?
		end
		if (DL:isConditionFulfilled("npc_koray", "business") and not DL:isConditionFulfilled("npc_koray", "interested")) then 
			DL:addChoice(15, "DL_Choice_InterestedAgain") -- Tell me more about why you need an escort.
		end
		if (DL:isConditionFulfilled("npc_koray", "interested") and not DL:isConditionFulfilled("npc_koray", "telekinesis")) then 
			DL:addChoice(19, "DL_Choice_Telekinesis") -- What's this "telekinesis"?
		end
		if (DL:isConditionFulfilled("npc_koray", "telekinesis") and DL:isQuestState("help_koray", "void")) then 
			DL:addChoice(20, "DL_Choice_StartQuest") -- I'll escort you if you teach me your magic.
		end
		DL:addChoice(-1, "") -- 
		DL:addNode()

		if (not DL:isConditionFulfilled("npc_koray", "what_doing")) then 

			DL:createNPCNode(2, 3, "DL_Koray_WhatDoing") -- I'm waiting... for someone like you maybe. 
			DL:addConditionProgress("npc_koray", "what_doing")
			DL:addNode()


			DL:createNPCNode(3, 4, "DL_Koray_WhatDoing2") -- That paladin on the other side of the bridge was quite a weirdo.
			DL:addNode()


			DL:createNPCNode(4, -2, "DL_Koray_WhatDoing3") -- But you... you may be the one.
			DL:addNode()

		end

		if (not DL:isConditionFulfilled("npc_koray", "nuray") and DL:isQuestState("missing_koray", "started")) then 

			DL:createNPCNode(5, 6, "DL_Koray_Nuray") -- Oh, she doesn't have to worry. Everything's fine here. 
			DL:addConditionProgress("npc_koray", "nuray")
			DL:addNode()


			DL:createNPCNode(6, -2, "DL_Koray_Nuray2") -- I just got unfinished business here. After that, I'll return to Gandria.
			DL:addQuestDescription("missing_koray", 1)
			DL:addNode()

		end

		if (not DL:isConditionFulfilled("npc_koray", "earth")) then 

			DL:createNPCNode(7, -2, "DL_Koray_Earth") -- Maybe... (smiles)
			DL:addConditionProgress("npc_koray", "earth")
			DL:addNode()

		end

		if (DL:isConditionFulfilled("npc_koray", "what_doing") and not DL:isConditionFulfilled("npc_koray", "business")) then 

			DL:createNPCNode(8, 9, "DL_Koray_Business") -- Yes, you look like someone who knows how to handle a weapon.
			DL:addNode()


			DL:createNPCNode(9, 10, "DL_Koray_Business2") -- I need an escort. Are you interested?
			DL:addConditionProgress("npc_koray", "business")
			DL:addNode()


			DL:createChoiceNode(10)
			DL:addChoice(11, "DL_Choice_Interested") -- Tell me more about it.
			DL:addChoice(-1, "DL_Choice_NotInterested") -- I don't have time for this at the moment.
			DL:addNode()


			DL:createNPCNode(11, 12, "DL_Koray_Interested") -- Well, I lost a very precious necklace, the "Eye of the Moon". 
			DL:addConditionProgress("npc_koray", "interested")
			DL:addNode()


			DL:createNPCNode(12, 13, "DL_Koray_Interested2") -- I had to run away with it and jumped down from a small hill into the forest.
			DL:addNode()


			DL:createNPCNode(13, 14, "DL_Koray_Interested3") -- Unfortunately, it got caught in the branches of a tree and I can't get near it alone.
			DL:addNode()


			DL:createNPCNode(14, -2, "DL_Koray_Interested4") -- But if you could get me near that tree, I could get it using some telekinesis.
			DL:addNode()

		end

		if (DL:isConditionFulfilled("npc_koray", "business") and not DL:isConditionFulfilled("npc_koray", "interested")) then 

			DL:createNPCNode(15, 16, "DL_Koray_Interested") -- 
			DL:addConditionProgress("npc_koray", "interested")
			DL:addNode()


			DL:createNPCNode(16, 17, "DL_Koray_Interested2") -- 
			DL:addNode()


			DL:createNPCNode(17, 18, "DL_Koray_Interested3") -- 
			DL:addNode()


			DL:createNPCNode(18, -2, "DL_Koray_Interested4") -- 
			DL:addNode()

		end

		if (DL:isConditionFulfilled("npc_koray", "interested") and not DL:isConditionFulfilled("npc_koray", "telekinesis")) then 

			DL:createNPCNode(19, 21, "DL_Koray_Telekinesis") -- That's a type of earth magic, using the earth's magnetism to move objects. Quite handy if you ask me.
			DL:addConditionProgress("npc_koray", "telekinesis")
			DL:addNode()


			DL:createNPCNode(21, -2, "DL_Koray_Telekinesis2") -- Actually, I'm affine to twilight magic, but this spell kind of complements my skills.
			DL:addNode()

		end

		if (DL:isConditionFulfilled("npc_koray", "telekinesis") and DL:isQuestState("help_koray", "void")) then 

			DL:createNPCNode(20, -1, "DL_Koray_StartQuest") -- Alright. Follow me.
			DL:changeQuestState("help_koray", "started")
			DL:addNode()

		end

	end

	if (DL:isQuestState("help_koray", "started")) then 

		DL:createNPCNode(22, 23, "DL_Koray_GoIn") -- Let's go.
		DL:addConditionProgress("npc_koray", "level_start")
		DL:addNode()


		DL:createNPCNode(23, -1, "") -- 
		DL:startLevel("res/level/swampforest/swampforest.tmx", 1750, 1210)
		DL:addNode()

	end

end