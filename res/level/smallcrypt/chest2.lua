-- chest (2) in level smallcrypt                                        

onLoot = function(W) 
	local enemyData = {
		id = 22,
		loot = {
				{id="gold", amount=2},
			},
		position = {x=1610, y=610}
	}
	W:spawnEnemy(enemyData)
end

