-- Behavior for observer (3) in the level storeroom

routine = function(O) 
	O:gotoTile(17, 23)
	O:wait(3)
	O:gotoTile(15.5, 23)
	O:gotoTile(15.5, 28)
	O:gotoTile(22, 28)
	O:gotoTile(22, 24)
	O:gotoTile(25, 24)
	O:gotoTile(22, 24)
	O:gotoTile(22, 28)
	O:gotoTile(15.5, 28)
	O:gotoTile(15.5, 23)
end
