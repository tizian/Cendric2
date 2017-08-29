-- Behavior for observer (2) in the level yasha_thief

routine = function(O) 
	O:gotoTile(2.5, 9.5)
	O:gotoTile(11, 9.5)
    O:gotoTile(11, 11.5)
    O:wait(3)
    O:gotoTile(11,9.5)
end
