-- Light

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('sp_light', 'spell', 300, 400, -1);
INSERT INTO item_spell (item_id, spell_id) values ('sp_light', 17);
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('sp_light', 'item', 'Scroll - Light','Schriftrolle - Licht','Schriftrolle - Liecht');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('sp_light', 'item_desc', 'There shall be light!','Es werde Licht!','Chasch chli heller mache, bitte?');

-- Summon Picklock

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('sp_unlock', 'spell', 200, 400, -1);
INSERT INTO item_spell (item_id, spell_id) values ('sp_unlock', 7);
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('sp_unlock', 'item','Scroll - Summon Picklock','Schriftrolle - Dietrich beschwören','Schriftrolle - Dietrich beschwöre');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('sp_unlock', 'item_desc','Summons a picklock that is able to unlock locked chests if its strength is big enough for the chest.','Beschwört einen Dietrich, der verschlossene Truhen knackt, sofern die Stärke des Zaubers ausreicht.','Beschwört en Dietrich ane wo e verschlosseni Chischte knackt sofern de Zauber gnueg starch isch für die Chischte.');
