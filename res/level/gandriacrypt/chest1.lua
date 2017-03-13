-- chest (1) in level smallcrypt                                        

onLoot = function(W) 
	local enemyData = {
		id = 22,
		loot = {
				{id="gold", amount=2},
                {id="mi_goldengoblet", amount=1},
			},
		position = {x=2410, y=2160}
	}
	W:spawnEnemy(enemyData)
end
