-- Dialogue for NPC "npc_morris"

loadDialogue = function(DL) 

	-- morris' coin toss.
	math.randomseed(os.time())
	local isWinUnfair = (math.random(0, 100) < 40)
	local isWinFair = (math.random(0, 1) < 1)
	local headNode = -1
	local tailsNode = -1

	if (not DL:isConditionFulfilled("npc_morris", "talked")) then 
		DL:setRoot(1) 
	elseif (not DL:isConditionFulfilled("npc_morris", "game")) then 
		DL:setRoot(8) 
	elseif (not DL:isConditionFulfilled("npc_morris", "rules")) then 
		DL:setRoot(11) 
	else 
		DL:setRoot(13) 
	end 

	if (not DL:isConditionFulfilled("npc_morris", "talked")) then 

		DL:createChoiceNode(1)
		DL:addChoice(2, "DL_Choice_Hi") -- Hi, I'm new here.
		DL:addChoice(-1, "") -- 
		DL:addNode()


		DL:createNPCNode(2, 3, "DL_Morris_IAm") -- Ah, a new prisoner. And what brought you here?
		DL:addConditionProgress("npc_morris", "talked")
		DL:addNode()


		DL:createChoiceNode(3)
		DL:addChoice(4, "DL_Choice_Stole") -- I stole something.
		DL:addChoice(5, "DL_Choice_Murdered") -- I've murdered someone.
		DL:addChoice(6, "DL_Choice_Innocent") -- I'm innocent!
		DL:addChoice(7, "DL_Choice_NoneBusiness") -- That's none of your business.
		DL:addNode()


		DL:createNPCNode(4, -2, "DL_Morris_Stole") -- Yes, the observer spells can be trouble, I can tell from my own experience.
		DL:addNode()


		DL:createNPCNode(5, -2, "DL_Morris_Murdered") -- Funny, you don't look like someone that is capable of murdering people. But I'll watch my back. (Grins)
		DL:addNode()


		DL:createNPCNode(6, -2, "DL_Morris_Innocent") -- Aren't we all?
		DL:addNode()


		DL:createNPCNode(7, -1, "DL_Morris_NoneBusiness") -- Oh well. Must be something really bad then. (Rolls his eyes)
		DL:addNode()

	end

	if (not DL:isConditionFulfilled("npc_morris", "game")) then 

		DL:createChoiceNode(8)
		DL:addChoice(9, "DL_Choice_WhatBroughtYou") -- And why are you here?
		DL:addChoice(-1, "") -- 
		DL:addNode()


		DL:createNPCNode(9, 10, "DL_Morris_MyCrime") -- This and that. Pilferage, gambling, fraud... Oh, and beating up the guy who snitched on me.
		DL:addNode()


		DL:createNPCNode(10, -2, "DL_Morris_Game") -- Speaking of gambling, I still offer some fun to the prisoners here. Interested?
		DL:addConditionProgress("npc_morris", "game")
		DL:addNode()

	end

	if (not DL:isConditionFulfilled("npc_morris", "rules")) then 

		DL:createChoiceNode(11)
		DL:addChoice(12, "DL_Choice_GameRules") -- How does your game work?
		DL:addChoice(-1, "") -- 
		DL:addNode()


		DL:createNPCNode(12, -2, "DL_Morris_GameRules") -- It's pretty simple. See this coin? You bet a certain amount of gold on head or tails, I toss it. If you're correct with your guess, I double your gold. Otherwise, I keep it.
		DL:addConditionProgress("npc_morris", "rules")
		DL:addNode()

	end


	DL:createChoiceNode(13)
	DL:addChoice(14, "DL_Choice_IWantPlay") -- I want to play...
	DL:addChoice(-1, "") -- 
	DL:addNode()


	DL:createChoiceNode(14)
	if (DL:hasItem("gold", 20)) then 
	DL:addItemChoice(15, "DL_Choice_Gold", "gold", 20) -- 
	end
	if (DL:hasItem("gold", 50)) then 
	DL:addItemChoice(19, "DL_Choice_Gold", "gold", 50) -- 
	end
	if (DL:hasItem("gold", 100) and not DL:isConditionFulfilled("npc_morris", "dagger")) then 
	DL:addItemChoice(20, "DL_Choice_Gold", "gold", 100) -- ... Here is my bet.
	end
	DL:addChoice(-1, "DL_Choice_NotEnoughGold") -- I'll come back later.
	DL:addNode()

	if (DL:hasItem("gold", 20)) then 

		DL:createNPCNode(15, 16, "DL_Morris_HeadOrTails") -- Fine. Head or tails?
		DL:removeGold(20)
		DL:addNode()


		DL:createChoiceNode(16)
		DL:addChoice(17, "DL_Choice_Head") -- Head.
		DL:addChoice(18, "DL_Choice_Tails") -- Tails.
		DL:addNode()
		
		if (isWinUnfair) then
			headNode = 201
			tailsNode = 204
		else
			headNode = 202
			tailsNode = 203
		end

		DL:createNPCNode(17, headNode, "DL_Morris_TossCoin") -- (Morris tosses the coin...)
		DL:addNode()

		DL:createNPCNode(18, tailsNode, "DL_Morris_TossCoin") -- 
		DL:addNode()
		
		--  winning head
		DL:createNPCNode(201, -2, "DL_Morris_WinHead") -- Head! You win. Here is your bet.
		DL:addGold(40)
		DL:addNode()
		
		--  losing head
		DL:createNPCNode(202, -2, "DL_Morris_LoseHead") -- Tails! I'm so sorry, I'll have to keep your gold.
		DL:addNode()
		
		--  winning tails
		DL:createNPCNode(204, -2, "DL_Morris_WinTails") -- Tails! You win. Here is your bet.
		DL:addGold(40)
		DL:addNode()
		
		--  losing tails
		DL:createNPCNode(203, -2, "DL_Morris_LoseTails") -- Head! I'm so sorry, I'll have to keep your gold.
		DL:addNode()

	end

	if (DL:hasItem("gold", 50)) then 

		DL:createNPCNode(19, 23, "DL_Morris_HeadOrTails") -- 
		DL:removeGold(50)
		DL:addNode()


		DL:createChoiceNode(23)
		DL:addChoice(24, "DL_Choice_Head") -- 
		DL:addChoice(25, "DL_Choice_Tails") -- 
		DL:addNode()
		
		if (isWinUnfair) then
			headNode = 501
			tailsNode = 503
		else
			headNode = 502
			tailsNode = 504
		end

		DL:createNPCNode(24, headNode, "DL_Morris_TossCoin") -- 
		DL:addNode()


		DL:createNPCNode(25, tailsNode, "DL_Morris_TossCoin") -- 
		DL:addNode()
		
		--  winning head
		DL:createNPCNode(501, -2, "DL_Morris_WinHead") -- Head! You win. Here is your bet.
		DL:addGold(100)
		DL:addNode()
		
		--  losing head
		DL:createNPCNode(502, -2, "DL_Morris_LoseHead") -- Tails! I'm so sorry, I'll have to keep your gold.
		DL:addNode()
		
		--  winning tails
		DL:createNPCNode(504, -2, "DL_Morris_WinTails") -- Tails! You win. Here is your bet.
		DL:addGold(100)
		DL:addNode()
		
		--  losing tails
		DL:createNPCNode(503, -2, "DL_Morris_LoseTails") -- Head! I'm so sorry, I'll have to keep your gold.
		DL:addNode()

	end

	if (DL:hasItem("gold", 100) and not DL:isConditionFulfilled("npc_morris", "dagger")) then 

		DL:createNPCNode(20, 21, "DL_Morris_DaggerBet") -- That's a really high bet. I don't have enough gold, but I put my dagger then, it's worth more than 100 gold.
		DL:removeGold(100)
		DL:addNode()

		DL:createNPCNode(21, 26, "DL_Morris_HeadOrTails") -- 
		DL:addNode()


		DL:createChoiceNode(26)
		DL:addChoice(28, "DL_Choice_Head") -- 
		DL:addChoice(29, "DL_Choice_Tails") -- 
		DL:addNode()
		
		if (isWinFair) then
			headNode = 1001
			tailsNode = 1004
		else
			headNode = 1002
			tailsNode = 1003
		end

		DL:createNPCNode(28, headNode, "DL_Morris_TossCoin") -- 
		DL:addNode()


		DL:createNPCNode(29, tailsNode, "DL_Morris_TossCoin") -- 
		DL:addNode()
		
		--  winning head
		DL:createNPCNode(1001, 1005, "DL_Morris_WinHeadDagger") -- Head! You win. I guess I have to leave you my beloved dagger then.
		DL:addConditionProgress("npc_morris", "dagger")
		DL:addItem("we_enchanteddagger", 1)
		DL:addNode()
		
		DL:createNPCNode(1005, -2, "DL_Morris_WinDagger2") -- You can't imagine what it took to sneak that in... Well, take good care of it.
		DL:addNode()
		
		--  losing head
		DL:createNPCNode(1002, -2, "DL_Morris_LoseHead") -- Tails! I'm so sorry, I'll have to keep your gold.
		DL:addNode()
		
		--  winning tails
		DL:createNPCNode(1004, 1005, "DL_Morris_WinTailsDagger") -- Tails! You win. I guess I have to leave you my beloved dagger then. Take good care of it.
		DL:addConditionProgress("npc_morris", "dagger")
		DL:addItem("we_enchanteddagger", 1)
		DL:addNode()
		
		--  losing tails
		DL:createNPCNode(1003, -2, "DL_Morris_LoseTails") -- Head! I'm so sorry, I'll have to keep your gold.
		DL:addNode()

	end

end