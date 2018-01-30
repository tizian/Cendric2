SELECT I.item_id FROM item AS I WHERE NOT EXISTS 
    (SELECT T.english FROM text AS T WHERE T.text_id = I.item_id AND T.text_type = "item");
	
SELECT I.item_id FROM item AS I WHERE NOT EXISTS 
    (SELECT T.english FROM text AS T WHERE T.text_id = I.item_id AND T.text_type = "item_desc");
    
SELECT text_id FROM text WHERE 
    english = '...' OR
    german = '...' OR
    swiss_german = '...' OR
    english = '' OR
    german = '' OR
    swiss_german = '' OR
    english = '?' OR
    german = '?' OR
    swiss_german = '?';
   