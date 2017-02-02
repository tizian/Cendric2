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
		if (DL:hasItem("mi_icecrystal", 1) and DL:hasItem("mi_ironore", 2)) then 
			DL:addChoice(18, "DL_Choice_IceScarf") -- 1 $eq_icescarf$ (1 $mi_icecrystal$, 2 $mi_ironore$)
		end
		if (DL:hasItem("mi_pearlpile", 1) and DL:hasItem("mi_ironore", 3)) then 
			DL:addChoice(20, "DL_Choice_PearlSabre") -- 1 $we_pearlsabre$ (1 $mi_pearlpile$, 3 $mi_ironore$)
		end
		if (DL:hasItem("mi_elysiatail", 1) and DL:hasItem("mi_ironore", 5)) then 
			DL:addChoice(21, "DL_Choice_LightningPike") -- 1 $we_lightningpike$ (1 $mi_elysiatail$, 5 $mi_ironore$)
		end
		DL:addChoice(-2, "DL_Choice_NoMaterial") -- I'll come back later. [BACK]
		DL:addNode()

		if (DL:hasItem("mi_icecrystal", 1) and DL:hasItem("mi_ironore", 2)) then 

			DL:createNPCNode(18, -2, "DL_Harek_IceScarf") -- An exeptionally beautiful stone! I can truly craft something magical from that.
			DL:removeItem("mi_icecrystal", 1)
			DL:removeItem("mi_ironore", 2)
			DL:addItem("eq_icescarf", 1)
			DL:addNode()

		end

		if (DL:hasItem("mi_pearlpile", 1) and DL:hasItem("mi_ironore", 3)) then 

			DL:createNPCNode(20, -2, "DL_Harek_PearlSabre") -- Magical pearls! It has been a long time since I've held some of them in my hands... They'll make a fine sabre.
			DL:removeItem("mi_pearlpile", 1)
			DL:removeItem("mi_ironore", 3)
			DL:addItem("we_pearlsabre", 1)
			DL:addNode()

		end

		if (DL:hasItem("mi_elysiatail", 1) and DL:hasItem("mi_ironore", 5)) then 

			DL:createNPCNode(21, -2, "DL_Harek_LightningPike") -- The tail feathers of a storm bird? Great material for a magical lightning pike.
			DL:removeItem("mi_ironore", 5)
			DL:removeItem("mi_elysiatail", 1)
			DL:addItem("we_lightningpike", 1)
			DL:addNode()

		end

	end

end