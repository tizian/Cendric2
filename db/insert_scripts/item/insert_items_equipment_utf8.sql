-- Steven's Helmet

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_stevenhelmet', 'item', 'Steven''s Helmet','Steven''s Helm','Em Steven sin Helm');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_stevenhelmet', 'item_desc', 'This helmet belongs to the paladin Steven.','Dieser Helm gehört dem Paladin Steven.','De Helm ghört em Paladin Steven.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_stevenhelmet', 'eq_head', 200, 550, 50, 1);
INSERT INTO item_attribute (item_id, dmg_light, max_health, res_physical, res_fire) values ('eq_stevenhelmet', 2, 10, 20, 10);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_stevenhelmet', 'res/texture/equipment/head/spritesheet_head_stevenhelmet.png', 'res/texture/equipment/head/spritesheet_map_head_stevenhelmet.png');
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('eq_stevenhelmet', -10, -29, 31, 27);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('eq_stevenhelmet', 1, 550, 100, 50, 100);

-- Captain's Hat

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_piratehat', 'item', 'Captain''s Hat','Kapitänshut','Huet vom Käpt''n');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_piratehat', 'item_desc', 'Arr!','Arr!','Arr!');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_piratehat', 'eq_head', 750, 700, 70, 2);
INSERT INTO item_attribute (item_id, dmg_fire, max_health, dmg_physical, res_physical, res_light, critical) values ('eq_piratehat', 2, 8, 2, 10, 5, -2);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_piratehat', 'res/texture/equipment/head/spritesheet_head_piratehat.png', 'res/texture/equipment/head/spritesheet_map_head_piratehat.png');
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('eq_piratehat', -22, -33, 37, 18);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('eq_piratehat', 1, 500, 450, 100, 50);

-- Sack

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_sack', 'item', 'Sack','Sack','Sack');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_sack', 'item_desc', 'Syrah''s solution for my blonde hair.','Syrah''s Lösung für mein blondes Haar.','De Syrah ihri Lösig für mini blonde Haar.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('eq_sack', 'eq_head', 550, 700, 5);
INSERT INTO item_attribute (item_id, dmg_physical, res_light, res_physical, res_ice, critical) values ('eq_sack', -5, 100, 5, 10, -5);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_sack', 'res/texture/equipment/head/spritesheet_head_sack.png', 'res/texture/equipment/head/spritesheet_map_head_sack.png');

-- Chicken Head

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_chicken', 'item', 'Chicken Mask','Hühnermaske','Hüehnermaske');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_chicken', 'item_desc', 'The chicken chaser''s penalty for chasing chicken.','Die Strafe des Hühnerschrecks für das Hühner Erschrecken.','D''Strof vom Hüehnli Schreck fürs Hüehnli verschrecke.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('eq_chicken', 'eq_head', 600, 700, 20);
INSERT INTO item_attribute (item_id, dmg_physical, res_light, res_physical, res_ice, critical) values ('eq_chicken', 2, 30, 10, 10, 1);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_chicken', 'res/texture/equipment/head/spritesheet_head_chicken.png', 'res/texture/equipment/head/spritesheet_map_head_chicken.png');

-- Lava Crown

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_lavahead', 'item', 'Lava Crown','Lavakrone','Lava Chrone');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_lavahead', 'item_desc', 'Keeping cool is required when wearing this hot headpiece.','Man muss einen kühlen Kopf bewahren wenn man dieses heisse Kopfstück trägt.','Me mun en chüele Chopf bewahre wenme das heisse Chopfstück treit.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_lavahead', 'eq_head', 700, 50, 105, 2);
INSERT INTO item_attribute (item_id, res_fire, res_ice, max_health, res_physical, dmg_fire) values ('eq_lavahead', 100, -50, 20, 20, 3);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_lavahead', 'res/texture/equipment/head/spritesheet_head_lavahead.png', 'res/texture/equipment/head/spritesheet_map_head_lavahead.png');

-- Lava Armour

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_lavaarmor', 'item', 'Lava Armour','Lavarüstung','Lava Rüschtig');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_lavaarmor', 'item_desc', 'A very hot armour made from the remains of lava wraiths. It fights fire with fire.','Eine sehr heisse Rüstung, angefertigt aus den Überresten von Lava Geistern. Sie bekämpft Feuer mit Feuer.','E sehr heissi Rüstig, gmacht us de Überreste vo Lava Geister. Si bekämpft Füür mit Füür.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_lavaarmor', 'eq_body', 700, 100, 220, 2);
INSERT INTO item_attribute (item_id, res_fire, res_ice, max_health, res_physical, dmg_fire) values ('eq_lavaarmor', 200, -100, 45, 50, 5);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_lavaarmor', 'res/texture/equipment/body/spritesheet_body_lavaarmor.png', 'res/texture/equipment/body/spritesheet_map_body_lavaarmor.png');

-- Lava Mantle

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_lavaback', 'item', 'Lava Mantle','Lavamantel','Lava Mantel');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_lavaback', 'item_desc', 'This magical lava holds great fire powers, but burns its wearer.','Diese magische Lava hat zwar grosse Feuerkräfte, verbrennt jedoch seinen Träger.','Die magisch Lava het zwar grossi Füürchräft, verbrennt aber sin Träger.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_lavaback', 'eq_back', 700, 250, 35, 2);
INSERT INTO item_attribute (item_id, res_fire, dmg_fire, res_ice, max_health, health_regeneration) values ('eq_lavaback', 100, 5, -50, 10, -2);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_lavaback', 'res/texture/equipment/back/spritesheet_back_lavaback.png', 'res/texture/equipment/back/spritesheet_map_back_lavaback.png');
INSERT INTO item_equipment_particle (item_id, particle_count, emit_rate, additive_blend_mode, texture_path, spawner_radius, spawner_offset_x, spawner_offset_y, size_start_min, size_start_max, size_end_min, size_end_max, color_start_min_r, color_start_min_g, color_start_min_b, color_start_min_a, color_start_max_r, color_start_max_g, color_start_max_b, color_start_max_a, color_end_min_r, color_end_min_g, color_end_min_b, color_end_min_a, color_end_max_r, color_end_max_g, color_end_max_b, color_end_max_a, goal_offset_x, goal_offset_y, speed_min, speed_max, time_min, time_max, attract_fraction) values ('eq_lavaback', 50, 30, 0, 'res/texture/particles/smoke.png', 20, 0, 30, 
20, 100, 80, 120,
44, 51, 77, 100,
71, 71, 91, 150,
196, 197, 218, 0,
218, 219, 230, 0,
0, -50,
50, 80,
1, 1.2, 0.9);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('eq_lavaback', -35, -3, 40, 18);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('eq_lavaback', 1, 650, 150, 50, 100);

-- Stone Crown

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_cairnhead', 'item', 'Stone Crown','Steinkrone','Stei Chrone');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_cairnhead', 'item_desc', 'The crown of a cairn wraith.','Die Krone eines Cairn Geistes.','D''Chrone vomne Steimännli Geischt.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_cairnhead', 'eq_head', 700, 450, 105, 2);
INSERT INTO item_attribute (item_id, dmg_ice, res_ice, max_health, res_physical, res_fire) values ('eq_cairnhead', 3, 100, 20, 20, -50);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_cairnhead', 'res/texture/equipment/head/spritesheet_head_cairnhead.png', 'res/texture/equipment/head/spritesheet_map_head_cairnhead.png');

-- Cairn Armour

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_cairnarmor', 'item', 'Cairn Armour','Cairn-Rüstung','Cairn-Rüschtig');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_cairnarmor', 'item_desc', 'Made from the remains of cairn wraiths, this armour helps protecting its wearer against ice magic.','Diese Rüstung wurde aus den Überresten von Cairn Geistern angefertigt und schützt ihren Träger vor Eismagie.','Die Rüschtig isch vode Überreste vo Steimännli Geischter gmacht worde und schützt ihre Träger vor Iismagie.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_cairnarmor', 'eq_body', 700, 500, 220, 2);
INSERT INTO item_attribute (item_id, dmg_ice, res_ice, max_health, res_physical, res_fire) values ('eq_cairnarmor', 5, 200, 45, 50, -100);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_cairnarmor', 'res/texture/equipment/body/spritesheet_body_cairnarmor.png', 'res/texture/equipment/body/spritesheet_map_body_cairnarmor.png');

-- Ice Hat

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_icehat', 'item', 'Zeff''s Masterpiece','Zeff''s Meisterwerk','Em Zeff sis Meisterwerk');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_icehat', 'item_desc', 'Zeff crafted this hat for me in return for saving him.','Zeff hat diesen Hut für mich gemacht, als Dank für seine Rettung.','De Zeff het de Huet für mich gmacht, als Dank für sini Rettig.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_icehat', 'eq_head', 250, 500, 40, 1);
INSERT INTO item_attribute (item_id, dmg_ice, max_health, res_physical, res_ice) values ('eq_icehat', 3, 5, 10, 30);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_icehat', 'res/texture/equipment/head/spritesheet_head_icehat.png', 'res/texture/equipment/head/spritesheet_map_head_icehat.png');

-- Ice Armour

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_icearmor', 'item', 'Icy Armour','Eisige Rüstung','Iisigi Rüschtig');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_icearmor', 'item_desc', 'Harek crafted this armour from the remains of Khajag.','Harek schmiedete diese Rüstung aus den Überbleibseln von Khajag.','De Harek het die Rüschtig us de Überbliibsel vom Khajag gschmiedet.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_icearmor', 'eq_body', 350, 500, 155, 2);
INSERT INTO item_attribute (item_id, res_fire, res_physical, critical, res_ice, dmg_ice, max_health) values ('eq_icearmor', 20, 30, 2, 50, 5, 14);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_icearmor', 'res/texture/equipment/body/spritesheet_body_icearmor.png', 'res/texture/equipment/body/spritesheet_map_body_icearmor.png');

