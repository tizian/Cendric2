-- Shell

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('co_shell', 'convertible', 400, 200, 2);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('co_shell', 0, -30, 30, 20);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('co_shell', 1, 350, 100, 50, 50);
INSERT INTO item_convertible (item_id, convertible_item_id, probability) values ('co_shell', 'mi_pearl', 30);

-- Small Pouch

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('co_smallpouch', 'convertible', 450, 150, 5);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('co_smallpouch', -15, -25, 30, 25);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('co_smallpouch', 1, 400, 100, 50, 50);
INSERT INTO item_convertible (item_id, convertible_item_id, convertible_amount) values ('co_smallpouch', 'gold', 25);

-- Medium Pouch

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('co_mediumpouch', 'convertible', 450, 200, 10);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('co_mediumpouch', -15, -25, 30, 25);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('co_mediumpouch', 1, 400, 150, 50, 50);
INSERT INTO item_convertible (item_id, convertible_item_id, convertible_amount) values ('co_mediumpouch', 'gold', 50);

-- Big Pouch

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('co_bigpouch', 'convertible', 450, 250, 20);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('co_bigpouch', -10, -25, 40, 25);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('co_bigpouch', 1, 400, 200, 50, 50);
INSERT INTO item_convertible (item_id, convertible_item_id, convertible_amount) values ('co_bigpouch', 'gold', 100);

-- Old Travel Bag

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('co_travelbag', 'convertible', 500, 350, -1);
INSERT INTO item_convertible (item_id, convertible_item_id, convertible_amount) values ('co_travelbag', 'gold', 5);
INSERT INTO item_convertible (item_id, convertible_item_id) values ('co_travelbag', 'qe_compass');
INSERT INTO item_convertible (item_id, convertible_item_id) values ('co_travelbag', 'do_elemap');
INSERT INTO item_convertible (item_id, convertible_item_id) values ('co_travelbag', 'do_eleletter');
INSERT INTO item_convertible (item_id, convertible_item_id) values ('co_travelbag', 'eq_travelhat');