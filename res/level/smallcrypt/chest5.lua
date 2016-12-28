-- chest (5) in level smallcrypt                                        

onLoot = function(W) 
	local enemyData = {
		id = 22,
		loot = {
				{id="fo_greaterhealingpotion", amount=1},
			},
		position = {x=2250, y=610}
	}
	local enemyData2 = {
		id = 22,
		loot = {
				{id="gold", amount=10},
			},
		position = {x=2170, y=610}
	}
	W:spawnEnemy(enemyData)
	W:spawnEnemy(enemyData2)
end