-- Ice Cloak

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_icescarf', 'item', 'Icy Cloak','Eisiger Umhang','Iisige Umhang');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_icescarf', 'item_desc', 'Shiny fragments of a magical ice crystal decorate this cloak.','Glänzende Fragmente eines magischen Eiskristalls zieren diesen Umhang.','Glänzigi Fragment vomne magische IIskristall ziered de Umhang.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_icescarf', 'eq_back', 300, 500, 75, 2);
INSERT INTO item_attribute (item_id, dmg_ice, res_ice, max_health, res_physical, res_fire) values ('eq_icescarf', 3, 30, 10, 10, 10);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path, frames_jump, frames_idle) values ('eq_icescarf', 'res/texture/equipment/back/spritesheet_back_icescarf.png', 'res/texture/equipment/back/spritesheet_map_back_icescarf.png', 6, 6);

-- Blue Wizard Hat

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_wizardhatice', 'item', 'Blue Wizardhat','Blauer Spitzhut','Blaue Magierhuet');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_wizardhatice', 'item_desc', 'A perfect fit for a mage.','Perfekt für einen Magier.','Perfekt für en Magier.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_wizardhatice', 'eq_head', 500, 100, 50, 1);
INSERT INTO item_attribute (item_id, dmg_ice, max_health, res_physical, res_ice) values ('eq_wizardhatice', 4, 5, 10, 15);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_wizardhatice', 'res/texture/equipment/head/spritesheet_head_wizardhat_ice.png', 'res/texture/equipment/head/spritesheet_map_head_wizardhat_ice.png');
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('eq_wizardhatice', -5, -26, 39, 31);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('eq_wizardhatice', 1, 0, 300, 50, 100);

-- Red Wizard Hat

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_wizardhatfire', 'item', 'Red Wizardhat','Roter Spitzhut','Rote Magierhuet');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_wizardhatfire', 'item_desc', 'A perfect fit for a mage.','Perfekt für einen Magier.','Perfekt für en Magier.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_wizardhatfire', 'eq_head', 500, 150, 50, 1);
INSERT INTO item_attribute (item_id, res_fire, max_health, res_physical, dmg_fire) values ('eq_wizardhatfire', 15, 5, 10, 4);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_wizardhatfire', 'res/texture/equipment/head/spritesheet_head_wizardhat_fire.png', 'res/texture/equipment/head/spritesheet_map_head_wizardhat_fire.png');
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('eq_wizardhatfire', -5, -26, 39, 31);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('eq_wizardhatfire', 1, 50, 300, 50, 100);

-- White Wizard Hat

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_wizardhatlight', 'item', 'White Wizardhat','Weisser Spitzhut','Wiisse Magierhuet');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_wizardhatlight', 'item_desc', 'A perfect fit for a mage.','Perfekt für einen Magier.','Perfekt für en Magier.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_wizardhatlight', 'eq_head', 500, 200, 50, 1);
INSERT INTO item_attribute (item_id, dmg_light, res_light, max_health, res_physical) values ('eq_wizardhatlight', 4, 15, 5, 10);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_wizardhatlight', 'res/texture/equipment/head/spritesheet_head_wizardhat_light.png', 'res/texture/equipment/head/spritesheet_map_head_wizardhat_light.png');
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('eq_wizardhatlight', -5, -26, 39, 31);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('eq_wizardhatlight', 1, 100, 300, 50, 100);

-- Grey Wizard Hat

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_wizardhatshadow', 'item', 'Grey Wizardhat','Grauer Spitzhut','Graue Magierhuet');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_wizardhatshadow', 'item_desc', 'A perfect fit for a mage.','Perfekt für einen Magier.','Perfekt für en Magier.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_wizardhatshadow', 'eq_head', 500, 250, 190, 1);
INSERT INTO item_attribute (item_id, res_shadow, max_health, res_physical, dmg_shadow) values ('eq_wizardhatshadow', 15, 5, 10, 4);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_wizardhatshadow', 'res/texture/equipment/head/spritesheet_head_wizardhat_shadow.png', 'res/texture/equipment/head/spritesheet_map_head_wizardhat_shadow.png');
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('eq_wizardhatshadow', -5, -26, 39, 31);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('eq_wizardhatshadow', 1, 150, 300, 50, 100);

-- Oversized Hat

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_oversizedhat', 'item', 'Oversized Hat','Übergrosser Hut','Übergrosse Huet');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_oversizedhat', 'item_desc', 'This hat is way too big for me.','Dieser Hut ist viel zu gross für mich.','De Huet isch viel z''gross für mich.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_oversizedhat', 'eq_head', 450, 300, 190, 2);
INSERT INTO item_attribute (item_id, dmg_shadow, res_light, dmg_fire, res_fire, res_physical, critical, res_ice, dmg_ice, max_health) values ('eq_oversizedhat', 5, 50, 4, 10, 14, 2, 15, 4, 20);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_oversizedhat', 'res/texture/equipment/head/spritesheet_head_oversizedhat.png', 'res/texture/equipment/head/spritesheet_map_head_oversizedhat.png');
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('eq_oversizedhat', -20, -20, 70, 40);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('eq_oversizedhat', 1, 250, 350, 100, 100);

-- Travel Hat

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_travelhat', 'item', 'Travel Hat','Reisehut','Reisehuet');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_travelhat', 'item_desc', 'This hat belonged to me once.','Dieser Hut gehörte einmal mir.','De Huet het mal mir ghört.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_travelhat', 'eq_head', 0, 150, 45, 1);
INSERT INTO item_attribute (item_id, dmg_ice, max_health, res_physical) values ('eq_travelhat', 2, 15, 20);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_travelhat', 'res/texture/equipment/head/spritesheet_head_travelhat.png', 'res/texture/equipment/head/spritesheet_map_head_travelhat.png');

-- Cendric's Hat

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_betahead', 'item', 'Cendric''s Hat','Cendric''s Hut','Em Cendric sin Huet');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_betahead', 'item_desc', 'Thanks!','Danke!','Merci!');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_betahead', 'eq_head', 0, 700, 60, 2);
INSERT INTO item_attribute (item_id, res_physical, res_ice, res_shadow, res_fire, res_light, max_health) values ('eq_betahead', 10, 10, 10, 10, 10, 5);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_betahead', 'res/texture/equipment/head/spritesheet_head_betahat.png', 'res/texture/equipment/head/spritesheet_map_head_betahat.png');

-- Bandana

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_bandana', 'item', 'Green Bandana','Grünes Kopftuch','Grüens Chopftuech');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_bandana', 'item_desc', 'The cloth is a bit tattered but it can still be worn.','Der Stoff ist ein wenig zerrissen, man kann es aber immer noch tragen.','De Stoff isch chli verschlisse, me chas aber immerna alege.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_bandana', 'eq_head', 0, 300, 25, 1);
INSERT INTO item_attribute (item_id, max_health, res_physical, res_fire) values ('eq_bandana', 5, 10, 10);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_bandana', 'res/texture/equipment/head/spritesheet_head_bandana.png', 'res/texture/equipment/head/spritesheet_map_head_bandana.png');

-- Mercenary Bandana

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_mercenarybandana', 'item', 'Mercenary Bandana','Söldnerkopftuch','Söldner Chopftuech');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_mercenarybandana', 'item_desc', 'They shouldn''t have messed with me.','Sie hätten sich nicht mit mir anlegen sollen.','Si heted sich nid mit mir alege söle.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_mercenarybandana', 'eq_head', 600, 50, 70, 2);
INSERT INTO item_attribute (item_id, dmg_physical, res_fire, max_health, res_physical, dmg_shadow) values ('eq_mercenarybandana', 2, 23, 7, 20, 2);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_mercenarybandana', 'res/texture/equipment/head/spritesheet_head_mercenarybandana.png', 'res/texture/equipment/head/spritesheet_map_head_mercenarybandana.png');

-- Wolf Bonnet

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_wolfbonnet', 'item', 'Wolf Bonnet','Wolfsmütze','Wolfchappe');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_wolfbonnet', 'item_desc', 'Warm and cuddly. Crafted by the leatherworker Zeff.','Warm und kuschlig. Zeff hat sie für mich genäht.','Warm und kuschlig. De Zeff het si für mich gneiht.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_wolfbonnet', 'eq_head', 750, 50, 55, 1);
INSERT INTO item_attribute (item_id, dmg_physical, res_light, max_health, res_physical, res_ice) values ('eq_wolfbonnet', 2, 10, 5, 10, 20);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_wolfbonnet', 'res/texture/equipment/head/spritesheet_head_wolfbonnet.png', 'res/texture/equipment/head/spritesheet_map_head_wolfbonnet.png');

-- Scarf of the Fire Rat

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_fireratscarf', 'item', 'Scarf of the Fire Rat','Schal der Feuerratte','Füürratte Halstuech');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_fireratscarf', 'item_desc', 'This scarf is made from the fur of a fire rat. The wearer is protected from both heat and flame.','Dieser Schal wurde aus Feuerrattenfell gefertigt. Sein Träger ist vor Hitze und Flammen geschützt.','Da Halstuech isch us Füürrattefell gmacht worde. Sin Träger isch vor Hitz und Flamme gschützt.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_fireratscarf', 'eq_back', 150, 350, 35, 2);
INSERT INTO item_attribute (item_id, res_fire, max_health, res_physical, dmg_fire) values ('eq_fireratscarf', 10, 5, 10, 2);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_fireratscarf', 'res/texture/equipment/back/spritesheet_back_fireratscarf.png', 'res/texture/equipment/back/spritesheet_map_back_fireratscarf.png');

