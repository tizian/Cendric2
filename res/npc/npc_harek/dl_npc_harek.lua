-- Dialogue for NPC "npc_harek"
loadDialogue = function(DL) 

	if (not DL:isConditionFulfilled("npc_harek", "talked")) then 
		DL:setRoot(1) 
	else 
		DL:setRoot(4) 
	end 

	if (not DL:isConditionFulfilled("npc_harek", "talked")) then 

		DL:createChoiceNode(1)
		DL:addChoice(2, "DL_Choice_WhoAreYou") -- Are you the blacksmith here?
		DL:addChoice(-1, "") -- 
		DL:addNode()


		DL:createNPCNode(2, 3, "DL_Harek_IAmSmith") -- Yes, and I'm the best one around here. I have some weapons and armour here, if you're interested.
		DL:addConditionProgress("npc_harek", "talked")
		DL:addNode()


		DL:createNPCNode(3, -2, "DL_Harek_WhoAreYou2") -- I also take on more special jobs, if you can bring me the materials.
		DL:addNode()

	end


	DL:createChoiceNode(4)
	DL:addChoice(5, "DL_Choice_Trade") -- Show me your wares.
	if (DL:isQuestState("theas_dream", "started") and not DL:isConditionFulfilled("npc_harek", "thea")) then 
		DL:addChoice(28, "DL_Choice_Thea") -- Could you use an apprentice?
	end
	if (DL:isQuestState("ice_armor", "void") and DL:hasItem("mi_firstguardianheart", 1)) then 
		DL:addChoice(6, "DL_Choice_RivetArmor") -- Could you smith me a special armour?
	end
	if ((DL:hasItem("qe_brokenstaff1", 1) or DL:hasItem("qe_brokenstaff2", 1) or DL:hasItem("qe_brokenstaff3", 1)) and DL:isQuestState("broken_staff", "void")) then 
		DL:addChoice(11, "DL_Choice_BrokenStaff") -- I got this broken weapon here, can you fix it?
	end
	if (DL:isQuestState("broken_staff", "started") and DL:isQuestComplete("broken_staff")) then 
		DL:addChoice(12, "DL_Choice_BrokenStaffComplete") -- I got all the parts of the broken staff.
	end
	if (DL:isQuestState("ice_armor", "started") and DL:isQuestComplete("ice_armor")) then 
		DL:addChoice(14, "DL_Choice_GotIronOre") -- I got the iron ore for you.
	end
	if (DL:isQuestState("ice_armor", "completed")) then 
		DL:addChoice(16, "DL_Choice_Craft") -- Could you craft something for me?
	end
	DL:addChoice(-1, "") -- 
	DL:addNode()


	DL:createTradeNode(5, -1)
	DL:addNode()

	if (DL:isQuestState("theas_dream", "started") and not DL:isConditionFulfilled("npc_harek", "thea")) then 

		DL:createNPCNode(28, 29, "DL_Harek_Thea") -- You?
		DL:addConditionProgress("npc_harek", "thea")
		DL:addNode()


		DL:createCendricNode(29, 30, "DL_Cendric_Thea") -- No, a girl called Thea is looking for a job.
		DL:addNode()


		DL:createNPCNode(30, 31, "DL_Harek_Thea2") -- Hm. No. This isn't a job for a girl.
		DL:addNode()


		DL:createNPCNode(31, -2, "DL_Harek_Thea3") -- But you could ask Syrah, the alchemist. She was looking for an apprentice.
		DL:addNode()

	end

	if (DL:isQuestState("ice_armor", "void") and DL:hasItem("mi_firstguardianheart", 1)) then 

		DL:createNPCNode(6, 7, "DL_Harek_IceArmor") -- Of course. You look like a mage, so I guess you could use a magic-infused armour.
		DL:addNode()


		DL:createNPCNode(7, 8, "DL_Harek_IceArmor2") -- But I'm currently really short on material. First I'd need something that holds the magic...
		DL:addNode()


		DL:createChoiceNode(8)
		DL:addChoice(9, "DL_Choice_IHaveHeart") -- I got this special looking stone here... (Show Heart of Khajag)
		DL:addChoice(-2, "DL_Choice_IDontHaveMagic") -- Maybe later.
		DL:addNode()


		DL:createNPCNode(9, 10, "DL_Harek_NiceStone") -- Oh, that's perfect. I think I could use that for the magic part of the armour.
		DL:addNode()


		DL:createNPCNode(10, -2, "DL_Harek_NiceStone2") -- However, I still need some iron ore. If you could bring me some from the mine of Gandria, I'll smith you your armour right away!
		DL:changeQuestState("ice_armor", "started")
		DL:addNode()

	end

	if ((DL:hasItem("qe_brokenstaff1", 1) or DL:hasItem("qe_brokenstaff2", 1) or DL:hasItem("qe_brokenstaff3", 1)) and DL:isQuestState("broken_staff", "void")) then 

		DL:createNPCNode(11, -2, "DL_Harek_BrokenStaff") -- (Looks at the broken part) Hm, that seems to be a part of a rather interesting staff. If you can bring me all pieces of it, I might be able to fix it.
		DL:changeQuestState("broken_staff", "started")
		DL:addNode()

	end

	if (DL:isQuestState("broken_staff", "started") and DL:isQuestComplete("broken_staff")) then 

		DL:createNPCNode(12, 13, "DL_Harek_BrokenStaffComplete") -- Great! Let's see... (Takes out some tools and starts working...)
		DL:removeItem("qe_brokenstaff1", 1)
		DL:removeItem("qe_brokenstaff2", 1)
		DL:removeItem("qe_brokenstaff3", 1)
		DL:addNode()


		DL:createNPCNode(13, -2, "DL_Harek_BrokenStaffComplete2") -- ... And finished. Here you go!
		DL:changeQuestState("broken_staff", "completed")
		DL:addItem("we_tearstaff", 1)
		DL:addNode()

	end

	if (DL:isQuestState("ice_armor", "started") and DL:isQuestComplete("ice_armor")) then 

		DL:createNPCNode(14, 15, "DL_Harek_SmithMagicArmor") -- Very good. (Harek takes the ingredients and starts hammering on his anvil furiously...)
		DL:removeItem("mi_firstguardianheart", 1)
		DL:removeItem("mi_ironore", 10)
		DL:addNode()


		DL:createNPCNode(15, 19, "DL_Harek_SmithMagicArmor2") -- Here you go. Splendid, isn't it.
		DL:changeQuestState("ice_armor", "completed")
		DL:addItem("eq_icearmor", 1)
		DL:addNode()


		DL:createNPCNode(19, -2, "DL_Harek_SmithMagicArmor3") -- You can always come back to me if you find other special materials, I'd love working with them.
		DL:addNode()

	end

	if (DL:isQuestState("ice_armor", "completed")) then 

		DL:createNPCNode(16, 17, "DL_Harek_Craft") -- Sure, did you find some special materials and some iron ore?
		DL:addNode()


		DL:createChoiceNode(17)
		if (DL:hasItem("mi_icecrystal", 1)) then 
			DL:addCraftingChoice(18, "DL_Choice_IceScarf") -- eq_icescarf,mi_icecrystal,1,mi_ironore,2,gold,40
		end
		if (DL:hasItem("mi_pearlpile", 1)) then 
			DL:addCraftingChoice(20, "DL_Choice_PearlSabre") -- we_pearlsabre,mi_pearlpile,1,mi_ironore,3,gold,75
		end
		if (DL:hasItem("mi_elysiatail", 1)) then 
			DL:addCraftingChoice(21, "DL_Choice_LightningPike") -- we_lightningpike,mi_elysiatail,1,mi_ironore,5,gold,100
		end
		if (DL:hasItem("mi_janusglaive", 1)) then 
			DL:addCraftingChoice(22, "DL_Choice_JanusGlaive") -- we_janusglaive,mi_janusglaive,1,mi_ironore,4,gold,100
		end
		if (DL:hasItem("mi_corrupt_stone_fire", 1)) then 
			DL:addCraftingChoice(24, "DL_Choice_LavaArmor") -- eq_lavaarmor,mi_corrupt_stone_fire,3,mi_shinystone,1,mi_ironore,5,gold,110
		end
		if (DL:hasItem("mi_corrupt_stone_ice", 1)) then 
			DL:addCraftingChoice(25, "DL_Choice_CairnArmor") -- eq_cairnarmor,mi_corrupt_stone_ice,3,mi_shinystone,1,mi_ironore,5,gold,110
		end
		if (DL:hasItem("mi_yashafangs", 1)) then 
			DL:addCraftingChoice(26, "DL_Choice_FireGlaive") -- we_fireglaive,mi_yashafangs,1,mi_shinystone,2,mi_ironore,4,gold,130
		end
		DL:addChoice(-2, "DL_Choice_NoMaterial") -- I'll come back later. [BACK]
		DL:addNode()

		if (DL:hasItem("mi_icecrystal", 1)) then 

			DL:createNPCNode(18, -2, "DL_Harek_IceScarf") -- An exeptionally beautiful stone! I can truly craft something magical from that.
			DL:gotoNode(17)
			DL:addNode()

		end

		if (DL:hasItem("mi_pearlpile", 1)) then 

			DL:createNPCNode(20, -2, "DL_Harek_PearlSabre") -- Magical pearls! It has been a long time since I've held some of them in my hands... They'll make a fine sabre.
			DL:gotoNode(17)
			DL:addNode()

		end

		if (DL:hasItem("mi_elysiatail", 1)) then 

			DL:createNPCNode(21, -2, "DL_Harek_LightningPike") -- The tail feathers of a storm bird? Great material for a magical lightning pike.
			DL:gotoNode(17)
			DL:addNode()

		end

		if (DL:hasItem("mi_janusglaive", 1)) then 

			DL:createNPCNode(22, 23, "DL_Harek_JanusGlaive") -- Hm, I really start to wonder where you get this stuff from.
			DL:addNode()


			DL:createNPCNode(23, -2, "DL_Harek_JanusGlaive2") -- I can fix this broken weapon but I can only restore a part of its magical power.
			DL:gotoNode(17)
			DL:addNode()

		end

		if (DL:hasItem("mi_corrupt_stone_fire", 1)) then 

			DL:createNPCNode(24, -2, "DL_Harek_LavaArmor") -- Lava stones from a lava wraith! These will make a fine, fire-resistant armour.
			DL:gotoNode(17)
			DL:addNode()

		end

		if (DL:hasItem("mi_corrupt_stone_ice", 1)) then 

			DL:createNPCNode(25, -2, "DL_Harek_CairnArmor") -- Magic-infused stones from a cairn wraith! These will make a fine, ice-resistant armour.
			DL:gotoNode(17)
			DL:addNode()

		end

		if (DL:hasItem("mi_yashafangs", 1)) then 

			DL:createNPCNode(26, 27, "DL_Harek_FireGlaive") -- Huge demonic fangs! And just look at the magical power flowing through them. This will make a powerful weapon.
			DL:addNode()


			DL:createNPCNode(27, -2, "DL_Harek_FireGlaive2") -- I've never worked with something like this before. A truly amazing material.
			DL:gotoNode(17)
			DL:addNode()

		end

	end

end