-- Behavior for observer (3) in the level storeroom

routine = function(O) 
	O:gotoTile(17.5, 22.5)
	O:wait(3)
	O:gotoTile(16, 22.5)
	O:gotoTile(16, 27.5)
	O:gotoTile(22.5, 27.5)
	O:gotoTile(22.5, 23.5)
	O:gotoTile(25.5, 23.5)
	O:gotoTile(22.5, 23.5)
	O:gotoTile(22.5, 27.5)
	O:gotoTile(16, 27.5)
	O:gotoTile(16, 22.5)
end
