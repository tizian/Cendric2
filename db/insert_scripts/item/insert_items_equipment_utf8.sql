-- Blue Wizard Hat

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('eq_wizardhatice', 'eq_head', 500, 100, 22);
INSERT INTO item_attribute (item_id, dmg_ice, res_physical, res_ice) values ('eq_wizardhatice', 5, 1, 2);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_wizardhatice', 'res/assets/equipment/head/spritesheet_head_wizardhat_ice.png', 'res/assets/equipment/head/spritesheet_map_head_wizardhat_ice.png');

-- Red Wizard Hat

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('eq_wizardhatfire', 'eq_head', 500, 150, 20);
INSERT INTO item_attribute (item_id, dmg_fire, res_physical, res_fire) values ('eq_wizardhatfire', 5, 1, 2);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_wizardhatfire', 'res/assets/equipment/head/spritesheet_head_wizardhat_fire.png', 'res/assets/equipment/head/spritesheet_map_head_wizardhat_fire.png');

-- White Wizard Hat

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('eq_wizardhatlight', 'eq_head', 500, 200, 20);
INSERT INTO item_attribute (item_id, dmg_light, res_physical, res_light) values ('eq_wizardhatlight', 5, 1, 2);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_wizardhatlight', 'res/assets/equipment/head/spritesheet_head_wizardhat_light.png', 'res/assets/equipment/head/spritesheet_map_head_wizardhat_light.png');

-- Grey Wizard Hat

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('eq_wizardhatshadow', 'eq_head', 500, 250, 20);
INSERT INTO item_attribute (item_id, max_health, res_shadow, res_shadow) values ('eq_wizardhatshadow', 5, 1, 2);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_wizardhatshadow', 'res/assets/equipment/head/spritesheet_head_wizardhat_shadow.png', 'res/assets/equipment/head/spritesheet_map_head_wizardhat_shadow.png');

-- Oversized Hat

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('eq_oversizedhat', 'eq_head', 450, 300, 40);
INSERT INTO item_attribute (item_id, max_health, res_light, res_fire, dmg_shadow) values ('eq_oversizedhat', 5, 10, 3, 5);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_oversizedhat', 'res/assets/equipment/head/spritesheet_head_oversizedhat.png', 'res/assets/equipment/head/spritesheet_map_head_oversizedhat.png');

-- Travel Hat

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('eq_travelhat', 'eq_head', 0, 150, 20);
INSERT INTO item_attribute (item_id, dmg_ice, res_physical, max_health) values ('eq_travelhat', 2, 2, 5);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_travelhat', 'res/assets/equipment/head/spritesheet_head_travelhat.png', 'res/assets/equipment/head/spritesheet_map_head_travelhat.png');

-- Bandana

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('eq_bandana', 'eq_head', 0, 300, 20);
INSERT INTO item_attribute (item_id, max_health, res_physical, res_fire) values ('eq_bandana', 5, 1, 2);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_bandana', 'res/assets/equipment/head/spritesheet_head_bandana.png', 'res/assets/equipment/head/spritesheet_map_head_bandana.png');

-- Scarf of the Fire Rat

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('eq_fireratscarf', 'eq_back', 150, 350, 12);
INSERT INTO item_attribute (item_id, res_fire, dmg_fire) values ('eq_fireratscarf', 10, 2);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_fireratscarf', 'res/assets/equipment/back/spritesheet_back_fireratscarf.png', 'res/assets/equipment/back/spritesheet_map_back_fireratscarf.png');

-- Demon Back

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('eq_demonback', 'eq_back', 0, 350, 30);
INSERT INTO item_attribute (item_id, res_physical, dmg_shadow) values ('eq_demonback', 7, 5);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path, frames_jump) values ('eq_demonback', 'res/assets/equipment/back/spritesheet_back_demon.png', 'res/assets/equipment/back/spritesheet_map_back_demon.png', 3);

-- Demon Head

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('eq_demonhead', 'eq_head', 50, 350, 50);
INSERT INTO item_attribute (item_id, res_physical, dmg_shadow, dmg_physical) values ('eq_demonhead', 8, 7, 7);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_demonhead', 'res/assets/equipment/head/spritesheet_head_demon.png', 'res/assets/equipment/head/spritesheet_map_head_demon.png');
INSERT INTO item_equipment_light(item_id, light_offset_x, light_offset_y, light_radius_x, light_radius_y, brightness) values ('eq_demonhead',65, 25, 25, 25, 0.3);

-- Demon Body

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('eq_demonbody', 'eq_body', 100, 350, 100);
INSERT INTO item_attribute (item_id, res_physical, dmg_shadow, dmg_physical) values ('eq_demonbody', 13, 9, 8);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_demonbody', 'res/assets/equipment/body/spritesheet_body_demon.png', 'res/assets/equipment/body/spritesheet_map_body_demon.png');

