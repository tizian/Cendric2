onWin = function(W)
	W:addConditionProgress("boss", "BossYasha")
	W:setMap("res/map/yashasanctuary/yashasanctuary.tmx", 580, 710)
end
	
onLose = function(W)
	W:setMap("res/map/yashasanctuary/yashasanctuary.tmx", 1560, 710)
end