-- Demon Back

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_demonback', 'item', 'Demon Cape','Dämonenumhang','Dämone Umhang');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_demonback', 'item_desc', 'Cape worn by demon hunters.','Umhang eines Dämonenjägers.','Umhang voneme Dämonejäger.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_demonback', 'eq_back', 0, 350, 170, 2);
INSERT INTO item_attribute (item_id, dmg_shadow, dmg_physical, res_shadow, res_fire, res_physical, max_health) values ('eq_demonback', 3, 3, 30, 30, 50, 30);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path, frames_jump) values ('eq_demonback', 'res/texture/equipment/back/spritesheet_back_demon.png', 'res/texture/equipment/back/spritesheet_map_back_demon.png', 3);

-- Demon Head

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_demonhead', 'item', 'Demon Helmet','Dämonenhelm','Dämone Helm');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_demonhead', 'item_desc', 'Helmet worn by demon hunters.','Helm eines Dämonenjägers.','Helm voneme Dämonejäger.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_demonhead', 'eq_head', 50, 350, 190, 2);
INSERT INTO item_attribute (item_id, dmg_shadow, dmg_physical, res_shadow, res_fire, res_physical, max_health) values ('eq_demonhead', 4, 4, 30, 30, 50, 40);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_demonhead', 'res/texture/equipment/head/spritesheet_head_demon.png', 'res/texture/equipment/head/spritesheet_map_head_demon.png');
INSERT INTO item_equipment_light(item_id, light_offset_x, light_offset_y, light_radius_x, light_radius_y, brightness) values ('eq_demonhead',5, 25, 25, 25, 0.3);

-- Demon Body

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_demonbody', 'item', 'Demon Armour','Dämonenrüstung','Dämone Rüschtig');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_demonbody', 'item_desc', 'Armour worn by demon hunters.','Rüstung eines Dämonenjägers.','Rüschtig voneme Dämonejäger.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_demonbody', 'eq_body', 100, 350, 380, 2);
INSERT INTO item_attribute (item_id, dmg_shadow, dmg_physical, res_shadow, res_fire, res_physical, max_health) values ('eq_demonbody', 8, 8, 60, 60, 100, 80);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_demonbody', 'res/texture/equipment/body/spritesheet_body_demon.png', 'res/texture/equipment/body/spritesheet_map_body_demon.png');

-- Nekomata Back

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_nekomataback', 'item', 'Nekomata Cloak','Nekomata-Mantel','Nekomata Mantel');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_nekomataback', 'item_desc', 'Both an experienced hunter and a skilled leatherworker are involved in the creation of such a cape.','Ein erfahrener Jäger sowie ein geschickter Lederer sind nötig um einen solchen Umhang herzustellen.','Es brucht en erfahrene Jäger und en gschickte Lederer zum sonen Umhang herz''stelle.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_nekomataback', 'eq_back', 0, 400, 145, 2);
INSERT INTO item_attribute (item_id, dmg_shadow, res_light, dmg_fire, dmg_physical, res_physical, critical, haste, max_health) values ('eq_nekomataback', 2, 40, 2, 1, 8, 4, 10, 20);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path, frames_jump) values ('eq_nekomataback', 'res/texture/equipment/back/spritesheet_back_nekomata.png', 'res/texture/equipment/back/spritesheet_map_back_nekomata.png', 3);

-- Nekomata Head

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_nekomatahead', 'item', 'Nekomata Head','Nekomata-Kopf','Nekomata Chopf');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_nekomatahead', 'item_desc', 'Looking in the eyes of a Nekomata can fill the bravest warriors with terror.','In die Augen einer Nekomata zu schauen kann den mutigsten Kriegern Angst einflössen.','I''d Auge vonere Nekomata z''luege cha de muetigste Chrieger Angst ihflösse.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_nekomatahead', 'eq_head', 50, 400, 200, 2);
INSERT INTO item_attribute (item_id, dmg_shadow, res_light, dmg_fire, dmg_physical, res_physical, critical, haste, max_health) values ('eq_nekomatahead', 3, 42, 3, 2, 10, 6, 10, 40);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_nekomatahead', 'res/texture/equipment/head/spritesheet_head_nekomata.png', 'res/texture/equipment/head/spritesheet_map_head_nekomata.png');

-- Shadow Cloak

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_shadowcloak', 'item', 'Shadow Cloak','Schattenumhang','Schatteumhang');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_shadowcloak', 'item_desc', 'A pliant piece of cloth.','Ein geschmeidiges Stück Stoff.','Es gschmeidigs Stück Stoff.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_shadowcloak', 'eq_back', 50, 200, 0, 1);
INSERT INTO item_attribute (item_id) values ('eq_shadowcloak');
INSERT INTO item_equipment (item_id, texture_path, map_texture_path, frames_jump) values ('eq_shadowcloak', 'res/texture/equipment/back/spritesheet_back_shadowcloak.png', 'res/texture/equipment/back/spritesheet_map_back_shadowcloak.png', 3);

-- Horned Scalemail

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_scalearmor', 'item', 'Horned Scalemail','Gehörnter Schuppenpanzer','Ghörnte Schuppepanzer');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_scalearmor', 'item_desc', '...','...','...');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_scalearmor', 'eq_body', 100, 200, 0, 2);
INSERT INTO item_attribute (item_id) values ('eq_scalearmor');
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_scalearmor', 'res/texture/equipment/body/spritesheet_body_scalearmor.png', 'res/texture/equipment/body/spritesheet_map_body_scalearmor.png');

-- Necro T1 Back

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_back_necrot1', 'item', 'Novice''s Cover','Novizenüberwurf','Novize-Überwurf');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_back_necrot1', 'item_desc', 'A short decorative cape.','Ein kurzer Zierumhang.','En churze Zierumhang');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_back_necrot1', 'eq_back', 150, 650, 50, 1);
INSERT INTO item_attribute (item_id, dmg_shadow, res_light, res_shadow, res_physical, critical, haste, dmg_ice, max_health) values ('eq_back_necrot1', 1, 10, 10, 5, 1, 2, 1, 3);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_back_necrot1', 'res/texture/equipment/back/spritesheet_back_necrot1.png', 'res/texture/equipment/back/spritesheet_map_back_necrot1.png');

-- Necro T2 Back

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_back_necrot2', 'item', 'Adept''s Cover','Adeptenüberwurf','Adepte-Überwurf');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_back_necrot2', 'item_desc', 'Magic symbols and ornaments decorate this cape.','Magische Zeichen und Ornamente zieren diesen Umhang.','Magischi Zeiche und Ornamentli ziered de Umhang.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_back_necrot2', 'eq_back', 200, 650, 100, 2);
INSERT INTO item_attribute (item_id, dmg_shadow, res_light, res_shadow, res_physical, critical, haste, dmg_ice, max_health) values ('eq_back_necrot2', 2, 20, 20, 10, 2, 4, 2, 6);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_back_necrot2', 'res/texture/equipment/back/spritesheet_back_necrot2.png', 'res/texture/equipment/back/spritesheet_map_back_necrot2.png');

-- Necro T3 Back

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_back_necrot3', 'item', 'Bone Scarf','Knochenschal','Chnoche-Halstuech');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_back_necrot3', 'item_desc', 'This scarf is made from the tail of a monstrous scorpion.','Dieser Schal wurde aus dem Schwanz eines monströsen Skorpions gefertigt.','Da Halstuech isch usem Schwanz vomne monströse Skorpion gmacht worde.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_back_necrot3', 'eq_back', 250, 650, 200, 3);
INSERT INTO item_attribute (item_id, dmg_shadow, res_light, res_shadow, res_physical, critical, haste, dmg_ice, max_health) values ('eq_back_necrot3', 4, 40, 40, 20, 4, 8, 4, 12);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_back_necrot3', 'res/texture/equipment/back/spritesheet_back_necrot3.png', 'res/texture/equipment/back/spritesheet_map_back_necrot3.png');

-- Necro T1 Body

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_body_necrot1', 'item', 'Novice''s Clothing','Novizenkleidung','Novize-Chleider');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_body_necrot1', 'item_desc', 'Worn by the novices of the circle.','Wird von den Novizen des Zirkels getragen.','Wird vo de Novize vom Zirkel treit.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_body_necrot1', 'eq_body', 500, 600, 75, 1);
INSERT INTO item_attribute (item_id, dmg_shadow, res_light, res_shadow, res_physical, critical, haste, dmg_ice, max_health) values ('eq_body_necrot1', 2, 5, 5, 10, 2, 2, 2, 15);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_body_necrot1', 'res/texture/equipment/body/spritesheet_body_necrot1.png', 'res/texture/equipment/body/spritesheet_map_body_necrot1.png');

-- Necro T2 Body

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_body_necrot2', 'item', 'Adept''s Robes','Adeptenroben','Adepte-Robe');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_body_necrot2', 'item_desc', 'For knowledge itself is power.','Denn Wissen selbst ist Macht.','Well Wüsse elei isch Macht.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_body_necrot2', 'eq_body', 550, 600, 150, 2);
INSERT INTO item_attribute (item_id, dmg_shadow, res_light, heal, res_shadow, res_physical, critical, haste, dmg_ice, max_health) values ('eq_body_necrot2', 4, 10, 1, 10, 15, 4, 4, 4, 30);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_body_necrot2', 'res/texture/equipment/body/spritesheet_body_necrot2.png', 'res/texture/equipment/body/spritesheet_map_body_necrot2.png');

-- Necro T3 Body

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_body_necrot3', 'item', 'Bone Vestments','Knochengewänder','Chnoche-Gwänder');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_body_necrot3', 'item_desc','Only those who have proven themselves worthy to the necromancers of Admantris are allowed to wear that piece of armour.','Nur diejenigen, die sich bei den Nekromanten von Admantris als würdig erwiesen haben, dürfen diese Rüstung tragen.','Nur die, wo sich bide Nekromante vo Admantris als würdig erwise hend, derfed die Rüschtig träge.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_body_necrot3', 'eq_body', 600, 600, 300, 3);
INSERT INTO item_attribute (item_id, dmg_shadow, res_light, heal, res_shadow, res_physical, critical, haste, dmg_ice, max_health) values ('eq_body_necrot3', 8, 20, 2, 20, 30, 8, 8, 8, 60);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_body_necrot3', 'res/texture/equipment/body/spritesheet_body_necrot3.png', 'res/texture/equipment/body/spritesheet_map_body_necrot3.png');

