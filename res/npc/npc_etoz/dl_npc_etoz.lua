-- Dialogue for NPC "npc_etoz"
loadDialogue = function(DL) 

	if (not DL:isConditionFulfilled("npc_etoz", "talked")) then 
		DL:setRoot(1) 
	elseif (DL:isQuestState("tower_key_1", "void")) then 
		DL:setRoot(2) 
	elseif (DL:isQuestState("tower_1", "void")) then 
		DL:setRoot(25) 
	elseif (not DL:isQuestState("tower_1", "completed")) then 
		DL:setRoot(34) 
	elseif (DL:isQuestState("tower_key_2", "void")) then 
		DL:setRoot(45) 
	elseif (DL:isQuestState("tower_2", "void")) then 
		DL:setRoot(46) 
	elseif (not DL:isQuestState("tower_2", "completed")) then 
		DL:setRoot(47) 
	elseif (DL:isQuestState("tower_key_3", "void")) then 
		DL:setRoot(48) 
	elseif (DL:isQuestState("tower_3", "void")) then 
		DL:setRoot(49) 
	elseif (not DL:isQuestState("tower_3", "completed")) then 
		DL:setRoot(50) 
	elseif (DL:isQuestState("tower_key_4", "void")) then 
		DL:setRoot(51) 
	elseif (DL:isQuestState("tower_4", "void")) then 
		DL:setRoot(62) 
	elseif (not DL:isQuestState("tower_4", "completed")) then 
		DL:setRoot(63) 
	else 
		DL:setRoot(52) 
	end 

	if (not DL:isConditionFulfilled("npc_etoz", "talked")) then 

		DL:createNPCNode(1, 7, "DL_Etoz_Hello") -- You there! What is a milksop like you doing in the sewers?
		DL:addConditionProgress("npc_etoz", "talked")
		DL:addNode()


		DL:createChoiceNode(7)
		DL:addChoice(8, "DL_Choice_YouMilksob") -- I could ask you the same thing.
		DL:addChoice(9, "DL_Choice_Unfriendly") -- Are you always this ill-bred?
		DL:addChoice(-1, "DL_Choice_NoTalk") -- I have more important stuff to do.
		DL:addNode()


		DL:createNPCNode(8, -2, "DL_Etoz_YouMilksob") -- Hn. You don't seem to know who you're talking to.
		DL:addNode()


		DL:createNPCNode(9, -2, "DL_Etoz_Unfriendly") -- Oh, I'm not ill-bred. On the contrary - I'm only telling the truth.
		DL:addNode()

	end

	if (DL:isQuestState("tower_key_1", "void")) then 

		DL:createChoiceNode(2)
		if (not DL:isConditionFulfilled("npc_etoz", "who_are_you")) then 
			DL:addChoice(10, "DL_Choice_WhoAreYou") -- Who are you?
		end
		if (not DL:isConditionFulfilled("npc_etoz", "questionable_method") and DL:isQuestState("questionable_method", "started")) then 
			DL:addChoice(3, "DL_Choice_QuestionableMethod") -- Have you seen any nasty creatures in the sewers?
		end
		if (not DL:isConditionFulfilled("npc_etoz", "sword")) then 
			DL:addChoice(14, "DL_Choice_Sword") -- What is this strange sword you're carrying around?
		end
		if (DL:isConditionFulfilled("npc_etoz", "who_are_you") and  not DL:isConditionFulfilled("npc_etoz", "powerful")) then 
			DL:addChoice(12, "DL_Choice_Powerful") -- And what exactly makes you this "powerful"?
		end
		if (DL:isConditionFulfilled("npc_etoz", "powerful") and not DL:isConditionFulfilled("npc_etoz", "challenge")) then 
			DL:addChoice(16, "DL_Choice_Challenge") -- You just seem to be talking big...
		end
		if (DL:isConditionFulfilled("npc_etoz", "challenge") and not DL:isConditionFulfilled("npc_etoz", "not_weak")) then 
			DL:addChoice(19, "DL_Choice_NotWeak") -- You underestimate me. I'm no weakling.
		end
		if (DL:isConditionFulfilled("npc_etoz", "not_weak")) then 
			DL:addChoice(22, "DL_Choice_StartKeyOne") -- Tell me more about that "dangerous" job.
		end
		DL:addChoice(-1, "") -- 
		DL:addNode()

		if (not DL:isConditionFulfilled("npc_etoz", "who_are_you")) then 

			DL:createNPCNode(10, 11, "DL_Etoz_WhoAreYou") -- It surprises me that you haven't heard of me yet.
			DL:addConditionProgress("npc_etoz", "who_are_you")
			DL:addNode()


			DL:createNPCNode(11, -2, "DL_Etoz_WhoAreYou2") -- (Flips his hair back) I'm Etoz, the most powerful mage you'll ever see. 
			DL:addNode()

		end

		if (not DL:isConditionFulfilled("npc_etoz", "questionable_method") and DL:isQuestState("questionable_method", "started")) then 

			DL:createNPCNode(3, 4, "DL_Etoz_QuestionableMethod") -- Well, one is standing right in front of me. (Giggles)
			DL:addConditionProgress("npc_etoz", "questionable_method")
			DL:addNode()


			DL:createNPCNode(4, 5, "DL_Etoz_QuestionableMethod2") -- But if you are looking for nasty sewer rats and stuff, I saw some in the southern part of the sewers.
			DL:addNode()


			DL:createNPCNode(5, 6, "DL_Etoz_QuestionableMethod3") -- But you'll need not only a key to go there...
			DL:addNode()


			DL:createNPCNode(6, -2, "DL_Etoz_QuestionableMethod4") -- You clearly lack what it takes to survive down here.
			DL:addNode()

		end

		if (not DL:isConditionFulfilled("npc_etoz", "sword")) then 

			DL:createNPCNode(14, 15, "DL_Etoz_Sword") -- That's no "strange" sword, you fool. You better hope that you'll never get in my way.
			DL:addConditionProgress("npc_etoz", "sword")
			DL:addNode()


			DL:createNPCNode(15, -2, "DL_Etoz_Sword2") -- This sword would be the last thing you'd see.
			DL:addNode()

		end

		if (DL:isConditionFulfilled("npc_etoz", "who_are_you") and  not DL:isConditionFulfilled("npc_etoz", "powerful")) then 

			DL:createNPCNode(12, 13, "DL_Etoz_Powerful") -- Oh, if I would tell you of my adventures, you would tremble in awe.
			DL:addConditionProgress("npc_etoz", "powerful")
			DL:addNode()


			DL:createNPCNode(13, -2, "DL_Etoz_Powerful2") -- I've slain countless foes, cast the most complex spells and explored the most dangerous areas... Those stories are nothing for your feeble ears.
			DL:addNode()

		end

		if (DL:isConditionFulfilled("npc_etoz", "powerful") and not DL:isConditionFulfilled("npc_etoz", "challenge")) then 

			DL:createNPCNode(16, 17, "DL_Etoz_Challenge") -- How dare you doubt my skills. I could make you burst into flames with a single word.
			DL:addConditionProgress("npc_etoz", "challenge")
			DL:addNode()


			DL:createNPCNode(17, 18, "DL_Etoz_Challenge2") -- But I understand your inexperienced eyes can't recognise a true master. I'll forgive you for this one time.
			DL:addNode()


			DL:createNPCNode(18, -2, "DL_Etoz_Challenge3") -- Hn. You seem so weak, so helpless... fighting you wouldn't even be fun.
			DL:addNode()

		end

		if (DL:isConditionFulfilled("npc_etoz", "challenge") and not DL:isConditionFulfilled("npc_etoz", "not_weak")) then 

			DL:createNPCNode(19, 20, "DL_Etoz_NotWeak") -- (Etoz raises one eyebrow) Oh, really? Well, you must be such a tough boy then!
			DL:addConditionProgress("npc_etoz", "not_weak")
			DL:addNode()


			DL:createNPCNode(20, 21, "DL_Etoz_NotWeak2") -- It is by sheer coincidence that I know some adventure that would be a perfect fit for such a tough one like you.
			DL:addNode()


			DL:createNPCNode(21, -2, "DL_Etoz_NotWeak3") -- Lots of gold and glory awaiting you. But, I guess you're not interested in the real dangerous jobs...
			DL:addNode()

		end

		if (DL:isConditionFulfilled("npc_etoz", "not_weak")) then 

			DL:createNPCNode(22, 23, "DL_Etoz_StartKeyOne") -- If you insist... (Etoz takes a part of a key out of his cloak)
			DL:addNode()


			DL:createNPCNode(23, 24, "DL_Etoz_StartKeyOne2") -- That's half of a key opening a dangerous tower.
			DL:addNode()


			DL:createNPCNode(24, -2, "DL_Etoz_StartKeyOne3") -- If you're really so tough, get the other part of the key and I'll give you this part.
			DL:changeQuestState("tower_key_1", "started")
			DL:addNode()

		end

	end

	if (DL:isQuestState("tower_1", "void")) then 

		DL:createChoiceNode(25)
		if (not DL:isConditionFulfilled("npc_etoz", "where_key_part") and not DL:hasItem("qe_tower_1", 1)) then 
			DL:addChoice(26, "DL_Choice_WhereKeyPart") -- Where can I find the other part?
		end
		if (not DL:isConditionFulfilled("npc_etoz", "where_your_part")) then 
			DL:addChoice(27, "DL_Choice_WhereYourPart") -- Where did you get your part from?
		end
		if (DL:isQuestComplete("tower_key_1")) then 
			DL:addChoice(30, "DL_Choice_GotKey") -- I got my part of the key.
		end
		DL:addChoice(-1, "") -- 
		DL:addNode()

		if (not DL:isConditionFulfilled("npc_etoz", "where_key_part") and not DL:hasItem("qe_tower_1", 1)) then 

			DL:createNPCNode(26, 28, "DL_Etoz_WhereKeyPart") -- This would've been up to you to find out. But as you're so clueless and pitiful, I'll tell you.
			DL:addConditionProgress("npc_etoz", "where_key_part")
			DL:addQuestDescription("tower_1", 1)
			DL:addNode()


			DL:createNPCNode(28, -2, "DL_Etoz_WhereKeyPart2") -- There's a small room in the East of the sewers. You'll find it there.
			DL:addNode()

		end

		if (not DL:isConditionFulfilled("npc_etoz", "where_your_part")) then 

			DL:createNPCNode(27, 29, "DL_Etoz_WhereYourPart") -- Oh. I had to climb the highest mountains, dive into the deepest sea... cross lava pits and fight dangerous beasts...
			DL:addConditionProgress("npc_etoz", "where_your_part")
			DL:addNode()


			DL:createNPCNode(29, -2, "DL_Etoz_WhereYourPart2") -- Your part is so much easier to get. Be glad.
			DL:addNode()

		end

		if (DL:isQuestComplete("tower_key_1")) then 

			DL:createNPCNode(30, 31, "DL_Etoz_GotKey1") -- Easy, wasn't it. Now let's combine it...
			DL:removeItem("qe_tower_1", 1)
			DL:changeQuestState("tower_key_1", "completed")
			DL:addNode()


			DL:createNPCNode(31, 32, "DL_Etoz_GotKey12") -- And it's done. Here, take it.
			DL:addItem("ke_tower_1", 1)
			DL:addNode()


			DL:createNPCNode(32, 33, "DL_Etoz_GotKey13") -- Find the old tower it opens and show me, what a big boy you are.
			DL:changeQuestState("tower_1", "started")
			DL:addNode()


			DL:createNPCNode(33, -2, "DL_Etoz_GotKey14") -- Reach the chest on top of the tower and come back to me if you survive...
			DL:addNode()

		end

	end

	if (not DL:isQuestState("tower_1", "completed")) then 

		DL:createChoiceNode(34)
		if (not DL:isConditionFulfilled("npc_etoz", "where_tower")) then 
			DL:addChoice(35, "DL_Choice_WhereTower") -- Where is this tower?
		end
		if (not DL:isConditionFulfilled("npc_etoz", "why_dont_you_go")) then 
			DL:addChoice(37, "DL_Choice_WhyDontYouGo") -- Why don't you go there yourself?
		end
		if (not DL:isConditionFulfilled("npc_etoz", "tower_info")) then 
			DL:addChoice(39, "DL_Choice_TowerInfo") -- Tell me more about this tower.
		end
		if (DL:isConditionFulfilled("npc_etoz", "tower_info") and not DL:isConditionFulfilled("npc_etoz", "anti_magic")) then 
			DL:addChoice(43, "DL_Choice_AntiMagic") -- What exactly are "anti-magic crystals"?
		end
		if (DL:isQuestComplete("tower_1")) then 
			DL:addChoice(36, "DL_Choice_FirstTowerClimbed") -- I climbed the tower. It was easy.
		end
		DL:addChoice(-1, "") -- 
		DL:addNode()

		if (not DL:isConditionFulfilled("npc_etoz", "where_tower")) then 

			DL:createNPCNode(35, -2, "DL_Etoz_WhereTower") -- On a forgotten island to the East of the marshland. I hope you know how to swim. (Grins)
			DL:addConditionProgress("npc_etoz", "where_tower")
			DL:addNode()

		end

		if (not DL:isConditionFulfilled("npc_etoz", "why_dont_you_go")) then 

			DL:createNPCNode(37, 38, "DL_Etoz_WhyDontYouGo") -- I've been there countless times. It's getting boring, you know.
			DL:addConditionProgress("npc_etoz", "why_dont_you_go")
			DL:addNode()


			DL:createNPCNode(38, -2, "DL_Etoz_WhyDontYouGo2") -- But you, you need to show me what you got. We'll see whether you make it.
			DL:addNode()

		end

		if (not DL:isConditionFulfilled("npc_etoz", "tower_info")) then 

			DL:createNPCNode(39, 40, "DL_Etoz_TowerInfo") -- The tower was built a long time ago by a mage who was nearly as powerful as me.
			DL:addConditionProgress("npc_etoz", "tower_info")
			DL:addNode()


			DL:createNPCNode(40, 41, "DL_Etoz_TowerInfo2") -- He put anti-magic crystals in there to test the real willpower and agility of fellow mages.
			DL:addNode()


			DL:createNPCNode(41, 42, "DL_Etoz_TowerInfo3") -- As most mages rely only on their magic, it was an impossible task for many of them.
			DL:addNode()


			DL:createNPCNode(42, -2, "DL_Etoz_TowerInfo4") -- But the ones who did it - including myself of course - grew stronger than ever before...
			DL:addNode()

		end

		if (DL:isConditionFulfilled("npc_etoz", "tower_info") and not DL:isConditionFulfilled("npc_etoz", "anti_magic")) then 

			DL:createNPCNode(43, 44, "DL_Etoz_AntiMagic") -- Oh, you're so pathetic. I really have to explain everything.
			DL:addConditionProgress("npc_etoz", "anti_magic")
			DL:addNode()


			DL:createNPCNode(44, -2, "DL_Etoz_AntiMagic2") -- Those crystals block your magic. You won't be able to use it there.
			DL:addNode()

		end

		if (DL:isQuestComplete("tower_1")) then 

			DL:createNPCNode(36, -2, "DL_Etoz_FirstTowerClimbed") -- Well done.
			DL:changeQuestState("tower_1", "completed")
			DL:addNode()

		end

	end

	if (DL:isQuestState("tower_key_2", "void")) then 

		DL:createChoiceNode(45)
		DL:addChoice(53, "DL_Choice_GimmeKeyQuest") -- Gimme the key quest.
		DL:addChoice(-1, "") -- 
		DL:addNode()


		DL:createNPCNode(53, -2, "DL_Etoz_GimmeKeyQuest") -- Here.
		DL:changeQuestState("tower_key_2", "started")
		DL:addNode()

	end

	if (DL:isQuestState("tower_2", "void")) then 

		DL:createChoiceNode(46)
		if (DL:isQuestComplete("tower_key_2")) then 
			DL:addChoice(54, "DL_Choice_GotKey") -- 
		end
		DL:addChoice(-1, "") -- 
		DL:addNode()

		if (DL:isQuestComplete("tower_key_2")) then 

			DL:createNPCNode(54, 55, "DL_Etoz_GotKey2") -- Good.
			DL:removeItem("qe_tower_2", 1)
			DL:changeQuestState("tower_key_2", "completed")
			DL:addNode()


			DL:createNPCNode(55, -2, "DL_Etoz_GotKey22") -- Here.
			DL:addItem("ke_tower_2", 1)
			DL:changeQuestState("tower_2", "started")
			DL:addNode()

		end

	end

	if (not DL:isQuestState("tower_2", "completed")) then 

		DL:createChoiceNode(47)
		if (DL:isQuestComplete("tower_2")) then 
			DL:addChoice(56, "DL_Choice_FirstTowerClimbed") -- 
		end
		DL:addChoice(-1, "") -- 
		DL:addNode()

		if (DL:isQuestComplete("tower_2")) then 

			DL:createNPCNode(56, -2, "DL_Etoz_FirstTowerClimbed") -- 
			DL:changeQuestState("tower_2", "completed")
			DL:addNode()

		end

	end

	if (DL:isQuestState("tower_key_3", "void")) then 

		DL:createChoiceNode(48)
		DL:addChoice(57, "DL_Choice_GimmeKeyQuest") -- 
		DL:addChoice(-1, "") -- 
		DL:addNode()


		DL:createNPCNode(57, -2, "DL_Etoz_GimmeKeyQuest") -- 
		DL:changeQuestState("tower_key_3", "started")
		DL:addNode()

	end

	if (DL:isQuestState("tower_3", "void")) then 

		DL:createChoiceNode(49)
		if (DL:isQuestComplete("tower_key_3")) then 
			DL:addChoice(58, "DL_Choice_GotKey") -- 
		end
		DL:addChoice(-1, "") -- 
		DL:addNode()

		if (DL:isQuestComplete("tower_key_3")) then 

			DL:createNPCNode(58, 59, "DL_Etoz_GotKey2") -- 
			DL:removeItem("qe_tower_3", 1)
			DL:changeQuestState("tower_key_3", "completed")
			DL:addNode()


			DL:createNPCNode(59, -2, "DL_Etoz_GotKey22") -- 
			DL:addItem("ke_tower_3", 1)
			DL:changeQuestState("tower_3", "started")
			DL:addNode()

		end

	end

	if (not DL:isQuestState("tower_3", "completed")) then 

		DL:createChoiceNode(50)
		if (DL:isQuestComplete("tower_3")) then 
			DL:addChoice(60, "DL_Choice_FirstTowerClimbed") -- 
		end
		DL:addChoice(-1, "") -- 
		DL:addNode()

		if (DL:isQuestComplete("tower_3")) then 

			DL:createNPCNode(60, -2, "DL_Etoz_FirstTowerClimbed") -- 
			DL:changeQuestState("tower_3", "completed")
			DL:addNode()

		end

	end

	if (DL:isQuestState("tower_key_4", "void")) then 

		DL:createChoiceNode(51)
		DL:addChoice(61, "DL_Choice_GimmeKeyQuest") -- 
		DL:addChoice(-1, "") -- 
		DL:addNode()


		DL:createNPCNode(61, -2, "DL_Etoz_GimmeKeyQuest") -- 
		DL:changeQuestState("tower_key_4", "started")
		DL:addNode()

	end

	if (DL:isQuestState("tower_4", "void")) then 

		DL:createChoiceNode(62)
		if (DL:isQuestComplete("tower_key_4")) then 
			DL:addChoice(64, "DL_Choice_GotKey") -- 
		end
		DL:addChoice(-1, "") -- 
		DL:addNode()

		if (DL:isQuestComplete("tower_key_4")) then 

			DL:createNPCNode(64, 65, "DL_Etoz_GotKey2") -- 
			DL:removeItem("qe_tower_4", 1)
			DL:changeQuestState("tower_key_4", "completed")
			DL:addNode()


			DL:createNPCNode(65, -2, "DL_Etoz_GotKey22") -- 
			DL:addItem("ke_tower_4", 1)
			DL:changeQuestState("tower_4", "started")
			DL:addNode()

		end

	end

	if (not DL:isQuestState("tower_4", "completed")) then 

		DL:createChoiceNode(63)
		if (DL:isQuestComplete("tower_4")) then 
			DL:addChoice(66, "DL_Choice_FirstTowerClimbed") -- 
		end
		DL:addChoice(-1, "") -- 
		DL:addNode()

		if (DL:isQuestComplete("tower_4")) then 

			DL:createNPCNode(66, -2, "DL_Etoz_FirstTowerClimbed") -- 
			DL:changeQuestState("tower_4", "completed")
			DL:addNode()

		end

	end


	DL:createChoiceNode(52)
	DL:addChoice(-1, "") -- 
	DL:addNode()

end