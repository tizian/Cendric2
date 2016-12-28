-- chest (cinderbloom chest) in level smallcrypt                                        

onLoot = function(W) 
	local enemyData = {
		id = 21,
		loot = {
				{id="gold", amount=2},
			},
		position = {x=2460, y=210}
	}
	W:spawnEnemy(enemyData)
end

