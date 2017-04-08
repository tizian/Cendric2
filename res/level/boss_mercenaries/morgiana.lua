-- Behavior for enemy morgiana
started = false

update = function(B, W)
		if (not started) then
			started = true
			B:say("MorgianaStart", 4)
		end
    end