-- Nekomata Back

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('eq_nekomataback', 'eq_back', 0, 400, 40);
INSERT INTO item_attribute (item_id, haste, critical, res_physical, res_shadow) values ('eq_nekomataback', 13, 12, 7, 6);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path, frames_jump) values ('eq_nekomataback', 'res/assets/equipment/back/spritesheet_back_nekomata.png', 'res/assets/equipment/back/spritesheet_map_back_fireratscarf.png', 3);

-- Nekomata Head

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('eq_nekomatahead', 'eq_head', 50, 400, 50);
INSERT INTO item_attribute (item_id, critical, dmg_physical, res_physical, res_shadow) values ('eq_nekomatahead', 15, 10, 10, 8);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_nekomatahead', 'res/assets/equipment/head/spritesheet_head_nekomata.png', 'res/assets/equipment/head/spritesheet_map_head_nekomata.png');

-- Necro T3 Head

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('eq_head_necrot3', 'eq_head', 0, 200, 120);
INSERT INTO item_attribute (item_id, max_health, res_physical, dmg_shadow) values ('eq_head_necrot3', 10, 5, 10);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_head_necrot3', 'res/assets/equipment/head/spritesheet_head_necrot3.png', 'res/assets/equipment/head/spritesheet_map_head_necrot3.png');

-- Necro T3 Back

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('eq_back_necrot3', 'eq_back', 50, 200, 150);
INSERT INTO item_attribute (item_id, max_health, haste, critical, res_physical, res_shadow, dmg_shadow) values ('eq_back_necrot3', 20, 20, 10, 5, 10, 10);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path, frames_jump) values ('eq_back_necrot3', 'res/assets/equipment/back/spritesheet_back_necrot3.png', 'res/assets/equipment/back/spritesheet_map_back_necrot3.png', 3);

-- Necro T3 Body

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('eq_body_necrot3', 'eq_body', 100, 200, 200);
INSERT INTO item_attribute (item_id, max_health, haste, critical, res_physical, res_shadow, dmg_shadow) values ('eq_body_necrot3', 30, 20, 10, 30, 20, 20);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_body_necrot3', 'res/assets/equipment/body/spritesheet_body_necrot3.png', 'res/assets/equipment/body/spritesheet_map_body_necrot3.png');

-- Divine T1 Back

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('eq_back_divinet1', 'eq_back', 0, 450, 50);
INSERT INTO item_attribute (item_id, max_health, haste, critical, dmg_fire, dmg_light, res_physical, res_light) values ('eq_back_divinet1', 4, 3, 2, 3, 4, 2, 3);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path, frames_jump) values ('eq_back_divinet1', 'res/assets/equipment/back/spritesheet_back_divinet1.png', 'res/assets/equipment/back/spritesheet_map_back_divinet1.png', 3);

-- Divine T2 Back

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('eq_back_divinet2', 'eq_back', 0, 500, 100);
INSERT INTO item_attribute (item_id, max_health, haste, critical, dmg_fire, dmg_light, res_physical, res_light) values ('eq_back_divinet2', 8, 6, 5, 6, 8, 3, 6);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path, frames_jump) values ('eq_back_divinet2', 'res/assets/equipment/back/spritesheet_back_divinet2.png', 'res/assets/equipment/back/spritesheet_map_back_divinet2.png', 3);

-- Divine T3 Back

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('eq_back_divinet3', 'eq_back', 0, 550, 200);
INSERT INTO item_attribute (item_id, max_health, haste, critical, dmg_fire, dmg_light, res_physical, res_light) values ('eq_back_divinet3', 15, 12, 10, 12, 16, 6, 12);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path, frames_jump) values ('eq_back_divinet3', 'res/assets/equipment/back/spritesheet_back_divinet3.png', 'res/assets/equipment/back/spritesheet_map_back_divinet3.png', 3);

-- Divine T1 Head

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('eq_head_divinet1', 'eq_head', 50, 500, 100);
INSERT INTO item_attribute (item_id, max_health, haste, critical, dmg_fire, dmg_light, res_physical, res_light) values ('eq_head_divinet1', 8, 6, 5, 6, 8, 3, 6);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_head_divinet1', 'res/assets/equipment/head/spritesheet_head_divinet1.png', 'res/assets/equipment/head/spritesheet_map_head_divinet1.png');

-- Divine T2 Head

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('eq_head_divinet2', 'eq_head', 100, 500, 200);
INSERT INTO item_attribute (item_id, max_health, haste, critical, dmg_fire, dmg_light, res_physical, res_light) values ('eq_head_divinet2', 15, 12, 10, 12, 16, 6, 12);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_head_divinet2', 'res/assets/equipment/head/spritesheet_head_divinet2.png', 'res/assets/equipment/head/spritesheet_map_head_divinet2.png');

