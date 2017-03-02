-- chest (1) in level smallcrypt                                        

onLoot = function(W) 
	local enemyData = {
		id = 5,
		loot = {
				{id="gold", amount=2},
			},
		position = {x=1310, y=610}
	}
	W:spawnEnemy(enemyData)
end

