-- Dialogue for NPC "npc_velius"
loadDialogue = function(DL) 

	if (not DL:isConditionFulfilled("npc_velius", "talked")) then 
		DL:setRoot(1) 
	else 
		DL:setRoot(2) 
	end 

	if (not DL:isConditionFulfilled("npc_velius", "talked")) then 

		DL:createNPCNode(1, -2, "DL_NPC_Talked") -- Finally. I waited so long for this.
		DL:addConditionProgress("npc_velius", "talked")
		DL:addNode()

	end


	DL:createChoiceNode(2)
	if (not DL:isConditionFulfilled("npc_velius", "finally")) then 
		DL:addChoice(6, "DL_Choice_Finally") -- Ehm... You waited for what?
	end
	if (not DL:isConditionFulfilled("npc_velius", "murderer")) then 
		DL:addChoice(5, "DL_Choice_Murderer") -- You... just murdered the king?!
	end
	if (DL:isConditionFulfilled("npc_velius", "murderer") and not DL:isConditionFulfilled("npc_velius", "eyes")) then 
		DL:addChoice(53, "DL_Choice_Eyes") -- What's the matter with the kings's eyes?
	end
	if (not DL:isConditionFulfilled("npc_velius", "know_me")) then 
		DL:addChoice(3, "DL_Choice_KnowMe") -- I assume, you know me?
	end
	if (not DL:isConditionFulfilled("npc_velius", "friends")) then 
		DL:addChoice(4, "DL_Choice_Friends") -- What did you do to the other mages?
	end
	if (not DL:isConditionFulfilled("npc_velius", "mark")) then 
		DL:addChoice(9, "DL_Choice_Mark") -- Is that your work? (Show the mark)
	end
	if (DL:isConditionFulfilled("npc_velius", "know_me") and not DL:isConditionFulfilled("npc_velius", "escape")) then 
		DL:addChoice(12, "DL_Choice_Escape") -- How exactly did I escape?
	end
	if (DL:isConditionFulfilled("npc_velius", "mark") and not DL:isConditionFulfilled("npc_velius", "track")) then 
		DL:addChoice(18, "DL_Choice_Track") -- So you could track me - why didn't you catch me earlier?
	end
	if (not DL:isConditionFulfilled("npc_velius", "old_king")) then 
		DL:addChoice(22, "DL_Choice_OldKing") -- What did you do to the old king?
	end
	if (DL:isConditionFulfilled("npc_velius", "murderer") and not DL:isConditionFulfilled("npc_velius", "plan")) then 
		DL:addChoice(21, "DL_Choice_Plan") -- You don't need a king? What are your plans then?
	end
	if (DL:isConditionFulfilled("npc_velius", "track") and not DL:isConditionFulfilled("npc_velius", "why_me")) then 
		DL:addChoice(30, "DL_Choice_WhyMe") -- Why do you need exactly me for your plans?
	end
	if (DL:isConditionFulfilled("npc_velius", "plan") and DL:isConditionFulfilled("npc_velius", "anti_magic") and not DL:isConditionFulfilled("npc_velius", "fight")) then 
		DL:addChoice(29, "DL_Choice_FightStart") -- What exactly are those anti-magic crystals?
	end
	if (DL:isConditionFulfilled("npc_velius", "plan") and not DL:isConditionFulfilled("npc_velius", "bitter")) then 
		DL:addChoice(34, "DL_Choice_Bitter") -- Not all magic is bad. Why do you hate it so much?
	end
	if (DL:isConditionFulfilled("npc_velius", "why_me") and not DL:isConditionFulfilled("npc_velius", "anti_magic")) then 
		DL:addChoice(37, "DL_Choice_AntiMagic") -- What did you steal from the elementalists' temple?
	end
	if (DL:isConditionFulfilled("npc_velius", "why_me") and not DL:isConditionFulfilled("npc_velius", "creatures")) then 
		DL:addChoice(39, "DL_Choice_Creatures") -- So... those challenges weren't there to hinder me?
	end
	if (DL:isConditionFulfilled("npc_velius", "anti_magic") and not DL:isConditionFulfilled("npc_velius", "illusion")) then 
		DL:addChoice(43, "DL_Choice_Illusion") -- Five magic types? I thought there were only four...
	end
	if (DL:isConditionFulfilled("npc_velius", "fight")) then 
		DL:addChoice(27, "DL_Choice_Fight") -- I've heard enough. Let's get this over with. [ATTACK VELIUS] 
	end
	DL:addChoice(-1, "") -- 
	DL:addNode()

	if (not DL:isConditionFulfilled("npc_velius", "finally")) then 

		DL:createNPCNode(6, -2, "DL_Velius_Finally") -- For you. To fall directly into my trap. (Grins)
		DL:addConditionProgress("npc_velius", "finally")
		DL:addNode()

	end

	if (not DL:isConditionFulfilled("npc_velius", "murderer")) then 

		DL:createNPCNode(5, 7, "DL_Velius_Murderer") -- No. He's just sleeping. But, he nearly eliminated my most precious subject -
		DL:addConditionProgress("npc_velius", "murderer")
		DL:addNode()


		DL:createNPCNode(7, 8, "DL_Velius_Murderer2") -- You.
		DL:addNode()


		DL:createNPCNode(8, -2, "DL_Velius_Murderer3") -- But now, that I got you... there's no need for a king anymore.
		DL:addNode()

	end

	if (DL:isConditionFulfilled("npc_velius", "murderer") and not DL:isConditionFulfilled("npc_velius", "eyes")) then 

		DL:createNPCNode(53, -2, "DL_Velius_Eyes") -- I couldn't hide that, right. He's been under my control for too long.
		DL:addConditionProgress("npc_velius", "eyes")
		DL:addNode()

	end

	if (not DL:isConditionFulfilled("npc_velius", "know_me")) then 

		DL:createNPCNode(3, 10, "DL_Velius_KnowMe") -- Yes, I do. The subject that escaped.
		DL:addConditionProgress("npc_velius", "know_me")
		DL:addNode()


		DL:createNPCNode(10, 11, "DL_Velius_KnowMe2") -- But you've been never really free. I always knew exactly where you were and what you did.
		DL:addNode()


		DL:createNPCNode(11, -2, "DL_Velius_KnowMe3") -- My curse has fulfilled its purpose.
		DL:addNode()

	end

	if (not DL:isConditionFulfilled("npc_velius", "friends")) then 

		DL:createNPCNode(4, 20, "DL_Velius_Friends") -- Most of them didn't survive it. The ones who did, went insane.
		DL:addConditionProgress("npc_velius", "friends")
		DL:addNode()


		DL:createNPCNode(20, 24, "DL_Velius_Friends2") -- But after all these experiments, I finally know what I need.
		DL:addNode()


		DL:createNPCNode(24, -2, "DL_Velius_Friends3") -- Mages with a strong affinity to a magic type. The three I got now are perfect.
		DL:addNode()

	end

	if (not DL:isConditionFulfilled("npc_velius", "mark")) then 

		DL:createNPCNode(9, 17, "DL_Velius_Mark") -- (Smiles) Of course it is.
		DL:addConditionProgress("npc_velius", "mark")
		DL:addNode()


		DL:createNPCNode(17, -2, "DL_Velius_Mark2") -- It made my subjects easier to track - the few, that managed to escape.
		DL:addNode()

	end

	if (DL:isConditionFulfilled("npc_velius", "know_me") and not DL:isConditionFulfilled("npc_velius", "escape")) then 

		DL:createNPCNode(12, 13, "DL_Velius_Escape") -- Well, you really surprised me.
		DL:addConditionProgress("npc_velius", "escape")
		DL:addNode()


		DL:createNPCNode(13, 14, "DL_Velius_Escape2") -- I extracted your magic the same way as I did with many other mages before.
		DL:addNode()


		DL:createNPCNode(14, 15, "DL_Velius_Escape3") -- But somehow, it didn't work as expected.
		DL:addNode()


		DL:createNPCNode(15, 16, "DL_Velius_Escape4") -- You managed to disappear before my very eyes. 
		DL:addNode()


		DL:createNPCNode(16, -2, "DL_Velius_Escape5") -- But now, here you are...
		DL:addNode()

	end

	if (DL:isConditionFulfilled("npc_velius", "mark") and not DL:isConditionFulfilled("npc_velius", "track")) then 

		DL:createNPCNode(18, 19, "DL_Velius_Track") -- You weren't ready. Your magic wasn't ready.
		DL:addConditionProgress("npc_velius", "track")
		DL:addNode()


		DL:createNPCNode(19, -2, "DL_Velius_Track2") -- Also, as you've lost your memory, you've never been a threat to my secrets.
		DL:addNode()

	end

	if (not DL:isConditionFulfilled("npc_velius", "old_king")) then 

		DL:createNPCNode(22, 23, "DL_Velius_OldKing") -- Hn. The old king. I thought he'd be a suitable subject for the divine magic.
		DL:addConditionProgress("npc_velius", "old_king")
		DL:addNode()


		DL:createNPCNode(23, -2, "DL_Velius_OldKing2") -- But I was wrong. He wasn't strong enough. He didn't make it.
		DL:addNode()

	end

	if (DL:isConditionFulfilled("npc_velius", "murderer") and not DL:isConditionFulfilled("npc_velius", "plan")) then 

		DL:createNPCNode(21, 25, "DL_Velius_Plan") -- You see what's happening in this world, right?
		DL:addConditionProgress("npc_velius", "plan")
		DL:addNode()


		DL:createNPCNode(25, 26, "DL_Velius_Plan2") -- Mages are dominating the commoners. Just because you are born without magic, you're inferior.
		DL:addNode()


		DL:createNPCNode(26, -2, "DL_Velius_Plan3") -- But soon, this age will be history. All magic will perish from this world. FOREVER!
		DL:addNode()

	end

	if (DL:isConditionFulfilled("npc_velius", "track") and not DL:isConditionFulfilled("npc_velius", "why_me")) then 

		DL:createNPCNode(30, 31, "DL_Velius_WhyMe") -- I need affine mages from all magic types. Elementalists are very rare.
		DL:addConditionProgress("npc_velius", "why_me")
		DL:addNode()


		DL:createNPCNode(31, 32, "DL_Velius_WhyMe2") -- As I first caught you, you were very weak. Too weak for my purposes.
		DL:addNode()


		DL:createNPCNode(32, 33, "DL_Velius_WhyMe3") -- I needed a full-fledged elementalist. Only such a mage could overcome the challenges I put in your way.
		DL:addNode()


		DL:createNPCNode(33, -2, "DL_Velius_WhyMe4") -- You mastered the elements and found me. That's proof enough that your magic is strong enough now.
		DL:addNode()

	end

	if (DL:isConditionFulfilled("npc_velius", "plan") and DL:isConditionFulfilled("npc_velius", "anti_magic") and not DL:isConditionFulfilled("npc_velius", "fight")) then 

		DL:createNPCNode(29, 46, "DL_Velius_FightStart") -- Condensed anti-magic. The one I'm planning to create now will be powerful enough to take the magic from every mage in this world.
		DL:addConditionProgress("npc_velius", "fight")
		DL:addNode()


		DL:createNPCNode(46, 47, "DL_Velius_FightStart2") -- I'm going to create a better world for everyone. You understand why I need your magic for that, right?
		DL:addNode()


		DL:createChoiceNode(47)
		DL:addChoice(48, "DL_Choice_KillMe") -- No. If that means you're going to kill me...
		DL:addChoice(49, "DL_Choice_YoureRight") -- Well, I kinda understand you. But it's still not the right thing to do.
		DL:addChoice(51, "DL_Choice_YoureVeryRight") -- Yes. Magic only brings destruction. Just look at you.
		DL:addNode()


		DL:createNPCNode(48, 50, "DL_Velius_KillMe") -- Mages survive the process of annihilating their magic with anti-magic, but not extracting it, I'm sorry.
		DL:addNode()


		DL:createNPCNode(50, -2, "DL_Velius_KillMe2") -- It's for a greater cause. I'll make it quick if you don't struggle.
		DL:addNode()


		DL:createNPCNode(49, 52, "DL_Velius_YoureRight") -- That's your opinion. But it's worth nothing.
		DL:addNode()


		DL:createNPCNode(52, -2, "DL_Velius_YoureRight2") -- You won't have anyone to tell it anymore.
		DL:addNode()


		DL:createNPCNode(51, -2, "DL_Velius_YoureVeryRight") -- The same goes for you. No one should be this powerful. It only brings pain.
		DL:addNode()

	end

	if (DL:isConditionFulfilled("npc_velius", "plan") and not DL:isConditionFulfilled("npc_velius", "bitter")) then 

		DL:createNPCNode(34, 35, "DL_Velius_Bitter") -- You're right. Magic is not bad, but what people do with it is.
		DL:addConditionProgress("npc_velius", "bitter")
		DL:addNode()


		DL:createNPCNode(35, 36, "DL_Velius_Bitter2") -- I was born into a family of commoners. I know what it feels like to be treated like dirt.
		DL:addNode()


		DL:createNPCNode(36, -2, "DL_Velius_Bitter3") -- I had to find a way to take away this power from all mages. And I did.
		DL:addNode()

	end

	if (DL:isConditionFulfilled("npc_velius", "why_me") and not DL:isConditionFulfilled("npc_velius", "anti_magic")) then 

		DL:createNPCNode(37, 38, "DL_Velius_AntiMagic") -- I needed more information on anti-magic to develop the perfect anti-magic crystal.
		DL:addConditionProgress("npc_velius", "anti_magic")
		DL:addNode()


		DL:createNPCNode(38, -2, "DL_Velius_AntiMagic2") -- Now I got it. Inverting and combining magic from affine mages from all five magic types will result in the perfect crystal.
		DL:addNode()

	end

	if (DL:isConditionFulfilled("npc_velius", "why_me") and not DL:isConditionFulfilled("npc_velius", "creatures")) then 

		DL:createNPCNode(39, 40, "DL_Velius_Creatures") -- They were there to test you. Khajag. Alastor. Who do you think summoned them?
		DL:addConditionProgress("npc_velius", "creatures")
		DL:addNode()


		DL:createChoiceNode(40)
		DL:addChoice(41, "DL_Choice_NotYou") -- Well, certainly not someone as weak as you.
		DL:addChoice(42, "DL_Choice_YouDid") -- Okay, now, when I think about it...
		DL:addNode()


		DL:createNPCNode(41, -2, "DL_Velius_NotYou") -- Oh. You'll see...
		DL:addNode()


		DL:createNPCNode(42, -2, "DL_Velius_YouDid") -- They were great creatures, weren't they. Holding back weaklings and letting only the best mages pass...
		DL:addNode()

	end

	if (DL:isConditionFulfilled("npc_velius", "anti_magic") and not DL:isConditionFulfilled("npc_velius", "illusion")) then 

		DL:createNPCNode(43, 44, "DL_Velius_Illusion") -- Magic is a spectrum, like colours. The magic type I call Illusion was discovered by myself recently.
		DL:addConditionProgress("npc_velius", "illusion")
		DL:addNode()


		DL:createNPCNode(44, 45, "DL_Velius_Illusion2") -- A very powerful type of magic. The power of controlling and deceiving people shouldn't be misused.
		DL:addNode()


		DL:createNPCNode(45, -2, "DL_Velius_Illusion3") -- That's why I kept it a secret.
		DL:addNode()

	end

	if (DL:isConditionFulfilled("npc_velius", "fight")) then 

		DL:createNPCNode(27, 28, "DL_Velius_Fight") -- As you wish. Your sacrifice won't be in vain.
		DL:addNode()


		DL:createNPCNode(28, -1, "DL_Velius_Beta") -- (To be continued...)
		DL:startLevel("res/level/boss_velius/boss_velius.tmx", 100, 400)
		DL:addNode()

	end

end