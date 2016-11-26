-- Test Key 1

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('ke_test_1', 'key', 50, 450, -1);
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('ke_test_1', 'item', 'Test Key 1','Test Schlüssel 1','Test Schlüssel 1');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('ke_test_1', 'item_desc', 'A small rusty key.','Ein kleiner rostiger Schlüssel.','En chline rostige Schlüssel.');

-- Test Key 2

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('ke_test_2', 'key', 100, 450, -1);
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('ke_test_2', 'item', 'Test Key 2','Test Schlüssel 2','Test Schlüssel 2');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('ke_test_2', 'item_desc', 'A heavy iron key.','Ein schwerer Eisenschlüssel.','En schwere Iiseschlüssel.');

-- Test Key 3

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('ke_test_3', 'key', 150, 450, -1);
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('ke_test_3', 'item', 'Test Key 3','Test Schlüssel 3','Test Schlüssel 3');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('ke_test_3', 'item_desc', 'A large brass key.','Ein grosser Messingschlüssel.','En grosse Messingschlüssel.');

-- Small Silver Key

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('ke_smallsilver', 'key', 400, 500, -1);
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('ke_smallsilver', 'item', 'Small Silver Key','Kleiner Silberschlüssel','Chliine Silberschlüssel');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('ke_smallsilver', 'item_desc', 'I found this key in the Elder''s chest. It may belong to a casket.','Diesen Schlüssel habe ich in der Truhe des Dorfältesten gefunden. Er könnte zu einer Schatulle gehören.','De Schlüssel hani ide True vom Älteste gfunde. Er chönt zunere Schatulle ghöre.');

-- Rhendal''s Chest Key

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('ke_rhendal', 'key', 450, 500, -1);
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('ke_rhendal', 'item', 'Rhendal''s Chest Key','Rhendals Truhenschlüssel','Em Rhendal sin Trueschlüssel');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('ke_rhendal', 'item_desc', 'Vincent gave me this key. It belongs to the chest of the Elder.','Vincent hat mir diesen Schlüssel gegeben. Er passt in das Schloss der Truhe des Dorfältesten.','De Vincent het mer de Schlüssel gä. Er passt is Schloss vode True vom Dorfälteste.');

-- Iron Key (Key to the basement in the basilisk inn)

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('ke_tavernbasement', 'key', 250, 550, -1);
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('ke_tavernbasement', 'item', 'Iron Key','Eisenschlüssel','Iseschlüssel');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('ke_tavernbasement', 'item_desc', 'The key to the basement of the Basilisk Inn.','Der Schlüssel zum Keller des Gasthauses zum Basilisken.','De Schlüssel zum Chäller vom Gasthuus zum Basilisk.');

-- Basement Key (Key to the basement of Jonathan's house)

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('ke_jonathanbasement', 'key', 300, 550, -1);
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('ke_jonathanbasement', 'item', 'Basement Key','Kellerschlüssel','Chällerschlüssel');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('ke_jonathanbasement', 'item_desc', 'The key to the basement of Jonathans house.','Der Schlüssel zum Keller von Jonathans Haus.','De Schlüssel zum Chäller vom Huus vom Jonathan.');

-- Basement Key (Key to the basement of Syrah's house)

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('ke_syrahbasement', 'key', 600, 150, -1);
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('ke_syrahbasement', 'item', 'Basement Key','Kellerschlüssel','Chällerschlüssel');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('ke_syrahbasement', 'item_desc', 'The key to the basement of Syrah''s house.','Der Schlüssel zum Keller von Syrahs Haus.','De Schlüssel zum Chäller vom Huus vode Syrah.');
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('ke_syrahbasement', -6, -38, 32, 12);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('ke_syrahbasement', 1, 550, 200, 50, 50);

-- Rusty Key (Key to the thieves' guild)

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('ke_thiefguild', 'key', 550, 550, -1);
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('ke_thiefguild', 'item', 'Rusty Key','Rostiger Schlüssel','Rostige Schlüssel');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('ke_thiefguild', 'item_desc', 'This key is partly corroded.','Dieser Schlüssel wurde teilweise von Rost zerfressen.','De Schlüssel isch es bitz vom Rost zerfresse worde.');

-- Sewer Key 

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('ke_sewerkey', 'key', 600, 150, -1);
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('ke_sewerkey', 'item', 'Sewer Key','Schlüssel zur Kanalisation','Kanalisations-Schlüssel');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('ke_sewerkey', 'item_desc', 'Opens doors in the sewers of Gandria.','Öffnet Türen in der Kanalisation von Gandria.','Macht Türe ide Kanalisation vo Gandria uf.');
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('ke_sewerkey', -6, -38, 32, 12);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('ke_sewerkey', 1, 500, 200, 50, 50);
