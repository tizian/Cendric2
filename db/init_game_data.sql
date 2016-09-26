-----------------------------------------
-- This script sets up the database    --
-- and describes the schema for        --
-- the read-only database 'game_data'  --
-- of the Cendric project. Run it to   --
-- set up the database anew. Run using -- 
-- sqlite> .read init_game_data.sql    --
-----------------------------------------
PRAGMA encoding="UTF-8";
PRAGMA foreign_keys = ON;

DROP TABLE text;
DROP TABLE spawn;
DROP TABLE levelitem_light;
DROP TABLE levelitem_frame;
DROP TABLE levelitem;
DROP TABLE item_document_page;
DROP TABLE item_document_quest;
DROP TABLE item_weapon_slot;
DROP TABLE item_weapon;
DROP TABLE item_equipment_light;
DROP TABLE item_equipment;
DROP TABLE item_attribute;
DROP TABLE item_food;
DROP TABLE item_convertible;
DROP TABLE item_spell;
DROP TABLE item;

CREATE TABLE text(
	text_id VARCHAR(50) NOT NULL, 
	text_type VARCHAR(50) NOT NULL DEFAULT 'core',
	english TEXT NOT NULL DEFAULT '',
	german TEXT NOT NULL DEFAULT '',
	swiss_german TEXT NOT NULL DEFAULT '',
	spanish TEXT NOT NULL DEFAULT '',
	PRIMARY KEY(text_id, text_type ASC)
);

CREATE INDEX text_type_x ON text(text_type);

CREATE TABLE item(
	item_id VARCHAR(50) NOT NULL, 
	item_type VARCHAR(10) NOT NULL,
	icon_left INTEGER NOT NULL DEFAULT 0,
	icon_top INTEGER NOT NULL DEFAULT 0,
	gold_value INTEGER NOT NULL,
	PRIMARY KEY(item_id ASC)
);

CREATE TABLE item_convertible (
	item_id VARCHAR(50) NOT NULL, 
	convertible_item_id VARCHAR(50) NOT NULL,
	convertible_amount INTEGER NOT NULL DEFAULT 1,
	probability INTEGER NOT NULL DEFAULT 100,
	PRIMARY KEY (item_id, convertible_item_id),
	FOREIGN KEY(item_id) REFERENCES item(item_id)
	FOREIGN KEY(convertible_item_id) REFERENCES item(item_id)
);

CREATE TABLE item_attribute(
	item_id VARCHAR(50) NOT NULL, 
	max_health INTEGER NOT NULL DEFAULT 0,
	health_regeneration INTEGER NOT NULL DEFAULT 0,
	haste INTEGER NOT NULL DEFAULT 0,
	critical INTEGER NOT NULL DEFAULT 0,
	heal INTEGER NOT NULL DEFAULT 0,
	dmg_physical INTEGER NOT NULL DEFAULT 0,
	dmg_fire INTEGER NOT NULL DEFAULT 0,
	dmg_ice INTEGER NOT NULL DEFAULT 0,
	dmg_shadow INTEGER NOT NULL DEFAULT 0,
	dmg_light INTEGER NOT NULL DEFAULT 0,
	res_physical INTEGER NOT NULL DEFAULT 0,
	res_fire INTEGER NOT NULL DEFAULT 0,
	res_ice INTEGER NOT NULL DEFAULT 0,
	res_shadow INTEGER NOT NULL DEFAULT 0,
	res_light INTEGER NOT NULL DEFAULT 0,
	PRIMARY KEY (item_id),
	FOREIGN KEY(item_id) REFERENCES item(item_id)
);

CREATE TABLE item_document_page(
	item_id VARCHAR(50) NOT NULL, 
	page_nr INTEGER NOT NULL,
	title VARCHAR(50) NOT NULL DEFAULT "",
	content VARCHAR(50) NOT NULL DEFAULT "",
	texture_path VARCHAR(100) NOT NULL DEFAULT "",
	PRIMARY KEY (item_id, page_nr),
	FOREIGN KEY(item_id) REFERENCES item(item_id)
);

CREATE TABLE item_document_quest(
	item_id VARCHAR(50) NOT NULL, 
	quest_name VARCHAR(50) NOT NULL, 
	quest_state VARCHAR(50) NOT NULL DEFAULT "",
	quest_desc INTEGER NOT NULL DEFAULT -1,
	PRIMARY KEY (item_id),
	FOREIGN KEY(item_id) REFERENCES item(item_id)
);

CREATE TABLE item_food(
	item_id VARCHAR(50) NOT NULL, 
	food_duration_s INTEGER NOT NULL,
	is_cookable BOOLEAN NOT NULL DEFAULT FALSE,
	cooked_item_id VARCHAR(50),
	PRIMARY KEY (item_id),
	FOREIGN KEY(item_id) REFERENCES item(item_id),
	FOREIGN KEY(cooked_item_id) REFERENCES item(item_id)
);

