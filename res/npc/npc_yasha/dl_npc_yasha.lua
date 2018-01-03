-- Dialogue for NPC "npc_yasha"
loadDialogue = function(DL) 

	if (not DL:isConditionFulfilled("npc_yasha", "talked")) then 
		DL:setRoot(1) 
	elseif (not DL:isConditionFulfilled("npc_yasha", "attacked") and not (DL:isConditionFulfilled("npc_yasha", "unfriendly") or DL:isConditionFulfilled("npc_yasha", "friendly"))) then 
		DL:setRoot(2) 
	elseif (not DL:isConditionFulfilled("npc_yasha", "attacked") and not DL:isConditionFulfilled("boss", "BossYasha") and not DL:isConditionFulfilled("npc_yasha", "attacked") and not (DL:isConditionFulfilled("npc_yasha", "unfriendly") and DL:isConditionFulfilled("npc_yasha", "question"))) then 
		DL:setRoot(9) 
	elseif (DL:isConditionFulfilled("npc_yasha", "attacked") or (DL:isConditionFulfilled("npc_yasha", "unfriendly") and DL:isConditionFulfilled("npc_yasha", "question"))) then 
		DL:setRoot(13) 
	elseif (not DL:isConditionFulfilled("npc_yasha", "reward")) then 
		DL:setRoot(17) 
	elseif (DL:hasItem("we_firesword_inactive",1)) then 
		DL:setRoot(23) 
	else 
		DL:setRoot(20) 
	end 

	if (not DL:isConditionFulfilled("npc_yasha", "talked")) then 

		DL:createNPCNode(1, -2, "DL_Yasha_Hi") -- (Growls) Another mortal dares to trespass into my temple. Any last words?
		DL:addConditionProgress("npc_yasha", "talked")
		DL:addNode()

	end

	if (not DL:isConditionFulfilled("npc_yasha", "attacked") and not (DL:isConditionFulfilled("npc_yasha", "unfriendly") or DL:isConditionFulfilled("npc_yasha", "friendly"))) then 

		DL:createChoiceNode(2)
		if (not DL:isConditionFulfilled("npc_yasha", "urn_open") and not DL:isConditionFulfilled("npc_yasha", "talk")) then 
			DL:addChoice(4, "DL_Choice_PeacefulTalk") -- Can we have a peaceful talk, demon?
		end
		if (DL:isConditionFulfilled("npc_yasha", "urn_open") and not DL:isConditionFulfilled("npc_yasha", "talk")) then 
			DL:addChoice(6, "DL_Choice_PeacefulTalk") -- 
		end
		DL:addChoice(3, "DL_Choice_Yasha_Attack") -- I'm not here to talk. [ATTACK]
		DL:addChoice(-1, "DL_Choice_End") -- (Back away slowly.)
		DL:addNode()

		if (not DL:isConditionFulfilled("npc_yasha", "urn_open") and not DL:isConditionFulfilled("npc_yasha", "talk")) then 

			DL:createNPCNode(4, 5, "DL_Yasha_YashaSanctuaryDone") -- (Yasha stares at you) Hrr. You did traverse my temple without touching my sacred urns. No human did overcome the temptation of touching them before. 
			DL:addConditionProgress("npc_yasha", "friendly")
			DL:addConditionProgress("npc_yasha", "talk")
			DL:addNode()


			DL:createNPCNode(5, -2, "DL_Yasha_YashaSanctuaryDone2") -- I will answer all of your questions, human.
			DL:changeQuestState("yasha_sanctuary", "completed")
			DL:addNode()

		end

		if (DL:isConditionFulfilled("npc_yasha", "urn_open") and not DL:isConditionFulfilled("npc_yasha", "talk")) then 

			DL:createNPCNode(6, 7, "DL_Yasha_PeacefulTalkNot") -- I've seen you stomping through my temple, opening my sacred urns and destroying the peaceful atmosphere.
			DL:addConditionProgress("npc_yasha", "talk")
			DL:addConditionProgress("npc_yasha", "unfriendly")
			DL:addNode()


			DL:createNPCNode(7, -2, "DL_Yasha_PeacefulTalkNot2") -- You're of the same vile kind as all humans. Before I crush your miserable existence, I grant you one question. Choose wisely. 
			DL:addNode()

		end


		DL:createNPCNode(3, 8, "DL_Yasha_Attack") -- Well, prepare to die then.
		DL:addConditionProgress("npc_yasha", "attacked")
		DL:addConditionProgress("npc_yasha", "unfriendly")
		DL:addNode()


		DL:createNPCNode(8, -1, "") -- 
		DL:startLevel("res/level/boss_yasha/boss_yasha_bad.tmx", 400, 810)
		DL:addNode()

	end

	if (not DL:isConditionFulfilled("npc_yasha", "attacked") and not DL:isConditionFulfilled("boss", "BossYasha") and not DL:isConditionFulfilled("npc_yasha", "attacked") and not (DL:isConditionFulfilled("npc_yasha", "unfriendly") and DL:isConditionFulfilled("npc_yasha", "question"))) then 

		DL:createChoiceNode(9)
		if (not DL:isConditionFulfilled("npc_yasha", "what_guarding")) then 
			DL:addChoice(10, "DL_Choice_WhatGuarding") -- What are you guarding?
		end
		if (not DL:isConditionFulfilled("npc_yasha", "mages")) then 
			DL:addChoice(11, "DL_Choice_Mages") -- Who did pass through here before me?
		end
		if (not DL:isConditionFulfilled("npc_yasha", "who_summoned")) then 
			DL:addChoice(12, "DL_Choice_WhoSummoned") -- Who summoned you?
		end
		if (DL:isConditionFulfilled("npc_yasha", "what_guarding") and not DL:isConditionFulfilled("npc_yasha", "secret")) then 
			DL:addChoice(15, "DL_Choice_Secret") -- What's that secret?
		end
		if (DL:isConditionFulfilled("npc_yasha", "what_guarding") and not DL:isConditionFulfilled("npc_yasha", "key")) then 
			DL:addChoice(16, "DL_Choice_Key") -- Where's that key you were guarding?
		end
		if (DL:isConditionFulfilled("npc_yasha", "friendly") and not DL:isConditionFulfilled("npc_yasha", "attackable")) then 
			DL:addChoice(18, "DL_Choice_LetPass") -- Will you let me pass through your temple?
		end
		if (DL:isConditionFulfilled("npc_yasha", "attackable")) then 
			DL:addChoice(19, "DL_Choice_LetsFight") -- Alright, I'm ready to fight. [ATTACK]
		end
		DL:addChoice(-1, "DL_Yasha_End") -- 
		DL:addNode()

		if (not DL:isConditionFulfilled("npc_yasha", "what_guarding")) then 

			DL:createNPCNode(10, -2, "DL_Yasha_WhatGuarding") -- I was guarding a key to a secret that should've never been revealed.
			DL:addConditionProgress("npc_yasha", "what_guarding")
			DL:addConditionProgress("npc_yasha", "question")
			DL:addNode()

		end

		if (not DL:isConditionFulfilled("npc_yasha", "mages")) then 

			DL:createNPCNode(11, -2, "DL_Yasha_Mages") -- A filthy thief and his henchmen. They outwit me with some magic not known to me. 
			DL:addConditionProgress("npc_yasha", "mages")
			DL:addConditionProgress("npc_yasha", "question")
			DL:addNode()

		end

		if (not DL:isConditionFulfilled("npc_yasha", "who_summoned")) then 

			DL:createNPCNode(12, -2, "DL_Yasha_WhoSummoned") -- A necromancer, a long time ago. As so many necromancers, he underestimated the powers he was playing with.
			DL:addConditionProgress("npc_yasha", "who_summoned")
			DL:addConditionProgress("npc_yasha", "question")
			DL:addNode()

		end

		if (DL:isConditionFulfilled("npc_yasha", "what_guarding") and not DL:isConditionFulfilled("npc_yasha", "secret")) then 

			DL:createNPCNode(15, -2, "DL_Yasha_Secret") -- ...
			DL:addConditionProgress("npc_yasha", "secret")
			DL:addNode()

		end

		if (DL:isConditionFulfilled("npc_yasha", "what_guarding") and not DL:isConditionFulfilled("npc_yasha", "key")) then 

			DL:createNPCNode(16, -2, "DL_Yasha_Key") -- Grr. It got stolen. But the sword will find its way back to me.
			DL:addConditionProgress("npc_yasha", "key")
			DL:addNode()

		end

		if (DL:isConditionFulfilled("npc_yasha", "friendly") and not DL:isConditionFulfilled("npc_yasha", "attackable")) then 

			DL:createNPCNode(18, -2, "DL_Yasha_LetPass") -- Hrrr. Yes, I will. But first you will have to prove that you're worthy. Fight me.
			DL:addConditionProgress("npc_yasha", "attackable")
			DL:addNode()

		end

		if (DL:isConditionFulfilled("npc_yasha", "attackable")) then 

			DL:createNPCNode(19, -1, "") -- 
			DL:startLevel("res/level/boss_yasha/boss_yasha_good.tmx", 400, 810)
			DL:addNode()

		end

	end

	if (DL:isConditionFulfilled("npc_yasha", "attacked") or (DL:isConditionFulfilled("npc_yasha", "unfriendly") and DL:isConditionFulfilled("npc_yasha", "question"))) then 

		DL:createNPCNode(13, 14, "DL_Yasha_DieNow") -- That's enough. Die now.
		DL:addNode()


		DL:createNPCNode(14, -1, "") -- 
		DL:startLevel("res/level/boss_yasha/boss_yasha_bad.tmx", 400, 810)
		DL:addNode()

	end

	if (not DL:isConditionFulfilled("npc_yasha", "reward")) then 

		DL:createNPCNode(17, 21, "DL_Yasha_Reward") -- Ahh. I see, your soul is pure and your magic is strong. 
		DL:addNode()


		DL:createNPCNode(21, 22, "DL_Yasha_Reward2") -- Maybe you're not as vile as the other humans I met. Maybe you can find that filthy thief and thwart his plans. 
		DL:addNode()


		DL:createNPCNode(22, -2, "DL_Yasha_Reward3") -- Here, take this weapon and this pendant. May they help you reaching your goals.
		DL:addItem("eq_lifependant", 1)
		DL:addItem("we_doublesickle", 1)
		DL:addConditionProgress("npc_yasha", "reward")
		DL:addNode()

	end

	if (DL:hasItem("we_firesword_inactive",1)) then 

		DL:createChoiceNode(23)
		DL:addChoice(25, "DL_Choice_Sword") -- I found this sword. Is it yours?
		DL:addChoice(-1, "") -- 
		DL:addNode()


		DL:createNPCNode(25, 24, "DL_Yasha_Sword") -- Hrr. Where did you find that, human? That's my sword, it was stolen.
		DL:addNode()


		DL:createChoiceNode(24)
		DL:addChoice(26, "DL_Choice_TempleFound") -- In the temple in the North.
		DL:addNode()


		DL:createNPCNode(26, 27, "DL_Yasha_TempleFound") -- It's flame went out. That means it has been used to enter the temple's library. It's too late.
		DL:addNode()


		DL:createNPCNode(27, 28, "DL_Yasha_TempleFound2") -- My only purpose was to keep intruders from finding the secrets of the library. I failed.
		DL:addNode()


		DL:createNPCNode(28, 29, "DL_Yasha_TempleFound3") -- But you, you could find the thief for me and punish him. Give me that sword. (Yasha takes the sword and puts it on the ground)
		DL:removeItem("we_firesword_inactive", 1)
		DL:addNode()


		DL:createNPCNode(29, 30, "DL_Yasha_TempleFound4") -- (Yasha uses her fiery breath to reignite the sword)
		DL:addNode()


		DL:createNPCNode(30, -2, "DL_Yasha_TempleFound5") -- Here. Take it and avenge me.
		DL:addItem("we_firesword", 1)
		DL:changeQuestState("yasha_sword", "completed")
		DL:addNode()

	end


	DL:createNPCNode(20, -1, "DL_Yasha_BeSafe") -- Goodbye, and stay safe, human.
	DL:addNode()

end