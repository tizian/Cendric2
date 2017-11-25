SELECT text_id, text_type FROM text 
	WHERE text_id LIKE 'DL_Choice%' 
		AND (LENGTH(english) > 80 
		OR LENGTH(german) > 80 
		OR LENGTH(swiss_german) > 80
		OR LENGTH(spanish) > 80);