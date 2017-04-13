-- Dialogue for NPC "Zeff"
loadDialogue = function(DL)
	
	DL:createChoiceNode(0)
	if (not DL:isConditionFulfilled("npc_zeff", "who_are_you")) then 
		DL:addChoice(1, "DL_Choice_WhoAreYou") -- Who are you?
		DL:addChoice(2, "DL_Choice_WhatAreYou") -- What are you?
	end
	if (not DL:isConditionFulfilled("npc_zeff", "what_are_you_doing")) then
		DL:addChoice(20, "DL_Choice_WhatAreYouDoing") -- What are you doing here?
	end
	if (DL:isConditionFulfilled("npc_zeff", "curse") and not DL:isConditionFulfilled("npc_zeff", "curse_talked")) then
		DL:addChoice(30, "DL_Choice_Curse") -- What's this curse you are talking about?
	end
	if (DL:isConditionFulfilled("npc_zeff", "curse_talked") and not DL:isConditionFulfilled("npc_zeff", "curse_stronger")) then
		DL:addChoice(30, "DL_Choice_CurseStronger") -- So, this curse is getting stronger?
	end
	if (DL:isQuestState("hungry_wolf", "completed") and DL:isConditionFulfilled("npc_zeff", "curse_talked") and DL:isQuestState("zeffs_curse", "void")) then
		DL:addChoice(40, "DL_Choice_HelpCurse") -- Can I help you somehow breaking that curse?
	end
	if (DL:isConditionFulfilled("npc_zeff", "curse_talked") and not DL:isConditionFulfilled("npc_zeff", "mark_curse")) then
		DL:addChoice(100, "DL_Choice_MyCurse") -- I got cursed too, do you know what it is? (Show the mark)
	end
	if (DL:isConditionFulfilled("npc_zeff", "leatherworker")) then
		DL:addChoice(50, "DL_Choice_Leatherworking") -- Could you craft something for me?
	end
	if (DL:isConditionFulfilled("npc_zeff", "who_are_you") and not DL:isConditionFulfilled("npc_zeff", "gandria")) then
		DL:addChoice(70, "DL_Choice_LivedInGandria") -- You were a citizen of Gandria?
	end
	
	if (DL:isQuestState("monster_problem","started") and DL:isConditionFulfilled("npc_zeff", "curse_talked") and not DL:isConditionFulfilled("npc_zeff", "sheep")) then
		DL:addChoice(60, "DL_Choice_Sheep") -- There went a few sheep missing...
	end
	if (DL:isQuestState("monster_problem","started") and not DL:isQuestComplete("monster_problem") and DL:isConditionFulfilled("npc_zeff", "curse_talked") and DL:isConditionFulfilled("npc_zeff", "sheep")) then
	
		if (DL:isQuestState("hungry_wolf", "void")) then
			DL:addChoice(80, "DL_Choice_SheepWhatDo") -- What can I do to convince you to stop stealing sheep?
		end
		DL:addChoice(81, "DL_Choice_AttackHim") -- I was asked to kill the sheep thief. And I will do so. [ATTACK]
		
	end
	if (DL:isQuestState("hungry_wolf", "started") and DL:isQuestComplete("hungry_wolf")) then
		DL:addChoice(90, "DL_Choice_IGotMeat") --  I got some meat for you. I hope that's enough.
	end
	if (DL:isQuestState("zeffs_curse", "started") and DL:isQuestComplete("zeffs_curse")) then
		DL:addChoice(110, "DL_Choice_BeastSlain") -- I found the beast that cursed you in the caverns...
	end
	if (DL:isQuestState("zeff_returns", "started") and DL:isQuestComplete("zeff_returns")) then
		DL:addChoice(120, "DL_Choice_GoHomeZeff") -- I've talked to the guards. They'll let you in.
	end

	DL:addChoice(-1, "DL_Choice_CU") --  See you later.
	DL:addNode()
	
	DL:setRoot(0)
	
	if (not DL:isConditionFulfilled("npc_zeff", "what_are_you_doing")) then 
	
		DL:createNPCNode(20, -2, "DL_Zeff_SittingHere") -- I'm sitting here at the fire, hoping not to devour anyone before that curse devours myself.
		DL:addConditionProgress("npc_zeff", "what_are_you_doing")
		DL:addConditionProgress("npc_zeff", "curse")
		DL:addNode()
	
	end

	if (not DL:isConditionFulfilled("npc_zeff", "who_are_you")) then 

		DL:createNPCNode(1, 3, "DL_Zeff_IAmWho") -- I'm Zeff.
		DL:addNode()
		
		DL:createNPCNode(2, 3, "DL_Zeff_IAmWhat") -- (Growls quietly) I see, my outer appearance is striking. 
		DL:addNode()
		
		DL:createNPCNode(3, 4, "DL_Zeff_IAmSuprised") -- I'm suprised you have the courage to talk to me. Most people I meet flee in terror when they see my face.
		DL:addConditionProgress("npc_zeff", "who_are_you")
		DL:addNode()
		
		DL:createChoiceNode(4)
		DL:addChoice(5, "DL_Choice_YouLookNice") -- I don't think you look terrifying.
		DL:addChoice(6, "DL_Choice_YouLookTerrible") -- That's not suprising. You truly look scary.
		DL:addChoice(7, "DL_Choice_IAmNotAfraid") -- I'm not afraid of you.
		DL:addNode()
		
		DL:createNPCNode(5, 10, "DL_Zeff_ThankYouLooks") -- Thank you. You're the first person to say something nice since I was turned into this monster.
		DL:addNode()
		
		DL:createNPCNode(6, 10, "DL_Zeff_IAmScary") -- (Hangs his head) Yes I do. I really wish I wouldn't have been turned in such a monster.
		DL:addNode()
		
		DL:createNPCNode(7, 10, "DL_Zeff_YouShouldBe") -- Maybe you should be. My mind is still human, but I feel that it will also be turned into the one of a monster soon.
		DL:addNode()
		
		DL:createChoiceNode(10)
		DL:addChoice(13, "DL_Choice_WhyMonster") -- Why were you turned into... this?
		DL:addChoice(11, "DL_Choice_Werewolf") -- Are you a werewolf?
		DL:addNode()
		
		DL:createNPCNode(11, 12, "DL_Zeff_Werewolf") -- You've never met a werewolf, have you? A werewolf wouldn't be able to talk to you.
		DL:addNode()
		
		DL:createCendricNode(12, 13, "DL_Choice_WhyMonster") -- Why were you turned into... this?
		DL:addNode()
		
		DL:createNPCNode(13, -2, "DL_Zeff_WhyMonster") -- A horrible curse was cast upon me - and now I can't return to the city of Gandria, I'd be a threat to the citizens.
		DL:addConditionProgress("npc_zeff", "curse")
		DL:addNode()
		
	end
	
	if (DL:isQuestState("hungry_wolf", "completed") and DL:isConditionFulfilled("npc_zeff", "curse_talked") and DL:isQuestState("zeffs_curse", "void")) then

		DL:createNPCNode(40, 41, "DL_Zeff_HelpCurse") -- Maybe. If you are brave enough, you can go and try to kill that thing. Killing the creator of a curse will break it in most cases.
		DL:addNode()
		
		DL:createNPCNode(41, -2, "DL_Zeff_HelpCurse2") -- The cave where it hides is right outside of the city. But be cautious and don't let it stare into your eyes.
		DL:changeQuestState("zeffs_curse", "started")
		DL:addNode()
		
	end
	
	if (DL:isConditionFulfilled("npc_zeff", "curse_talked") and not DL:isConditionFulfilled("npc_zeff", "mark_curse")) then
		
		DL:createNPCNode(100, 101, "DL_Zeff_ShowMark") -- Hrr... (Glares at your mark) I've never seen something like that. 
		DL:addConditionProgress("npc_zeff", "mark_curse")
		DL:addNode()
		
		DL:createNPCNode(101, 102, "DL_Zeff_ShowMark2") -- But I think you got off cheaply... I mean look at my face! Having a fancy tattoo can't be worse than be forced to kill sheep every night.
		DL:addNode()
		
		DL:createChoiceNode(102)
		if (DL:isQuestState("monster_problem","started") and not DL:isConditionFulfilled("npc_zeff", "sheep")) then
			DL:addChoice(61, "DL_Choice_ItWasYou") -- So it was YOU who stole the sheep from the farmers!
		end
		DL:addChoice(-2, "DL_Choice_YouAreRight") -- Maybe you're right...
		DL:addChoice(-2, "DL_Choice_MyCurseIsWorse") -- My curse could be worse, I just don't know.
		DL:addNode()
		
	end
	
	if (DL:isConditionFulfilled("npc_zeff", "curse_talked") and not DL:isConditionFulfilled("npc_zeff", "curse_stronger")) then
	
		DL:createNPCNode(30, -2, "DL_Zeff_CurseStronger") -- Yes. In the beginning, this fur grew and shortly after that, I got wolf ears and eyes. Then this magnificent tail popped out and now I'm wondering for how long I can still walk on two legs.
		DL:addConditionProgress("npc_zeff", "curse_stronger")
		DL:addNode()
	
	end
	
	if (DL:isConditionFulfilled("npc_zeff", "curse") and not DL:isConditionFulfilled("npc_zeff", "curse_talked")) then
	
		DL:createNPCNode(30, 31, "DL_Zeff_Curse") -- The curse that forces me to have the appearance of a monster. And soon, also its mind.
		DL:addConditionProgress("npc_zeff", "curse_talked")
		DL:addNode()
		
		DL:createChoiceNode(31)
		DL:addChoice(34, "DL_Choice_HowCursed") -- Why did you get cursed?
		DL:addChoice(32, "DL_Choice_CurseStronger") -- So, this curse is getting stronger?
		DL:addNode()
		
		DL:createNPCNode(32, 33, "DL_Zeff_CurseStronger") -- Yes. In the beginning, this fur grew and shortly after that, I got wolf ears and eyes. Then this magnificent tail popped out and now I'm wondering for how long I can still walk on two legs.
		DL:addConditionProgress("npc_zeff", "curse_stronger")
		DL:addNode()
		
		DL:createCendricNode(33, 34, "DL_Choice_HowCursed") -- Why did you get cursed?
		DL:addNode()
		
		DL:createNPCNode(34, 35, "DL_Zeff_Hunting") -- I went out of the city to hunt for fur, when I discovered a cave full of wolves. 
		DL:addNode()
		
		DL:createNPCNode(35, 36, "DL_Zeff_Hunting2") -- So I entered that cave, hoping to get one or two of them with my "skinning knife" (holds up his sword). But I didn't get far.
		DL:addNode()
		
		DL:createNPCNode(36, -2, "DL_Zeff_Hunting3") -- Then this... thing appeared. I couldn't see much, because it was dark in there, but it was huge. One glowing eye, staring at me... The next thing I remember is waking up at the entry of the cave, covered in fur.
		DL:addNode()
		
	end
	
	if (DL:isConditionFulfilled("npc_zeff", "who_are_you") and not DL:isConditionFulfilled("npc_zeff", "gandria")) then
		
		DL:createNPCNode(70, 71, "DL_Zeff_LivedGandria") -- Yes, I did. I had a beautiful little leather workshop in Gandria. (Sobs) But it seems like I'll never see it again.
		DL:addConditionProgress("npc_zeff", "gandria")
		DL:addNode()
		
		DL:createChoiceNode(71)
		DL:addChoice(72, "DL_Choice_WhyDontReturn") -- Why don't you try to go back?
		DL:addChoice(-2, "DL_Choice_Sorry") -- I'm so sorry for you.
		DL:addNode()
		
		DL:createNPCNode(72, -2, "DL_Zeff_BadGuards") -- The city guards won't believe what happened and try to kill me on sight. I'd rather die here, alone and without harming anyone.
		DL:addNode()

	end
	
	if (DL:isQuestState("monster_problem","started") and DL:isConditionFulfilled("npc_zeff", "curse_talked") and not DL:isConditionFulfilled("npc_zeff", "sheep")) then
		
		DL:createNPCNode(60, 61, "DL_Zeff_YouSuspect") -- Yes... yes... and now, only because I look like a wolf, I'm your first suspect.
		DL:addNode()
		
		DL:createNPCNode(61, 62, "DL_Zeff_YoureRight") -- Well, to be honest, you're totally right. I was the one who stole the sheep. But I can't just walk into the tavern and buy some meat, can I?
        DL:addConditionProgress("npc_zeff", "sheep")
		DL:addNode()
		
		DL:createChoiceNode(62)
		DL:addChoice(63, "DL_Choice_AttackHim") -- I was asked to kill the sheep thief. And I will do so. [ATTACK]
		DL:addChoice(65, "DL_Choice_BadassSword") -- You have a badass sword, killing sheep is just low.
		DL:addChoice(66, "DL_Choice_SheepWhatDo") -- What can I do to convince you to stop stealing sheep?
		DL:addNode()
		
		DL:createNPCNode(63, 64, "DL_Zeff_ComeAndTry") -- That's the last mistake you'll ever make.
		DL:addNode()
		
		DL:createNPCNode(64, -1, "") -- ...
		DL:startLevel("res/level/boss_zeff/boss_zeff.tmx", 150, 500)
		DL:addNode()

		DL:createNPCNode(65, -2, "DL_Zeff_BadassSword") -- Yes, but when I get hungry, I can't control myself anymore. But better sheep than people, I guess.
		DL:addNode()
		
		DL:createNPCNode(66, -2, "DL_Zeff_HungryWolf") -- If you can get me some meat from the tavern, I can stop killing sheep, at least for a while. And after that, I will go for other prey, promised.
		DL:changeQuestState("hungry_wolf", "started")
		DL:addNode()
		
	end
	
	if (DL:isQuestState("monster_problem","started") and DL:isConditionFulfilled("npc_zeff", "curse_talked") and DL:isConditionFulfilled("npc_zeff", "sheep")) then
	
		if (DL:isQuestState("hungry_wolf", "void")) then
		
			DL:createNPCNode(80, -2, "DL_Zeff_HungryWolf") -- If you can get me some meat from the tavern, I can stop killing sheep, at least for a while. And after that, I will go for other prey, promised.
			DL:changeQuestState("hungry_wolf", "started")
			DL:addNode()
		
		end
		
		DL:createNPCNode(81, 82, "DL_Zeff_ComeAndTry") -- That's the last mistake you'll ever make.
		DL:addNode()
		
		DL:createNPCNode(82, -1, "") -- ...
		DL:startLevel("res/level/boss_zeff/boss_zeff.tmx", 150, 500)
		DL:addNode()
		
	end
	
	if (DL:isQuestState("zeffs_curse", "started") and DL:isQuestComplete("zeffs_curse")) then
	
		DL:createCendricNode(110, 111, "DL_Cendric_BeastSlain") -- ... It's dead.
		DL:addNode()
	
		DL:createNPCNode(111, 112, "DL_Zeff_BeastSlain") -- Really? (Stares at his paws) I don't feel any difference. 
		DL:addConditionProgress("npc_zeff", "beast_slain")
		DL:addNode()
		
		DL:createNPCNode(112, 113, "DL_Zeff_BeastSlain2") -- The curse didn't get worse though lately. Maybe the transformation just stopped...
		DL:addNode()
		
		DL:createChoiceNode(113)
		DL:addChoice(114, "DL_Choice_StayWolf") -- Yes, it seems like you'll stay a wolf.
		DL:addChoice(115, "DL_Choice_ReturnToGandria") -- Will you be able to return to Gandria anyway?
		DL:addNode()
		
		DL:createNPCNode(114, 115, "DL_Zeff_StayWolf") -- I guess so. If my mind keeps clear, I think I can even get to like that form. But when I think about returning home...
		DL:addNode()
		
		DL:createNPCNode(115, -2, "DL_Zeff_ReturningHome") -- I'm afraid, I still can't get past the guards. Maybe you could try to tell them what happened and convince them to let me in? When I get my workshop back, I'm sure I could craft you a piece of armour as a reward.
		DL:changeQuestState("zeffs_curse", "completed")
		DL:changeQuestState("zeff_returns", "started")
		DL:addNode()
		
	end
	
	if (DL:isQuestState("hungry_wolf", "started") and DL:isQuestComplete("hungry_wolf")) then
		
		DL:createNPCNode(90, -2, "DL_Zeff_ThanksForMeat") -- Thank you, that's great. You can tell the farmers that the beast won't come again. And if you wish, I can help you out with my leatherworking skills, as long as I'm still human enough to craft something.
		DL:removeItem("fo_rawmeat", 3)
		DL:removeItem("fo_sausage", 2)
		DL:removeItem("fo_ham", 1)
		DL:changeQuestState("hungry_wolf", "completed")
		DL:addQuestProgress("monster_problem", "monster_banished")
		DL:addConditionProgress("npc_zeff", "leatherworker")
		DL:addNode()
		
	end
	
	if (DL:isConditionFulfilled("npc_zeff", "leatherworker")) then	
		
		DL:createNPCNode(50, 51, "DL_Zeff_Craft") -- Sure, if you got a fur for me?
		DL:addNode()
		
		DL:createChoiceNode(51)
		if (DL:hasItem("mi_firerat_fur", 1)) then
			DL:addItemChoice(52, "DL_Choice_FireRatFur", "mi_firerat_fur", 1) -- Scarf of the Fire Rat
		end
		DL:addChoice(-2, "DL_Choice_NothingToCraft") -- I'll come back later. [BACK]
		DL:addNode()
		
		DL:createNPCNode(52, -2, "DL_Zeff_FireRatFur") -- The fur of a fire rat, how nice. It has the ability to protect you against fire. 
		DL:addItem("eq_fireratscarf", 1)
		DL:removeItem("mi_firerat_fur", 1)
		DL:gotoNode(51)
		DL:addNode()
		
	end
	
	if (DL:isQuestState("zeff_returns", "started") and DL:isQuestComplete("zeff_returns")) then
	
		DL:createNPCNode(120, -1, "DL_Zeff_IWillGoHome") -- Thank you for the great news. I can't wait to see my beautiful house again! Don't forget to visit me in Gandria to get your reward.
		DL:changeQuestState("zeff_returns", "completed")
		DL:addConditionProgress("npc_zeff", "zeff_goes")
		DL:addNode()
		
	end

end	
	