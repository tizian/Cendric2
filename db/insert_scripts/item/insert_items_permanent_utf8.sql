-- Ingrid's Stew

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('pe_ingridstew', 'permanent', 350, 150, 20);
INSERT INTO item_attribute (item_id, max_health) values ('pe_ingridstew', 2);

-- Feudal Fire

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('pe_feudalfire', 'permanent', 350, 250, -1);
INSERT INTO item_attribute (item_id, res_fire) values ('pe_feudalfire', 3);

-- Glass of Feudal Fire

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('pe_glassoffeudalfire', 'permanent', 350, 300, -1);
INSERT INTO item_attribute (item_id, res_fire) values ('pe_glassoffeudalfire', 2);

-- Elixir of Fire

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('pe_elixirfire', 'permanent', 400, 450, 100);
INSERT INTO item_attribute (item_id, dmg_fire) values ('pe_elixirfire', 2);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('pe_elixirfire', -10, -5, 30, 45);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('pe_elixirfire', 1, 200, 250, 50, 50);

-- Elixir of Ice

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('pe_elixirice', 'permanent', 450, 450, 100);
INSERT INTO item_attribute (item_id, dmg_ice) values ('pe_elixirice', 2);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('pe_elixirice', -10, -5, 30, 45);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('pe_elixirice', 1, 250, 250, 50, 50);

-- Elixir of Light

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('pe_elixirlight', 'permanent', 500, 450, 100);
INSERT INTO item_attribute (item_id, dmg_light) values ('pe_elixirlight', 2);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('pe_elixirlight', -10, -5, 30, 45);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('pe_elixirlight', 1, 300, 250, 50, 50);

-- Elixir of Shadows

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('pe_elixirshadow', 'permanent', 550, 450, 100);
INSERT INTO item_attribute (item_id, dmg_shadow) values ('pe_elixirshadow', 2);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('pe_elixirshadow', -10, -5, 30, 45);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('pe_elixirshadow', 1, 350, 250, 50, 50);


