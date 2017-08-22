-- Ice Staff

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('we_icestaff', 'item', 'Ice Staff','Eisstab','Iisstab');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('we_icestaff', 'item_desc', 'Feels ice cold.','Fühlt sich eiskalt an.','So chalt we Iis.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('we_icestaff', 'eq_weapon', 200, 150, 150);
INSERT INTO item_attribute (item_id, max_health, dmg_physical, dmg_ice) values ('we_icestaff', 10, 15, 30);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('we_icestaff', 'res/assets/equipment/weapon/spritesheet_weapon_icestaff.png', 'res/assets/equipment/weapon/spritesheet_map_weapon_icestaff.png');
INSERT INTO item_weapon (item_id, chop_cooldown_ms, chop_offset_left, chop_rect_width, chop_rect_height, chop_damage) values ('we_icestaff', 1000, 10, 45, 80, 8);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_icestaff', 1, 'elemental', 3);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_icestaff', 2, 'elemental', 3);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_icestaff', 3, 'divine', 3);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_icestaff', 4, 'divine', 3);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_icestaff', 5, 'necromancy', 3);

-- Fire Staff

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('we_firestaff', 'item', 'Fire Staff','Feuerstab','Füürstab');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('we_firestaff', 'item_desc', 'The ruby stone reflects different shades of red.','Der grosse Rubinkopf leuchtet in warmen Rottönen.', 'De gross Rubinchopf lüüchtet in warme Rottön.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('we_firestaff', 'eq_weapon', 50, 150, 80);
INSERT INTO item_attribute (item_id, max_health, dmg_physical, dmg_fire) values ('we_firestaff', 10, 5, 8);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('we_firestaff', 'res/assets/equipment/weapon/spritesheet_weapon_firestaff.png', 'res/assets/equipment/weapon/spritesheet_map_weapon_firestaff.png');
INSERT INTO item_weapon (item_id, chop_cooldown_ms, chop_offset_left, chop_rect_width, chop_rect_height, chop_damage) values ('we_firestaff', 1000, 10, 45, 80, 8);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_firestaff', 1, 'elemental', 2);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_firestaff', 2, 'elemental', 2);

-- Tear Staff

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('we_tearstaff', 'item', 'Tear Staff','Tränenstab','Tränestab');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('we_tearstaff', 'item_desc', 'A tear shaped aquamarine decorates the head of this staff.','Ein tränenförmiger Aquamarin ziert den Kopf dieses Stabes.', 'En träneförmige Aquamarin ziert de Chopf vo dem Stab.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('we_tearstaff', 'eq_weapon', 550, 350, 150);
INSERT INTO item_attribute (item_id, max_health, dmg_physical, dmg_ice) values ('we_tearstaff', 20, 5, 10);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('we_tearstaff', 'res/assets/equipment/weapon/spritesheet_weapon_tearstaff.png', 'res/assets/equipment/weapon/spritesheet_map_weapon_tearstaff.png');
INSERT INTO item_weapon (item_id, chop_cooldown_ms, chop_offset_left, chop_rect_width, chop_rect_height, chop_damage) values ('we_tearstaff', 1000, 10, 45, 80, 10);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_tearstaff', 1, 'elemental', 2);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_tearstaff', 2, 'elemental', 2);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_tearstaff', 3, 'meta', 1);

-- Pearl Sabre

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('we_pearlsabre', 'item', 'Pearl Sabre','Perlensäbel','Perlesäbel');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('we_pearlsabre', 'item_desc', 'Its nacre blade shines dimly.','Seine aus Perlmutt gefertigte Klinge glimmt schwach.', 'Sini us Perlmutt gmachti Klinge glimmt schwach.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('we_pearlsabre', 'eq_weapon', 350, 650, 70);
INSERT INTO item_attribute (item_id, max_health, critical, dmg_physical, dmg_ice, dmg_light) values ('we_pearlsabre', 5, 2, 2, 3, 4);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('we_pearlsabre', 'res/assets/equipment/weapon/spritesheet_weapon_pearlsabre.png', 'res/assets/equipment/weapon/spritesheet_map_weapon_pearlsabre.png');
INSERT INTO item_weapon (item_id, chop_cooldown_ms, chop_offset_left, chop_rect_width, chop_rect_height, chop_damage) values ('we_pearlsabre', 600, 10, 40, 80, 12);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_pearlsabre', 1, 'elemental', 2);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_pearlsabre', 2, 'meta', 2);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_pearlsabre', 3, 'meta', 2);
INSERT INTO item_equipment_light(item_id, light_offset_x, light_offset_y, light_radius_x, light_radius_y, map_light_radius_x, map_light_radius_y, brightness) values ('we_pearlsabre', 20, 30, 100, 150, 80, 100, 0.2);

