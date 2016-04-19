-- Rope

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('qe_rope', 'quest', 50, 0, -1);

-- Silkweed

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('qe_silkweed', 'quest', 150, 250, -1);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('qe_silkweed', -5, -25, 40, 70);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('qe_silkweed', 1, 100, 150, 50, 100);

-- Spoiled Feudal Fire

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('qe_spoiledfeudalfire', 'quest', 350, 250, -1);

