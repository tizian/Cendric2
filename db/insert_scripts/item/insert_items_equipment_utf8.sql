-- Blue Wizard Hat

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('eq_wizardhatblue', 'eq_head', 0, 150, 22);
INSERT INTO item_attribute (item_id, dmg_ice, res_physical, res_ice) values ('eq_wizardhatblue', 5, 1, 2);
INSERT INTO item_equipment (item_id, texture_path) values ('eq_wizardhatblue', 'res/assets/equipment/head/spritesheet_head_wizardhat_blue.png');

-- Grey Wizard Hat

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('eq_wizardhatgrey', 'eq_head', 50, 150, 20);
INSERT INTO item_attribute (item_id, max_health, res_physical, res_fire) values ('eq_wizardhatgrey', 5, 1, 2);
INSERT INTO item_equipment (item_id, texture_path) values ('eq_wizardhatgrey', 'res/assets/equipment/head/spritesheet_head_wizardhat_grey.png');

-- Purple Back

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('eq_purpleback', 'eq_back', 100, 150, 10);
INSERT INTO item_attribute (item_id, res_physical, res_ice) values ('eq_purpleback', 5, 1);
INSERT INTO item_equipment (item_id, texture_path) values ('eq_purpleback', 'res/assets/equipment/back/spritesheet_back_purple.png');

-- Necro T3 Head

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('eq_head_necrot3', 'eq_head', 0, 200, 120);
INSERT INTO item_attribute (item_id, max_health, res_physical, dmg_shadow) values ('eq_head_necrot3', 10, 5, 10);
INSERT INTO item_equipment (item_id, texture_path) values ('eq_head_necrot3', 'res/assets/equipment/head/spritesheet_head_necrot3.png');

-- Necro T3 Back

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('eq_back_necrot3', 'eq_back', 50, 200, 150);
INSERT INTO item_attribute (item_id, max_health, haste, critical, res_physical, res_shadow, dmg_shadow) values ('eq_back_necrot3', 20, 20, 10, 5, 10, 10);
INSERT INTO item_equipment (item_id, texture_path) values ('eq_back_necrot3', 'res/assets/equipment/back/spritesheet_back_necrot3.png');

-- Necro T3 Body

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('eq_body_necrot3', 'eq_body', 100, 200, 200);
INSERT INTO item_attribute (item_id, max_health, haste, critical, res_physical, res_shadow, dmg_shadow) values ('eq_body_necrot3', 30, 20, 10, 30, 20, 20);
INSERT INTO item_equipment (item_id, texture_path) values ('eq_body_necrot3', 'res/assets/equipment/body/spritesheet_body_necrot3.png');

-- Divine T3 Head

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('eq_head_divinet3', 'eq_head', 150, 200, 120);
INSERT INTO item_attribute (item_id, max_health, haste, critical, dmg_fire, dmg_light, res_physical, res_light) values ('eq_head_divinet3', 10, 10, 10, 10, 10, 2, 5);
INSERT INTO item_equipment (item_id, texture_path) values ('eq_head_divinet3', 'res/assets/equipment/head/spritesheet_head_divinet3.png');

-- Rafis Hat

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('eq_rafishat', 'eq_head', 0, 250, 10);
INSERT INTO item_attribute (item_id, haste, critical, res_fire, res_light, res_physical) values ('eq_rafishat', -20, 10, 2, 3, 1);
INSERT INTO item_equipment (item_id, texture_path) values ('eq_rafishat', 'res/assets/equipment/head/spritesheet_head_rafishat.png');

-- Ring of lesser health

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('eq_ringoflesserhealth', 'eq_ring', 100, 50, 10);
INSERT INTO item_attribute (item_id) values ('eq_ringoflesserhealth');
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('eq_ringoflesserhealth', -14, -40, 5, 5);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('eq_ringoflesserhealth', 1, 200, 0, 50, 50);

-- The one ring

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('eq_theonering', 'eq_ring', 200, 100, 100);
INSERT INTO item_attribute (item_id, haste, critical, health_regeneration, res_fire, res_light, res_physical) values ('eq_theonering', 20, 20, -2, 5, 5, 5);