onWin = function(W)
	W:addConditionProgress("boss", "BossVelius")
	W:setMap("res/map/veliusroom/veliusroom.tmx", 400, 400)
end
	
onLose = function(W)
	W:setMap("res/map/veliusroom/veliusroom.tmx", 400, 400)
end