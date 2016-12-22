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

-- Divine Shield

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('sp_divineshield', 'spell', 300, 400, -1);
INSERT INTO item_spell (item_id, spell_id) values ('sp_divineshield', 19);
INSERT INTO text (text_id, text_type, english, german, swiss_german, spanish) values ('sp_divineshield', 'item', 'Scroll - Divine Shield','Schriftrolle - Göttlicher Schild','Schriftrolle - Göttlichs Schild', 'Pergamino - Escudo divino');
INSERT INTO text (text_id, text_type, english, german, swiss_german, spanish) values ('sp_divineshield', 'item_desc', 'Grants a shield that enhances Cendrics resistances and heals him once.','Umhüllt Cendric mit einem Schild, der ihn einmalig heilt und für eine gewisse Dauer seine Resistenzen erhöht.','Umhüllt de Cendric mitme Schild wo ihn eimalig heilt und sini Resistenze für e gwüssi Ziit erhöht.','Otorga un escudo que aumenta la resistencia de Cendric y lo cura una vez.');

-- Summon Picklock

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('sp_unlock', 'spell', 200, 400, -1);
INSERT INTO item_spell (item_id, spell_id) values ('sp_unlock', 7);
INSERT INTO text (text_id, text_type, english, german, swiss_german, spanish) values ('sp_unlock', 'item','Scroll - Summon Picklock','Schriftrolle - Dietrich beschwören','Schriftrolle - Dietrich beschwöre', 'Pergamino - Convocación de ganzúa');
INSERT INTO text (text_id, text_type, english, german, swiss_german, spanish) values ('sp_unlock', 'item_desc','Summons a picklock that is able to unlock locked chests if its strength is big enough for the chest.','Beschwört einen Dietrich, der verschlossene Truhen knackt, sofern die Stärke des Zaubers ausreicht.','Beschwört en Dietrich ane wo e verschlosseni Chischte knackt sofern de Zauber gnueg starch isch für die Chischte.', 'Convoca a una ganzúa que puede abrir cofres si es lo suficientemente fuerte para el cofre.');

-- Shadow Trap

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('sp_shadowtrap', 'spell', 200, 400, -1);
INSERT INTO item_spell (item_id, spell_id) values ('sp_shadowtrap', 8);
INSERT INTO text (text_id, text_type, english, german, swiss_german, spanish) values ('sp_shadowtrap', 'item','Scroll - Shadow Trap','Schriftrolle - Schattenfalle','Schriftrolle - Schattefalle', 'Pergamino - Trampa de sombra');
INSERT INTO text (text_id, text_type, english, german, swiss_german, spanish) values ('sp_shadowtrap', 'item_desc','Cendric places a shadow trap on the ground that stuns enemies and deals damage over time when triggered.','Cendric platziert eine Schattenfalle am Boden, die Gegner betäubt und Schaden über Zeit verursacht, wenn sie ausgelöst wird.','De Cendric setzt e Schattefalle uf de Bode wo Gegner betäubt und Schade über Ziit macht wenn si usglöst wird.','Cendric coloca una trampa de sombra en el piso, la cual, al activarse, aturde a los enemigos y ocasiona daño por cierto tiempo.');

-- Invisibility

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('sp_invisibility', 'spell', 200, 400, -1);
INSERT INTO item_spell (item_id, spell_id) values ('sp_invisibility', 9);
INSERT INTO text (text_id, text_type, english, german, swiss_german, spanish) values ('sp_invisibility', 'item','Scroll - Invisibility','Schriftrolle - Unsichtbarkeit','Schriftrolle - Unsichtbarkeit', 'Pergamino - Invisibilidad');
INSERT INTO text (text_id, text_type, english, german, swiss_german, spanish) values ('sp_invisibility', 'item_desc','Summons a picklock that is able to unlock locked chests if its strength is big enough for the chest.','Renders Cendric invisible for a certain duration for certain enemies','Cendric wird unsichtbar für eine gewisse Zeit und für gewisse Gegner.','Cendric wird unsichtbar für e gwüssi Ziit und gwüssi Gegner.','Hace invisible a Cendric ante algunos enemigos por cierto tiempo.');

-- Fear

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('sp_fear', 'spell', 250, 400, -1);
INSERT INTO item_spell (item_id, spell_id) values ('sp_fear', 12);
INSERT INTO text (text_id, text_type, english, german, swiss_german, spanish) values ('sp_fear', 'item','Scroll - Fear','Schriftrolle - Furcht','Schriftrolle - Angscht', 'Pergamino - Temor');
INSERT INTO text (text_id, text_type, english, german, swiss_german, spanish) values ('sp_fear', 'item_desc','Enemies will flee in terror for a certain duration if the spell is strong enough.','Gegner fliehen für eine gewisse Zeit, allerdings nur, wenn der Zauber genug stark für sie ist.','Gegner flüchtet für e gwüssi Ziit, aber nur, wenn de Zauber gnueg starch isch für si.','Si el hechizo es suficientemente fuerte, los enemigos huirán aterrorizados por un lapso de tiempo.');
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('sp_fear', 0, -30, 50, 20);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('sp_fear', 1, 150, 0, 50, 50);

-- Fireball

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('sp_fireball', 'spell', 150, 400, -1);
INSERT INTO item_spell (item_id, spell_id) values ('sp_fireball', 3);
INSERT INTO text (text_id, text_type, english, german, swiss_german, spanish) values ('sp_fireball', 'item','Scroll - Fireball','Schriftrolle - Feuerball','Schriftrolle - Füürball', 'Pergamino - Bola de fuego');
INSERT INTO text (text_id, text_type, english, german, swiss_german, spanish) values ('sp_fireball', 'item_desc','The good old fireball. Burns enemies and melts ice.','Der gute alte Feuerball. Verbrennt Gegner und schmilzt Eis.','De guet alt Füürball. Verbrennt Gegner und schmelzt Iis.','La siempre leal bola de fuego. Quema enemigos y derrite el hielo.');

