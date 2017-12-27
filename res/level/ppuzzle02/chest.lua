-- chest in level ppuzzle02                               

onLoot = function(W) 
	W:addConditionProgress("trigger", "tower_2_done")
    W:addQuestProgress("tower_2", "tower_climbed")
    W:startMap("res/map/island/island.tmx", 1215, 660)
end

