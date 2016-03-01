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
DROP TABLE enemy_progress;

CREATE TABLE core(
	core_id INTEGER NOT NULL, 
	PRIMARY KEY(core_id)
);

CREATE TABLE position(
	core_id INTEGER NOT NULL, 
	is_in_level BOOLEAN NOT NULL DEFAULT FALSE,
	map_id VARCHAR(100) NOT NULL DEFAULT '', 
	level_id VARCHAR(100) NOT NULL DEFAULT '',
	map_pos_x INTEGER NOT NULL DEFAULT 0,
	map_pos_y INTEGER NOT NULL DEFAULT 0,
	level_pos_x INTEGER NOT NULL DEFAULT 0,
	level_pos_y INTEGER NOT NULL DEFAULT 0,
	PRIMARY KEY(core_id),
	FOREIGN KEY(core_id) REFERENCES core(core_id)
);

CREATE TABLE enemy_progress(
	core_id INTEGER NOT NULL, 
	level_id VARCHAR(100) NOT NULL, 
	enemy_id INTEGER NOT NULL,
	is_killed BOOLEAN NOT NULL DEFAULT FALSE,
	is_looted BOOLEAN NOT NULL DEFAULT FALSE,
	PRIMARY KEY(core_id, level_id, enemy_id),
	FOREIGN KEY(core_id) REFERENCES core(core_id)
);

