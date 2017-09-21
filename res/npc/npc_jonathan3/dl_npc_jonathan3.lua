-- Dialogue for NPC "npc_jonathan3"
loadDialogue = function(DL) 

	if (not DL:isConditionFulfilled("npc_jonathan3", "talked")) then 
		DL:setRoot(1) 
	else 
		DL:setRoot(2) 
	end 

	if (not DL:isConditionFulfilled("npc_jonathan3", "talked")) then 

		DL:createNPCNode(1, 3, "DL_Jonathan_Hey") -- You're back, perfect. I found the mages who robbed me and secretly followed them to this crypt.
		DL:addNode()


		DL:createNPCNode(3, 20, "DL_Jonathan_Hey2") -- But they locked it with a spell. I need the name of the mage who cast the spell to unlock it.
		DL:addConditionProgress("npc_jonathan3", "talked")
		DL:addNode()


		DL:createNPCNode(20, -2, "DL_Jonathan_Hey3") -- And you, did you find something interesting in the temple?
		DL:addNode()

	end


	DL:createChoiceNode(2)
	if (not DL:isConditionFulfilled("default","gandriacrypt_open")) then 
		DL:addChoice(11, "DL_Choice_Name") -- I know the name of the mage who cast the spell...
	end
	if (DL:isQuestState("jonathan_knowledge", "started") and DL:isQuestComplete("jonathan_knowledge")) then 
		DL:addChoice(4, "DL_Choice_Books") -- I found some books for you. (Give documents)
	end
	if (DL:isQuestState("jonathan_knowledge", "started") and not DL:isQuestComplete("jonathan_knowledge") and not DL:isConditionFulfilled("npc_jonathan3", "no_books")) then 
		DL:addChoice(10, "DL_Choice_NoBook") -- I've found a library but I still need some books for you.
	end
	DL:addChoice(-1, "") -- 
	DL:addNode()

	if (not DL:isConditionFulfilled("default","gandriacrypt_open")) then 

		DL:createChoiceNode(11)
		DL:addChoice(13, "DL_Choice_Name1") -- Tarabas.
		if (DL:isQuestDescriptionUnlocked("find_velius",3)) then 
			DL:addChoice(14, "DL_Choice_NameVelius") -- Velius.
		end
		DL:addChoice(12, "DL_Choice_Name2") -- Inina.
		DL:addChoice(16, "DL_Choice_Name3") -- Tassadan.
		DL:addChoice(17, "DL_Choice_Name4") -- Bob.
		DL:addChoice(18, "DL_Choice_Name5") -- Koray.
		DL:addChoice(-2, "DL_Choice_DontKnow") -- Erm, never mind.
		DL:addNode()


		DL:createNPCNode(13, -2, "DL_Jonathan_WrongName") -- Okay, let's try it... (Jonathan mumbles a spell) Hm. It didn't work.
		DL:addNode()

		if (DL:isQuestDescriptionUnlocked("find_velius",3)) then 

			DL:createNPCNode(14, 15, "DL_Jonathan_CorrectName") -- Okay, let's try it... (Jonathan mumbles a spell)
			DL:addNode()


			DL:createNPCNode(15, 19, "DL_Jonathan_CorrectName2") -- Yes. We did it! Now you can follow them.
			DL:addConditionProgress("default", "gandriacrypt_open")
			DL:addNode()


			DL:createNPCNode(19, -1, "") -- 
			DL:startCutscene("chapter_5")
			DL:addNode()

		end


		DL:createNPCNode(12, -2, "DL_Jonathan_WrongName") -- 
		DL:addNode()


		DL:createNPCNode(16, -2, "DL_Jonathan_WrongName") -- 
		DL:addNode()


		DL:createNPCNode(17, -2, "DL_Jonathan_WrongName") -- 
		DL:addNode()


		DL:createNPCNode(18, -2, "DL_Jonathan_WrongName") -- 
		DL:addNode()

	end

	if (DL:isQuestState("jonathan_knowledge", "started") and DL:isQuestComplete("jonathan_knowledge")) then 

		DL:createNPCNode(4, 5, "DL_Jonathan_Books") -- Woah. Woah! (Jonathan skims the documents) 
		DL:removeItem("do_antigravity", 1)
		DL:removeItem("do_antimagic", 1)
		DL:addNode()


		DL:createNPCNode(5, 6, "DL_Jonathan_Books2") -- That's exactly what we were looking for. The old, forgotten anti-magic. Dangerous magic.
		DL:changeQuestState("jonathan_knowledge", "completed")
		DL:addNode()


		DL:createNPCNode(6, 7, "DL_Jonathan_Books3") -- Even though some pages are missing in this book, I should return as fast as possible to the academy. Will you come with me?
		DL:addNode()


		DL:createChoiceNode(7)
		DL:addChoice(8, "DL_Choice_Business") -- I still got business here.
		DL:addChoice(9, "DL_Choice_LoveTo") -- I'd love to...
		DL:addNode()


		DL:createNPCNode(8, -1, "DL_Jonathan_Business") -- You're right. You should follow those mages. Maybe you even find the missing pages. See you at the academy!
		DL:addNode()


		DL:createNPCNode(9, -1, "DL_Jonathan_LoveTo") -- But you need to follow those mages, right? Maybe you even find the missing pages. Anyway, see you at the academy!
		DL:addNode()

	end

	if (DL:isQuestState("jonathan_knowledge", "started") and not DL:isQuestComplete("jonathan_knowledge") and not DL:isConditionFulfilled("npc_jonathan3", "no_books")) then 

		DL:createNPCNode(10, -1, "DL_Jonathan_NoBook") -- Alright. I'll wait here then.
		DL:addConditionProgress("npc_jonathan3", "no_books")
		DL:addNode()

	end

end