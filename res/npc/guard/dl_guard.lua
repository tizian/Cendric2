-- Dialogue for NPC "guard"
loadDialogue = function(DL)
		DL:createChoiceNode(0)
		DL:addChoice(1, "DL_Choice_TalkToMe") -- Talk to me.
		DL:addChoice(5, "DL_Choice_SellMeSomething") --  Sell me something.
		DL:addChoice(-1, "DL_Choice_Bye")
		DL:addNode()
		
		DL:setRoot(0)
		
		DL:createTradeNode(5, 0, "DL_Trade_TakeALook") -- Take a look.
		DL:addNode()
		
        if (DL:isNPCState("guard", "never_talked")) then
            
			DL:createNPCNode(1, 2, "DL_Guard_Hello") -- Hey you, stop! What are you doing here, so very alone? It's dangerous out here.
			DL:changeNPCState("guard", "passive")
			DL:addQuestProgress("a_cave_of_rats", "Test")
			DL:addNode()
			
			DL:createChoiceNode(2)
			DL:addChoice(3, "DL_Choice_GiveMeAQuest") -- Give me a quest.
			DL:addChoice(-1, "DL_Choice_Nothing") --  None of your business.
			DL:addNode()
			
			DL:createNPCNode(3, 4, "DL_Guard_KillSomeRats") -- Okay, go and kill some rats for me.
			DL:addNode()
			
			DL:createCendricNode(4, -1, "DL_Cendric_Okay") -- Okay, I will do that
			DL:changeQuestState("a_cave_of_rats", "started")
			DL:addNode()
			
        elseif (DL:isQuestState("a_cave_of_rats", "started")) then
			
			if (DL:isQuestComplete("a_cave_of_rats")) then
			
				DL:createNPCNode(1, 2, "DL_Guard_GoodJob") -- Good job, boy! What do you want as a reward? 
				DL:changeNPCState("guard", "passive")
				DL:addNode()
			
				DL:createChoiceNode(2)
				DL:addChoice(3, "DL_Choice_GiveMeGold") -- Give me some gold.
				DL:addChoice(4, "DL_Choice_GiveMeItem") --  I'd rather have the item.
				DL:addNode()
			
				DL:createNPCNode(3, -1, "DL_Guard_GivesReward") -- Okay, here you go.
				DL:changeQuestState("a_cave_of_rats", "completed")
				DL:addGold(5)
				DL:addNode()
			
				DL:createNPCNode(4, -1, "DL_Guard_GivesReward") -- Okay, here you go.
				DL:changeQuestState("a_cave_of_rats", "completed")
				DL:addItem("mi_goldengoblet", 1)
				DL:addNode()
			
				DL:setRoot(0)
			
			else
			
				DL:createNPCNode(1, -1, "DL_Guard_KillThemAlready") -- You haven't killed those rats! Go and do your job.
				DL:addNode()
			
			end
			
		elseif (DL:isQuestState("a_cave_of_rats", "completed")) then
            
			DL:createNPCNode(1, -1, "DL_Guard_GoAway") -- Go away, I have nothing to do for you.
			DL:addNode()
			
		else
		
			DL:createNPCNode(1, 2, "DL_Guard_StillHere") -- Are you sure you don't want to help me?
			DL:addNode()
			
			DL:createChoiceNode(2)
			DL:addChoice(-1, "DL_Choice_Yes") --  Yes, totally.
			DL:addChoice(3, "DL_Choice_GiveMeAQuest") -- Give me a quest.
			DL:addNode()
			
			DL:createNPCNode(3, 4, "DL_Guard_KillSomeRats") -- Okay, go and kill some rats for me.
			DL:addNode()
			
			DL:createCendricNode(4, -1, "DL_Cendric_Okay") -- Okay, I will do that
			DL:changeQuestState("a_cave_of_rats", "started")
			DL:addNode()
			
        end
    end