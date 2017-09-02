-- Behavior for yaslaw, the dead necromancer

speechBubbleState = 0

update = function(B, W)
    if (speechBubbleState == 0) then
        B:say("Yaslaw1", 4)
        B:wait(5)
        B:setMovingTarget(1700, 710)
        speechBubbleState = 1
    elseif (speechBubbleState == 1) then
        B:say("Yaslaw2", 5)
        B:wait(6)
        B:setMovingTarget(1225, 710)
        speechBubbleState = 2
    elseif (speechBubbleState == 2) then
        B:say("Yaslaw3", 3)
        B:wait(3)
        speechBubbleState = 3
    elseif (speechBubbleState == 3) then
        B:setMovingTarget(1500, 710)
        B:say("Yaslaw4", 5)
        B:wait(5)
        speechBubbleState = 4
    elseif (speechBubbleState == 4) then
        B:say("Yaslaw5", 4)
        B:wait(5)
        speechBubbleState = 5
    elseif (speechBubbleState == 5) then
        B:setMovingTarget(1870, 710)
        B:say("Yaslaw6", 4)
        B:wait(4)
        speechBubbleState = 6
    elseif (speechBubbleState == 6) then
        B:setMovingTarget(2140, 610)
        W:addConditionProgress("npc_yaslaw", "door_open")
        B:say("Yaslaw7", 4)
        B:wait(5)
        speechBubbleState = 7
    elseif (speechBubbleState == 7) then 
        B:setMovingTarget(2220, 510)
        B:say("Yaslaw8", 5)
        B:wait(6)
        speechBubbleState = 8
    elseif (speechBubbleState == 8) then 
        B:say("Yaslaw9", 5)
        B:wait(6)
        speechBubbleState = 9
    else
        B:leaveLevel()
    end
end
