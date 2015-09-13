-- Dialogue for NPC "npc_guard"
loadDialogue = function(DL)
        if (DL:isNPCState("npc_guard", "never_talked")) then
            
			DL:createNPCNode(0, 1, "DL_Guard_Hello") -- Hey you, stop! What are you doing here, so very alone? It's dangerous out here.
			DL:changeNPCState("npc_guard", "passive")
			DL:addNode()
			
			DL:createChoiceNode(1)
			DL:addChoice(2, "DL_Choice_GiveMeAQuest") -- Give me a quest.
			DL:addChoice(-1, "DL_Choice_Nothing") --  None of your business.
			DL:addNode()
			
			DL:createNPCNode(2, 3, "DL_Guard_KillSomeRats") -- Okay, go and kill some rats for me.
			DL:addNode()
			
			DL:createCendricNode(3, -1, "DL_Cendric_Okay") -- Okay, I will do that
			DL:changeQuestState("quest_acaveofrats", "started")
			DL:addNode()
			
			DL:setRoot(0)
			
        elseif (DL:isQuestState("quest_acaveofrats", "started")) then
			
			DL:createNPCNode(0, -1, "DL_Guard_KillThemAlready") -- You haven't killed those rats! Go and do your job.
			DL:addNode()
			
			DL:setRoot(0)
			
		else
            
			DL:createNPCNode(0, 1, "DL_Guard_StillHere") -- Are you sure you don't want to help me?
			DL:addNode()
			
			DL:createChoiceNode(1)
			DL:addChoice(-1, "DL_Choice_Yes") --  Yes, totally.
			DL:addChoice(2, "DL_Choice_GiveMeAQuest") -- Give me a quest.
			DL:addNode()
			
			DL:createNPCNode(2, 3, "DL_Guard_KillSomeRats") -- Okay, go and kill some rats for me.
			DL:addNode()
			
			DL:createCendricNode(3, -1, "DL_Cendric_Okay") -- Okay, I will do that
			DL:changeQuestState("quest_acaveofrats", "started")
			DL:addNode()
			
			DL:setRoot(0)
			
        end
    end