-- Necro T1 Head

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_head_necrot1', 'item', 'Novice''s Cowl','Novizengugel','Novize-Gugle');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_head_necrot1', 'item_desc', 'Shields perfectly against any light.','Schirmt perfekt jedwedes Licht ab.','Schirmt perfekt ab gege Liecht.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_head_necrot1', 'eq_head', 0, 650, 45, 1);
INSERT INTO item_attribute (item_id, dmg_shadow, res_light, res_shadow, res_physical, critical, haste, dmg_ice, max_health) values ('eq_head_necrot1', 1, 4, 4, 5, 1, 4, 1, 6);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_head_necrot1', 'res/texture/equipment/head/spritesheet_head_necrot1.png', 'res/texture/equipment/head/spritesheet_map_head_necrot1.png');

-- Necro T2 Head

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_head_necrot2', 'item', 'Adept''s Cowl','Adeptengugel','Adepte-Gugle');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_head_necrot2', 'item_desc', 'Those spikes look menacing.','Diese Stacheln sehen bedrohlich aus.','Die Stachle gsehnd zum fürchte us.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_head_necrot2', 'eq_head', 50, 650, 90, 2);
INSERT INTO item_attribute (item_id, dmg_shadow, res_light, res_shadow, res_physical, critical, haste, dmg_ice, max_health) values ('eq_head_necrot2', 2, 8, 8, 10, 2, 8, 2, 12);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_head_necrot2', 'res/texture/equipment/head/spritesheet_head_necrot2.png', 'res/texture/equipment/head/spritesheet_map_head_necrot2.png');

-- Necro T3 Head

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_head_necrot3', 'item', 'Bone Mask','Knochenmaske','Chnoche-Maske');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_head_necrot3', 'item_desc', 'An indispensable utensil for a necromancer. Protects it''s wearer against pestilence.','Ein unverzichtbares Utensil für einen Nekromanten. Schützt ihren Träger gegen Seuchen.','Es unverzichtbars Teili für en Nekromant. Schützt ire Träger gege Süüchene.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_head_necrot3', 'eq_head', 100, 650, 180, 3);
INSERT INTO item_attribute (item_id, dmg_shadow, res_light, heal, res_shadow, res_physical, critical, haste, dmg_ice, max_health) values ('eq_head_necrot3', 4, 14, 1, 15, 20, 4, 15, 4, 24);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_head_necrot3', 'res/texture/equipment/head/spritesheet_head_necrot3.png', 'res/texture/equipment/head/spritesheet_map_head_necrot3.png');

-- Divine T1 Back

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_back_divinet1', 'item', 'Cover of the Recruit','Überwurf des Rekruten','Rekrute-Überwurf');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_back_divinet1', 'item_desc', 'The grey cloth shields its wearer against dark magic.','Der graue Stoff schützt seinen Träger vor dunkler Magie.','De grau Stoff schützt sin Träger vor dunkler Magie.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_back_divinet1', 'eq_back', 0, 450, 50, 1);
INSERT INTO item_attribute (item_id, res_light, dmg_fire, dmg_light, heal, res_shadow, res_physical, critical, haste, max_health) values ('eq_back_divinet1', 8, 1, 1, 1, 8, 5, 1, 2, 2);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_back_divinet1', 'res/texture/equipment/back/spritesheet_back_divinet1.png', 'res/texture/equipment/back/spritesheet_map_back_divinet1.png');

-- Divine T2 Back

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_back_divinet2', 'item', 'Cover of the Order','Überwurf des Ordens','Ordens-Überwurf');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_back_divinet2', 'item_desc', 'A noble cape with nice ornaments.','Ein nobler Umhang mit hübschen Ornamenten.','En noble Umhang mit hübsche Ornamentli.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_back_divinet2', 'eq_back', 0, 500, 100, 2);
INSERT INTO item_attribute (item_id, res_light, dmg_fire, dmg_light, heal, res_shadow, res_physical, critical, haste, max_health) values ('eq_back_divinet2', 19, 2, 2, 1, 18, 10, 2, 4, 4);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_back_divinet2', 'res/texture/equipment/back/spritesheet_back_divinet2.png', 'res/texture/equipment/back/spritesheet_map_back_divinet2.png');

-- Divine T3 Back

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_back_divinet3', 'item', 'Cover of the Enlighted','Überwurf des Erleuchteten','Erlüüchtete-Überwurf');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_back_divinet3', 'item_desc', 'Worn by the high priests of the order.','Wird von den Hohepriestern des Ordens getragen.','Wird vode Hohespriester vom Orde treit.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_back_divinet3', 'eq_back', 0, 550, 200, 3);
INSERT INTO item_attribute (item_id, res_light, dmg_fire, dmg_light, heal, res_shadow, res_physical, critical, haste, max_health) values ('eq_back_divinet3', 37, 4, 4, 2, 37, 20, 4, 8, 8);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_back_divinet3', 'res/texture/equipment/back/spritesheet_back_divinet3.png', 'res/texture/equipment/back/spritesheet_map_back_divinet3.png');

-- Divine T1 Head

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_head_divinet1', 'item', 'Circlet of the Recruit','Reif des Rekruten','Rekrute-Reif');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_head_divinet1', 'item_desc', 'Simple but elegant.','Schlicht aber elegant.','Schlicht aber elegant.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_head_divinet1', 'eq_head', 50, 500, 45, 1);
INSERT INTO item_attribute (item_id, res_light, dmg_fire, dmg_light, heal, res_shadow, res_physical, critical, haste, max_health) values ('eq_head_divinet1', 2, 1, 1, 1, 1, 10, 1, 2, 5);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_head_divinet1', 'res/texture/equipment/head/spritesheet_head_divinet1.png', 'res/texture/equipment/head/spritesheet_map_head_divinet1.png');

-- Divine T2 Head

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_head_divinet2', 'item', 'Circlet of the Order','Reif des Ordens','Ordens-Reif');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_head_divinet2', 'item_desc', 'Wearing this headpiece shows devotion to the Eternal Light.','Diesen Reif zu tragen zeigt Hingabe zum Ewigen Licht.','De Reif z''träge zeigt, dasme em Ewige Liecht hiigä isch.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_head_divinet2', 'eq_head', 100, 500, 90, 2);
INSERT INTO item_attribute (item_id, res_light, dmg_fire, dmg_light, heal, res_shadow, res_physical, critical, haste, max_health) values ('eq_head_divinet2', 6, 2, 2, 2, 5, 15, 2, 4, 10);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_head_divinet2', 'res/texture/equipment/head/spritesheet_head_divinet2.png', 'res/texture/equipment/head/spritesheet_map_head_divinet2.png');

-- Divine T3 Head

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_head_divinet3', 'item', 'Circlet of the Enlighted','Reif des Erleuchteten','Erlüüchtete-Reif');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_head_divinet3', 'item_desc', 'The majestic wings give the wearer of this headpiece a sublime appearance.','Die majestätischen Flügel lassen den Träger dieses Reifs erhaben wirken.','Di majestätische Flügel lönd de Träger vo dem Reif erhabe würke.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_head_divinet3', 'eq_head', 150, 500, 180, 3);
INSERT INTO item_attribute (item_id, res_light, dmg_fire, dmg_light, heal, res_shadow, res_physical, critical, haste, max_health) values ('eq_head_divinet3', 14, 4, 4, 3, 13, 30, 4, 8, 20);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_head_divinet3', 'res/texture/equipment/head/spritesheet_head_divinet3.png', 'res/texture/equipment/head/spritesheet_map_head_divinet3.png');

-- Divine T1 Body

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_body_divinet1', 'item', 'Tunic of the Recruit','Tunika des Rekruten','Rekrute-Tunika');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_body_divinet1', 'item_desc', 'A light but noble armour.','Eine leichte und dennoch edle Rüstung.','E liechti und trotzdem edli Rüschtig.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_body_divinet1', 'eq_body', 350, 600, 75, 1);
INSERT INTO item_attribute (item_id, res_light, dmg_fire, dmg_light, heal, res_shadow, res_physical, critical, haste, max_health) values ('eq_body_divinet1', 5, 2, 2, 1, 5, 6, 2, 4, 10);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_body_divinet1', 'res/texture/equipment/body/spritesheet_body_divinet1.png', 'res/texture/equipment/body/spritesheet_map_body_divinet1.png');

-- Divine T2 Body

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_body_divinet2', 'item', 'Robes of the Order','Roben des Orden','Ordens-Robe');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_body_divinet2', 'item_desc', 'Worn by the priests of the order.','Wird von den Priestern des Ordens getragen.','Wird vode Priester vom Orde treit.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_body_divinet2', 'eq_body', 400, 600, 150, 2);
INSERT INTO item_attribute (item_id, res_light, dmg_fire, dmg_light, heal, res_shadow, res_physical, critical, haste, max_health) values ('eq_body_divinet2', 10, 4, 4, 2, 10, 12, 4, 8, 20);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_body_divinet2', 'res/texture/equipment/body/spritesheet_body_divinet2.png', 'res/texture/equipment/body/spritesheet_map_body_divinet2.png');

-- Divine T3 Body

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_body_divinet3', 'item', 'Vestments of the Enlighted','Gewänder des Erleuchteten','Erlüüchtete-Gwand');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_body_divinet3', 'item_desc', 'The golden plates on this armour shine.','Die goldenen Platten an dieser Rüstung strahlen.','Di goldige Platte a dere Rüschtig strahled.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_body_divinet3', 'eq_body', 450, 600, 300, 3);
INSERT INTO item_attribute (item_id, res_light, dmg_fire, dmg_light, heal, res_shadow, res_physical, critical, haste, max_health) values ('eq_body_divinet3', 20, 8, 8, 3, 19, 30, 7, 15, 50);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_body_divinet3', 'res/texture/equipment/body/spritesheet_body_divinet3.png', 'res/texture/equipment/body/spritesheet_map_body_divinet3.png');

