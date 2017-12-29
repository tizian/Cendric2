onWin = function(W)
	W:addConditionProgress("boss", "BossYasha")
	W:setMap("res/map/yashasanctuary/yashasanctuary.tmx", 615, 620)
    if (W:isConditionFulfilled("npc_yasha", "friendly")) then
        W:unlockAchievement("ACH_FRIEND_YASHA")
    else
        W:unlockAchievement("ACH_KILL_YASHA")
    end
end
	
onLose = function(W)
	W:setMap("res/map/yashasanctuary/yashasanctuary.tmx", 615, 800)
end