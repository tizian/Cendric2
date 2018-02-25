SELECT text_id, text_type FROM text 
	WHERE text_type LIKE 'dl_npc_%'
		AND (LENGTH(english) > 200 
		OR LENGTH(german) > 200
		OR LENGTH(swiss_german) > 200
		OR LENGTH(spanish) > 200);