-- Twilight T1 Head

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_head_twilightt1', 'item', 'Headband of the Prowler','Stirnband des Herumtreibers','Streuner-Stirnband');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_head_twilightt1', 'item_desc', 'Worn by daredevils and sneaky thieves.','Wird gerne von Draufgängern und hinterhältigen Dieben getragen.','Wird gern vo Drufgänger und schliichigem Diebesgsindel treit.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_head_twilightt1', 'eq_head', 100, 250, 45, 1);
INSERT INTO item_attribute (item_id, dmg_shadow, dmg_physical, res_shadow, res_physical, critical, haste, max_health) values ('eq_head_twilightt1', 1, 1, 5, 2, 2, 4, 4);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_head_twilightt1', 'res/texture/equipment/head/spritesheet_head_twilightt1.png', 'res/texture/equipment/head/spritesheet_map_head_twilightt1.png');


-- Twilight T2 Head

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_head_twilightt2', 'item', 'Mask of the Marauder','Maske des Plünderers','Plünderer-Maske');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_head_twilightt2', 'item_desc', 'Perfect if one doesn''t want to be recognized.','Perfekt wenn man nicht erkannt werden will.','Perfekt wenme nid erkennt werde wett.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_head_twilightt2', 'eq_head', 600, 400, 90, 2);
INSERT INTO item_attribute (item_id, dmg_shadow, dmg_physical, res_shadow, res_physical, critical, haste, max_health) values ('eq_head_twilightt2', 2, 2, 8, 6, 4, 8, 8);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_head_twilightt2', 'res/texture/equipment/head/spritesheet_head_twilightt2.png', 'res/texture/equipment/head/spritesheet_map_head_twilightt2.png');

-- Twilight T3 Head

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_head_twilightt3', 'item', 'Hood of the Shadow Stalker','Kapuze des Schattenläufers','Schatteläufer-Kapuze');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_head_twilightt3', 'item_desc', 'This hood is only worn by the most talented thieves of the guild.','Diese Kapuze wird nur von den talentiertesten Dieben der Gilde getragen.','Die Kabuze wird nur vode talentiertiste Diebe vode Gilde treit.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_head_twilightt3', 'eq_head', 600, 450, 180, 3);
INSERT INTO item_attribute (item_id, dmg_shadow, dmg_physical, res_shadow, res_physical, critical, haste, max_health) values ('eq_head_twilightt3', 4, 4, 20, 16, 8, 12, 16);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_head_twilightt3', 'res/texture/equipment/head/spritesheet_head_twilightt3.png', 'res/texture/equipment/head/spritesheet_map_head_twilightt3.png');

-- Twilight T1 Back

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_back_twilightt1', 'item', 'Scarf of the Prowler','Schal des Herumtreibers','Streuner-Halstuech');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_back_twilightt1', 'item_desc', 'Simple, inconspicuous and convenient.','Einfach, unauffällig und praktisch.','Eifach, unuffällig und praktisch.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_back_twilightt1', 'eq_back', 0, 600, 50, 1);
INSERT INTO item_attribute (item_id, dmg_shadow, res_light, dmg_physical, res_shadow, res_physical, critical, haste, max_health) values ('eq_back_twilightt1', 1, 2, 1, 10, 2, 2, 3, 4);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_back_twilightt1', 'res/texture/equipment/back/spritesheet_back_twilightt1.png', 'res/texture/equipment/back/spritesheet_map_back_twilightt1.png');

-- Twilight T2 Back

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_back_twilightt2', 'item', 'Cloak of the Marauder','Umhang des Plünderers','Plünderer-Umhang');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_back_twilightt2', 'item_desc', 'Mages wearing this kind of cloak are usually up to no good.','Magier, die diese Art von Umhang tragen sind haben meistens nichts gutes vor.','Magier, wo die Art vo Umhang träged, hend meistens nüt liebs im Sinn.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_back_twilightt2', 'eq_back', 50, 600, 100, 2);
INSERT INTO item_attribute (item_id, dmg_shadow, res_light, dmg_physical, res_shadow, res_physical, critical, haste, max_health) values ('eq_back_twilightt2', 2, 15, 2, 10, 3, 4, 6, 8);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_back_twilightt2', 'res/texture/equipment/back/spritesheet_back_twilightt2.png', 'res/texture/equipment/back/spritesheet_map_back_twilightt2.png');

-- Twilight T3 Back

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_back_twilightt3', 'item', 'Cover of the Shadow Stalker','Überwurf des Schattenläufers','Schatteläufer-Überwurf');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_back_twilightt3', 'item_desc', 'The sharp blades on the tails of this cloak are still reminiscent of its original owner.','Die scharfen Klingen an den Enden dieses Umhangs erinnern immer noch an seinen ursprünglichen Besitzer.','Di scharfe Messer ade Spitze vo dem Umhang erinnered immer na a de, dem er emol ghört het.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_back_twilightt3', 'eq_back', 100, 600, 200, 3);
INSERT INTO item_attribute (item_id, dmg_shadow, res_light, dmg_physical, res_shadow, res_physical, critical, haste, max_health) values ('eq_back_twilightt3', 4, 20, 4, 40, 20, 5, 12, 16);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_back_twilightt3', 'res/texture/equipment/back/spritesheet_back_twilightt3.png', 'res/texture/equipment/back/spritesheet_map_back_twilightt3.png');

-- Twilight T1 Body

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_body_twilightt1', 'item', 'Prowler Vest','Weste des Herumtreibers','Streuner-Weste');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_body_twilightt1', 'item_desc', 'A very light and confortable armour.','Eine sehr leichte und bequeme Rüstung.','E sehr liechti und bequemi Rüschtig.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_body_twilightt1', 'eq_body', 200, 600, 75, 1);
INSERT INTO item_attribute (item_id, dmg_shadow, dmg_physical, res_shadow, res_physical, critical, haste, max_health) values ('eq_body_twilightt1', 2, 2, 10, 10, 2, 5, 9);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_body_twilightt1', 'res/texture/equipment/body/spritesheet_body_twilightt1.png', 'res/texture/equipment/body/spritesheet_map_body_twilightt1.png');

-- Twilight T2 Body

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_body_twilightt2', 'item', 'Marauder Jerkin','Wams des Plünderers','Plünderer-Wams');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_body_twilightt2', 'item_desc', 'Be one with the shadows.','Sei eins mit den Schatten.','Sig eis mit de Schatte.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_body_twilightt2', 'eq_body', 250, 600, 150, 2);
INSERT INTO item_attribute (item_id, dmg_shadow, dmg_physical, res_shadow, res_physical, critical, haste, max_health) values ('eq_body_twilightt2', 4, 4, 15, 20, 5, 8, 19);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_body_twilightt2', 'res/texture/equipment/body/spritesheet_body_twilightt2.png', 'res/texture/equipment/body/spritesheet_map_body_twilightt2.png');

-- Twilight T3 Body

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_body_twilightt3', 'item', 'Shadow Stalker Armour','Rüstung des Schattenläufers','Schatteläufer-Rüschtig');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_body_twilightt3', 'item_desc', 'They''ll never know what hit them.','Sie werden dich nicht kommen sehen.','Wenn si dich gsehnd isches scho z''spat.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_body_twilightt3', 'eq_body', 300, 600, 300, 3);
INSERT INTO item_attribute (item_id, dmg_shadow, dmg_physical, res_shadow, res_physical, critical, haste, max_health) values ('eq_body_twilightt3', 8, 8, 30, 50, 10, 15, 30);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_body_twilightt3', 'res/texture/equipment/body/spritesheet_body_twilightt3.png', 'res/texture/equipment/body/spritesheet_map_body_twilightt3.png');

-- Crown

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_crown', 'item', 'Crown','Krone','Chrone');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_crown', 'item_desc', 'Golden and shiny.','Golden und glänzig.','Goldig und glänzig.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_crown', 'eq_head', 150, 200, 200, 2);
INSERT INTO item_attribute (item_id, heal, res_shadow, res_physical, critical, haste, max_health) values ('eq_crown', 2, 30, 30, 10, 10, 30);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_crown', 'res/texture/equipment/head/spritesheet_head_crown.png', 'res/texture/equipment/head/spritesheet_map_head_crown.png');

-- Leather Armour

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_leatherarmor', 'item', 'Leather Armour','Lederrüstung','Lederrüschtig');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_leatherarmor', 'item_desc', 'The reinforced leather protects from attacks.','Das verstärkte Leder schützt vor Angriffen.','Es verstärcht Leder schützt vor Aagriff.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_leatherarmor', 'eq_body', 50, 300, 45, 1);
INSERT INTO item_attribute (item_id, res_ice, max_health, res_physical, res_fire) values ('eq_leatherarmor', 10, 10, 15, 10);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_leatherarmor', 'res/texture/equipment/body/spritesheet_body_leatherarmor.png', 'res/texture/equipment/body/spritesheet_map_body_leatherarmor.png');

-- Prisoner''s Clothes

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_prisonerclothes', 'item', 'Prisoner''s Clothes','Sträflingskleidung','Ströflingschleider');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_prisonerclothes', 'item_desc', 'These rags are worn by the prisoners in the mines of Gandria.','Diese Lumpen werden von den Häftlingen in den Minen von Gandria getragen.','Die verluuste Lumpe werded vo Häftling ide Mine vo Gandria treit.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('eq_prisonerclothes', 'eq_body', 100, 550, 2);
INSERT INTO item_attribute (item_id, res_physical) values ('eq_prisonerclothes', 2);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_prisonerclothes', 'res/texture/equipment/body/spritesheet_body_prisonerclothes.png', 'res/texture/equipment/body/spritesheet_map_body_prisonerclothes.png');

