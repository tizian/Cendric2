-- Behavior for boss Velius

started = false

onDeath = function(B, W)
	B:say("VeliusDeath", 5)
end

update = function(B, W)
    if (not started) then
        started = true
        B:say("VeliusStart", 5)
    end
end