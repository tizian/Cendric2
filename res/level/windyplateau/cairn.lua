-- Behavior for cairn enemy

onDeath = function(B, W)
	W:removeItem("qe_portstone_inactive", 1)
    W:addItem("mi_portstone", 1)
    W:addQuestProgress("teleport_own", "portstone_activated")
    W:changeQuestState("teleport_own", "completed")
end