-- Default Armour

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_defaultarmor', 'item', 'Grey Cloth Armour','Graue Stoffrüstung','Graui Stoffrüschtig');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_defaultarmor', 'item_desc', 'I wore that piece of cloth when I lost my memory.','Das habe ich getragen, als ich meine Erinnerung verlor.','Das hani treit woni mini Erinnerige verlore han.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('eq_defaultarmor', 'eq_body', 150, 300, 6);
INSERT INTO item_attribute (item_id, max_health, res_physical) values ('eq_defaultarmor', 1, 5);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_defaultarmor', 'res/texture/equipment/body/spritesheet_body_defaultarmor.png', 'res/texture/equipment/body/spritesheet_map_body_defaultarmor.png');

-- Rafi's Hat

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_rafishat', 'item', 'Rafi''s Hat','Rafis Hut','Rafis Huet');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_rafishat', 'item_desc', 'What a strange hat...','Was für ein seltsamer Hut...','Was füren komische Huet...');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('eq_rafishat', 'eq_head', 0, 250, 50);
INSERT INTO item_attribute (item_id, res_light, res_fire, res_physical, critical, haste, max_health) values ('eq_rafishat', 3, 2, 1, 10, -20, 4);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_rafishat', 'res/texture/equipment/head/spritesheet_head_rafishat.png', 'res/texture/equipment/head/spritesheet_map_head_rafishat.png');

-- Hunter's Hat

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_huntershat', 'item', 'Hunter''s Hat','Jägerhut','Jägershuet');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_huntershat', 'item_desc', 'A brown leather hat with a red feather. Protects its wearer from foul weather.','Ein brauner Lederhut mit einer roten Feder. Schützt seinen Besitzer vor schlechtem Wetter.','En bruune Lederhuet mit enere rote Federe. Schützt sin Träger vor gruusigem Wetter.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_huntershat', 'eq_head', 50, 250, 80, 1);
INSERT INTO item_attribute (item_id, res_light, dmg_physical, res_physical, critical, haste, max_health) values ('eq_huntershat', 20, 3, 20, 2, 3, 3);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_huntershat', 'res/texture/equipment/head/spritesheet_head_huntershat.png', 'res/texture/equipment/head/spritesheet_map_head_huntershat.png');

-- Ruby Ring

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_rubyring', 'item', 'Ruby Ring','Rubinring','Rubinring');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_rubyring', 'item_desc', 'The small ruby sparkles.','Der kleine Rubin funkelt.','De chli Rubin funklet.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_rubyring', 'eq_ring', 100, 50, 25, 1);
INSERT INTO item_attribute (item_id, res_fire, max_health, dmg_fire) values ('eq_rubyring', 10, 5, 2);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('eq_rubyring', -12, -37, 8, 8);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('eq_rubyring', 1, 200, 0, 50, 50);

-- Aquamarine Ring

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_aquamarinering', 'item', 'Aquamarine Ring','Aquamarinring','Aquamarinring');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_aquamarinering', 'item_desc', 'There is a coldness radiating from its blue stone.','Der blaue Stein strahlt eine Kälte aus.','De blau Stei strahlt e Chälti uus.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_aquamarinering', 'eq_ring', 100, 300, 25, 1);
INSERT INTO item_attribute (item_id, dmg_ice, max_health, res_ice) values ('eq_aquamarinering', 2, 5, 10);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('eq_aquamarinering', -12, -37, 8, 8);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('eq_aquamarinering', 1, 500, 100, 50, 50);

-- Iron Ring

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_ironring', 'item', 'Iron Ring','Eisener Ring','Isering');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_ironring', 'item_desc', 'Looks quite chunky.','Wirkt klobig.','Gseht zimli klobig us.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_ironring', 'eq_ring', 350, 100, 20, 1);
INSERT INTO item_attribute (item_id, dmg_physical, max_health, res_physical) values ('eq_ironring', 1, 5, 10);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('eq_ironring', -12, -37, 8, 8);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('eq_ironring', 1, 300, 100, 50, 50);

-- Bone Ring

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_bonering', 'item', 'Bone Ring','Knochenring','Chnochering');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_bonering', 'item_desc', 'Made from smoothed bones.','Aus geschliffenen Knochen gefertigt.','Us gschliffene Chnoche gmacht.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_bonering', 'eq_ring', 600, 250, 15, 1);
INSERT INTO item_attribute (item_id, res_shadow, max_health, dmg_shadow) values ('eq_bonering', 10, -10, 3);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('eq_bonering', -3, -39, 19, 14);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('eq_bonering', 1, 250, 300, 50, 50);

-- Holy Ring

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_holyring', 'item', 'Holy Ring','Heiliger Ring','Heilige Ring');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_holyring', 'item_desc', 'I found this shiny ring in the cathedral of Gandria.','Diesen glänzenden Ring habe ich in der Kathedrale von Gandria gefunden.','De glänzig Ring hani ide Kathedrale vo Gandria gfunde.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_holyring', 'eq_ring', 650, 450, 28, 1);
INSERT INTO item_attribute (item_id, dmg_light, heal, res_shadow, max_health) values ('eq_holyring', 2, 1, 10, 3);

-- Ring of the Sunblade

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_sunring', 'item', 'Ring of the Sunblade','Ring der Sonnenklinge','Sunneklingering');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_sunring', 'item_desc', 'Its spikes look terrifying.','Seine Stacheln sehen füchterlich aus.','Sini Stachle gsehnd fürchterlich us.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_sunring', 'eq_ring', 750, 100, 70, 2);
INSERT INTO item_attribute (item_id, dmg_physical, dmg_light, critical, haste, max_health) values ('eq_sunring', 2, 2, 3, 10, 6);

-- Eye of the Moon

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_eyeofthemoon', 'item', 'The Eye of the Moon','Das Auge des Mondes','Es Mondaug');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_eyeofthemoon', 'item_desc', 'Many legends surround this mysterious necklace.','Viele Legenden ranken sich um diese geheimnisvolle Halskette.','Viel Legende ranked sich um die gheimnisvoll Halschetti');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_eyeofthemoon', 'eq_neck', 350, 50, 60, 2);
INSERT INTO item_attribute (item_id, dmg_shadow, res_light) values ('eq_eyeofthemoon', 6, 30);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('eq_eyeofthemoon', -13, -26, 20, 40);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('eq_eyeofthemoon', 1, 650, 0, 50, 100);

-- Silver Necklace

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_silvernecklace', 'item', 'Silver Necklace','Silberne Halskette','Silbrigi Halschette');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_silvernecklace', 'item_desc', 'The small chain links shine.','Die kleinen Kettenglieder glänzen.','Di chliine Ketteglider glänzed.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_silvernecklace', 'eq_neck', 350, 200, 20, 1);
INSERT INTO item_attribute (item_id, critical, max_health, res_fire) values ('eq_silvernecklace', 1, 5, 7);

-- Ruby Necklace

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_rubynecklace', 'item', 'Ruby Necklace','Rubinhalskette','Rubinhalschette');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_rubynecklace', 'item_desc', 'The ruby feels warm, as if it had been worn lately.','Der Rubin fühlt sich warm an, als wäre er kürzlich von jemandem getragen worden.','De Rubin fühlt sich warm ah, als wär er erst grad vo öpperem treit worde.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_rubynecklace', 'eq_neck', 550, 300, 35, 1);
INSERT INTO item_attribute (item_id, critical, max_health, dmg_fire) values ('eq_rubynecklace', 1, 2, 5);

-- Star Talisman

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_startalisman', 'item', 'Star Talisman','Sternentalisman','Sterne Talisman');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_startalisman', 'item_desc', 'The small gems that form the star are sharp and spiky.','Die kleinen Edelsteine, die den Stern bilden, sind scharf und spitzig.','Di chliine Edelstei wo de Stern formed sind scharf und spitzig.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_startalisman', 'eq_neck', 550, 250, 35, 1);
INSERT INTO item_attribute (item_id, dmg_ice, critical, max_health) values ('eq_startalisman', 5, 1, 2);

-- Morgiana's Bloodstone

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_bloodstone', 'item', 'Morgiana''s Bloodstone','Morgiana''s Blutstein','Bluetstei vode Morgiana');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_bloodstone', 'item_desc', 'This amulet belonged to the mercenary Morgiana and gave her incredible stamina.','Dieses Amulett gehörte der Söldnerin Morgiana und hat ihr unglaubliche Ausdauer verliehen.','Das Amulett het de Morgiana ghört und het ihre unglaublichi Usduur gä.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_bloodstone', 'eq_neck', 650, 500, 60, 2);
INSERT INTO item_attribute (item_id, max_health, res_physical) values ('eq_bloodstone', 30, 30);

-- Sword Pendant

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_swordpendant', 'item', 'Sword Pendant','Schwertanhänger','Schwertahänger');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_swordpendant', 'item_desc', 'The small blade is sharp. I don''t know if having this near my throat is a good idea.','Die kleine Klinge ist scharf. Ich weiss nicht, ob es eine gute Idee ist, das Ding an meiner Kehle zu tragen.','Di chlii Klinge isch scharf. Ich weiss nid obs e gueti Idee isch, das Teil a mim Hals z''träge.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_swordpendant', 'eq_neck', 750, 150, 50, 2);
INSERT INTO item_attribute (item_id, dmg_physical, critical, haste, res_physical) values ('eq_swordpendant', 6, 3, 10, -24);

