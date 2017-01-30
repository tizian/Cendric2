-- chest (strange egg) in level jacks lighthouse                                        

onLoot = function(W) 
	local enemyData = {
		id = 26,
		position = {x=1360, y=1200}
	}
	W:spawnEnemy(enemyData)
	W:addConditionProgress("default","elysia_intro")
end

