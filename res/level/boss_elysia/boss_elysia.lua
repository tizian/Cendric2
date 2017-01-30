onWin = function(W)
	W:setLevel("res/level/jacklighthouse/jacklighthouse.tmx", 1360, 1460)
	W:addConditionProgress("boss", "BossElysia")	
end
	
onLose = function(W)
	W:setLevel("res/level/jacklighthouse/jacklighthouse.tmx", 1360, 1460)
end