-- Dialogue for NPC "npc_jason"
loadDialogue = function(DL) 

	if (not DL:isConditionFulfilled("npc_jason", "talked")) then 
		DL:setRoot(1) 
	else 
		DL:setRoot(2) 
	end 

	if (not DL:isConditionFulfilled("npc_jason", "talked")) then 

		DL:createNPCNode(1, -2, "DL_Jason_Hello") -- Hm? Just because Jana loves to talk does not mean I do. Keep it short.
		DL:addConditionProgress("npc_jason", "talked")
		DL:addNode()

	end


	DL:createChoiceNode(2)
	if (DL:isQuestState("jasons_glaive", "void")) then 
		DL:addChoice(3, "DL_Choice_JasonQuest") -- Can I help you somehow?
	end
	if (not DL:isQuestState("jasons_glaive", "void") and not DL:isConditionFulfilled("npc_jason", "crafting")) then 
		DL:addChoice(7, "DL_Choice_StartCrafting") -- Reinforce your weapon? Are you a blacksmith then?
	end
	if (DL:isQuestState("jasons_glaive", "started") and DL:isQuestComplete("jasons_glaive")) then 
		DL:addChoice(5, "DL_Choice_GargoyleDust") -- I got some gargoyle dust for you.
	end
	if (DL:isConditionFulfilled("npc_jason", "crafting") and DL:hasItem("we_firesword_inactive", 1) and DL:isQuestState("yasha_sword", "void")) then 
		DL:addChoice(12, "DL_Choice_YashaSword") -- I found this sword in the temple in the North. Do you know what it is?
	end
	if (DL:isQuestState("yasha_sword", "started") and DL:isQuestComplete("yasha_sword")) then 
		DL:addChoice(14, "DL_Choice_CompleteYashaSword") -- I got all you asked for to reignite that sword.
	end
	if (DL:isConditionFulfilled("npc_jason", "crafting")) then 
		DL:addChoice(10, "DL_Choice_Crafting") -- Could you reinforce a Weapon for me?
	end
	DL:addChoice(-1, "") -- 
	DL:addNode()

	if (DL:isQuestState("jasons_glaive", "void")) then 

		DL:createNPCNode(3, 4, "DL_Jason_JasonQuest") -- Hrm. Yes, actually, you can. I found a new weapon recently that I want to reinforce with gargoyle dust.
		DL:addNode()


		DL:createNPCNode(4, -2, "DL_Jason_JasonQuest2") -- Some gargoyles live in the old temple in the North. If you bring me some gargoyle dust, you can have my old glaive in exchange.
		DL:changeQuestState("jasons_glaive", "started")
		DL:addNode()

	end

	if (not DL:isQuestState("jasons_glaive", "void") and not DL:isConditionFulfilled("npc_jason", "crafting")) then 

		DL:createNPCNode(7, 8, "DL_Jason_StartCrafting") -- No, not exactly a blacksmith. I don't work with hammer and anvil.
		DL:addConditionProgress("npc_jason", "crafting")
		DL:addNode()


		DL:createNPCNode(8, 9, "DL_Jason_StartCrafting2") -- But I use material that contains magic - like gargoyle dust - to reinforce weapons of demonic origin.
		DL:addNode()


		DL:createNPCNode(9, -2, "DL_Jason_StartCrafting3") -- If you have a demonic weapon and some magic material, I can reinforce it for you as well. But it'll cost you. A lot. (Grins)
		DL:addNode()

	end

	if (DL:isQuestState("jasons_glaive", "started") and DL:isQuestComplete("jasons_glaive")) then 

		DL:createNPCNode(5, 6, "DL_Jason_GargoyleDust") -- Oh? What a surprise. I never thought you'd be able to kill them.
		DL:removeItem("mi_gargoyledust", 3)
		DL:addNode()


		DL:createNPCNode(6, -2, "DL_Jason_GargoyleDust2") -- Yeah. But promise is promise. Here, take my old glaive. It's still a good weapon.
		DL:changeQuestState("jasons_glaive", "completed")
		DL:addItem("we_jasonsglaive", 1)
		DL:addNode()

	end

	if (DL:isConditionFulfilled("npc_jason", "crafting") and DL:hasItem("we_firesword_inactive", 1) and DL:isQuestState("yasha_sword", "void")) then 

		DL:createNPCNode(12, 13, "DL_Jason_YashaSword") -- Hm. Looks like a fire sword. But it has been snuffed out.
		DL:addNode()


		DL:createNPCNode(13, -2, "DL_Jason_YashaSword2") -- That was a pretty powerful sword. If you can bring me the fiery essence of a very strong demon, I can reignite it for you. Oh and some gold, I need gold.
		DL:changeQuestState("yasha_sword", "started")
		DL:addNode()

	end

	if (DL:isQuestState("yasha_sword", "started") and DL:isQuestComplete("yasha_sword")) then 

		DL:createNPCNode(14, 16, "DL_Jason_CompleteYashaSword") -- Good. Let me see... (Jason takes the sword and the essence)
		DL:removeItem("we_firesword_inactive", 1)
		DL:removeItem("qe_yashaessence", 1)
		DL:removeGold(50)
		DL:addNode()


		DL:createNPCNode(16, 17, "DL_Jason_CompleteYashaSword2") -- (After combining them, the sword ignites into a flash of magic fire)
		DL:addNode()


		DL:createNPCNode(17, -2, "DL_Jason_CompleteYashaSword3") -- Here you go.
		DL:changeQuestState("yasha_sword", "completed")
		DL:addItem("we_firesword", 1)
		DL:addNode()

	end

	if (DL:isConditionFulfilled("npc_jason", "crafting")) then 

		DL:createChoiceNode(10)
		if (DL:hasItem("we_doublesickle", 1)) then 
			DL:addCraftingChoice(11, "DL_Choice_DoubleSickleFire") -- we_doublesickle_fire,we_doublesickle,1,mi_neko_ember,3,gold,200
		end
		if (DL:hasItem("we_doublesickle", 1)) then 
			DL:addCraftingChoice(15, "DL_Choice_DoubleSickleDark") -- we_doublesickle_dark,we_doublesickle,1,mi_gargoyle_dust,3,gold,200
		end
		if (DL:hasItem("we_jasonsglaive", 1)) then 
			DL:addCraftingChoice(18, "DL_Choice_JasonsGlaiveFire") -- we_jasonsglaive_fire,we_jasonsglaive,1,mi_neko_ember,3,gold,200
		end
		if (DL:hasItem("we_jasonsglaive", 1)) then 
			DL:addCraftingChoice(19, "DL_Choice_JasonsGlaiveDark") -- we_jasonsglaive_dark,we_jasonsglaive,1,mi_gargoyle_dust,3,gold,200
		end
		if (DL:hasItem("we_fireglaive", 1)) then 
			DL:addCraftingChoice(20, "DL_Choice_FireGlaiveFire") -- we_fireglaive_fire,we_fireglaive,1,mi_neko_ember,3,gold,200
		end
		if (DL:hasItem("we_fireglaive", 1)) then 
			DL:addCraftingChoice(21, "DL_Choice_FireGlaiveDark") -- we_fireglaive_dark,we_fireglaive,1,mi_gargoyle_dust,3,gold,200
		end
		DL:addChoice(-2, "DL_Choice_Back") -- I'll come back later. [BACK]
		DL:addNode()

		if (DL:hasItem("we_doublesickle", 1)) then 

			DL:createNPCNode(11, -2, "DL_Jason_HereYouGo") -- Here you go.
			DL:gotoNode(10)
			DL:addNode()

		end

		if (DL:hasItem("we_doublesickle", 1)) then 

			DL:createNPCNode(15, -2, "DL_Jason_HereYouGo") -- 
			DL:gotoNode(10)
			DL:addNode()

		end

		if (DL:hasItem("we_jasonsglaive", 1)) then 

			DL:createNPCNode(18, -2, "DL_Jason_HereYouGo") -- 
			DL:gotoNode(10)
			DL:addNode()

		end

		if (DL:hasItem("we_jasonsglaive", 1)) then 

			DL:createNPCNode(19, -2, "DL_Jason_HereYouGo") -- 
			DL:gotoNode(10)
			DL:addNode()

		end

		if (DL:hasItem("we_fireglaive", 1)) then 

			DL:createNPCNode(20, -2, "DL_Jason_HereYouGo") -- 
			DL:gotoNode(10)
			DL:addNode()

		end

		if (DL:hasItem("we_fireglaive", 1)) then 

			DL:createNPCNode(21, -2, "DL_Jason_HereYouGo") -- 
			DL:gotoNode(10)
			DL:addNode()

		end

	end

end