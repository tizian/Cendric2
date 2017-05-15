-- Cheese

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('fo_cheese', 'item', 'Cheese','Käse','Chääs');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('fo_cheese', 'item_desc', 'Smelly.','Riecht nicht sonderlich gut.','Er müffelet echli.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('fo_cheese', 'consumable', 0, 100, 3);
INSERT INTO item_attribute (item_id, health_regeneration, haste) values ('fo_cheese', 2, 5);
INSERT INTO item_food (item_id, food_duration_s) values ('fo_cheese', 10);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('fo_cheese', -14, -28, 22, 22);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('fo_cheese', 1, 0, 0, 50, 50);

-- Bread

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('fo_bread', 'item', 'Bread','Brot','Brot');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('fo_bread', 'item_desc', 'An ordinary loaf of bread.','Ein einfaches Brot.','Es Brot halt.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('fo_bread', 'consumable', 50, 100, 2);
INSERT INTO item_attribute (item_id, health_regeneration) values ('fo_bread', 2);
INSERT INTO item_food (item_id, food_duration_s) values ('fo_bread', 15);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('fo_bread', -10, -30, 30, 20);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('fo_bread', 1, 50, 100, 50, 50);

-- Water

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('fo_water', 'item', 'Water bottle','Wasserflasche','Wasserfläsche');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('fo_water', 'item_desc', 'Fresh clear water.','Frisches klares Wasser.','Frischs klars Wasser.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('fo_water', 'consumable', 100, 0, 2);
INSERT INTO item_attribute (item_id, health_regeneration,  dmg_ice, res_fire) values ('fo_water', 2, 5, 5);
INSERT INTO item_food (item_id, food_duration_s) values ('fo_water', 5);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('fo_water', -16, -20, 18, 30);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('fo_water', 1, 200, 50, 50, 50);

-- Potato

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('fo_potato', 'item', 'Potato','Kartoffel','Herdöpfel');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('fo_potato', 'item_desc', 'Sorry for the long post.','Sorry for the long post.','Sorry for the long post.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('fo_potato', 'consumable', 650, 650, 1);
INSERT INTO item_attribute (item_id, health_regeneration) values ('fo_potato', 1);
INSERT INTO item_food (item_id, food_duration_s) values ('fo_potato', 5);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('fo_potato', -14, -34, 24, 16);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('fo_potato', 1, 650, 100, 50, 50);

-- Apple

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('fo_apple', 'item', 'Apple', 'Apfel', 'Öpfel');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('fo_apple', 'item_desc', 'Fresh and juicy.', 'Frisch und saftig.', 'Frisch und saftig.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('fo_apple', 'consumable', 300, 50, 2);
INSERT INTO item_attribute (item_id, health_regeneration, critical, res_physical, res_shadow) values ('fo_apple', 3, 3, 5, 5);
INSERT INTO item_food (item_id, food_duration_s) values ('fo_apple', 6);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('fo_apple', -16, -20, 18, 30);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('fo_apple', 1, 0, 150, 50, 50);

-- Cave Berries

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('fo_caveberry', 'item', 'Cave Berries','Höhlenbeeren','Höhlebeeri');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('fo_caveberry', 'item_desc', 'Sweet, but its pip is poisonous.','Süss aber der Kern ist giftig.','Süess aber de Chern isch giftig.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('fo_caveberry', 'consumable', 500, 0, 3);
INSERT INTO item_attribute (item_id, health_regeneration, critical, dmg_shadow, res_light) values ('fo_caveberry', -1, 5, 5, 3);
INSERT INTO item_food (item_id, food_duration_s) values ('fo_caveberry', 20);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('fo_caveberry', 0, -35, 50, 70);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('fo_caveberry', 1, 0, 0, 50, 100);

-- Wind Rose

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('fo_windrose', 'item', 'Wind Rose','Windrose','Windrose');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('fo_windrose', 'item_desc', 'A rare herb that grants incredible reflexes when eaten.','Ein seltenes Kraut das unglaubliche Reflexe gewährt wenn man es verzehrt.','Es seltenigs Chrütli wo eim unglaublichi Reflex git wenmes isst.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('fo_windrose', 'consumable', 550, 100, 20);
INSERT INTO item_attribute (item_id, haste) values ('fo_windrose', 50);
INSERT INTO item_food (item_id, food_duration_s) values ('fo_windrose', 15);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('fo_windrose', -10, -25, 40, 75);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('fo_windrose', 1, 450, 50, 50, 100);

-- Devil's Berry

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('fo_devilberry', 'item', 'Devil''s Berry', 'Teufelsbeere', 'Tüüfelsbeeri');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('fo_devilberry', 'item_desc', 'Eating those will increase all resistances dramatically. But at a price...', 'Erhöht alle Resistenzen dramatisch... aber das hat seinen Preis.', 'Mit dem ischme ziemlich resistent gege alles... Aber das het sin Priis.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('fo_devilberry', 'consumable', 100, 100, 4);
INSERT INTO item_attribute (item_id, health_regeneration, res_physical, res_fire, res_ice, res_shadow, res_light) values ('fo_devilberry', -6, 50, 50, 50, 50, 50);
INSERT INTO item_food (item_id, food_duration_s) values ('fo_devilberry', 20);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('fo_devilberry', -13, -10, 34, 40);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('fo_devilberry', 1, 150, 200, 50, 50);

-- Storm Weed

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('fo_stormweed', 'item', 'Storm Weed', 'Sturmkraut', 'Sturm Chruut');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('fo_stormweed', 'item_desc', 'Is only fully effective as a component of a potion.', 'Entfaltet seine volle Wirkung erst als Zutat eines Trankes.', 'Entfaltet sini voll Würkig erst als Zuetat vomne Trank.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('fo_stormweed', 'consumable', 550, 150, 100);
INSERT INTO item_attribute (item_id, health_regeneration) values ('fo_stormweed', 1);
INSERT INTO item_food (item_id, food_duration_s) values ('fo_stormweed', 10);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('fo_stormweed', 0, 0, 50, 50);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('fo_stormweed', 1, 450, 150, 50, 50);

-- Red Hat

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('fo_redhat', 'item', 'Red Hat', 'Rotkappe', 'Rotchappe');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('fo_redhat', 'item_desc', 'Mildly toxic.', 'Leicht giftig.', 'Liecht giftig.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('fo_redhat', 'consumable', 550, 200, 4);
INSERT INTO item_attribute (item_id, health_regeneration, critical) values ('fo_redhat', -1, 8);
INSERT INTO item_food (item_id, food_duration_s) values ('fo_redhat', 20);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('fo_redhat', -15, -20, 30, 30);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('fo_redhat', 1, 450, 200, 50, 50);

-- Milk

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('fo_milk', 'item', 'Milk', 'Milch', 'Milch');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('fo_milk', 'item_desc', 'Milk is good for your bones.', 'Milch ist gut für die Knochen.', 'Milch isch guet für d''Chnoche.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('fo_milk', 'consumable', 500, 50, 6);
INSERT INTO item_attribute (item_id, health_regeneration, critical, dmg_ice, dmg_physical) values ('fo_milk', 2, 2, 5, 3);
INSERT INTO item_food (item_id, food_duration_s) values ('fo_milk', 20);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('fo_milk', -7, -20, 35, 35);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('fo_milk', 1, 350, 150, 50, 100);

-- Physalis

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('fo_physalis', 'item', 'Physalis', 'Physalis', 'Physalis');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('fo_physalis', 'item_desc', 'Looks tasty.', 'Sieht lecker aus.', 'Gseht fein uus.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('fo_physalis', 'consumable', 200, 200, 5);
INSERT INTO item_attribute (item_id, health_regeneration, dmg_shadow, dmg_physical, dmg_light) values ('fo_physalis', -2, 7, 7, 7);
INSERT INTO item_food (item_id, food_duration_s) values ('fo_physalis', 20);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('fo_physalis', 0, -20, 50, 45);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('fo_physalis', 1, 250, 0, 50, 100);

-- Glowing Shroom

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('fo_glowingshroom', 'item', 'Glowing shroom','Leuchtpilz','Lüchtpilz');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('fo_glowingshroom', 'item_desc', 'It still glows...','Er leuchtet immer noch...','Er lüchtet immer na...');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('fo_glowingshroom', 'consumable', 150, 50, 2);
INSERT INTO item_attribute (item_id, health_regeneration, dmg_light, res_shadow) values ('fo_glowingshroom', 1, 5, 5);
INSERT INTO item_food (item_id, food_duration_s) values ('fo_glowingshroom', 15);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('fo_glowingshroom', -10, -20, 30, 30);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('fo_glowingshroom', 1, 100, 50, 50, 50);
INSERT INTO levelitem_light(item_id, light_offset_x, light_offset_y, light_radius_x, light_radius_y, brightness) values ('fo_glowingshroom',10, 10, 50, 50, 0.2);

-- Healing Herb

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('fo_healingherb', 'item', 'Healing herb','Heilpflanze','Heilpflanze');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('fo_healingherb', 'item_desc', 'Heals wounds.','Heilt Wunden.','Heilt Wunde.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('fo_healingherb', 'consumable', 50, 50, 5);
INSERT INTO item_attribute (item_id, health_regeneration) values ('fo_healingherb', 5);
INSERT INTO item_food (item_id, food_duration_s) values ('fo_healingherb', 8);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('fo_healingherb', -10, 0, 30, 50);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('fo_healingherb', 1, 50, 50, 50, 50);

-- Lesser Healing Potion

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('fo_lesserhealingpotion', 'item', 'Lesser Healing Potion','Geringer Heiltrank','Chliine Heiltrank');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('fo_lesserhealingpotion', 'item_desc', 'Heals small wounds and scratches.','Heilt kleine Wunden und Kratzer.','Heilt chliini Wunde und Chratzer.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('fo_lesserhealingpotion', 'consumable', 200, 450, 10);
INSERT INTO item_attribute (item_id, health_regeneration) values ('fo_lesserhealingpotion', 10);
INSERT INTO item_food (item_id, food_duration_s) values ('fo_lesserhealingpotion', 5);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('fo_lesserhealingpotion', -15, -4, 20, 46);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('fo_lesserhealingpotion', 1, 100, 100, 50, 50);

-- Medium Healing Potion

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('fo_mediumhealingpotion', 'item', 'Medium Healing Potion','Mittlere Heiltrank','Mittlere Heiltrank');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('fo_mediumhealingpotion', 'item_desc', 'Heals most moderate injuries.','Heilt die meisten mittelschweren Verletzungen.','Heilt di meiste mittleschwere Verletzige.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('fo_mediumhealingpotion', 'consumable', 250, 450, 20);
INSERT INTO item_attribute (item_id, health_regeneration) values ('fo_mediumhealingpotion', 20);
INSERT INTO item_food (item_id, food_duration_s) values ('fo_mediumhealingpotion', 5);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('fo_mediumhealingpotion', -15, -4, 20, 46);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('fo_mediumhealingpotion', 1, 0, 250, 50, 50);

-- Greater Healing Potion

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('fo_greaterhealingpotion', 'item', 'Greater Healing Potion','Grosser Heiltrank','Grosse Heiltrank');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('fo_greaterhealingpotion', 'item_desc', 'Heals nearly everything.','Heilt fast alles.','Heilt fascht alles.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('fo_greaterhealingpotion', 'consumable', 300, 450, 30);
INSERT INTO item_attribute (item_id, health_regeneration) values ('fo_greaterhealingpotion', 30);
INSERT INTO item_food (item_id, food_duration_s) values ('fo_greaterhealingpotion', 5);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('fo_greaterhealingpotion', -15, -4, 20, 46);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('fo_greaterhealingpotion', 1, 100, 250, 50, 50);

-- Ultimate Healing Potion

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('fo_ultimatehealingpotion', 'item', 'Ultimate Healing Potion','Ultimativer Heiltrank','Ultimative Heiltrank');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('fo_ultimatehealingpotion', 'item_desc', 'Ultimate healing power!','Ultimative Heilkraft!','Ultimativi Heilchraft!');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('fo_ultimatehealingpotion', 'consumable', 350, 450, 50);
INSERT INTO item_attribute (item_id, health_regeneration) values ('fo_ultimatehealingpotion', 50);
INSERT INTO item_food (item_id, food_duration_s) values ('fo_ultimatehealingpotion', 5);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('fo_ultimatehealingpotion', -15, -4, 20, 46);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('fo_ultimatehealingpotion', 1, 150, 250, 50, 50);

-- Roasted Meat

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('fo_roastedmeat', 'item', 'Roasted Meat','Gebratenes Fleisch','Brötlets Fleisch');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('fo_roastedmeat', 'item_desc', 'A juicy piece of meat, seared, just as it should be.','Ein saftiges Stück Fleisch, kurz angebraten, wie es sein soll.','Es saftigs Stück Fleisch, churz abrote, genau wes sii söt.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('fo_roastedmeat', 'consumable', 250, 50, 4);
INSERT INTO item_attribute (item_id, health_regeneration, dmg_physical) values ('fo_roastedmeat', 10, 7);
INSERT INTO item_food (item_id, food_duration_s) values ('fo_roastedmeat', 10);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('fo_roastedmeat', -7, -26, 40, 24);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('fo_roastedmeat', 1, 150, 150, 50, 50);

-- Raw Meat

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('fo_rawmeat', 'item', 'Raw Meat','Rohes Fleisch','Rohs Fleisch');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('fo_rawmeat', 'item_desc', 'Would taste better if it was roasted on a fire.','Würde gebraten noch besser schmecken.','Wär chli feiner wenmes wür brötle.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('fo_rawmeat', 'consumable', 200, 50, 2);
INSERT INTO item_attribute (item_id, health_regeneration, dmg_physical) values ('fo_rawmeat', 5, 5);
INSERT INTO item_food (item_id, food_duration_s, is_cookable, cooked_item_id) values ('fo_rawmeat', 10, 1, 'fo_roastedmeat');
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('fo_rawmeat', -7, -26, 40, 24);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('fo_rawmeat', 1, 150, 100, 50, 50);

-- Roasted Chicken

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('fo_chicken', 'item', 'Roasted Chicken','Gebratenes Geflügel','Brötlets Poulet');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('fo_chicken', 'item_desc', 'Crispy outside and tender inside.','Aussen knusprig und innen zart.','Usse chnuschperig und inine zart.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('fo_chicken', 'consumable', 450, 650, 3);
INSERT INTO item_attribute (item_id, health_regeneration, dmg_physical, haste) values ('fo_chicken', 5, 3, 10);
INSERT INTO item_food (item_id, food_duration_s) values ('fo_chicken', 10);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('fo_chicken', -5, -36, 40, 14);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('fo_chicken', 1, 600, 300, 50, 50);

-- Raw Chicken

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('fo_rawchicken', 'item', 'Raw Chicken','Rohes Geflügel','Rohs Poulet');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('fo_rawchicken', 'item_desc', 'Should not be eaten raw.','Sollte nicht roh verzehrt werden.','Das sötme nid roh esse.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('fo_rawchicken', 'consumable', 400, 650, 2);
INSERT INTO item_attribute (item_id, health_regeneration, dmg_physical, haste) values ('fo_rawchicken', -1, -1, -5);
INSERT INTO item_food (item_id, food_duration_s, is_cookable, cooked_item_id) values ('fo_rawchicken', 10, 1, 'fo_chicken');
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('fo_rawchicken', -5, -36, 40, 14);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('fo_rawchicken', 1, 550, 300, 50, 50);

-- Egg

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('fo_egg', 'item', 'Egg','Ei','Ei');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('fo_egg', 'item_desc', 'Can be used as a cooking ingredient.','Kann als Kochzutat verwendet werden.','Chame zum choche bruche.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('fo_egg', 'consumable', 500, 650, 2);
INSERT INTO item_attribute (item_id, health_regeneration, dmg_physical) values ('fo_egg', 2, 10);
INSERT INTO item_food (item_id, food_duration_s, is_cookable, cooked_item_id) values ('fo_egg', 5, 1, 'fo_omelette');
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('fo_egg', -18, -31, 15, 19);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('fo_egg', 1, 500, 300, 50, 50);

-- Omelette

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('fo_omelette', 'item', 'Omelette','Pfannkuchen','Omelette');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('fo_omelette', 'item_desc', 'Delicious taste and strengthening effect.','Schmeckt lecker und hat einen stärkenden Effekt.','Isch fein und het en stärchende Effekt.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('fo_omelette', 'consumable', 550, 650, 3);
INSERT INTO item_attribute (item_id, health_regeneration, dmg_physical, res_physical) values ('fo_omelette', 3, 10, 20);
INSERT INTO item_food (item_id, food_duration_s) values ('fo_omelette', 10);

-- Fish Soup

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('fo_fishsoup', 'item', 'Fish Soup', 'Fischsuppe', 'Fischsuppe');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('fo_fishsoup', 'item_desc', 'Hot and delicious.', 'Heiss und lecker.', 'Heiss und gluschtig.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('fo_fishsoup', 'consumable', 400, 100, 5);
INSERT INTO item_attribute (item_id, health_regeneration, dmg_ice) values ('fo_fishsoup', 6, 8);
INSERT INTO item_food (item_id, food_duration_s) values ('fo_fishsoup', 10);

-- Raw Fish

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('fo_rawfish', 'item', 'Raw Fish', 'Roher Fisch', 'Rohe Fisch');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('fo_rawfish', 'item_desc', 'I should cook this better first.', 'Den sollte ich wohl besser zuerst kochen.', 'Söttme glaub besser zersch choche.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('fo_rawfish', 'consumable', 400, 0, 3);
INSERT INTO item_attribute (item_id, health_regeneration, dmg_ice) values ('fo_rawfish', 5, 5);
INSERT INTO item_food (item_id, food_duration_s, is_cookable, cooked_item_id) values ('fo_rawfish', 6, 1, 'fo_fishsoup');
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('fo_rawfish', 0, -30, 50, 35);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('fo_rawfish', 1, 50, 200, 50, 100);


-- Wine

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('fo_wine', 'item', 'Wine', 'Wein', 'Wii');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('fo_wine', 'item_desc', 'A noble wine.', 'Ein edler Wein.', 'En guete Tropfe.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('fo_wine', 'consumable', 400, 50, 10);
INSERT INTO item_attribute (item_id, health_regeneration, res_physical, dmg_ice, dmg_light, dmg_fire, dmg_shadow) values ('fo_wine', -1, 10, 5, 5, 5, 5);
INSERT INTO item_food (item_id, food_duration_s) values ('fo_wine', 15);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('fo_wine', -15, -10, 20, 40);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('fo_wine', 1, 450, 250, 50, 50);

-- Beer

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('fo_beer', 'item', 'Beer', 'Bier', 'Bier');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('fo_beer', 'item_desc', 'Should be consumed in moderation.', 'Zu viel davon sollte man besser nicht geniessen.', 'Z''viel devo söt me besser nid trinke.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('fo_beer', 'consumable', 400, 150, 6);
INSERT INTO item_attribute (item_id, health_regeneration, res_physical, dmg_physical) values ('fo_beer', -1, 10, 10);
INSERT INTO item_food (item_id, food_duration_s) values ('fo_beer', 10);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('fo_beer', -15, -10, 20, 40);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('fo_beer', 1, 600, 100, 50, 50);

-- Sausage

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('fo_sausage', 'item', 'Sausage', 'Wurst', 'Wurscht');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('fo_sausage', 'item_desc', 'Tasty.', 'Lecker.', 'Fein.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('fo_sausage', 'consumable', 200, 300, 6);
INSERT INTO item_attribute (item_id, health_regeneration, dmg_physical) values ('fo_sausage', 15, 10);
INSERT INTO item_food (item_id, food_duration_s) values ('fo_sausage', 10);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('fo_sausage', -3, -40, 44, 10);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('fo_sausage', 1, 600, 150, 50, 50);

-- Ham

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('fo_ham', 'item', 'Ham', 'Schinken', 'Schinke');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('fo_ham', 'item_desc', 'A juicy ham.', 'Ein saftiger Schinken.', 'En saftige Schinke.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('fo_ham', 'consumable', 250, 300, 20);
INSERT INTO item_attribute (item_id, health_regeneration, res_ice, dmg_physical) values ('fo_ham', 20, 10, 10);
INSERT INTO item_food (item_id, food_duration_s) values ('fo_ham', 15);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('fo_ham', -12, -15, 23, 47);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('fo_ham', 1, 600, 200, 50, 100);