CREATE TABLE item_equipment(
	item_id VARCHAR(50) NOT NULL, 
	texture_path VARCHAR(100) NOT NULL, 
	map_texture_path VARCHAR(100) NOT NULL, 
	frames_walk INTEGER NOT NULL DEFAULT 8,
	frames_idle INTEGER NOT NULL DEFAULT 1,
	frames_jump INTEGER NOT NULL DEFAULT 1,
	frames_fight INTEGER NOT NULL DEFAULT 5,
	frames_climb1 INTEGER NOT NULL DEFAULT 1,
	frames_climb2 INTEGER NOT NULL DEFAULT 1,
	PRIMARY KEY (item_id),
	FOREIGN KEY(item_id) REFERENCES item(item_id)
);

CREATE TABLE item_spell(
	item_id VARCHAR(50) NOT NULL, 
	spell_id VARCHAR(200) NOT NULL,
	PRIMARY KEY (item_id),
	FOREIGN KEY(item_id) REFERENCES item(item_id)
);

CREATE TABLE item_equipment_light(
	item_id VARCHAR(50) NOT NULL, 
	light_offset_x INTEGER NOT NULL DEFAULT 0,
	light_offset_y INTEGER NOT NULL DEFAULT 0,
	light_radius_x INTEGER NOT NULL DEFAULT 0,
	light_radius_y INTEGER NOT NULL DEFAULT 0,
	map_light_radius_x INTEGER NOT NULL DEFAULT 0,
	map_light_radius_y INTEGER NOT NULL DEFAULT 0,
	brightness DOUBLE NOT NULL DEFAULT 1.0,
	PRIMARY KEY (item_id),
	FOREIGN KEY(item_id) REFERENCES equipment(item_id)
);

CREATE TABLE item_weapon(
	item_id VARCHAR(50) NOT NULL, 
	chop_cooldown_ms INTEGER NOT NULL,
	chop_offset_left INTEGER NOT NULL DEFAULT 0,
	chop_offset_top INTEGER NOT NULL DEFAULT 0,
	chop_rect_width INTEGER NOT NULL,
	chop_rect_height INTEGER NOT NULL,
	chop_damage INTEGER NOT NULL,
	PRIMARY KEY (item_id),
	FOREIGN KEY(item_id) REFERENCES equipment(item_id)
);

CREATE TABLE item_weapon_slot(
	item_id VARCHAR(50) NOT NULL, 
	slot_nr INTEGER NOT NULL,
	slot_type VARCHAR(10) NOT NULL,
	modifier_count INTEGER NOT NULL,
	PRIMARY KEY (item_id, slot_nr ASC),
	FOREIGN KEY(item_id) REFERENCES item_weapon(item_id)
);

CREATE TABLE levelitem(
	item_id VARCHAR(50) NOT NULL, 
	sprite_offset_left INTEGER NOT NULL DEFAULT 0,
	sprite_offset_top INTEGER NOT NULL DEFAULT 0,
	bounding_box_width INTEGER NOT NULL DEFAULT 0,
	bounding_box_height INTEGER NOT NULL DEFAULT 0,
	frame_time_ms INTEGER NOT NULL DEFAULT 100,
	PRIMARY KEY (item_id),
	FOREIGN KEY(item_id) REFERENCES item(item_id)
);

CREATE TABLE levelitem_frame(
	item_id VARCHAR(50) NOT NULL, 
	frame_nr INTEGER NOT NULL,
	frame_left INTEGER NOT NULL DEFAULT 0,
	frame_top INTEGER NOT NULL DEFAULT 0,
	frame_width INTEGER NOT NULL DEFAULT 0,
	frame_height INTEGER NOT NULL DEFAULT 0,
	PRIMARY KEY (item_id, frame_nr ASC),
	FOREIGN KEY(item_id) REFERENCES levelitem(item_id)
);

CREATE TABLE levelitem_light(
	item_id VARCHAR(50) NOT NULL, 
	light_offset_x INTEGER NOT NULL DEFAULT 0,
	light_offset_y INTEGER NOT NULL DEFAULT 0,
	light_radius_x INTEGER NOT NULL DEFAULT 0,
	light_radius_y INTEGER NOT NULL DEFAULT 0,
	brightness DOUBLE NOT NULL DEFAULT 1.0,
	PRIMARY KEY (item_id),
	FOREIGN KEY(item_id) REFERENCES levelitem(item_id)
);

CREATE TABLE spawn (
	spawn_id VARCHAR(50) NOT NULL, 
	map_id VARCHAR(100) NOT NULL,
	map_x INTEGER NOT NULL,
	map_y INTEGER NOT NULL,
	weapon_id VARCHAR(50) NOT NULL DEFAULT '', 
	armor_id VARCHAR(50) NOT NULL DEFAULT '', 
	PRIMARY KEY(spawn_id ASC),
	FOREIGN KEY(weapon_id) REFERENCES item(item_id),
	FOREIGN KEY(armor_id) REFERENCES item(item_id)
);