-- Lightning Pike

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('we_lightningpike', 'item', 'Lightning Pike','Blitzlanze','Blitzlanze');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('we_lightningpike', 'item_desc', 'Made from the tail of a storm bird.','Aus dem Schwanz eines Sturmvogels gefertigt.', 'Usem Schwanz vomne Sturmvogel gmacht.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('we_lightningpike', 'eq_weapon', 650, 0, 100);
INSERT INTO item_attribute (item_id, max_health, critical, dmg_physical, dmg_light) values ('we_lightningpike', 8, 4, 4, 8);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('we_lightningpike', 'res/assets/equipment/weapon/spritesheet_weapon_lightningpike.png', 'res/assets/equipment/weapon/spritesheet_map_weapon_lightningpike.png');
INSERT INTO item_weapon (item_id, chop_cooldown_ms, chop_offset_left, chop_rect_width, chop_rect_height, chop_damage) values ('we_lightningpike', 1000, 10, 45, 80, 14);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_lightningpike', 1, 'elemental', 3);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_lightningpike', 2, 'elemental', 2);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_lightningpike', 3, 'meta', 2);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_lightningpike', 4, 'meta', 2);
INSERT INTO item_equipment_light(item_id, light_offset_x, light_offset_y, light_radius_x, light_radius_y, map_light_radius_x, map_light_radius_y, brightness) values ('we_lightningpike', 30, 20, 100, 100, 80, 100, 0.3);

-- Sickle Staff

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('we_sicklestaff', 'item','Sickle Staff', 'Sichelstab','Sichlestab');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('we_sicklestaff', 'item_desc', 'The ruby gem glows and concentrates elemental energy.','Der rubinfarbene Edelstein glimmt und bündelt Elementarenergie.','De rubinfarbig Stei lüchtet schwach und bündlet Elementarenergie.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('we_sicklestaff', 'eq_weapon', 250, 150, 300);
INSERT INTO item_attribute (item_id, max_health, dmg_physical, dmg_fire, dmg_ice) values ('we_sicklestaff', 10, 15, 15, 15);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('we_sicklestaff', 'res/assets/equipment/weapon/spritesheet_weapon_sicklestaff.png', 'res/assets/equipment/weapon/spritesheet_map_weapon_sicklestaff.png');
INSERT INTO item_weapon (item_id, chop_cooldown_ms, chop_offset_left, chop_rect_width, chop_rect_height, chop_damage) values ('we_sicklestaff', 1000, 10, 45, 80, 10);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_sicklestaff', 1, 'elemental', 3);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_sicklestaff', 2, 'elemental', 3);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_sicklestaff', 3, 'elemental', 3);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_sicklestaff', 4, 'elemental', 3);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_sicklestaff', 5, 'elemental', 3);

-- Claw

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('we_claw', 'item','Iron Claw', 'Eisenklaue','Isechlaue');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('we_claw', 'item_desc', 'The curved sharp blades and its range create a terrible weapon.','Die gebogenen scharfen Klingen und ihre Reichweite machen sie zu einer fürchterlichen Waffe.','Di bogene scharfe Klinge und iri Riichwiiti mached si zunere fürchterliche Waffe.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('we_claw', 'eq_weapon', 250, 250, 100);
INSERT INTO item_attribute (item_id, max_health, dmg_physical, dmg_shadow, dmg_ice) values ('we_claw', 10, 8, 5, 5);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('we_claw', 'res/assets/equipment/weapon/spritesheet_weapon_claw.png', 'res/assets/equipment/weapon/spritesheet_map_weapon_claw.png');
INSERT INTO item_weapon (item_id, chop_cooldown_ms, chop_offset_left, chop_rect_width, chop_rect_height, chop_damage) values ('we_claw', 800, 10, 45, 80, 10);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_claw', 1, 'elemental', 1);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_claw', 2, 'elemental', 2);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_claw', 3, 'necromancy', 3);

