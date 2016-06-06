-- Cheese

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('fo_cheese', 'consumable', 0, 100, 3);
INSERT INTO item_attribute (item_id, health_regeneration, haste) values ('fo_cheese', 2, 5);
INSERT INTO item_food (item_id, food_duration_s) values ('fo_cheese', 10);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('fo_cheese', -14, -28, 22, 22);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('fo_cheese', 1, 0, 0, 50, 50);

-- Bread

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('fo_bread', 'consumable', 50, 100, 2);
INSERT INTO item_attribute (item_id, health_regeneration) values ('fo_bread', 2);
INSERT INTO item_food (item_id, food_duration_s) values ('fo_bread', 15);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('fo_bread', -10, -30, 30, 20);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('fo_bread', 1, 50, 100, 50, 50);

-- Water

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('fo_water', 'consumable', 100, 0, 2);
INSERT INTO item_attribute (item_id, health_regeneration,  dmg_ice, res_fire) values ('fo_water', 2, 5, 5);
INSERT INTO item_food (item_id, food_duration_s) values ('fo_water', 5);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('fo_water', -16, -20, 18, 30);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('fo_water', 1, 200, 50, 50, 50);

-- Apple

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('fo_apple', 'consumable', 300, 50, 2);
INSERT INTO item_attribute (item_id, health_regeneration, critical, res_physical, res_shadow) values ('fo_apple', 3, 3, 5, 5);
INSERT INTO item_food (item_id, food_duration_s) values ('fo_apple', 6);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('fo_apple', -16, -20, 18, 30);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('fo_apple', 1, 0, 150, 50, 50);

-- Cave Berries

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('fo_caveberry', 'consumable', 500, 0, 3);
INSERT INTO item_attribute (item_id, health_regeneration, critical, dmg_shadow, res_light) values ('fo_caveberry', -1, 5, 5, 3);
INSERT INTO item_food (item_id, food_duration_s) values ('fo_caveberry', 20);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('fo_caveberry', 0, -35, 50, 70);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('fo_caveberry', 1, 0, 0, 50, 100);

-- Milk

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('fo_milk', 'consumable', 500, 50, 6);
INSERT INTO item_attribute (item_id, health_regeneration, critical, dmg_ice, dmg_physical) values ('fo_milk', 5, 2, 5, 3);
INSERT INTO item_food (item_id, food_duration_s) values ('fo_milk', 20);

-- Physalis

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('fo_physalis', 'consumable', 200, 200, 5);
INSERT INTO item_attribute (item_id, health_regeneration, dmg_shadow, dmg_physical, dmg_light) values ('fo_physalis', -2, 7, 7, 7);
INSERT INTO item_food (item_id, food_duration_s) values ('fo_physalis', 20);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('fo_physalis', 0, -20, 50, 45);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('fo_physalis', 1, 250, 0, 50, 100);

-- Glowing Shroom

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('fo_glowingshroom', 'consumable', 150, 50, 2);
INSERT INTO item_attribute (item_id, health_regeneration, dmg_light, res_shadow) values ('fo_glowingshroom', 1, 5, 5);
INSERT INTO item_food (item_id, food_duration_s) values ('fo_glowingshroom', 15);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('fo_glowingshroom', -10, -20, 30, 30);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('fo_glowingshroom', 1, 100, 50, 50, 50);

-- Healing Herb

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('fo_healingherb', 'consumable', 50, 50, 5);
INSERT INTO item_attribute (item_id, health_regeneration) values ('fo_healingherb', 5);
INSERT INTO item_food (item_id, food_duration_s) values ('fo_healingherb', 10);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('fo_healingherb', -10, 0, 30, 50);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('fo_healingherb', 1, 50, 50, 50, 50);

-- Lesser Healing Potion

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('fo_lesserhealingpotion', 'consumable', 100, 100, 15);
INSERT INTO item_attribute (item_id, health_regeneration) values ('fo_lesserhealingpotion', 10);
INSERT INTO item_food (item_id, food_duration_s) values ('fo_lesserhealingpotion', 5);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('fo_lesserhealingpotion', -15, -4, 20, 46);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('fo_lesserhealingpotion', 1, 100, 100, 50, 50);

-- Roasted Meat

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('fo_roastedmeat', 'consumable', 250, 50, 4);
INSERT INTO item_attribute (item_id, health_regeneration, dmg_physical) values ('fo_roastedmeat', 10, 7);
INSERT INTO item_food (item_id, food_duration_s) values ('fo_roastedmeat', 10);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('fo_roastedmeat', -7, -26, 40, 24);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('fo_roastedmeat', 1, 150, 150, 50, 50);

-- Raw Meat

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('fo_rawmeat', 'consumable', 200, 50, 2);
INSERT INTO item_attribute (item_id, health_regeneration, dmg_physical) values ('fo_rawmeat', 5, 5);
INSERT INTO item_food (item_id, food_duration_s, is_cookable, cooked_item_id) values ('fo_rawmeat', 10, 1, 'fo_roastedmeat');
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('fo_rawmeat', -7, -26, 40, 24);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('fo_rawmeat', 1, 150, 100, 50, 50);

-- Raw Fish

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('fo_rawfish', 'consumable', 400, 0, 3);
INSERT INTO item_attribute (item_id, health_regeneration, dmg_ice) values ('fo_rawfish', 5, 5);
INSERT INTO item_food (item_id, food_duration_s, is_cookable, cooked_item_id) values ('fo_rawfish', 6, 1, 'fo_fishsoup');
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('fo_rawfish', 0, -30, 50, 35);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('fo_rawfish', 1, 50, 200, 50, 100);

-- Fish Soup

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('fo_fishsoup', 'consumable', 400, 100, 5);
INSERT INTO item_attribute (item_id, health_regeneration, dmg_ice) values ('fo_fishsoup', 6, 8);
INSERT INTO item_food (item_id, food_duration_s) values ('fo_fishsoup', 10);

-- Wine

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('fo_wine', 'consumable', 400, 50, 10);
INSERT INTO item_attribute (item_id, health_regeneration, res_physical, dmg_ice, dmg_light, dmg_fire, dmg_shadow) values ('fo_wine', -1, 10, 5, 5, 5, 5);
INSERT INTO item_food (item_id, food_duration_s) values ('fo_wine', 15);

-- Beer

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('fo_beer', 'consumable', 400, 150, 6);
INSERT INTO item_attribute (item_id, health_regeneration, res_physical, dmg_physical) values ('fo_beer', -1, 10, 10);
INSERT INTO item_food (item_id, food_duration_s) values ('fo_beer', 10);

-- Sausage

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('fo_sausage', 'consumable', 200, 300, 6);
INSERT INTO item_attribute (item_id, health_regeneration, dmg_physical) values ('fo_sausage', 15, 10);
INSERT INTO item_food (item_id, food_duration_s) values ('fo_sausage', 10);

-- Ham

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('fo_ham', 'consumable', 250, 300, 20);
INSERT INTO item_attribute (item_id, health_regeneration, res_ice, dmg_physical) values ('fo_ham', 20, 10, 10);
INSERT INTO item_food (item_id, food_duration_s) values ('fo_ham', 15);


