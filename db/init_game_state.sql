--[DEPRECATED]
------------------------------------------
-- This script sets up the database     --
-- and describes the schema for         --
-- the read-write database 'game_state' --
-- of the Cendric project. Run it to    --
-- set up the database anew. Run using  -- 
-- sqlite> .read init_game_state.sql     --
------------------------------------------
PRAGMA encoding="UTF-8";
PRAGMA foreign_keys = ON;

DROP TABLE core;
DROP TABLE position;
DROP TABLE enemy_progress;
DROP TABLE item_looted;
DROP TABLE chest_looted;
DROP TABLE waypoint_unlocked;
DROP TABLE condition_progress;
DROP TABLE quest_progress;
DROP TABLE merchand_state;
DROP TABLE merchand_wares;

CREATE TABLE core (
	core_id INTEGER NOT NULL, 
	PRIMARY KEY(core_id)
);

CREATE TABLE position (
	core_id INTEGER NOT NULL, 
	is_in_level BOOLEAN NOT NULL DEFAULT FALSE,
	map_id VARCHAR(100) NOT NULL DEFAULT '', 
	level_id VARCHAR(100) NOT NULL DEFAULT '',
	map_pos_x INTEGER NOT NULL DEFAULT 0,
	map_pos_y INTEGER NOT NULL DEFAULT 0,
	level_pos_x INTEGER NOT NULL DEFAULT 0,
	level_pos_y INTEGER NOT NULL DEFAULT 0,
	PRIMARY KEY(core_id),
	FOREIGN KEY(core_id) REFERENCES core(core_id) ON DELETE CASCADE
);

CREATE TABLE enemy_progress (
	core_id INTEGER NOT NULL, 
	level_id VARCHAR(100) NOT NULL, 
	enemy_id INTEGER NOT NULL,
	is_killed BOOLEAN NOT NULL DEFAULT FALSE,
	is_looted BOOLEAN NOT NULL DEFAULT FALSE,
	PRIMARY KEY(core_id, level_id, enemy_id),
	FOREIGN KEY(core_id) REFERENCES core(core_id) ON DELETE CASCADE
);

CREATE TABLE item_looted (
	core_id INTEGER NOT NULL, 
	level_id VARCHAR(100) NOT NULL, 
	item_id INTEGER NOT NULL,
	PRIMARY KEY(core_id, level_id, item_id),
	FOREIGN KEY(core_id) REFERENCES core(core_id) ON DELETE CASCADE
);

CREATE TABLE chest_looted (
	core_id INTEGER NOT NULL, 
	level_id VARCHAR(100) NOT NULL, 
	chest_id INTEGER NOT NULL,
	PRIMARY KEY(core_id, level_id, chest_id),
	FOREIGN KEY(core_id) REFERENCES core(core_id) ON DELETE CASCADE
);

CREATE TABLE waypoint_unlocked (
	core_id INTEGER NOT NULL, 
	level_id VARCHAR(100) NOT NULL, 
	waypoint_id INTEGER NOT NULL,
	PRIMARY KEY(core_id, level_id, waypoint_id),
	FOREIGN KEY(core_id) REFERENCES core(core_id) ON DELETE CASCADE
);

CREATE TABLE condition_progress (
	core_id INTEGER NOT NULL, 
	condition_type VARCHAR(100) NOT NULL DEFAULT 'none', 
	condition VARCHAR(100) NOT NULL, 
	PRIMARY KEY(core_id, condition_type, condition),
	FOREIGN KEY(core_id) REFERENCES core(core_id) ON DELETE CASCADE
);

CREATE TABLE quest_progress (
	core_id INTEGER NOT NULL, 
	quest_id VARCHAR(100) NOT NULL, 
	condition VARCHAR(100) NOT NULL, 
	PRIMARY KEY(core_id, condition_type, condition),
	FOREIGN KEY(core_id) REFERENCES core(core_id) ON DELETE CASCADE
);


CREATE TABLE merchand_state (
	core_id INTEGER NOT NULL, 
	merchand_id VARCHAR(100) NOT NULL,
	selling_multiplier DOUBLE NOT NULL DEFAULT 1.5
	PRIMARY KEY(core_id, merchand_id),
	FOREIGN KEY(core_id) REFERENCES core(core_id) ON DELETE CASCADE
);

CREATE TABLE merchand_wares (
	core_id INTEGER NOT NULL, 
	merchand_id VARCHAR(100) NOT NULL,
	item_id VARCHAR (100) NOT NULL,
	amount INTEGER NOT NULL,
	PRIMARY KEY(core_id, merchand_id, item_id),
	FOREIGN KEY(merchand_id) REFERENCES merchand_state(merchand_id) ON DELETE CASCADE
	FOREIGN KEY(core_id) REFERENCES merchand_state(core_id) ON DELETE CASCADE
);







