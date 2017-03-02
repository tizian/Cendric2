-- chest (6) in level smallcrypt                                        

onLoot = function(W) 
	local enemyData = {
		id = 5,
		loot = {
				{id="gold", amount=3},
				{id="fo_lesserhealingpotion", amount=1},
			},
		position = {x=1610, y=1260}
	}
	W:spawnEnemy(enemyData)
end

