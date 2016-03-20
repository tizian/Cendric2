-- Letter

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('qe_letter', 'document', 200, 0, -1);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('qe_letter', 0, -30, 50, 20);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('qe_letter', 1, 150, 0, 50, 50);
