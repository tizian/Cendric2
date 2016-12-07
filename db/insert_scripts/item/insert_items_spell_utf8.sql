-- Light

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('sp_light', 'spell', 300, 400, -1);
INSERT INTO item_spell (item_id, spell_id) values ('sp_light', 17);
INSERT INTO text (text_id, text_type, english, german, swiss_german, spanish) values ('sp_light', 'item', 'Scroll - Light','Schriftrolle - Licht','Schriftrolle - Liecht', 'Pergamino - Resplandor');
INSERT INTO text (text_id, text_type, english, german, swiss_german, spanish) values ('sp_light', 'item_desc', 'There shall be light!','Es werde Licht!','Chasch chli heller mache, bitte?', '¡Hágase la luz!');

-- Holy Fire

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('sp_holyfire', 'spell', 300, 400, -1);
INSERT INTO item_spell (item_id, spell_id) values ('sp_holyfire', 18);
INSERT INTO text (text_id, text_type, english, german, swiss_german, spanish) values ('sp_holyfire', 'item', 'Scroll - Holy Fire','Schriftrolle - Heiliges Feuer','Schriftrolle - Heiligs Füür', 'Pergamino - Fuego divino');
INSERT INTO text (text_id, text_type, english, german, swiss_german, spanish) values ('sp_holyfire', 'item_desc', 'Cendric summons a holy fire that burns enemies in range.','Cendric beschwört ein heiliges Feuer, das Gegner in der Nähe verbrennt.','De Cendric beschwört es heiligs Füür wo Gegner ide Nöchi verbrennt.','Cendric convoca a un fuego divino que quema a enemigos al alcance.');

-- Summon Picklock

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('sp_unlock', 'spell', 200, 400, -1);
INSERT INTO item_spell (item_id, spell_id) values ('sp_unlock', 7);
INSERT INTO text (text_id, text_type, english, german, swiss_german, spanish) values ('sp_unlock', 'item','Scroll - Summon Picklock','Schriftrolle - Dietrich beschwören','Schriftrolle - Dietrich beschwöre', 'Pergamino - Convocación de ganzúa');
INSERT INTO text (text_id, text_type, english, german, swiss_german, spanish) values ('sp_unlock', 'item_desc','Summons a picklock that is able to unlock locked chests if its strength is big enough for the chest.','Beschwört einen Dietrich, der verschlossene Truhen knackt, sofern die Stärke des Zaubers ausreicht.','Beschwört en Dietrich ane wo e verschlosseni Chischte knackt sofern de Zauber gnueg starch isch für die Chischte.', 'Convoca a una ganzúa que puede abrir cofres si es lo suficientemente fuerte para el cofre.');

-- Fireball

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('sp_fireball', 'spell', 150, 400, -1);
INSERT INTO item_spell (item_id, spell_id) values ('sp_fireball', 3);
INSERT INTO text (text_id, text_type, english, german, swiss_german, spanish) values ('sp_fireball', 'item','Scroll - Fireball','Schriftrolle - Feuerball','Schriftrolle - Füürball', 'Pergamino - Bola de fuego');
INSERT INTO text (text_id, text_type, english, german, swiss_german, spanish) values ('sp_fireball', 'item_desc','The good old fireball. Burns enemies and melts ice.','Der gute alte Feuerball. Verbrennt Gegner und schmilzt Eis.','De guet alt Füürball. Verbrennt Gegner und schmelzt Iis.','La siempre leal bola de fuego. Quema enemigos y derrite el hielo.');

