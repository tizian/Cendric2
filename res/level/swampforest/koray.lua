-- Behavior for koray, the glowing moon

isCheckpoint = false

update = function(B, W)
    if (not isCheckpoint and W:isConditionFulfilled("npc_koray","level_checkpoint")) then
        B:setPosition(150, 750)
        isCheckpoint = true
    end
end
