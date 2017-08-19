onWin = function(W)
	W:addConditionProgress("boss", "BossYasha")
	W:setMap("res/map/yashasanctuary/yashasanctuary.tmx", 615, 620)
end
	
onLose = function(W)
	W:setMap("res/map/yashasanctuary/yashasanctuary.tmx", 615, 800)
end