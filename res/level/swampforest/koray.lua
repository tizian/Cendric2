-- Behavior for koray, the glowing moon

isCheckpoint = false
speechBubbleState = 0

update = function(B, W)
    if (not isCheckpoint and W:isConditionFulfilled("npc_koray","level_checkpoint")) then
        B:setPosition(150, 750)
        isCheckpoint = true
        speechBubbleState = 5
        return
    end
    if (speechBubbleState == 0) then
        B:say("KorayStart", 6)
        B:wait(6)
        speechBubbleState = 1
        return
    end
    if (speechBubbleState == 1) then
        B:say("KorayStart2", 4)
        B:wait(4)
        speechBubbleState = 2
    end
    if (speechBubbleState == 2 and B:getPosX() < 1300) then
        B:say("KorayPiranhas", 4)
        speechBubbleState = 3
        return
    end
    if (speechBubbleState == 3 and B:getPosX() < 700) then
        B:say("KorayTricky", 4)
        speechBubbleState = 4
        return
    end
    if (speechBubbleState == 4 and B:getPosX() > 600) then
        B:say("KoraySoon", 4)
        speechBubbleState = 5
        return
    end
    if (speechBubbleState == 5 and B:getPosY() < 750 and B:getPosX() > 1100) then
        B:say("KorayLetMe", 4)
        B:setMovingTarget(1100,660)
        B:executeSpell(0, 1200, 450)
        B:wait(4)
        speechBubbleState = 6
        return
    end
    if (speechBubbleState == 6) then
        B:resetMovingTarget()
        speechBubbleState = 7
        return
    end
    if (speechBubbleState == 7 and B:getPosY() < 600 and B:getPosX() > 1300) then
        B:say("KorayThere", 4)
        speechBubbleState = 8
        return
    end
    if (speechBubbleState == 8 and B:getPosY() < 600 and B:getPosX() > 1800) then
        B:say("KoraySpell", 4)
        B:setMovingTarget(1900,660)
        B:wait(4)
        speechBubbleState = 9
        return
    end
    if (speechBubbleState == 9) then
        B:executeSpell(0, 1920, 430)
        B:wait(3)
        speechBubbleState = 10
        return
    end
    if (speechBubbleState == 10) then
        B:say("KorayGotIt", 4)
        B:wait(4)
        speechBubbleState = 11
        return
    end
    if (speechBubbleState == 11) then
        B:say("KorayTelekinesis", 4)
        B:wait(5)
        speechBubbleState = 12
        return
    end
    if (speechBubbleState == 12) then
        B:say("KorayTelekinesis2", 4)
        W:learnSpell(5)
        W:addQuestProgress("element_master", "master_earth")
        W:changeQuestState("help_koray", "completed")
        W:addConditionProgress("npc_koray", "level_stop")
        B:setReplaceDistance(10000)
        B:wait(5)
        speechBubbleState = 13
        return
    end
    if (speechBubbleState == 13) then
        B:setMovingTarget(1720, 710)
        B:wait(2)
        speechBubbleState = 14
        return
    end
    if (speechBubbleState == 14) then
        B:setMovingTarget(1620, 860)
        B:wait(1)
        speechBubbleState = 15
        return
    end
    if (speechBubbleState == 15) then
        B:setMovingTarget(1820, 1010)
        B:wait(2)
        speechBubbleState = 16
        return
    end
    if (speechBubbleState == 16) then
        B:setMovingTarget(1960, 1210)
        B:wait(1)
        speechBubbleState = 17
        return
    end
    if (speechBubbleState == 17) then
        B:setMovingTarget(1780,1210)
        B:wait(1)
        speechBubbleState = 18
        return
    end
    if (speechBubbleState == 18) then
        B:say("KorayBye", 4)
        B:wait(4)
        speechBubbleState = 19
        return
    end
    if (speechBubbleState == 19) then
        B:leaveLevel()
        speechBubbleState = 20
        return
    end
end
