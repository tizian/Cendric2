-- Dialogue for NPC "Innkeeper"
loadDialogue = function(DL)

		DL:createChoiceNode(0)
		DL:addChoice(1, "DL_Choice_Rumors") -- Heard any rumours?
		DL:addChoice(5, "DL_Choice_ImThirsty") --  I'm thirsty. Give me something to drink. 
		DL:addChoice(-1, "") --
		DL:addNode()
		
		DL:setRoot(0)
		
		DL:createNPCNode(1, -1, "No") -- NO (untranslated)
		DL:addNode()
		
		DL:createTradeNode(5, 0, "DL_Trade_TakeALook") -- Take a look at my wares.
		DL:addNode()
		
    end