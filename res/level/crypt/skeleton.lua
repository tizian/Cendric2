-- Behavior for the skeleton in the crypt

onDeath = function(B, W)
    W:addConditionProgress("default", "plateau_open")
end
