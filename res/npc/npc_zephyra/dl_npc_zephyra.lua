-- Dialogue for NPC "npc_zephyra"
loadDialogue = function(DL) 

	if (not DL:isConditionFulfilled("npc_zephyra", "talked")) then 
		DL:setRoot(1) 
	else 
		DL:setRoot(2) 
	end 

	if (not DL:isConditionFulfilled("npc_zephyra", "talked")) then 

		DL:createNPCNode(1, -2, "DL_Zephyra_Hello") -- Don't come any closer! There's already enough damage done. Go home to where you belong!
		DL:addConditionProgress("npc_zephyra", "talked")
		DL:addNode()

	end


	DL:createChoiceNode(2)
	if (DL:isConditionFulfilled("npc_zephyra", "what_matter")) then 
		DL:addChoice(3, "DL_Choice_WhatsTheMatter") -- What's the matter?
	end
	if (not DL:isConditionFulfilled("npc_zephyra", "who_are_you")) then 
		DL:addChoice(4, "DL_Choice_WhoAreYou") -- Who are you?
	end
	if (not DL:isConditionFulfilled("npc_zephyra", "dead_cat")) then 
		DL:addChoice(5, "DL_Choice_DeadCat") -- Why are you wearing a dead cat on your head?
	end
	if (DL:isConditionFulfilled("npc_zephyra", "yasha") and not DL:isConditionFulfilled("npc_zephyra", "who_yasha")) then 
		DL:addChoice(9, "DL_Choice_WhoYasha") -- Tell me more about that "Yasha".
	end
	if (not DL:isConditionFulfilled("npc_zephyra", "who_droves")) then 
		DL:addChoice(10, "DL_Choice_WhoDroves") -- Who did pass through here?
	end
	if (DL:isConditionFulfilled("npc_zephyra", "who_droves") and not DL:isConditionFulfilled("npc_zephyra", "mages")) then 
		DL:addChoice(13, "DL_Choice_WhoMages") -- How did those mages look like?
	end
	if (DL:isQuestState("yasha_sanctuary", "completed") and not DL:isConditionFulfilled("npc_zephyra", "yasha_completed")) then 
		DL:addChoice(18, "DL_Choice_YashaDone") -- I talked to Yasha. She's pacified again.
	end
	if (DL:isConditionFulfilled("npc_zephyra", "droves") and not DL:isConditionFulfilled("npc_zephyra", "gotta_go")) then 
		DL:addChoice(19, "DL_Choice_GottaGo") -- Well, I gotta go now. Thanks for the information.
	end
	if (DL:isConditionFulfilled("npc_zephyra", "gotta_go")) then 
		DL:addChoice(-1, "") -- 
	end
	DL:addNode()

	if (DL:isConditionFulfilled("npc_zephyra", "what_matter")) then 

		DL:createNPCNode(3, -2, "DL_Zephyra_WhatsTheMatter") -- I haven't seen anyone passing through here in ages. And suddenly, they are coming in droves and destroying this sanctuary. 
		DL:addConditionProgress("npc_zephyra", "what_matter")
		DL:addNode()

	end

	if (not DL:isConditionFulfilled("npc_zephyra", "who_are_you")) then 

		DL:createNPCNode(4, -2, "DL_Zephyra_WhoAreYou") -- I'm Zephrya, hermit and shaman of Yasha.
		DL:addConditionProgress("npc_zephyra", "who_are_you")
		DL:addConditionProgress("npc_zephyra", "yasha")
		DL:addNode()

	end

	if (not DL:isConditionFulfilled("npc_zephyra", "dead_cat")) then 

		DL:createNPCNode(5, 6, "DL_Zephyra_DeadCat") -- That's no dead cat, it's a demon's fur. It's nice and warm.
		DL:addConditionProgress("npc_zephyra", "dead_cat")
		DL:addNode()


		DL:createChoiceNode(6)
		DL:addChoice(7, "DL_Choice_DemonSlayer") -- So you're a demon slayer?
		DL:addChoice(8, "DL_Choice_NiceCat") -- It's looking nice. Where can I get one too?
		DL:addNode()


		DL:createNPCNode(7, -2, "DL_Zephyra_DemonSlayer") -- No, I would never hurt one. They're protecting this valley, you know.
		DL:addNode()


		DL:createNPCNode(8, -2, "DL_Zephyra_NiceCat") -- Don't you dare! Only Yasha's shamans are allowed to wear this headpiece.
		DL:addConditionProgress("npc_zephyra", "yasha")
		DL:addNode()

	end

	if (DL:isConditionFulfilled("npc_zephyra", "yasha") and not DL:isConditionFulfilled("npc_zephyra", "who_yasha")) then 

		DL:createNPCNode(9, 11, "DL_Zephyra_WhoYasha") -- Yasha is a powerful demon who watches over the secrets of this valley.
		DL:addConditionProgress("npc_zephyra", "who_yasha")
		DL:addNode()


		DL:createNPCNode(11, -2, "DL_Zephyra_WhoYasha2") -- She's been sleeping for many years, but those yokels did wake her up. I hope they're paying right now for their foolishness.
		DL:addNode()

	end

	if (not DL:isConditionFulfilled("npc_zephyra", "who_droves")) then 

		DL:createNPCNode(10, 12, "DL_Zephyra_WhoDroves") -- A group of foolish mages. I wanted to keep their from heading straight for disaster, but they wouldn't listen.
		DL:addConditionProgress("npc_zephyra", "who_droves")
		DL:addNode()


		DL:createNPCNode(12, -2, "DL_Zephyra_WhoDroves2") -- It's always those "adventurers", looking for big treasure, without any respect. Unbelievable.
		DL:addNode()

	end

	if (DL:isConditionFulfilled("npc_zephyra", "who_droves") and not DL:isConditionFulfilled("npc_zephyra", "mages")) then 

		DL:createNPCNode(13, 14, "DL_Zephyra_WhoMages") -- Hm. There was one women, fair hair, white dress. One of them was a boy, about your age, with red shaggy hair.
		DL:addConditionProgress("npc_zephyra", "mages")
		DL:addNode()


		DL:createNPCNode(14, 15, "DL_Zephyra_WhoMages2") -- The third one had dark dreadlocks... but their leader frightened me the most.
		DL:addNode()


		DL:createNPCNode(15, -2, "DL_Zephyra_WhoMages3") -- Tall, grey hair, piercing eyes. He was the only one that talked to me. The other three wouldn't react. But they're probably all dead now anyway.
		DL:addNode()

	end

	if (DL:isQuestState("yasha_sanctuary", "completed") and not DL:isConditionFulfilled("npc_zephyra", "yasha_completed")) then 

		DL:createNPCNode(18, -2, "DL_Zephyra_YashaDone") -- (Zephyra nods) Great. Thank you very much for your help. Take this potion with you, may it help you on your ways.
		DL:addConditionProgress("npc_zephyra", "yasha_completed")
		DL:addItem("fo_ultimatehealingpotion", 1)
		DL:addNode()

	end

	if (DL:isConditionFulfilled("npc_zephyra", "droves") and not DL:isConditionFulfilled("npc_zephyra", "gotta_go")) then 

		DL:createNPCNode(19, 16, "DL_Zephyra_GottaGo") -- I see, I can't keep you from going in as well. But, just promise me one thing:
		DL:addConditionProgress("npc_zephyra", "gotta_go")
		DL:addNode()


		DL:createNPCNode(16, 17, "DL_Zephyra_GottaGo2") -- Don't open any of the sacred urns in the temple, if your life means anything to you.
		DL:addNode()


		DL:createChoiceNode(17)
		DL:addChoice(20, "DL_Choice_YashaOK") -- I will try to respect it, m'lady.
		DL:addChoice(21, "DL_Choice_YashaNOK") -- (Evil grin) I will take anything I want, understood?
		DL:addNode()


		DL:createNPCNode(20, -2, "DL_Zephyra_YashaOK") -- I was hoping you'd say this. Be careful, and the demon may spare your life.
		DL:changeQuestState("yasha_sanctuary", "started")
		DL:addNode()


		DL:createNPCNode(21, -2, "DL_Zephyra_YashaNOK") -- I hope the demon will rip your soul apart.
		DL:addNode()

	end

	if (DL:isConditionFulfilled("npc_zephyra", "gotta_go")) then 
	end

end