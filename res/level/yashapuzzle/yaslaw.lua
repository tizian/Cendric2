-- Behavior for yaslaw, the dead necromancer

speechBubbleState = 0

update = function(B, W)
    if (speechBubbleState == 0) then
        B:say("YaslawStart", 6)
        B:wait(6)
        B:setMovingTarget(1700, 710)
        speechBubbleState = 1
        return
    end
    if (speechBubbleState == 1) then
        B:setMovingTarget(1500, 710)
        B:say("YaslawStart2", 4)
        B:wait(4)
        speechBubbleState = 2
    end
    if (speechBubbleState == 2) then 
        B:setMovingTarget(1850, 710)
        W:addConditionProgress("npc_yaslaw", "door_open")
    end
end