-- Light-Touched Pendant

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_lightpendant', 'item', 'Light-Touched Pendant','Lichtberührter Anhänger','Vom Liecht berührte Ahänger');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_lightpendant', 'item_desc', 'The pendant shines for those who believe in the Eternal Light. This trinket belongs to the High Priestess Inina.','Dieser Anhänger leuchtet für diejenigen, die an das Ewige Licht glauben. Das Schmuckstück gehört der Hohepriesterin Inina.','De Ahänger lüüchted für die, wo as Ewige Liecht glaubed. Da Schmuckstück ghört de Hohepriesterin Inina.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_lightpendant', 'eq_neck', 450, 0, 50, 2);
INSERT INTO item_attribute (item_id, dmg_light, heal, res_shadow) values ('eq_lightpendant', 5, 1, 20);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_lightpendant', '', '');
INSERT INTO item_equipment_light(item_id, light_offset_x, light_offset_y, light_radius_x, light_radius_y, map_light_radius_x, map_light_radius_y, brightness) values ('eq_lightpendant',60, 30, 300, 300, 100, 100, 0.8);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('eq_lightpendant', -13, -26, 20, 40);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('eq_lightpendant', 1, 450, 350, 50, 100);
INSERT INTO levelitem_light(item_id, light_offset_x, light_offset_y, light_radius_x, light_radius_y, brightness) values ('eq_lightpendant', 14, 73, 50, 80, 1.0);

-- Pendant of Eternal Youth

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_lifependant', 'item', 'Pendant of Eternal Youth','Anhänger der Ewigen Jugend','Ahänger vode Ewige Jugend');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_lifependant', 'item_desc', 'It is said that people wearing this magical pendant do not age.','Man erzählt sich, dass diejenigen, die diesen magischen Anhänger tragen, nicht altern.','Me verzellt sich, dass die Lüüt wo de magisch Ahänger träged, nid älter werded.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_lifependant', 'eq_neck', 700, 550, 60, 3);
INSERT INTO item_attribute (item_id, max_health, health_regeneration) values ('eq_lifependant', 10, 1);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_lifependant', '', '');
INSERT INTO item_equipment_light(item_id, light_offset_x, light_offset_y, light_radius_x, light_radius_y, map_light_radius_x, map_light_radius_y, brightness) values ('eq_lifependant',60, 30, 300, 300, 100, 100, 0.3);
INSERT INTO item_equipment_particle (item_id, particle_count, emit_rate, additive_blend_mode, texture_path, spawner_radius, spawner_offset_x, spawner_offset_y, size_start_min, size_start_max, size_end_min, size_end_max, color_start_min_r, color_start_min_g, color_start_min_b, color_start_min_a, color_start_max_r, color_start_max_g, color_start_max_b, color_start_max_a, color_end_min_r, color_end_min_g, color_end_min_b, color_end_min_a, color_end_max_r, color_end_max_g, color_end_max_b, color_end_max_a, goal_offset_x, goal_offset_y, speed_min, speed_max, time_min, time_max, attract_fraction) values ('eq_lifependant', 50, 5, 1, 'res/texture/particles/star.png', 20, 0, 0, 
5, 10, 0, 0,
110, 255, 100, 255,
110, 255, 100, 255,
20, 200, 0, 200,
20, 200, 0, 200,
0, -50,
40, 60,
1.0, 2.0, 0.0);

-- Anti-Magic Pendant

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_antimagicpendant', 'item', 'Anti-Magic Pendant','Antimagischer Anhänger','Antimagische Ahänger');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_antimagicpendant', 'item_desc', 'This crystal cancels most of the magic around it and "should" keep mages away.','Dieser Kristall löscht fast alle Magie um sich herum aus und "sollte" Magier fernhalten.','De Kristall löscht fascht all Magie um sich ume us und "sött" Magier fernhalte.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_antimagicpendant', 'eq_neck', 800, 700, 25, 1);
INSERT INTO item_attribute (item_id, res_light, res_fire, res_ice, res_shadow, dmg_light, dmg_fire, dmg_shadow, dmg_ice) values ('eq_antimagicpendant', 50, 50, 50, 50, -50, -50, -50, -50);

-- Magic Pendant

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_magicpendant', 'item', 'Magic Pendant','Magischer Anhänger','Magische Ahänger');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_magicpendant', 'item_desc', 'This pendant enhances all magic around it, not only my own...','Dieser Anhänger verstärkt alle Magie um sich herum, nicht nur meine eigene...','De Ahänger verstärcht all Magie um sich ume, nid nur mini eiget...');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_magicpendant', 'eq_neck', 50, 750, 25, 1);
INSERT INTO item_attribute (item_id, res_light, res_fire, res_ice, res_shadow, dmg_light, dmg_fire, dmg_shadow, dmg_ice) values ('eq_magicpendant', -100, -100, -100, -100, 2, 2, 2, 2);

-- Sparkling Green Ring

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_sparkling_green', 'item', 'Sparkling Green Ring','Funkelnder Grüner Ring','Funkelnde Grüene Ring');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_sparkling_green', 'item_desc', 'The green magic rejuvenates its wearer.','Die grüne Magie verjüngt seinen Träger.','Di grüen Magie verjüngt ihren Träger.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_sparkling_green', 'eq_ring', 800, 300, 200, 3);
INSERT INTO item_attribute (item_id, max_health, heal) values ('eq_sparkling_green', 30, 2);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_sparkling_green', '', '');
INSERT INTO item_equipment_light(item_id, light_offset_x, light_offset_y, light_radius_x, light_radius_y, map_light_radius_x, map_light_radius_y, brightness) values ('eq_sparkling_green', 17, 70, 50, 50, 0, 0, 1.0);
INSERT INTO item_equipment_particle (item_id, particle_count, emit_rate, additive_blend_mode, texture_path, spawner_radius, spawner_offset_x, spawner_offset_y, size_start_min, size_start_max, size_end_min, size_end_max, color_start_min_r, color_start_min_g, color_start_min_b, color_start_min_a, color_start_max_r, color_start_max_g, color_start_max_b, color_start_max_a, color_end_min_r, color_end_min_g, color_end_min_b, color_end_min_a, color_end_max_r, color_end_max_g, color_end_max_b, color_end_max_a, goal_offset_x, goal_offset_y, speed_min, speed_max, time_min, time_max, attract_fraction) values ('eq_sparkling_green', 10, 10, 1, 'res/texture/particles/star.png', 5, 17, 70, 
10, 20, 0, 0,
110, 255, 100, 255,
110, 255, 100, 255,
210, 255, 200, 200,
210, 255, 200, 200,
0, -50,
0, 5,
0.5, 1.0, 0.0);

-- Sparkling Purple Ring

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_sparkling_purple', 'item', 'Sparkling Purple Ring','Funkelnder Violetter Ring','Funkelnde Violette Ring');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_sparkling_purple', 'item_desc', 'The purple stone reinforces the agility of its wearer.','Der blaue Stein verstärkt die Beweglichkeit seines Trägers.','De blau Stei verstärcht d''Beweglichkeit vo sim Träger.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_sparkling_purple', 'eq_ring', 800, 350, 200, 3);
INSERT INTO item_attribute (item_id, haste, critical) values ('eq_sparkling_purple', 10, 3);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_sparkling_purple', '', '');
INSERT INTO item_equipment_light(item_id, light_offset_x, light_offset_y, light_radius_x, light_radius_y, map_light_radius_x, map_light_radius_y, brightness) values ('eq_sparkling_purple', 17, 70, 50, 50, 0, 0, 1.0);
INSERT INTO item_equipment_particle (item_id, particle_count, emit_rate, additive_blend_mode, texture_path, spawner_radius, spawner_offset_x, spawner_offset_y, size_start_min, size_start_max, size_end_min, size_end_max, color_start_min_r, color_start_min_g, color_start_min_b, color_start_min_a, color_start_max_r, color_start_max_g, color_start_max_b, color_start_max_a, color_end_min_r, color_end_min_g, color_end_min_b, color_end_min_a, color_end_max_r, color_end_max_g, color_end_max_b, color_end_max_a, goal_offset_x, goal_offset_y, speed_min, speed_max, time_min, time_max, attract_fraction) values ('eq_sparkling_purple', 10, 10, 1, 'res/texture/particles/star.png', 5, 17, 70, 
10, 20, 0, 0,
170, 100, 255, 255,
170, 100, 255, 255,
230, 200, 255, 200,
230, 200, 255, 200,
0, -50,
0, 5,
0.5, 1.0, 0.0);

-- Sparkling Yellow Ring

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_sparkling_yellow', 'item', 'Sparkling Yellow Ring','Funkelnder Gelber Ring','Funkelnde Gäle Ring');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_sparkling_yellow', 'item_desc', 'The yellow stone reinforces the divine magic of its wearer.','Der gelbe Stein verstärkt die heilige Magie seines Trägers.','De blau Stei verstärcht di heilig Magie vo sim Träger.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_sparkling_yellow', 'eq_ring', 800, 400, 200, 3);
INSERT INTO item_attribute (item_id, dmg_light, res_light) values ('eq_sparkling_yellow', 5, 20);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_sparkling_yellow', '', '');
INSERT INTO item_equipment_light(item_id, light_offset_x, light_offset_y, light_radius_x, light_radius_y, map_light_radius_x, map_light_radius_y, brightness) values ('eq_sparkling_yellow', 17, 70, 50, 50, 0, 0, 1.0);
INSERT INTO item_equipment_particle (item_id, particle_count, emit_rate, additive_blend_mode, texture_path, spawner_radius, spawner_offset_x, spawner_offset_y, size_start_min, size_start_max, size_end_min, size_end_max, color_start_min_r, color_start_min_g, color_start_min_b, color_start_min_a, color_start_max_r, color_start_max_g, color_start_max_b, color_start_max_a, color_end_min_r, color_end_min_g, color_end_min_b, color_end_min_a, color_end_max_r, color_end_max_g, color_end_max_b, color_end_max_a, goal_offset_x, goal_offset_y, speed_min, speed_max, time_min, time_max, attract_fraction) values ('eq_sparkling_yellow', 10, 10, 1, 'res/texture/particles/star.png', 5, 17, 70, 
10, 20, 0, 0,
255, 255, 100, 255,
255, 255, 100, 255,
255, 255, 200, 200,
255, 255, 200, 200,
0, -50,
0, 5,
0.5, 1.0, 0.0);

