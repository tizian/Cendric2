-- Ice Staff

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('we_icestaff', 'eq_weapon', 200, 150, 120);
INSERT INTO item_attribute (item_id, max_health, dmg_physical, dmg_ice) values ('we_icestaff', 10, 15, 30);
INSERT INTO item_equipment (item_id, texture_path) values ('we_icestaff', 'res/assets/equipment/weapon/spritesheet_weapon_icestaff.png');
INSERT INTO item_weapon (item_id, chop_cooldown_ms, chop_rect_width, chop_rect_height, chop_damage) values ('we_icestaff', 500, 45, 80, 10);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_icestaff', 1, 'elemental', 3);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_icestaff', 2, 'elemental', 3);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_icestaff', 3, 'divine', 3);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_icestaff', 4, 'divine', 3);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_icestaff', 5, 'necromancy', 3);

-- Rusty Sword

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('we_rustysword', 'eq_weapon', 150, 150, 10);
INSERT INTO item_attribute (item_id, dmg_physical) values ('we_rustysword', 5);
INSERT INTO item_equipment (item_id, texture_path) values ('we_rustysword', 'res/assets/equipment/weapon/spritesheet_weapon_rustysword.png');
INSERT INTO item_weapon (item_id, chop_cooldown_ms, chop_rect_width, chop_rect_height, chop_damage) values ('we_rustysword', 350, 40, 80, 5);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_rustysword', 1, 'elemental', 3);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_rustysword', 2, 'elemental', 3);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_rustysword', 3, 'necromancy', 3);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_rustysword', 4, 'twilight', 3);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_rustysword', 5, 'twilight', 1);
