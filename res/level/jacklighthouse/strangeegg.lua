-- chest (strange egg) in level jacks lighthouse                                        

onLoot = function(W) 
	local enemyData = {
		id = 27,
		position = {x=2330, y=840},
		luapath = "res/level/jacklighthouse/elysia.lua",
	}
	W:spawnEnemy(enemyData)
	W:addConditionProgress("default","elysia_intro")
end

