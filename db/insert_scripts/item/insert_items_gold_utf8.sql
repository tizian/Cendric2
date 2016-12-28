-- Gold

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('gold', 'item', 'Gold','Gold','Gold');

-- Gold - Three Coins

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('go_threecoins', 'item', 'A few coins','Ein paar Münzen','Es paar Münze');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('go_threecoins', 'gold', 0, 0, 3);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('go_threecoins', -10, -40, 25, 10);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('go_threecoins', 1, 150, 50, 50, 50);

-- Gold - One Coin

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('go_onecoin', 'item', 'Golden coin','Goldmünze','Goldmünze');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('go_onecoin', 'gold', 0, 0, 1);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('go_onecoin', -18, -42, 8, 8);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('go_onecoin', 1, 100, 0, 50, 50);