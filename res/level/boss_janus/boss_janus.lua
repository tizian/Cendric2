onWin = function(W)
	W:addConditionProgress("boss", "BossJanus")
	W:setLevel("res/level/janusroom/janusroom.tmx", 580, 710)
    W:unlockAchievement("ACH_KILL_JANUS")
end
	
onLose = function(W)
	W:setLevel("res/level/forgottenpassage/forgottenpassage.tmx", 1560, 710)
end