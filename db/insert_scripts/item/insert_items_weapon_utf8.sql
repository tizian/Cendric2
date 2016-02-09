-- Ice Staff

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('we_icestaff', 'eq_weapon', 200, 150, 120);
INSERT INTO item_attribute (item_id, max_health, dmg_physical, dmg_ice) values ('we_icestaff', 10, 15, 30);
INSERT INTO item_equipment (item_id, texture_path) values ('we_icestaff', 'res/assets/equipment/weapon/spritesheet_weapon_icestaff.png');
INSERT INTO item_weapon (item_id, chop_cooldown_ms, chop_offset_left, chop_rect_width, chop_rect_height, chop_damage) values ('we_icestaff', 600, 10, 45, 80, 10);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_icestaff', 1, 'elemental', 3);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_icestaff', 2, 'elemental', 3);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_icestaff', 3, 'divine', 3);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_icestaff', 4, 'divine', 3);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_icestaff', 5, 'necromancy', 3);

-- Sickle Staff

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('we_sicklestaff', 'eq_weapon', 250, 150, 120);
INSERT INTO item_attribute (item_id, max_health, dmg_physical, dmg_fire, dmg_ice) values ('we_sicklestaff', 10, 15, 15, 15);
INSERT INTO item_equipment (item_id, texture_path) values ('we_sicklestaff', 'res/assets/equipment/weapon/spritesheet_weapon_sicklestaff.png');
INSERT INTO item_weapon (item_id, chop_cooldown_ms, chop_offset_left, chop_rect_width, chop_rect_height, chop_damage) values ('we_sicklestaff', 600, 10, 45, 80, 10);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_sicklestaff', 1, 'elemental', 3);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_sicklestaff', 2, 'elemental', 3);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_sicklestaff', 3, 'elemental', 3);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_sicklestaff', 4, 'elemental', 3);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_sicklestaff', 5, 'elemental', 3);

-- Rusty Sword

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('we_rustysword', 'eq_weapon', 150, 150, 10);
INSERT INTO item_attribute (item_id, dmg_physical) values ('we_rustysword', 5);
INSERT INTO item_equipment (item_id, texture_path) values ('we_rustysword', 'res/assets/equipment/weapon/spritesheet_weapon_rustysword.png');
INSERT INTO item_weapon (item_id, chop_cooldown_ms, chop_offset_left, chop_rect_width, chop_rect_height, chop_damage) values ('we_rustysword', 500, 10, 40, 80, 5);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_rustysword', 1, 'elemental', 1);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_rustysword', 2, 'necromancy', 3);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_rustysword', 3, 'twilight', 1);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_rustysword', 4, 'meta', 3);

-- Oathbreaker

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('we_oathbreaker', 'eq_weapon', 150, 100, 120);
INSERT INTO item_attribute (item_id, dmg_physical, dmg_ice, critical, haste) values ('we_oathbreaker', 10, 10, 15, 10);
INSERT INTO item_equipment (item_id, texture_path) values ('we_oathbreaker', 'res/assets/equipment/weapon/spritesheet_weapon_oathbreaker.png');
INSERT INTO item_weapon (item_id, chop_cooldown_ms, chop_rect_width, chop_rect_height, chop_offset_left, chop_damage) values ('we_oathbreaker', 350, 80, 80, -40, 20);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_oathbreaker', 1, 'elemental', 3);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_oathbreaker', 2, 'divine', 3);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_oathbreaker', 3, 'necromancy', 3);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_oathbreaker', 4, 'twilight', 3);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_oathbreaker', 5, 'divine', 3);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('we_oathbreaker', -7, -24, 50, 24);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('we_oathbreaker', 1, 200, 100, 50, 50);

-- Torch

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('we_torch', 'eq_weapon', 200, 100, 10);
INSERT INTO item_attribute (item_id, dmg_fire) values ('we_torch', 10);
INSERT INTO item_equipment (item_id, texture_path) values ('we_torch', 'res/assets/equipment/weapon/spritesheet_weapon_torch.png');
INSERT INTO item_weapon (item_id, chop_cooldown_ms, chop_offset_left, chop_rect_width, chop_rect_height, chop_damage) values ('we_torch', 400, 10, 40, 80, 10);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_torch', 1, 'elemental', 1);
INSERT INTO item_equipment_light(item_id, light_offset_x, light_offset_y, light_radius_x, light_radius_y) values ('we_torch', 30, 30, 300, 400);
