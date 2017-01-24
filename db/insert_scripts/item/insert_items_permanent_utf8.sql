-- Ingrid's Stew

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('pe_ingridstew', 'item', 'Ingrid''s Stew', 'Ingrids Eintopf', 'Ingrids Eintopf');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('pe_ingridstew', 'item_desc', 'A delicious dish cooked by Ingrid. Simply irresistible!', 'Ein köstliches Gericht, gekocht von Ingrid. Einfach unwiderstehlich!', 'Es köstlichs Gricht, kocht vode Ingrid. Eifach unwiderstehlich!');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('pe_ingridstew', 'permanent', 350, 150, 20);
INSERT INTO item_attribute (item_id, max_health) values ('pe_ingridstew', 2);

-- Feudal Fire

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('pe_feudalfire', 'item', 'Feudal Fire', 'Feudales Feuer', 'Feudals Füür');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('pe_feudalfire', 'item_desc', 'A strong spirit, is used for alchemistic purposes.', 'Ziemlich hochprozentig, wird für alchemistische Zwecke verwendet.', 'Zimlich hochprozentig, wird für alchemistischi Zweck bruucht.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('pe_feudalfire', 'permanent', 350, 250, -1);
INSERT INTO item_attribute (item_id, res_fire) values ('pe_feudalfire', 4);

-- Glass of Feudal Fire

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('pe_glassoffeudalfire', 'item', 'Glass of Feudal Fire', 'Ein Glas Feudales Feuer', 'Es Glas Feudals Füür');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('pe_glassoffeudalfire', 'item_desc', 'A strong spirit.', 'Ziemlich hochprozentig.', 'Zimlich hochprozentig.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('pe_glassoffeudalfire', 'permanent', 350, 300, 30);
INSERT INTO item_attribute (item_id, res_fire) values ('pe_glassoffeudalfire', 2);

-- Elixir of Fire

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('pe_elixirfire', 'item', 'Elixir of Fire', 'Elixier des Feuers', 'Füür Elixier');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('pe_elixirfire', 'item_desc', 'Increases the fire damage permanently.', 'Erhöht den Feuerschaden permanent.', 'Erhöht de Füürschade permanent.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('pe_elixirfire', 'permanent', 400, 450, 150);
INSERT INTO item_attribute (item_id, dmg_fire) values ('pe_elixirfire', 2);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('pe_elixirfire', -10, -5, 30, 45);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('pe_elixirfire', 1, 200, 250, 50, 50);

-- Elixir of Ice

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('pe_elixirice', 'item', 'Elixir of Ice', 'Elixier des Eises', 'Iis Elixier');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('pe_elixirice', 'item_desc', 'Increases the ice damage permanently.', 'Erhöht den Eisschaden permanent.', 'Erhöht de Iisschade permanent.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('pe_elixirice', 'permanent', 450, 450, 150);
INSERT INTO item_attribute (item_id, dmg_ice) values ('pe_elixirice', 2);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('pe_elixirice', -10, -5, 30, 45);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('pe_elixirice', 1, 250, 250, 50, 50);

-- Elixir of Light

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('pe_elixirlight', 'item', 'Elixir of Light', 'Elixier des Lichts', 'Liecht Elixier');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('pe_elixirlight', 'item_desc', 'Increases the light damage permanently.', 'Erhöht den Lichtschaden permanent.', 'Erhöht de Liechtschade permanent.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('pe_elixirlight', 'permanent', 500, 450, 150);
INSERT INTO item_attribute (item_id, dmg_light) values ('pe_elixirlight', 2);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('pe_elixirlight', -10, -5, 30, 45);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('pe_elixirlight', 1, 300, 250, 50, 50);

-- Elixir of Shadows

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('pe_elixirshadow', 'item', 'Elixir of Shadows', 'Elixier der Schatten', 'Schatte Elixier');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('pe_elixirshadow', 'item_desc', 'Increases the shadow damage permanently.', 'Erhöht den Schattenschaden permanent.', 'Erhöht de Schatteschade permanent.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('pe_elixirshadow', 'permanent', 550, 450, 150);
INSERT INTO item_attribute (item_id, dmg_shadow) values ('pe_elixirshadow', 2);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('pe_elixirshadow', -10, -5, 30, 45);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('pe_elixirshadow', 1, 350, 250, 50, 50);


