-- chest in level ppuzzle03                               

onLoot = function(W) 
	W:addConditionProgress("trigger", "tower_3_done")
    W:addQuestProgress("tower_3", "tower_climbed")
    W:startMap("res/map/island/island.tmx", 1215, 660)
end

