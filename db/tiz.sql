SELECT text_id, max(length(english), length(german), length(swiss_german)) FROM text 
	WHERE text_id LIKE 'ATTR%'
	AND text_type = 'core'
	ORDER BY max(length(english), length(german), length(swiss_german)) DESC;