-- Enchanted Dagger

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('we_enchanteddagger', 'item','Enchanted Dagger', 'Verzauberter Dolch','Verzauberete Dolch');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('we_enchanteddagger', 'item_desc', 'This dagger was smuggled into the prison of Gandria.','Dieser Dolch wurde in das Gefängnis von Gandria geschmuggelt.','De Dolch isch is Gfängnis vo Gandria gschmugglet worde.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('we_enchanteddagger', 'eq_weapon', 550, 500, 40);
INSERT INTO item_attribute (item_id, max_health, critical) values ('we_enchanteddagger', 5, 8);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('we_enchanteddagger', 'res/assets/equipment/weapon/spritesheet_weapon_enchanteddagger.png', 'res/assets/equipment/weapon/spritesheet_map_weapon_enchanteddagger.png');
INSERT INTO item_weapon (item_id, chop_cooldown_ms, chop_offset_left, chop_rect_width, chop_rect_height, chop_damage) values ('we_enchanteddagger', 500, 10, 35, 70, 8);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_enchanteddagger', 1, 'elemental', 2);

-- Inconspicuous Dagger

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('we_inconspicuousdagger', 'item','Inconspicuous Dagger', 'Unscheinbarer Dolch','Unschiinbare Dolch');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('we_inconspicuousdagger', 'item_desc', 'A very small blade, but something about it seems to be magical.','Eine sehr kleine Klinge, doch etwas an ihm scheint magisch.','E sehr chlini Klinge, aber er het öppis magisches.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('we_inconspicuousdagger', 'eq_weapon', 600, 200, 30);
INSERT INTO item_attribute (item_id, dmg_shadow, critical) values ('we_inconspicuousdagger', 3, 8);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('we_inconspicuousdagger', 'res/assets/equipment/weapon/spritesheet_weapon_inconspicuousdagger.png', 'res/assets/equipment/weapon/spritesheet_map_weapon_inconspicuousdagger.png');
INSERT INTO item_weapon (item_id, chop_cooldown_ms, chop_offset_left, chop_rect_width, chop_rect_height, chop_damage) values ('we_inconspicuousdagger', 500, 10, 30, 70, 7);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_inconspicuousdagger', 1, 'twilight', 1);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('we_inconspicuousdagger', -14, -16, 17, 34);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('we_inconspicuousdagger', 1, 0, 200, 50, 50);

-- Herbalist's Blade

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('we_herbalistblade', 'item','Herbalist''s Blade', 'Klinge der Kräuterhexe','Krütlihäx-Klinge');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('we_herbalistblade', 'item_desc', 'This sickle could be used to collect herbs, but it makes also a fine weapon.','Diese Sichel könnte zum Kräutersammeln verwendet werden, aber sie gibt auch eine gute Waffe ab.','Die Sichle chöntme zum Krütli sammle bruche, aber si git au e gueti Waffe ab.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('we_herbalistblade', 'eq_weapon', 600, 300, 30);
INSERT INTO item_attribute (item_id, dmg_shadow, dmg_ice, critical) values ('we_herbalistblade', 2, 2, 7);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('we_herbalistblade', 'res/assets/equipment/weapon/spritesheet_weapon_herbalistblade.png', 'res/assets/equipment/weapon/spritesheet_map_weapon_herbalistblade.png');
INSERT INTO item_weapon (item_id, chop_cooldown_ms, chop_offset_left, chop_rect_width, chop_rect_height, chop_damage) values ('we_herbalistblade', 800, 10, 40, 70, 8);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_herbalistblade', 1, 'necromancy', 2);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_herbalistblade', 2, 'elemental', 1);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('we_herbalistblade', 0, 0, 39, 57);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('we_herbalistblade', 1, 200, 300, 50, 100);

-- Yasha''s Claws

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('we_doublesickle', 'item','Yasha''s Claws', 'Yasha''s Klauen','Yasha''s Klaue');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('we_doublesickle', 'item_desc', 'The demon Yasha gifted me this weapon after I defeated her in battle.','Yasha hat mir diese Waffe überlassen nachdem ich sie im Kampf besiegt habe.','D''Yasha het mer die Waffe ge nachdem ich si im Kampf besiegt ha.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('we_doublesickle', 'eq_weapon', 600, 350, 300);
INSERT INTO item_attribute (item_id, dmg_physical, dmg_shadow, dmg_ice, dmg_fire, critical) values ('we_doublesickle', 5, 5, 5, 10, 10);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('we_doublesickle', 'res/assets/equipment/weapon/spritesheet_weapon_doublesickle.png', 'res/assets/equipment/weapon/spritesheet_map_weapon_doublesickle.png');
INSERT INTO item_weapon (item_id, chop_cooldown_ms, chop_rect_width, chop_rect_height, chop_offset_left, chop_damage) values ('we_doublesickle', 350, 90, 80, -45, 20);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_doublesickle', 1, 'elemental', 3);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_doublesickle', 2, 'elemental', 3);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_doublesickle', 3, 'meta', 2);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_doublesickle', 4, 'meta', 2);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_doublesickle', 5, 'meta', 2);

-- Pickaxe

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('we_pickaxe', 'item','Pickaxe', 'Spitzhacke','Pickel');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('we_pickaxe', 'item_desc', 'Is used to mine ore.','Wird gebraucht, um Erz abzubauen.','Da demit chame Erz abbaue.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('we_pickaxe', 'eq_weapon', 50, 550, 25);
INSERT INTO item_attribute (item_id, max_health, dmg_physical, critical) values ('we_pickaxe', 3, 1, 1);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('we_pickaxe', 'res/assets/equipment/weapon/spritesheet_weapon_pickaxe.png', 'res/assets/equipment/weapon/spritesheet_map_weapon_pickaxe.png');
INSERT INTO item_weapon (item_id, chop_cooldown_ms, chop_offset_left, chop_rect_width, chop_rect_height, chop_damage) values ('we_pickaxe', 1000, 10, 35, 70, 5);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('we_pickaxe', 0, -15, 30, 35);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('we_pickaxe', 1, 500, 250, 50, 50);

-- Oathbreaker (Thief Weapon)

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('we_oathbreaker', 'item', 'Oathbreaker','Eidbrecher','Eidbrecher');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('we_oathbreaker', 'item_desc', 'The legendary twin-daggers Oath and Breaker. Some say that the cyan stones hold captured souls.','Die legendären Zwillingsdolche Eid und Brecher. Es wird erzählt, dass die zyanblauen Steine gefangene Seelen beherrbergen.','Di legendäre Zwillingsdolch Eid und Brecher. Me munklet dass ide zyanblaue Stei Seele gfange sind.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('we_oathbreaker', 'eq_weapon', 150, 100, 500);
INSERT INTO item_attribute (item_id, dmg_physical, dmg_shadow, dmg_ice, critical, haste) values ('we_oathbreaker', 10, 10, 10, 15, 20);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('we_oathbreaker', 'res/assets/equipment/weapon/spritesheet_weapon_oathbreaker.png', 'res/assets/equipment/weapon/spritesheet_map_weapon_oathbreaker.png');
INSERT INTO item_weapon (item_id, chop_cooldown_ms, chop_rect_width, chop_rect_height, chop_offset_left, chop_damage) values ('we_oathbreaker', 350, 80, 80, -40, 20);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_oathbreaker', 1, 'elemental', 2);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_oathbreaker', 2, 'elemental', 2);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_oathbreaker', 3, 'twilight', 3);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_oathbreaker', 4, 'twilight', 3);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_oathbreaker', 5, 'twilight', 3);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('we_oathbreaker', -7, -30, 50, 24);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('we_oathbreaker', 1, 200, 100, 50, 100);

-- Pike of Light (Cleric Weapon)

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('we_pikeoflight', 'item','Pike of the Eternal Light', 'Pike des Ewigen Lichts','Lanze vom Ewige Liecht');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('we_pikeoflight', 'item_desc', 'This glorious weapon is only wielded by true warriors of the Eternal Light.','Diese prächtige Waffe wird nur von wahren Kriegern des Ewigen Lichts geführt.','Die prächtig Waffe wird nur vo wohre Chrieger vom Ewige Liecht gführt.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('we_pikeoflight', 'eq_weapon', 300, 250, 500);
INSERT INTO item_attribute (item_id, max_health, dmg_physical, dmg_light, dmg_fire, haste) values ('we_pikeoflight', 20, 5, 15, 15, 10);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('we_pikeoflight', 'res/assets/equipment/weapon/spritesheet_weapon_pikeoflight.png', 'res/assets/equipment/weapon/spritesheet_map_weapon_pikeoflight.png');
INSERT INTO item_weapon (item_id, chop_cooldown_ms, chop_offset_left, chop_rect_width, chop_rect_height, chop_damage) values ('we_pikeoflight', 1500, 10, 45, 80, 15);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_pikeoflight', 1, 'elemental', 2);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_pikeoflight', 2, 'elemental', 2);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_pikeoflight', 3, 'divine', 3);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_pikeoflight', 4, 'divine', 3);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_pikeoflight', 5, 'divine', 3);

