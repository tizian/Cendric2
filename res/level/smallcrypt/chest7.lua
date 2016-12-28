-- chest (7) in level smallcrypt                                        

onLoot = function(W) 
	local enemyData = {
		id = 22, -- skeleton rogue
		loot = {
				{id="gold", amount=2},
				{id="fo_glowingshroom", amount=1},
			},
		position = {x=2360, y=1260}
	}
	W:spawnEnemy(enemyData)
end

