SELECT text_id, text_type FROM text 
	WHERE english LIKE '%  %' 
		OR german LIKE '%  %'
		OR swiss_german LIKE '%  %'
		OR spanish LIKE '%  %'
        OR english LIKE '%''''%' 
		OR german LIKE '%''''%'
		OR swiss_german LIKE '%''''%'
		OR spanish LIKE '%''''%';