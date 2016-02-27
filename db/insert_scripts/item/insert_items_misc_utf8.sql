-- Golden Goblet

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('mi_goldengoblet', 'misc', 0, 50, 20);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('mi_goldengoblet', -10, -5, 30, 40);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('mi_goldengoblet', 1, 50, 0, 50, 50);

-- Teeth of a Nekomata

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('mi_neko_teeth', 'misc', 250, 0, 10);

-- Fur of a Nekomata

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('mi_neko_fur', 'misc', 300, 0, 30);

-- Pile of gargoyle dust

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('mi_gargoyle_dust', 'misc', 350, 0, 20);

-- Teeth

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('mi_teeth', 'misc', 150, 0, 5);
