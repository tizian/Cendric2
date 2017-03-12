-- Dialogue for NPC "npc_jeremy"
loadDialogue = function(DL) 

	if (DL:isConditionFulfilled("npc_jeremy", "who_are_you") == DL:isConditionFulfilled("npc_jeremy", "buddy_guy")) then 
		DL:setRoot(1) 
	else 
		DL:setRoot(9) 
	end 

	if (DL:isConditionFulfilled("npc_jeremy", "who_are_you") == DL:isConditionFulfilled("npc_jeremy", "buddy_guy")) then 

		DL:createChoiceNode(1)
		if (DL:isQuestState("stephens_helmet", "started") and not DL:isConditionFulfilled("npc_jeremy", "helmet")) then 
			DL:addChoice(2, "DL_Choice_Helmet") -- Do you know something about the helmet of a certain paladin?
		end
		if (DL:isQuestState("leeroy_chicken", "started") and DL:isConditionFulfilled("npc_helena","leeroy_chicken") and not DL:isConditionFulfilled("npc_jeremy", "leeroy_chicken")) then 
			DL:addChoice(7, "DL_Choice_LeeroyChicken") -- Helena wants you to leave her inn. Get out.
		end
		if (not DL:isConditionFulfilled("npc_jeremy", "who_are_you")) then 
			DL:addChoice(3, "DL_Choice_WhoAreYou") -- Who are you guys?
		end
		if (not DL:isConditionFulfilled("npc_jeremy", "confront_inina") and DL:isQuestState("ininas_abduction", "started")) then 
			DL:addChoice(16, "DL_Choice_ConfrontInina") -- (Confront Jeremy about Inina)
		end
		if (DL:isConditionFulfilled("npc_jeremy", "confront_inina") and not DL:isConditionFulfilled("npc_jeremy", "confront_inina2")) then 
			DL:addChoice(22, "DL_Choice_WontLeave") -- I won't leave. Where have you taken her?
		end
		if (DL:isConditionFulfilled("npc_jeremy", "confront_inina2") and not DL:isConditionFulfilled("boss", "BossMercenaries")) then 
			DL:addChoice(23, "DL_Choice_AttackBoss") -- I have to beat it out of you then. [ATTACK]
		end
		DL:addChoice(-1, "") -- 
		DL:addNode()

		if (DL:isQuestState("stephens_helmet", "started") and not DL:isConditionFulfilled("npc_jeremy", "helmet")) then 

			DL:createNPCNode(2, 8, "DL_Jeremy_Helmet") -- Oh, you mean that pathetic paladin that gambled away everything he had? I'll have to disappoint you, we don't have that helmet anymore.
			DL:addNode()


			DL:createNPCNode(8, -2, "DL_Jeremy_Helmet2") -- We lost it somewhere in the sewers of Gandria. Good luck finding it there, between the nasty rats and oozes...
			DL:addQuestDescription("stephens_helmet", 1)
			DL:addConditionProgress("npc_jeremy", "helmet")
			DL:addNode()

		end

		if (DL:isQuestState("leeroy_chicken", "started") and DL:isConditionFulfilled("npc_helena","leeroy_chicken") and not DL:isConditionFulfilled("npc_jeremy", "leeroy_chicken")) then 

			DL:createNPCNode(7, 15, "DL_Jeremy_LeeroyChicken") -- (Chuckles) And why exactly should we leave? We like it here.  
			DL:addNode()


			DL:createNPCNode(15, -2, "DL_Jeremy_LeeroyChicken2") -- You should have a really good reason to make us leave - and you don't seem to have one. Get lost, kid.
			DL:addConditionProgress("npc_jeremy", "leeroy_chicken")
			DL:addQuestDescription("leeroy_chicken", 2)
			DL:addNode()

		end

		if (not DL:isConditionFulfilled("npc_jeremy", "who_are_you")) then 

			DL:createNPCNode(3, -2, "DL_Jeremy_WhoAreYou") -- Just some mercenaries. None of your business, buddy.
			DL:addConditionProgress("npc_jeremy", "who_are_you")
			DL:addNode()

		end

		if (not DL:isConditionFulfilled("npc_jeremy", "confront_inina") and DL:isQuestState("ininas_abduction", "started")) then 

			DL:createChoiceNode(16)
			if (not DL:isConditionFulfilled("npc_jeremy", "confront_direct")) then 
				DL:addChoice(17, "DL_Choice_ConfrontDirect") -- Have you heard, the High Priestess Inina was abducted!
			end
			if (not DL:isConditionFulfilled("npc_jeremy", "confront_nice")) then 
				DL:addChoice(18, "DL_Choice_ConfrontNicely") -- Have you seen the High Priestess Inina lately?
			end
			DL:addChoice(19, "DL_Choice_ConfrontHarsh") -- We know that you abducted Inina. Confess!
			DL:addNode()

			if (not DL:isConditionFulfilled("npc_jeremy", "confront_direct")) then 

				DL:createNPCNode(17, 20, "DL_Jeremy_ConfrontDirect") -- Interesting. And why are you telling me this?
				DL:addConditionProgress("npc_jeremy", "confront_direct")
				DL:addNode()


				DL:createChoiceNode(20)
				DL:addChoice(21, "DL_Choice_ConfrontDirect1") -- We suspect you have something to do with her abduction.
				DL:addChoice(-1, "DL_Choice_Nevermind") -- Eh, nevermind...
				DL:addNode()


				DL:createNPCNode(21, -2, "DL_Jeremy_ConfrontHarsh") -- 
				DL:addConditionProgress("npc_jeremy", "confront_inina")
				DL:addNode()

			end

			if (not DL:isConditionFulfilled("npc_jeremy", "confront_nice")) then 

				DL:createNPCNode(18, -2, "DL_Jeremy_ConfrontNicely") -- No, we haven't.
				DL:addConditionProgress("npc_jeremy", "confront_nice")
				DL:addNode()

			end


			DL:createNPCNode(19, -2, "DL_Jeremy_ConfrontHarsh") -- (Dangerously quiet) You don't have any evidence. Mind your own business. Shove off.
			DL:addConditionProgress("npc_jeremy", "confront_inina")
			DL:addNode()

		end

		if (DL:isConditionFulfilled("npc_jeremy", "confront_inina") and not DL:isConditionFulfilled("npc_jeremy", "confront_inina2")) then 

			DL:createNPCNode(22, -2, "DL_Jeremy_WontLeave") -- You're skating on thin ice now. I won't warn you again. Go.
			DL:addConditionProgress("npc_jeremy", "confront_inina2")
			DL:addNode()

		end

		if (DL:isConditionFulfilled("npc_jeremy", "confront_inina2") and not DL:isConditionFulfilled("boss", "BossMercenaries")) then 

			DL:createNPCNode(23, 24, "DL_Jeremy_AttackBoss") -- (Sighs) You won't listen, eh. Roy, Morgiana! Let's teach him a lesson.
			DL:addNode()


			DL:createNPCNode(24, -2, "") -- 
			DL:startLevel("res/level/boss_mercenaries/boss_mercenaries.tmx", 1, 1)
			DL:addNode()

		end

	end


	DL:createChoiceNode(9)
	DL:addChoice(10, "DL_Choice_NotYourBuddy") -- I'm not your buddy, guy.
	DL:addChoice(11, "DL_Choice_Okay") -- Okay...
	DL:addNode()


	DL:createNPCNode(10, 12, "DL_Jeremy_NotYourGuy") -- I'm not your guy, friend.
	DL:addNode()


	DL:createChoiceNode(12)
	DL:addChoice(13, "DL_Choice_NotYourFriend") -- I'm not your friend, buddy.
	DL:addChoice(14, "DL_Choice_Okay") -- 
	DL:addNode()


	DL:createNPCNode(13, 4, "DL_Choice_NotYourBuddy") -- 
	DL:addNode()


	DL:createChoiceNode(4)
	DL:addChoice(6, "DL_Jeremy_NotYourGuy") -- 
	DL:addChoice(5, "DL_Choice_Okay") -- 
	DL:addNode()


	DL:createNPCNode(6, -2, "DL_Choice_NotYourFriend") -- 
	DL:addNode()


	DL:createNPCNode(5, -1, "DL_Jeremy_Nothing") -- 
	DL:addConditionProgress("npc_jeremy", "buddy_guy")
	DL:addNode()


	DL:createNPCNode(14, -1, "DL_Jeremy_Nothing") -- 
	DL:addConditionProgress("npc_jeremy", "buddy_guy")
	DL:addNode()


	DL:createNPCNode(11, -1, "DL_Jeremy_Nothing") -- ...
	DL:addConditionProgress("npc_jeremy", "buddy_guy")
	DL:addNode()

end