-- Bone Scythe (Necromancer Weapon)

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('we_bonescythe', 'item','Bone Scythe', 'Knochensense','Chnochesense');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('we_bonescythe', 'item_desc', 'Perfect for reaping souls.','Perfekt, um Seelen zu sammeln.','Perfekt zum Seele sammle.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('we_bonescythe', 'eq_weapon', 700, 600, 500);
INSERT INTO item_attribute (item_id, max_health, dmg_ice, dmg_shadow, dmg_physical) values ('we_bonescythe', 30, 5, 15, 15);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('we_bonescythe', 'res/assets/equipment/weapon/spritesheet_weapon_bonescythe.png', 'res/assets/equipment/weapon/spritesheet_map_weapon_bonescythe.png');
INSERT INTO item_weapon (item_id, chop_cooldown_ms, chop_offset_left, chop_rect_width, chop_rect_height, chop_damage) values ('we_bonescythe', 1500, 10, 45, 80, 15);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_bonescythe', 1, 'elemental', 2);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_bonescythe', 2, 'elemental', 2);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_bonescythe', 3, 'necromancy', 3);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_bonescythe', 4, 'necromancy', 3);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_bonescythe', 5, 'necromancy', 3);

-- Hawthorn

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('we_hawthorn', 'item','Hawthorn', 'Weissdorn','Wiisdorn');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('we_hawthorn', 'item_desc', 'This staff grew on a magic tree but was broken off before it reached its full magical power.','Dieser Stab wuchs an einem magischen Baum und wurde abgebrochen bevor er seine volle magische Kraft entfaltete.','De Stab isch amne magische Bäumli gwachse und isch abbroche worde bevor er sini voll magisch Kraft entfaltet het.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('we_hawthorn', 'eq_weapon', 300, 100, 50);
INSERT INTO item_attribute (item_id, max_health, dmg_fire, dmg_ice) values ('we_hawthorn', 10, 2, 2);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('we_hawthorn', 'res/assets/equipment/weapon/spritesheet_weapon_hawthorn.png', 'res/assets/equipment/weapon/spritesheet_map_weapon_hawthorn.png');
INSERT INTO item_weapon (item_id, chop_cooldown_ms, chop_offset_left, chop_rect_width, chop_rect_height, chop_damage) values ('we_hawthorn', 1200, 10, 45, 80, 5);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_hawthorn', 1, 'elemental', 1);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('we_hawthorn', 0, -10, 40, 80);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('we_hawthorn', 1, 250, 150, 50, 100);

-- Rusty Sword

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('we_rustysword', 'item', 'Rusty sword','Rostiges Schwert','Rostigs Schwert');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('we_rustysword', 'item_desc', 'This sword has seen better days.','Das Schwert hat schon bessere Tage gesehen.','Da Schwert het glaub scho besseri Täg gse.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('we_rustysword', 'eq_weapon', 150, 150, 12);
INSERT INTO item_attribute (item_id, dmg_physical) values ('we_rustysword', 2);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('we_rustysword', 'res/assets/equipment/weapon/spritesheet_weapon_rustysword.png', 'res/assets/equipment/weapon/spritesheet_map_weapon_rustysword.png');
INSERT INTO item_weapon (item_id, chop_cooldown_ms, chop_offset_left, chop_rect_width, chop_rect_height, chop_damage) values ('we_rustysword', 800, 10, 40, 80, 5);

-- Simple Sword

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('we_simplesword', 'item','Simple Sword', 'Einfaches Schwert','Eifachs Schwert');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('we_simplesword', 'item_desc', 'There''s nothing special about this sword.','Ein ganz normales Schwert.','Es hundskomuns Schwert.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('we_simplesword', 'eq_weapon', 200, 250, 30);
INSERT INTO item_attribute (item_id, dmg_physical) values ('we_simplesword', 5);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('we_simplesword', 'res/assets/equipment/weapon/spritesheet_weapon_simplesword.png', 'res/assets/equipment/weapon/spritesheet_map_weapon_simplesword.png');
INSERT INTO item_weapon (item_id, chop_cooldown_ms, chop_offset_left, chop_rect_width, chop_rect_height, chop_damage) values ('we_simplesword', 700, 10, 40, 80, 7);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('we_simplesword', -25, -40, 25, 60);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('we_simplesword', 1, 200, 150, 50, 100);

-- Fire Sword

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('we_firesword', 'item','Fire Sword', 'Feuerschwert','Füürschwert');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('we_firesword', 'item_desc', 'A holy relic from Yasha''s Temple.','Ein heiliges Relikt aus Yasha''s Tempel','Es heiligs Relikt us de Yasha ihrem Tempel.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('we_firesword', 'eq_weapon', 700, 400, 300);
INSERT INTO item_attribute (item_id, dmg_fire, dmg_light, critical) values ('we_firesword', 12, 8, 4);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('we_firesword', 'res/assets/equipment/weapon/spritesheet_weapon_firesword.png', 'res/assets/equipment/weapon/spritesheet_map_weapon_firesword.png');
INSERT INTO item_weapon (item_id, chop_cooldown_ms, chop_offset_left, chop_rect_width, chop_rect_height, chop_damage) values ('we_firesword', 700, 10, 40, 80, 12);
INSERT INTO item_equipment_particle (item_id, particle_count, emit_rate, additive_blend_mode, texture_path, spawner_radius, spawner_offset_x, spawner_offset_y, size_start_min, size_start_max, size_end_min, size_end_max, color_start_min_r, color_start_min_g, color_start_min_b, color_start_min_a, color_start_max_r, color_start_max_g, color_start_max_b, color_start_max_a, color_end_min_r, color_end_min_g, color_end_min_b, color_end_min_a, color_end_max_r, color_end_max_g, color_end_max_b, color_end_max_a, goal_offset_x, goal_offset_y, speed_min, speed_max, time_min, time_max) values ('we_firesword', 40, 20, 1, 'res/assets/particles/flame.png', 5, 20, 55, 
10, 20, 30, 40,
255, 160, 64, 255,
255, 160, 64, 255,
255, 0, 0, 200,
255, 0, 0, 200,
60, -100,
70, 100,
0.4, 0.8);
INSERT INTO item_equipment_light(item_id, light_offset_x, light_offset_y, light_radius_x, light_radius_y, map_light_radius_x, map_light_radius_y, brightness) values ('we_firesword', 30, 30, 300, 400, 100, 120, 0.5);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_firesword', 1, 'elemental', 3);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_firesword', 2, 'elemental', 2);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_firesword', 3, 'elemental', 2);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_firesword', 4, 'meta', 3);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_firesword', 5, 'meta', 2);

-- Zeff's Skinning Knife

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('we_zeffssword', 'item', 'Zeff''s Skinning Knife','Zeffs Kürschnermesser','Zeffs Kürschnermesser');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('we_zeffssword', 'item_desc', 'This oversized knife belonged to the leatherworker Zeff.','Dieses übergrosse Messer gehörte dem Lederer Zeff.','Da übergrosse Messer het em Lederer Zeff ghört.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('we_zeffssword', 'eq_weapon', 350, 350, 70);
INSERT INTO item_attribute (item_id, dmg_physical, critical) values ('we_zeffssword', 7, 5);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('we_zeffssword', 'res/assets/equipment/weapon/spritesheet_weapon_zeffssword.png', 'res/assets/equipment/weapon/spritesheet_map_weapon_zeffssword.png');
INSERT INTO item_weapon (item_id, chop_cooldown_ms, chop_offset_left, chop_rect_width, chop_rect_height, chop_damage) values ('we_zeffssword', 1000, 10, 50, 80, 15);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_zeffssword', 1, 'elemental', 2);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_zeffssword', 2, 'meta', 1);

-- Leeroy's Sword

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('we_leeroysword', 'item', 'Leeroy''s Sword','Leeroy''s Schwert','Leeroy''s Schwertli');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('we_leeroysword', 'item_desc', 'The sword of a legend... or merely a fool?','Das Schwert einer Legende... oder bloss eines Narren?','Es Schwert vonere Legende... oder eifach vomne Tubel?');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('we_leeroysword', 'eq_weapon', 650, 600, 80);
INSERT INTO item_attribute (item_id, dmg_physical, critical) values ('we_leeroysword', 10, 10);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('we_leeroysword', 'res/assets/equipment/weapon/spritesheet_weapon_leeroysword.png', 'res/assets/equipment/weapon/spritesheet_map_weapon_leeroysword.png');
INSERT INTO item_weapon (item_id, chop_cooldown_ms, chop_offset_left, chop_rect_width, chop_rect_height, chop_damage) values ('we_leeroysword', 1000, 10, 50, 80, 15);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_leeroysword', 1, 'meta', 2);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_leeroysword', 2, 'meta', 2);

-- Mace of the Order

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('we_ordermace', 'item', 'Mace of the Order','Streitkolben des Ordens','Striitcholbe vom Orde');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('we_ordermace', 'item_desc', 'This weapon is mostly used by paladins of the Order.','Diese Waffe wird meistens von den Paladinen des Ordens getragen.', 'Die Waffe wird meistens vo de Paladin vom Orde treit.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('we_ordermace', 'eq_weapon', 400, 350, 50);
INSERT INTO item_attribute (item_id, dmg_physical, dmg_light, critical) values ('we_ordermace', 2, 3, 5);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('we_ordermace', 'res/assets/equipment/weapon/spritesheet_weapon_ordermace.png', 'res/assets/equipment/weapon/spritesheet_map_weapon_ordermace.png');
INSERT INTO item_weapon (item_id, chop_cooldown_ms, chop_offset_left, chop_rect_width, chop_rect_height, chop_damage) values ('we_ordermace', 1000, 10, 35, 70, 8);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_ordermace', 1, 'divine', 2);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_ordermace', 2, 'elemental', 1);

-- Walking Pole

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('we_walkingpole', 'item','Walking Pole', 'Wanderstab','Wanderstock');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('we_walkingpole', 'item_desc', 'A simple walking pole, it belonged to the hunter Edmond.','Ein einfacher Wanderstock, er gehörte dem Jäger Edmond.','En eifache Wanderstock, er het em Jäger Edmond ghört.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('we_walkingpole', 'eq_weapon', 250, 100, 15);
INSERT INTO item_attribute (item_id, max_health) values ('we_walkingpole', 10);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('we_walkingpole', 'res/assets/equipment/weapon/spritesheet_weapon_walkingpole.png', 'res/assets/equipment/weapon/spritesheet_map_weapon_walkingpole.png');
INSERT INTO item_weapon (item_id, chop_cooldown_ms, chop_offset_left, chop_rect_width, chop_rect_height, chop_damage) values ('we_walkingpole', 800, 10, 40, 80, 5);

-- Fire Glaive

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('we_fireglaive', 'item', 'Fire Glaive','Feuergleve','Füür Gleve');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('we_fireglaive', 'item_desc', 'A very powerful weapon, crafted from Yasha''s fangs.','Eine sehr mächtige Waffe, geschmieded aus Yasha''s Fangzähnen.','E sehr mächtigi Waffe, gschmieded us de Fangzäh vode Yasha.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('we_fireglaive', 'eq_weapon', 700, 200, 300);
INSERT INTO item_attribute (item_id, dmg_physical, dmg_fire, critical, haste) values ('we_fireglaive', 10, 20, 15, 10);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('we_fireglaive', 'res/assets/equipment/weapon/spritesheet_weapon_fireglaive.png', 'res/assets/equipment/weapon/spritesheet_map_weapon_fireglaive.png');
INSERT INTO item_weapon (item_id, chop_cooldown_ms, chop_rect_width, chop_rect_height, chop_offset_left, chop_damage) values ('we_fireglaive', 500, 80, 80, -40, 20);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_fireglaive', 1, 'elemental', 3);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_fireglaive', 2, 'elemental', 2);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_fireglaive', 3, 'elemental', 2);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_fireglaive', 4, 'meta', 2);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_fireglaive', 5, 'meta', 2);

-- Jason's Glaive

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('we_jasonsglaive', 'item', 'Jason''s Glaive','Jasons Gleve','Jasons Gleve');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('we_jasonsglaive', 'item_desc', 'If you open something, you have to close it again eventually.','Wenn man etwas öffnet, muss man es irgendwann auch wieder schliessen.','Wenn me öppis ufmacht munmes irgendwenn au wider zuemache.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('we_jasonsglaive', 'eq_weapon', 300, 150, 300);
INSERT INTO item_attribute (item_id, dmg_physical, dmg_ice, critical, haste) values ('we_jasonsglaive', 20, 10, 15, 10);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('we_jasonsglaive', 'res/assets/equipment/weapon/spritesheet_weapon_jasonsglaive.png', 'res/assets/equipment/weapon/spritesheet_map_weapon_jasonsglaive.png');
INSERT INTO item_weapon (item_id, chop_cooldown_ms, chop_rect_width, chop_rect_height, chop_offset_left, chop_damage) values ('we_jasonsglaive', 500, 80, 80, -40, 20);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_jasonsglaive', 1, 'elemental', 3);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_jasonsglaive', 2, 'divine', 3);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_jasonsglaive', 3, 'necromancy', 3);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_jasonsglaive', 4, 'twilight', 3);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_jasonsglaive', 5, 'divine', 3);

