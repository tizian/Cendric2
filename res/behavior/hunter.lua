-- Behavior for enemy hunter
speechBubbleState = 0

update = function(B)
		if (speechBubbleState == 5) then
			B:leaveLevel()
		elseif (speechBubbleState == 4 and B:getPosX() > 2050) then
			B:say("IamLeaving", 3)
			B:wait(3)
			speechBubbleState = speechBubbleState + 1
		elseif (speechBubbleState == 3 and B:getPosX() > 1750) then
			B:say("KillRats", 5)
			speechBubbleState = speechBubbleState + 1
		elseif (speechBubbleState == 2 and B:getPosX() > 1500) then
			B:say("FindAWay", 5)
			speechBubbleState = speechBubbleState + 1
		elseif (speechBubbleState == 1 and B:getPosX() > 850) then
			B:say("SpikesAreBad", 5)
			speechBubbleState = speechBubbleState + 1
		elseif (speechBubbleState == 0 and B:getPosX() > 0) then
			B:say("DestroyBlocks", 5)
			speechBubbleState = speechBubbleState + 1
		end
    end