-- Sparkling Orange Ring

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_sparkling_orange', 'item', 'Sparkling Orange Ring','Funkelnder Oranger Ring','Funkelnde Orange Ring');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_sparkling_orange', 'item_desc', 'The orange stone reinforces the physical strenght of its wearer.','Der blaue Stein verstärkt die physische Stärke seines Trägers.','De blau Stei verstärcht di physisch Stärchi vo sim Träger.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_sparkling_orange', 'eq_ring', 800, 450, 200, 3);
INSERT INTO item_attribute (item_id, dmg_physical, res_physical) values ('eq_sparkling_orange', 5, 20);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_sparkling_orange', '', '');
INSERT INTO item_equipment_light(item_id, light_offset_x, light_offset_y, light_radius_x, light_radius_y, map_light_radius_x, map_light_radius_y, brightness) values ('eq_sparkling_orange', 17, 70, 50, 50, 0, 0, 1.0);
INSERT INTO item_equipment_particle (item_id, particle_count, emit_rate, additive_blend_mode, texture_path, spawner_radius, spawner_offset_x, spawner_offset_y, size_start_min, size_start_max, size_end_min, size_end_max, color_start_min_r, color_start_min_g, color_start_min_b, color_start_min_a, color_start_max_r, color_start_max_g, color_start_max_b, color_start_max_a, color_end_min_r, color_end_min_g, color_end_min_b, color_end_min_a, color_end_max_r, color_end_max_g, color_end_max_b, color_end_max_a, goal_offset_x, goal_offset_y, speed_min, speed_max, time_min, time_max, attract_fraction) values ('eq_sparkling_orange', 10, 10, 1, 'res/texture/particles/star.png', 5, 17, 70, 
10, 20, 0, 0,
255, 170, 100, 255,
255, 170, 100, 255,
255, 230, 200, 200,
255, 230, 200, 200,
0, -50,
0, 5,
0.5, 1.0, 0.0);

-- Sparkling Red Ring

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_sparkling_red', 'item', 'Sparkling Red Ring','Funkelnder Roter Ring','Funkelnde Rote Ring');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_sparkling_red', 'item_desc', 'The red stone reinforces the fire magic of its wearer.','Der rote Stein verstärkt die Feuermagie seines Trägers.','De rot Stei verstärcht d''Füürmagie vo sim Träger.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_sparkling_red', 'eq_ring', 800, 500, 200, 3);
INSERT INTO item_attribute (item_id, dmg_fire, res_fire) values ('eq_sparkling_red', 5, 20);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_sparkling_red', '', '');
INSERT INTO item_equipment_light(item_id, light_offset_x, light_offset_y, light_radius_x, light_radius_y, map_light_radius_x, map_light_radius_y, brightness) values ('eq_sparkling_red', 17, 70, 50, 50, 0, 0, 1.0);
INSERT INTO item_equipment_particle (item_id, particle_count, emit_rate, additive_blend_mode, texture_path, spawner_radius, spawner_offset_x, spawner_offset_y, size_start_min, size_start_max, size_end_min, size_end_max, color_start_min_r, color_start_min_g, color_start_min_b, color_start_min_a, color_start_max_r, color_start_max_g, color_start_max_b, color_start_max_a, color_end_min_r, color_end_min_g, color_end_min_b, color_end_min_a, color_end_max_r, color_end_max_g, color_end_max_b, color_end_max_a, goal_offset_x, goal_offset_y, speed_min, speed_max, time_min, time_max, attract_fraction) values ('eq_sparkling_red', 10, 10, 1, 'res/texture/particles/star.png', 5, 17, 70, 
10, 20, 0, 0,
255, 120, 120, 255,
255, 120, 120, 255,
255, 210, 210, 200,
255, 210, 210, 200,
0, -50,
0, 5,
0.5, 1.0, 0.0);

-- Sparkling Cyan Ring

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_sparkling_cyan', 'item', 'Sparkling Cyan Ring','Funkelnder Cyanfarbener Ring','Funkelnde Cyanfarbige Ring');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_sparkling_cyan', 'item_desc', 'The cyan stone reinforces the ice magic of its wearer.','Der cyanfarbene Stein verstärkt die Eismagie seines Trägers.','De cyanfarbig Stei verstärcht d''Iismagie vo sim Träger.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_sparkling_cyan', 'eq_ring', 800, 550, 200, 3);
INSERT INTO item_attribute (item_id, dmg_ice, res_ice) values ('eq_sparkling_cyan', 5, 20);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_sparkling_cyan', '', '');
INSERT INTO item_equipment_light(item_id, light_offset_x, light_offset_y, light_radius_x, light_radius_y, map_light_radius_x, map_light_radius_y, brightness) values ('eq_sparkling_cyan', 17, 70, 50, 50, 0, 0, 1.0);
INSERT INTO item_equipment_particle (item_id, particle_count, emit_rate, additive_blend_mode, texture_path, spawner_radius, spawner_offset_x, spawner_offset_y, size_start_min, size_start_max, size_end_min, size_end_max, color_start_min_r, color_start_min_g, color_start_min_b, color_start_min_a, color_start_max_r, color_start_max_g, color_start_max_b, color_start_max_a, color_end_min_r, color_end_min_g, color_end_min_b, color_end_min_a, color_end_max_r, color_end_max_g, color_end_max_b, color_end_max_a, goal_offset_x, goal_offset_y, speed_min, speed_max, time_min, time_max, attract_fraction) values ('eq_sparkling_cyan', 10, 10, 1, 'res/texture/particles/star.png', 5, 17, 70, 
10, 20, 0, 0,
100, 255, 255, 255,
100, 255, 255, 255,
200, 255, 255, 200,
200, 255, 255, 200,
0, -50,
0, 5,
0.5, 1.0, 0.0);

-- Sparkling Blue Ring

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_sparkling_blue', 'item', 'Sparkling Blue Ring','Funkelnder Blauer Ring','Funkelnde Blaue Ring');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_sparkling_blue', 'item_desc', 'The blue stone reinforces the shadow magic of its wearer.','Der blaue Stein verstärkt die Schattenmagie seines Trägers.','De blau Stei verstärcht d''Schattemagie vo sim Träger.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_sparkling_blue', 'eq_ring', 800, 600, 200, 3);
INSERT INTO item_attribute (item_id, dmg_shadow, res_shadow) values ('eq_sparkling_blue', 5, 20);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_sparkling_blue', '', '');
INSERT INTO item_equipment_light(item_id, light_offset_x, light_offset_y, light_radius_x, light_radius_y, map_light_radius_x, map_light_radius_y, brightness) values ('eq_sparkling_blue', 17, 70, 50, 50, 0, 0, 1.0);
INSERT INTO item_equipment_particle (item_id, particle_count, emit_rate, additive_blend_mode, texture_path, spawner_radius, spawner_offset_x, spawner_offset_y, size_start_min, size_start_max, size_end_min, size_end_max, color_start_min_r, color_start_min_g, color_start_min_b, color_start_min_a, color_start_max_r, color_start_max_g, color_start_max_b, color_start_max_a, color_end_min_r, color_end_min_g, color_end_min_b, color_end_min_a, color_end_max_r, color_end_max_g, color_end_max_b, color_end_max_a, goal_offset_x, goal_offset_y, speed_min, speed_max, time_min, time_max, attract_fraction) values ('eq_sparkling_blue', 10, 10, 1, 'res/texture/particles/star.png', 5, 17, 70, 
10, 20, 0, 0,
120, 120, 255, 255,
120, 120, 255, 255,
210, 210, 255, 200,
210, 210, 255, 200,
0, -50,
0, 5,
0.5, 1.0, 0.0);

-- Sparkling Rainbow Ring

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_sparkling_rainbow', 'item', 'Sparkling Rainbow Ring','Funkelnder Regenbogen Ring','Funkelnde Rägeboge Ring');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('eq_sparkling_rainbow', 'item_desc', 'To each his own, and I take everything.','Jedem das seine und mir das meiste.','Jedem das seine und mir es meiste.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('eq_sparkling_rainbow', 'eq_ring', 800, 650, 200, 3);
INSERT INTO item_attribute (item_id, max_health, dmg_physical, dmg_fire, dmg_ice, dmg_light, dmg_shadow, res_physical, res_fire, res_ice, res_light, res_shadow, haste) values ('eq_sparkling_rainbow', 5, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('eq_sparkling_rainbow', '', '');
INSERT INTO item_equipment_light(item_id, light_offset_x, light_offset_y, light_radius_x, light_radius_y, map_light_radius_x, map_light_radius_y, brightness) values ('eq_sparkling_rainbow', 17, 70, 50, 50, 0, 0, 1.0);
INSERT INTO item_equipment_particle (item_id, particle_count, emit_rate, additive_blend_mode, texture_path, spawner_radius, spawner_offset_x, spawner_offset_y, size_start_min, size_start_max, size_end_min, size_end_max, color_start_min_r, color_start_min_g, color_start_min_b, color_start_min_a, color_start_max_r, color_start_max_g, color_start_max_b, color_start_max_a, color_end_min_r, color_end_min_g, color_end_min_b, color_end_min_a, color_end_max_r, color_end_max_g, color_end_max_b, color_end_max_a, goal_offset_x, goal_offset_y, speed_min, speed_max, time_min, time_max, attract_fraction) values ('eq_sparkling_rainbow', 10, 10, 1, 'res/texture/particles/star.png', 5, 17, 70, 
10, 20, 0, 0,
100, 100, 100, 255,
200, 200, 200, 255,
200, 200, 200, 200,
255, 255, 255, 200,
0, -50,
0, 5,
0.5, 1.0, 0.0);
