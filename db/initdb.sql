----------------------------------------
-- This script sets up the database   --
-- and describes the schema for       --
-- the Cendric project. Run it to     --
-- set up the database anew.          --
-- run using sqlite> .read initdb.sql --
----------------------------------------
pragma encoding="UTF-8";

DROP TABLE text;
DROP TABLE item;

CREATE TABLE text(
	text_id VARCHAR(50) NOT NULL, 
	english TEXT NOT NULL DEFAULT '',
	german TEXT NOT NULL DEFAULT '',
	swiss_german TEXT NOT NULL DEFAULT '',
	PRIMARY KEY(text_id ASC)
);

CREATE TABLE item(
	item_id VARCHAR(50), 
	description_id VARCHAR(50) NOT NULL,
	item_type INTEGER NOT NULL,
	icon_texture_x INTEGER,
	icon_texture_y INTEGER,
	gold_value INTEGER NOT NULL,
	PRIMARY KEY(item_id ASC)
);