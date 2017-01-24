-- Routine for NPC "Adelya"

loadRoutine = function(R, W)

	R:setTilePosition(12,3.6)
	
	R:setFacingDown()
	R:wait(2000)
	R:goToTile(10,3.6)
	R:setFacingDown()
	R:wait(2000)
	
	-- dance, biatch, dance :)
	for i=10,1,-1 
	do 
		R:setFacingLeft()
		R:wait(100)
		R:setFacingUp()
		R:wait(100)
		R:setFacingRight()
		R:wait(100)
		R:setFacingDown()
		R:wait(100)
	end
	
	R:goToTile(14,3.6)
	R:setFacingDown()
	R:wait(2000)
	
	-- dance, biatch, dance :)
	for i=10,1,-1 
	do 
		R:setFacingRight()
		R:wait(100)
		R:setFacingUp()
		R:wait(100)
		R:setFacingLeft()
		R:wait(100)
		R:setFacingDown()
		R:wait(100)
	end
	
	R:goToTile(12,3.6)
	
end	