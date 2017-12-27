-- chest in level ppuzzle04                               

onLoot = function(W) 
	W:addConditionProgress("trigger", "tower_4_done")
    W:addQuestProgress("tower_4", "tower_climbed")
    W:startMap("res/map/island/island.tmx", 1215, 660)
end

