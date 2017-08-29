-- Behavior for observer (2) in the level yasha_thief

routine = function(O) 
	O:gotoTile(2.5, 9.5)
	O:gotoTile(12, 9.5)
    O:gotoTile(12, 12)
    O:wait(5)
    O:gotoTile(12,9.5)
end
