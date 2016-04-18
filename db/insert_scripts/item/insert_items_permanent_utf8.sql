-- Ingrid's Stew

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('pe_ingridstew', 'permanent', 350, 150, 20);
INSERT INTO item_attribute (item_id, max_health) values ('pe_ingridstew', 2);

-- Feudal Fire

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('pe_feudalfire', 'permanent', 350, 250, -1);
INSERT INTO item_attribute (item_id, res_fire) values ('pe_feudalfire', 3);

-- Glass of Feudal Fire

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('pe_glassoffeudalfire', 'permanent', 350, 300, -1);
INSERT INTO item_attribute (item_id, res_fire) values ('pe_glassoffeudalfire', 2);

