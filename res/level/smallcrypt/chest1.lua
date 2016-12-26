-- chest (1) in level smallcrypt                                        

onLoot = function(W) 
	local enemyData = {
		id = 21,
		loot = {
				{id="gold", amount=2},
			},
		position = {x=200, y=500}
	}
	W:spawnEnemy(enemyData)
end

