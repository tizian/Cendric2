-- chest (2) in level gandriacrypt                                        

onLoot = function(W) 
	local enemyData = {
		id = 5,
		loot = {
				{id="gold", amount=2},
			},
		position = {x=3000, y=2160}
	}
	W:spawnEnemy(enemyData)
end


