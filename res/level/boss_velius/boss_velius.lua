onWin = function(W)
	W:addConditionProgress("boss", "BossVelius")
    W:unlockAchievement("ACH_KILL_VELIUS")
	W:setMap("res/map/veliusroom/veliusroom.tmx", 645, 530)
end
	
onLose = function(W)
	W:setMap("res/map/veliusroom/veliusroom.tmx", 570, 430)
end