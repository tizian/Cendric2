-- Dialogue for NPC "npc_loganthird"
loadDialogue = function(DL) 

	if (not DL:isConditionFulfilled("npc_loganthird", "greeting")) then 
		DL:setRoot(1) 
	elseif (not DL:isConditionFulfilled("npc_loganthird", "engage")) then 
		DL:setRoot(5) 
	elseif (not DL:isConditionFulfilled("boss", "BossRoyalguards")) then 
		DL:setRoot(25) 
	else 
		DL:setRoot(34) 
	end 

	if (not DL:isConditionFulfilled("npc_loganthird", "greeting")) then 

		DL:createChoiceNode(1)
		if (not DL:isConditionFulfilled("npc_loganthird", "bow")) then 
			DL:addChoice(2, "DL_Choice_Bow") -- (Bow to Logan)
		end
		if (not DL:isConditionFulfilled("npc_loganthird", "highness")) then 
			DL:addChoice(3, "DL_Choice_Highness") -- (Cough) Your highness?
		end
		if (DL:isConditionFulfilled("npc_loganthird", "highness") or DL:isConditionFulfilled("npc_loganthird", "bow")) then 
			DL:addChoice(4, "DL_Choice_Mark") -- (Show him the mark)
		end
		DL:addChoice(-1, "") -- 
		DL:addNode()

		if (not DL:isConditionFulfilled("npc_loganthird", "bow")) then 

			DL:createNPCNode(2, -2, "DL_Logan_Bow") -- ... (Logan doesn't seem to be present)
			DL:addConditionProgress("npc_loganthird", "bow")
			DL:addNode()

		end

		if (not DL:isConditionFulfilled("npc_loganthird", "highness")) then 

			DL:createNPCNode(3, -2, "DL_Logan_Highness") -- ... (Logan stares past you)
			DL:addConditionProgress("npc_loganthird", "highness")
			DL:addNode()

		end

		if (DL:isConditionFulfilled("npc_loganthird", "highness") or DL:isConditionFulfilled("npc_loganthird", "bow")) then 

			DL:createNPCNode(4, -2, "DL_Logan_Mark") -- ... (Logan finally turns his head and looks at you) You may speak.
			DL:addConditionProgress("npc_loganthird", "greeting")
			DL:addNode()

		end

	end

	if (not DL:isConditionFulfilled("npc_loganthird", "engage")) then 

		DL:createChoiceNode(5)
		if (not DL:isConditionFulfilled("npc_loganthird", "velius")) then 
			DL:addChoice(7, "DL_Choice_Velius") -- Your royal mage, Velius, we have to talk about him.
		end
		if (not DL:isConditionFulfilled("npc_loganthird", "eyes")) then 
			DL:addChoice(6, "DL_Choice_Eyes") -- What's the matter with your eyes?
		end
		if (not DL:isConditionFulfilled("npc_loganthird", "father")) then 
			DL:addChoice(8, "DL_Choice_Father") -- Where is your father?
		end
		if (not DL:isConditionFulfilled("npc_loganthird", "young")) then 
			DL:addChoice(9, "DL_Choice_Young") -- Don't you think you're a bit too young to be king?
		end
		if (not DL:isConditionFulfilled("npc_loganthird", "outside")) then 
			DL:addChoice(10, "DL_Choice_Outside") -- Have you ever been outside the castle?
		end
		if (not DL:isConditionFulfilled("npc_loganthird", "mage")) then 
			DL:addChoice(11, "DL_Choice_Mage") -- Are you a mage?
		end
		if (not DL:isConditionFulfilled("npc_loganthird", "know_me")) then 
			DL:addChoice(12, "DL_Choice_KnowMe") -- Do you know me?
		end
		if (DL:isConditionFulfilled("npc_loganthird", "velius")) then 
			DL:addChoice(13, "DL_Choice_WhatMark") -- What does this mark even mean?
		end
		DL:addChoice(-1, "") -- 
		DL:addNode()

		if (not DL:isConditionFulfilled("npc_loganthird", "velius")) then 

			DL:createNPCNode(7, 14, "DL_Logan_Velius") -- Why don't you talk to him yourself?
			DL:addConditionProgress("npc_loganthird", "velius")
			DL:addNode()


			DL:createChoiceNode(14)
			DL:addChoice(15, "DL_Choice_WhereVelius") -- Where can I find him?
			DL:addChoice(16, "DL_Choice_Ookay") -- Oookay...
			DL:addNode()


			DL:createNPCNode(15, -2, "DL_Logan_WhereVelius") -- (Points at your mark) You should know.
			DL:addNode()


			DL:createNPCNode(16, -2, "DL_Logan_Oookay") -- ...
			DL:addNode()

		end

		if (not DL:isConditionFulfilled("npc_loganthird", "eyes")) then 

			DL:createNPCNode(6, -2, "DL_Logan_Eyes") -- That's no concern of yours. Don't waste my time.
			DL:addConditionProgress("npc_loganthird", "eyes")
			DL:addNode()

		end

		if (not DL:isConditionFulfilled("npc_loganthird", "father")) then 

			DL:createNPCNode(8, 17, "DL_Logan_Father") -- He's fine.
			DL:addConditionProgress("npc_loganthird", "father")
			DL:addNode()


			DL:createNPCNode(17, 18, "DL_Logan_Father2") -- ... (Logan's eyes suddenly light up) No, he isn't. Please help me!
			DL:addNode()


			DL:createChoiceNode(18)
			DL:addChoice(19, "DL_Choice_HowHelp") -- How can I help you?
			DL:addChoice(20, "DL_Choice_WhatFather") -- So, what exactly happened to him?
			DL:addNode()


			DL:createNPCNode(19, -2, "DL_Logan_HowHelp") -- (Logan is grappling for words, but then falls back into his old, absent state) ...
			DL:addNode()


			DL:createNPCNode(20, -2, "DL_Logan_HowHelp") -- 
			DL:addNode()

		end

		if (not DL:isConditionFulfilled("npc_loganthird", "young")) then 

			DL:createNPCNode(9, 22, "DL_Logan_Young") -- ... No. Do you have a problem with it?
			DL:addConditionProgress("npc_loganthird", "young")
			DL:addNode()


			DL:createChoiceNode(22)
			DL:addChoice(23, "DL_Choice_NotMyKing") -- You're not my king. I didn't vote for you.
			DL:addChoice(24, "DL_Choice_MyKing") -- No, not at all.
			DL:addNode()


			DL:createNPCNode(23, -2, "DL_Logan_NotMyKing") -- You don't vote for kings.
			DL:addNode()


			DL:createNPCNode(24, -2, "DL_Logan_MyKing") -- ...
			DL:addNode()

		end

		if (not DL:isConditionFulfilled("npc_loganthird", "outside")) then 

			DL:createNPCNode(10, 27, "DL_Logan_Outside") -- Why should I.
			DL:addConditionProgress("npc_loganthird", "outside")
			DL:addNode()


			DL:createChoiceNode(27)
			DL:addChoice(28, "DL_Choice_KingShouldKnow") -- A king should know his people.
			DL:addChoice(29, "DL_Choice_BeautifulOutside") -- It's beautiful outside!
			DL:addChoice(30, "DL_Choice_NotOutside") -- Yeah, you're right. The life outside is nothing for you.
			DL:addNode()


			DL:createNPCNode(28, -2, "DL_Logan_KingShouldKnow") -- Don't you tell me what a king should.
			DL:addNode()


			DL:createNPCNode(29, -2, "DL_Logan_BeautifulOutside") -- I don't care.
			DL:addNode()


			DL:createNPCNode(30, -2, "DL_Logan_NotOutside") -- ...
			DL:addNode()

		end

		if (not DL:isConditionFulfilled("npc_loganthird", "mage")) then 

			DL:createNPCNode(11, -2, "DL_Logan_Mage") -- ... Yes.
			DL:addConditionProgress("npc_loganthird", "mage")
			DL:addNode()

		end

		if (not DL:isConditionFulfilled("npc_loganthird", "know_me")) then 

			DL:createNPCNode(12, -2, "DL_Logan_KnowMe") -- ... No.
			DL:addConditionProgress("npc_loganthird", "know_me")
			DL:addNode()

		end

		if (DL:isConditionFulfilled("npc_loganthird", "velius")) then 

			DL:createNPCNode(13, 21, "DL_Logan_WhatMark") -- You should know that. It's the reason you're here. 
			DL:addNode()


			DL:createNPCNode(21, -2, "DL_Logan_WhatMark2") -- And it's the reason you will *die* here.
			DL:addConditionProgress("npc_loganthird", "engage")
			DL:addNode()

		end

	end

	if (not DL:isConditionFulfilled("boss", "BossRoyalguards")) then 

		DL:createChoiceNode(25)
		DL:addChoice(31, "DL_Choice_BadDecision") -- That's a very bad decision, boy. You against me?
		DL:addChoice(26, "DL_Choice_CantKill") -- Why would you do that?
		DL:addNode()


		DL:createNPCNode(31, 33, "DL_Logan_BadDecision") -- No. Just you and my GUARDS!
		DL:addNode()


		DL:createNPCNode(33, -1, "") -- 
		DL:startLevel("res/level/boss_royalguards/boss_royalguards.tmx", 600, 560)
		DL:addNode()


		DL:createNPCNode(26, 32, "DL_Logan_ForcedStay") -- I've answered enough questions. Guards, kill him!
		DL:addNode()


		DL:createNPCNode(32, -1, "") -- 
		DL:startLevel("res/level/boss_royalguards/boss_royalguards.tmx", 600, 560)
		DL:addNode()

	end


	DL:createNPCNode(34, 35, "DL_Logan_Afraid") -- You... you killed my guards!
	DL:addConditionProgress("npc_loganthird", "afraid")
	DL:addConditionProgress("default", "hallway_open")
	DL:addNode()


	DL:createNPCNode(35, -1, "DL_Logan_Afraid2") -- AAAAaaaaaaAAAAAH!
	DL:addNode()

end