-- Divine T3 Head

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('eq_head_divinet3', 'eq_head', 150, 500, 400);
INSERT INTO item_attribute (item_id, max_health, haste, critical, dmg_fire, dmg_light, res_physical, res_light) values ('eq_head_divinet3', 30, 24, 20, 24, 32, 12, 24);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_head_divinet3', 'res/assets/equipment/head/spritesheet_head_divinet3.png', 'res/assets/equipment/head/spritesheet_map_head_divinet3.png');

-- Crown

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('eq_crown', 'eq_head', 150, 200, 300);
INSERT INTO item_attribute (item_id, max_health, haste, critical, res_physical) values ('eq_crown', 30, 10, 10, 5);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_crown', 'res/assets/equipment/head/spritesheet_head_crown.png', 'res/assets/equipment/head/spritesheet_map_head_crown.png');

-- Leather Armour

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('eq_leatherarmor', 'eq_body', 50, 300, 100);
INSERT INTO item_attribute (item_id, max_health, res_physical, res_fire, res_ice) values ('eq_leatherarmor', 10, 5, 3, 3);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_leatherarmor', 'res/assets/equipment/body/spritesheet_body_leatherarmor.png', 'res/assets/equipment/body/spritesheet_map_body_leatherarmor.png');

-- Default Armour

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('eq_defaultarmor', 'eq_body', 150, 300, 10);
INSERT INTO item_attribute (item_id, max_health, res_physical) values ('eq_defaultarmor', 1, 1);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_defaultarmor', 'res/assets/equipment/body/spritesheet_body_defaultarmor.png', 'res/assets/equipment/body/spritesheet_map_body_defaultarmor.png');

-- Rafi's Hat

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('eq_rafishat', 'eq_head', 0, 250, 10);
INSERT INTO item_attribute (item_id, haste, critical, res_fire, res_light, res_physical) values ('eq_rafishat', -20, 10, 2, 3, 1);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_rafishat', 'res/assets/equipment/head/spritesheet_head_rafishat.png', 'res/assets/equipment/head/spritesheet_map_head_rafishat.png');

-- Hunter's Hat

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('eq_huntershat', 'eq_head', 50, 250, 20);
INSERT INTO item_attribute (item_id, haste, critical, res_light, res_physical, dmg_physical) values ('eq_huntershat', 2, 2, 10, 3, 5);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_huntershat', 'res/assets/equipment/head/spritesheet_head_huntershat.png', 'res/assets/equipment/head/spritesheet_map_head_huntershat.png');

-- Twilight T1 Head

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('eq_head_twilightt1', 'eq_head', 100, 250, 30);
INSERT INTO item_attribute (item_id, haste, critical, res_fire) values ('eq_head_twilightt1', 3, 2, 5);
INSERT INTO item_equipment (item_id, texture_path) values ('eq_head_twilightt1', 'res/assets/equipment/head/spritesheet_head_twilightt1.png');

-- Ruby Ring

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('eq_rubyring', 'eq_ring', 100, 50, 30);
INSERT INTO item_attribute (item_id, dmg_fire, critical) values ('eq_rubyring', 5, 2);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('eq_rubyring', -12, -37, 8, 8);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('eq_rubyring', 1, 200, 0, 50, 50);

-- Aquamarine Ring

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('eq_aquamarinering', 'eq_ring', 100, 300, 30);
INSERT INTO item_attribute (item_id, dmg_ice, critical) values ('eq_aquamarinering', 5, 1);

-- Iron Ring

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('eq_ironring', 'eq_ring', 350, 100, 15);
INSERT INTO item_attribute (item_id, max_health, dmg_physical) values ('eq_ironring', 6, 2);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('eq_ironring', -12, -37, 8, 8);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('eq_ironring', 1, 300, 100, 50, 50);

-- Eye of the moon

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('eq_eyeofthemoon', 'eq_neck', 350, 50, 20);
INSERT INTO item_attribute (item_id, dmg_shadow) values ('eq_eyeofthemoon', 10);

-- Silver Necklace

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('eq_silvernecklace', 'eq_neck', 350, 200, 10);
INSERT INTO item_attribute (item_id, res_fire, critical) values ('eq_silvernecklace', 5, 1);

-- Light-Touched Pendant

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('eq_lightpendant', 'eq_neck', 450, 0, 50);
INSERT INTO item_attribute (item_id, dmg_light, res_shadow) values ('eq_lightpendant', 5, 5);
INSERT INTO item_equipment (item_id, texture_path) values ('eq_lightpendant', '');
INSERT INTO item_equipment_light(item_id, light_offset_x, light_offset_y, light_radius_x, light_radius_y, brightness) values ('eq_lightpendant',60, 30, 300, 300, 0.8);
