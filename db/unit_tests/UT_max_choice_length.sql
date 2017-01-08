SELECT text_id, text_type FROM text 
	WHERE text_id LIKE 'DL_Choice%' 
		AND (LENGTH(english) > 72 
		OR LENGTH(german) > 72 
		OR LENGTH(swiss_german) > 72
		OR LENGTH(spanish) > 72);