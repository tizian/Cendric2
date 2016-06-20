-- Golden Goblet

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('mi_goldengoblet', 'misc', 0, 50, 20);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('mi_goldengoblet', -10, -10, 30, 40);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('mi_goldengoblet', 1, 50, 0, 50, 50);

-- Silver Plate

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('mi_silverplate', 'misc', 250, 200, 15);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('mi_silverplate', 0, -30, 50, 20);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('mi_silverplate', 1, 50, 150, 50, 50);

-- Candleholder

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('mi_candleholder', 'misc', 300, 200, 18);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('mi_candleholder', 0, -40, 50, 65);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('mi_candleholder', 1, 300, 0, 50, 100);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('mi_candleholder', 2, 350, 0, 50, 100);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('mi_candleholder', 3, 400, 0, 50, 100);
INSERT INTO levelitem_light(item_id, light_offset_x, light_offset_y, light_radius_x, light_radius_y) values ('mi_candleholder', 25, 40, 350, 300);

-- Teeth of a Nekomata

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('mi_neko_teeth', 'misc', 250, 0, 10);

-- Fur of a Nekomata

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('mi_neko_fur', 'misc', 300, 0, 30);

-- Fur of a Fire Rat

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('mi_firerat_fur', 'misc', 200, 350, -1);

-- Fur of a Wolf

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('mi_wolf_fur', 'misc', 400, 300, 20);

-- Pile of gargoyle dust

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('mi_gargoyle_dust', 'misc', 350, 0, 20);

-- Teeth

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('mi_teeth', 'misc', 150, 0, 5);

-- Feather

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('mi_feather', 'misc', 450, 100, 2);

-- Pearl

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('mi_pearl', 'misc', 400, 250, 15);

-- Broken Shaft (part 1 of the broken staff)

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('mi_brokenstaff1', 'misc', 450, 50, -1);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('mi_brokenstaff1', 0, -20, 50, 40);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('mi_brokenstaff1', 1, 300, 150, 50, 100);

-- Heart of the First Guardian

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('mi_firstguardianheart', 'misc', 450, 350, -1);

-- Stone

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('mi_stone', 'misc', 250, 350, 4);

-- Corrupt stone

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('mi_corrupt_stone', 'misc', 300, 350, 28);