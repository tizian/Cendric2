-- Letter

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('qe_letter', 'document', 200, 0, -1);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('qe_letter', 0, -30, 50, 20);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('qe_letter', 1, 150, 0, 50, 50);

-- Note (Paladin Marcus)

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('do_notemarcus', 'document', 400, 400, -1);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('do_notemarcus', -25, 0, 50, 50);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('do_notemarcus', 1, 450, 0, 100, 50);

-- Bloodstained Note (Paladin Marcus)

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('do_bloodnotemarcus', 'document', 450, 400, -1);

