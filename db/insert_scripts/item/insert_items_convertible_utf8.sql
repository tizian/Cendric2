-- Shell

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('co_shell', 'item', 'Shell','Muschel','Muschle');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('co_shell', 'item_desc', 'Can be opened and contains a pearl, if you''re lucky.','Mit etwas Glück findet man eine Perle darin.','Mit echli Glück hets da e Perle dinne.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('co_shell', 'convertible', 400, 200, 2);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('co_shell', 0, -30, 30, 20);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('co_shell', 1, 350, 100, 50, 50);
INSERT INTO item_convertible (item_id, convertible_item_id, probability) values ('co_shell', 'mi_pearl', 30);

-- Small Pouch

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('co_smallpouch', 'item', 'Small Pouch','Schmaler Geldbeutel','Schmale Geldbüütel');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('co_smallpouch', 'item_desc', 'A pouch with a few jingling coins.','Ein paar Münzen klimpern darin.','Es paar Münze klimperet drin.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('co_smallpouch', 'convertible', 450, 150, 10);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('co_smallpouch', -15, -25, 30, 2);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('co_smallpouch', 1, 400, 100, 50, 50);
INSERT INTO item_convertible (item_id, convertible_item_id, convertible_amount) values ('co_smallpouch', 'gold', 10);

-- Medium Pouch

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('co_mediumpouch', 'item', 'Leather Pouch','Lederbeutel','Lederbüütel');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('co_mediumpouch', 'item_desc', 'The pouch is filled with coins.','Der Beutel ist voller Münzen.','De Büütel isch gfüllt mit Münze.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('co_mediumpouch', 'convertible', 450, 200, 20);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('co_mediumpouch', -15, -25, 30, 4);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('co_mediumpouch', 1, 400, 150, 50, 50);
INSERT INTO item_convertible (item_id, convertible_item_id, convertible_amount) values ('co_mediumpouch', 'gold', 20);

-- Big Pouch

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('co_bigpouch', 'item', 'Big Pouch','Praller Geldbeutel','Pralle Geldbüütel');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('co_bigpouch', 'item_desc', 'The pouch is bulging with golden coins.','Der Beutel ist prall gefüllt mit goldenen Münzen.','De Büütel isch prall gfüllt mit goldige Münze.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('co_bigpouch', 'convertible', 450, 250, 40);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('co_bigpouch', -10, -25, 40, 10);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('co_bigpouch', 1, 400, 200, 50, 50);
INSERT INTO item_convertible (item_id, convertible_item_id, convertible_amount) values ('co_bigpouch', 'gold', 50);

-- Old Travel Bag

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('co_travelbag', 'item', 'Old Travel Bag','Alter Reiserucksack','Alte Reiserucksack');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('co_travelbag', 'item_desc', 'I wonder what''s inside?','Was ist da wohl drin?','Wa isch do echt drin?');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('co_travelbag', 'convertible', 500, 350, -1, 1);
INSERT INTO item_convertible (item_id, convertible_item_id, convertible_amount) values ('co_travelbag', 'gold', 5);
INSERT INTO item_convertible (item_id, convertible_item_id) values ('co_travelbag', 'do_elemap');
INSERT INTO item_convertible (item_id, convertible_item_id) values ('co_travelbag', 'do_eleletter');
INSERT INTO item_convertible (item_id, convertible_item_id) values ('co_travelbag', 'eq_travelhat');