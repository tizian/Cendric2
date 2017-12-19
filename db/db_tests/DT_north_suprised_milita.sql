PRAGMA case_sensitive_like = ON;

SELECT text_id, text_type FROM text 
	WHERE 
        english LIKE '% north %' OR
        english LIKE '% south %' OR
        english LIKE '% east %' OR
        english LIKE '% west %' OR
        english LIKE '% north.%' OR
        english LIKE '% south.%' OR
        english LIKE '% east.%' OR
        english LIKE '% west.%' OR
        english LIKE '%supri%' OR
        english LIKE '%Supri%' OR
        english LIKE '%Milita%' OR
        english LIKE '%milita%' OR
        english LIKE '%Grimiore%' OR
        english LIKE '%grimiore%';