-- Alastor's Sickles

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('we_janusglaive', 'item', 'Alastor''s Sickles','Alastor''s Sicheln','Sichle vom Alastor');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('we_janusglaive', 'item_desc', 'This weapon was once used against me, now it''s mine.','Diese Waffe wurde einst gegen mich verwendet, jetzt gehört sie mir.','Die Waffe isch mal gege mich brucht worde, jetz ghört si mir.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('we_janusglaive', 'eq_weapon', 650, 150, 200);
INSERT INTO item_attribute (item_id, dmg_physical, dmg_fire, dmg_ice, critical, haste) values ('we_janusglaive', 5, 8, 8, 3, 10);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path) values ('we_janusglaive', 'res/assets/equipment/weapon/spritesheet_weapon_janusglaive.png', 'res/assets/equipment/weapon/spritesheet_map_weapon_janusglaive.png');
INSERT INTO item_weapon (item_id, chop_cooldown_ms, chop_rect_width, chop_rect_height, chop_offset_left, chop_damage) values ('we_janusglaive', 500, 80, 80, -40, 15);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_janusglaive', 1, 'elemental', 3);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_janusglaive', 2, 'elemental', 2);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_janusglaive', 3, 'meta', 2);
INSERT INTO item_weapon_slot(item_id, slot_nr, slot_type, modifier_count) values ('we_janusglaive', 4, 'meta', 2);

-- Torch

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('we_torch', 'item', 'Torch', 'Fackel', 'Fackle');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('we_torch', 'item_desc', 'Lights the way.','Spendet Licht.','Git Liecht.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('we_torch', 'eq_weapon', 200, 100, 20);
INSERT INTO item_attribute (item_id, dmg_fire) values ('we_torch', 10);
INSERT INTO item_equipment (item_id, texture_path, map_texture_path, frames_idle, frames_jump, frames_climb1, frames_climb2) values ('we_torch', 'res/assets/equipment/weapon/spritesheet_weapon_torch.png', 'res/assets/equipment/weapon/spritesheet_map_weapon_torch.png', 2, 2, 2, 2);
INSERT INTO item_weapon (item_id, chop_cooldown_ms, chop_offset_left, chop_rect_width, chop_rect_height, chop_damage) values ('we_torch', 600, 10, 40, 80, 10);
INSERT INTO item_equipment_light(item_id, light_offset_x, light_offset_y, light_radius_x, light_radius_y, map_light_radius_x, map_light_radius_y) values ('we_torch', 30, 30, 300, 400, 100, 120);
