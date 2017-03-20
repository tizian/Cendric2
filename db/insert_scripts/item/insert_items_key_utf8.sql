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

-- Storeroom Key (Key to the basement in the basilisk inn)

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('ke_tavernbasement', 'key', 250, 550, -1);
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('ke_tavernbasement', 'item', 'Storeroom Key','Schlüssel zur Vorratskammer','Schlüssel vode Vorratschammere');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('ke_tavernbasement', 'item_desc', 'The key to the basement of the Basilisk Inn.','Der Schlüssel zum Keller des Gasthauses zum Basilisken.','De Schlüssel zum Chäller vom Gasthuus zum Basilisk.');
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('ke_tavernbasement', -6, -38, 32, 12);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('ke_tavernbasement', 1, 500, 200, 50, 50);

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
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('ke_thiefguild', 'item_desc', 'This key is partly corroded.','Dieser Schlüssel wurde teilweise von Rost zerfressen.','De Schlüssel isch es bitz vom Rost aknaberet worde.');

-- Sewer Key 

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('ke_sewerkey', 'key', 600, 100, -1);
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('ke_sewerkey', 'item', 'Sewer Key','Schlüssel zur Kanalisation','Kanalisations-Schlüssel');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('ke_sewerkey', 'item_desc', 'Opens certain doors in the sewers of Gandria.','Öffnet gewisse Türen in der Kanalisation von Gandria.','Macht gwüssi Türe ide Kanalisation vo Gandria uf.');
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('ke_sewerkey', -6, -38, 32, 12);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('ke_sewerkey', 1, 500, 200, 50, 50);

-- Lighthouse Key

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('ke_lighthouse', 'key', 600, 550, -1);
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('ke_lighthouse', 'item', 'Lighthouse Key','Leuchtturm-Schlüssel','Lüüchtturm-Schlüssel');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('ke_lighthouse', 'item_desc', 'The key to Jack''s Lighthouse.','Der Schlüssel von Jack''s Leuchtturm.','De Schlüssel vom Jack sim Lüüchtturm.');

-- Ornate Key (Key to the forgotten passage)

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('ke_forgottenpassage', 'key', 650, 100, -1);
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('ke_forgottenpassage', 'item', 'Ornate Key','Verschnörkelter Kanalisation','Verschnörklete Schlüssel');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('ke_forgottenpassage', 'item_desc', 'I found this key in the forgotten passage in the sewers.','Diesen Schlüssel habe ich im vergessenen Durchgang in der Kanalisation gefunden','De Schlüssel hani im vergessene Durchgang ide Kanalisation gfunde.');
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('ke_forgottenpassage', -6, -38, 32, 12);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('ke_forgottenpassage', 1, 500, 200, 50, 50);

-- Organist's Key (Opens shortpaths in the cathedral)

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('ke_cathedral', 'key', 650, 400, -1);
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('ke_cathedral', 'item', 'Organist''s Key','Schlüssel des Organisten','Schlüssel vom Organist');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('ke_cathedral', 'item_desc', 'A small organ pipe. It must have belonged to the organist of the cathedral.','Eine kleine Orgelpfeife. Sie wird wohl dem Organisten der Kathedrale gehört haben.', 'Es chlises Orgelpfiifli. Es het glaub em Organist vode Kathedrale ghört.');
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('ke_cathedral', -2, -38, 48, 12);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('ke_cathedral', 1, 600, 50, 50, 50);
