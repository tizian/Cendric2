-- chest in level ppuzzle01                               

onLoot = function(W) 
	W:addConditionProgress("trigger", "tower_1_done")
    W:addQuestProgress("tower_1", "tower_climbed")
    W:startMap("res/map/island/island.tmx", 1215, 660)
end

