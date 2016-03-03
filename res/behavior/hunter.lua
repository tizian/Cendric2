-- Behavior for enemy hunter
speechBubbleState = 0

update = function(B)
		if (speechBubbleState == 5) then
			B:leaveLevel()
		elseif (speechBubbleState == 4 and B:getPosX() > 2650) then
			B:say("IAmLeaving", 5)
			B:setMovingTarget(2900, 0)
			B:wait(5)
			B:addConditionProgress("default","tutorial_complete")
			B:addHint("LeaveLevel")
			B:setDisposed()
			speechBubbleState = speechBubbleState + 1
		elseif (speechBubbleState == 3 and B:getPosX() > 2000) then
			B:say("KillRats", 6)
			speechBubbleState = speechBubbleState + 1
		elseif (speechBubbleState == 2 and B:getPosX() > 1450) then
			B:say("FindAWay", 6)
			speechBubbleState = speechBubbleState + 1
		elseif (speechBubbleState == 1 and B:getPosX() > 850) then
			B:say("SpikesAreBad", 5)
			speechBubbleState = speechBubbleState + 1
		elseif (speechBubbleState == 0 and B:getPosX() > 0) then
			B:say("DestroyBlocks", 5)
			B:addHint("Chop")
			speechBubbleState = speechBubbleState + 1
		end
    end