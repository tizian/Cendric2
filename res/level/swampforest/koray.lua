-- Behavior for koray, the glowing moon

isCheckpoint = false
speechBubbleState = 0

update = function(B, W)
    if (not isCheckpoint and W:isConditionFulfilled("npc_koray","level_checkpoint")) then
        B:setPosition(150, 750)
        isCheckpoint = true
        speechBubbleState = 4
        return
    end
    if (speechBubbleState == 0) then
        B:say("KorayStart", 5)
        B:wait(5)
        B:say("KorayStart1", 4)
        speechBubbleState = 1
        return
    end
    if (speechBubbleState == 1 and B:getPosX() < 1300) then
        B:say("KorayPiranhas", 4)
        speechBubbleState = 2
        return
    end
    if (speechBubbleState == 2 and B:getPosX() < 700) then
        B:say("KorayTricky", 4)
        speechBubbleState = 3
        return
    end
    if (speechBubbleState == 4 and B:getPosX() > 600) then
        B:say("KoraySoon", 4)
        speechBubbleState = 5
        return
    end
    if (speechBubbleState == 5 and B:getPosY() < 600 and B:getPosX() > 1300) then
        B:say("KorayThere", 4)
        speechBubbleState = 6
        return
    end
    if (speechBubbleState == 6 and B:getPosY() < 600 and B:getPosX() > 1800) then
        B:say("KorayTSpell", 4)
        B:wait(4)
        B:executeSpell(0, 1920, 430)
        B:wait(5)
        W:removeItem("eq_mooneye",1)
        speechBubbleState = 6
        return
    end
    if (speechBubbleState == 6) then
        B:say("KorayThere", 4)
        speechBubbleState = 7
        return
    end
end
