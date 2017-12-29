-- Light

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('sp_light', 'spell', 300, 400, -1, 2);
INSERT INTO item_spell (item_id, spell_id) values ('sp_light', 17);
INSERT INTO text (text_id, text_type, english, german, swiss_german, spanish) values ('sp_light', 'item', 'Scroll - Light','Schriftrolle - Licht','Schriftrolle - Liecht', 'Pergamino - Resplandor');
INSERT INTO text (text_id, text_type, english, german, swiss_german, spanish) values ('sp_light', 'item_desc', 'There shall be light!','Es werde Licht!','Chasch chli heller mache, bitte?', '¡Hágase la luz!');

-- Holy Fire

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('sp_holyfire', 'spell', 300, 400, -1, 2);
INSERT INTO item_spell (item_id, spell_id) values ('sp_holyfire', 18);
INSERT INTO text (text_id, text_type, english, german, swiss_german, spanish) values ('sp_holyfire', 'item', 'Scroll - Holy Fire','Schriftrolle - Heiliges Feuer','Schriftrolle - Heiligs Füür', 'Pergamino - Fuego divino');
INSERT INTO text (text_id, text_type, english, german, swiss_german, spanish) values ('sp_holyfire', 'item_desc', 'Cendric summons a holy fire that burns enemies in range.','Cendric beschwört ein heiliges Feuer, das Gegner in der Nähe verbrennt.','De Cendric beschwört es heiligs Füür wo Gegner ide Nöchi verbrennt.','Cendric convoca a un fuego divino que quema a enemigos al alcance.');

-- Divine Shield

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('sp_divineshield', 'spell', 300, 400, -1, 2);
INSERT INTO item_spell (item_id, spell_id) values ('sp_divineshield', 19);
INSERT INTO text (text_id, text_type, english, german, swiss_german, spanish) values ('sp_divineshield', 'item', 'Scroll - Divine Shield','Schriftrolle - Göttlicher Schild','Schriftrolle - Göttlichs Schild', 'Pergamino - Escudo divino');
INSERT INTO text (text_id, text_type, english, german, swiss_german, spanish) values ('sp_divineshield', 'item_desc', 'Grants a shield that enhances Cendrics resistances and heals him once.','Umhüllt Cendric mit einem Schild, der ihn einmalig heilt und für eine gewisse Dauer seine Resistenzen erhöht.','Umhüllt de Cendric mitme Schild wo ihn eimalig heilt und sini Resistenze für e gwüssi Ziit erhöht.','Otorga un escudo que aumenta la resistencia de Cendric y lo cura una vez.');

-- Summon Picklock

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('sp_unlock', 'spell', 200, 400, -1, 2);
INSERT INTO item_spell (item_id, spell_id) values ('sp_unlock', 7);
INSERT INTO text (text_id, text_type, english, german, swiss_german, spanish) values ('sp_unlock', 'item','Scroll - Summon Picklock','Schriftrolle - Dietrich beschwören','Schriftrolle - Dietrich beschwöre', 'Pergamino - Convocación de ganzúa');
INSERT INTO text (text_id, text_type, english, german, swiss_german, spanish) values ('sp_unlock', 'item_desc','Summons a picklock that is able to unlock locked chests if its strength is big enough for the chest.','Beschwört einen Dietrich, der verschlossene Truhen knackt, sofern die Stärke des Zaubers ausreicht.','Beschwört en Dietrich ane wo e verschlosseni Chischte knackt sofern de Zauber gnueg starch isch für die Chischte.', 'Convoca a una ganzúa que puede abrir cofres si es lo suficientemente fuerte para el cofre.');

-- Shadow Trap

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('sp_shadowtrap', 'spell', 200, 400, -1, 2);
INSERT INTO item_spell (item_id, spell_id) values ('sp_shadowtrap', 8);
INSERT INTO text (text_id, text_type, english, german, swiss_german, spanish) values ('sp_shadowtrap', 'item','Scroll - Shadow Trap','Schriftrolle - Schattenfalle','Schriftrolle - Schattefalle', 'Pergamino - Trampa de sombra');
INSERT INTO text (text_id, text_type, english, german, swiss_german, spanish) values ('sp_shadowtrap', 'item_desc','Cendric places a shadow trap on the ground that stuns enemies and deals damage over time when triggered.','Cendric platziert eine Schattenfalle am Boden, die Gegner betäubt und Schaden über Zeit verursacht, wenn sie ausgelöst wird.','De Cendric setzt e Schattefalle uf de Bode wo Gegner betäubt und Schade über Ziit macht wenn si usglöst wird.','Cendric coloca una trampa de sombra en el piso, la cual, al activarse, aturde a los enemigos y ocasiona daño por cierto tiempo.');

-- Invisibility

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('sp_invisibility', 'spell', 200, 400, -1, 2);
INSERT INTO item_spell (item_id, spell_id) values ('sp_invisibility', 9);
INSERT INTO text (text_id, text_type, english, german, swiss_german, spanish) values ('sp_invisibility', 'item','Scroll - Invisibility','Schriftrolle - Unsichtbarkeit','Schriftrolle - Unsichtbarkeit', 'Pergamino - Invisibilidad');
INSERT INTO text (text_id, text_type, english, german, swiss_german, spanish) values ('sp_invisibility', 'item_desc','Renders Cendric invisible for a certain duration for certain enemies','Cendric wird unsichtbar für eine gewisse Zeit und für gewisse Gegner.','Cendric wird unsichtbar für e gwüssi Ziit und gwüssi Gegner.','Hace invisible a Cendric ante algunos enemigos por cierto tiempo.');

-- Fear

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('sp_fear', 'spell', 250, 400, -1, 2);
INSERT INTO item_spell (item_id, spell_id) values ('sp_fear', 12);
INSERT INTO text (text_id, text_type, english, german, swiss_german, spanish) values ('sp_fear', 'item','Scroll - Fear','Schriftrolle - Furcht','Schriftrolle - Angscht', 'Pergamino - Temor');
INSERT INTO text (text_id, text_type, english, german, swiss_german, spanish) values ('sp_fear', 'item_desc','Enemies will flee in terror for a certain duration if the spell is strong enough.','Gegner fliehen für eine gewisse Zeit, allerdings nur, wenn der Zauber genug stark für sie ist.','Gegner flüchtet für e gwüssi Ziit, aber nur, wenn de Zauber gnueg starch isch für si.','Si el hechizo es suficientemente fuerte, los enemigos huirán aterrorizados por un lapso de tiempo.');
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('sp_fear', 0, -30, 50, 20);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('sp_fear', 1, 150, 0, 50, 50);

-- Leech

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('sp_leech', 'spell', 250, 400, -1, 2);
INSERT INTO item_spell (item_id, spell_id) values ('sp_leech', 13);
INSERT INTO text (text_id, text_type, english, german, swiss_german, spanish) values ('sp_leech', 'item','Scroll - Leech','Schriftrolle - Lebensraub','Schriftrolle - Lebensraub', 'Pergamino - Sanguijuela');
INSERT INTO text (text_id, text_type, english, german, swiss_german, spanish) values ('sp_leech', 'item_desc','Sends out leeching spells that damage enemies and return to Cendric to heal him if they hit an enemy.','Schickt lebensraubende Zauber aus, die zu Cendric zurückkehren, um ihn zu heilen, falls sie einen Gegner getroffen haben.','Schickt lebensraubendi Zauber uus wo zum Cendric zruggchömed zum ihn heile falls si en Gegner troffe hend.','Envía un hechizo de sanguijuelas que dañan a los enemigos y regresan a Cendric para curarlo cuando golpean a un enemigo.');

-- Raise the Dead

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('sp_raisethedead', 'spell', 250, 400, -1, 2);
INSERT INTO item_spell (item_id, spell_id) values ('sp_raisethedead', 14);
INSERT INTO text (text_id, text_type, english, german, swiss_german, spanish) values ('sp_raisethedead', 'item','Scroll - Raise The Dead','Schriftrolle - Tote Erwecken','Schriftrolle - Toti Erwecke', 'Pergamino - Resurreción de los muertos');
INSERT INTO text (text_id, text_type, english, german, swiss_german, spanish) values ('sp_raisethedead', 'item_desc','Raises fallen enemies that will fight for Cendric for a certain duration, if the spell is strong enough.','Erweckt gefallene Gegner wieder, die für eine gewisse Zeit für Cendric kämpfen, falls der Zauber genügend stark ist.','Erweckt toti Gegner wider und laht si für e gwüssi Ziit für de Cendric kämpfe, falls de Zauber gnueg starch isch.','Resucita enemigos caídos que lucharán por Cendric durante cierto tiempo, si el hechizo es suficientemente fuerte.');

-- Fireball

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('sp_fireball', 'spell', 150, 400, -1, 2);
INSERT INTO item_spell (item_id, spell_id) values ('sp_fireball', 3);
INSERT INTO text (text_id, text_type, english, german, swiss_german, spanish) values ('sp_fireball', 'item','Scroll - Fireball','Schriftrolle - Feuerball','Schriftrolle - Füürball', 'Pergamino - Bola de fuego');
INSERT INTO text (text_id, text_type, english, german, swiss_german, spanish) values ('sp_fireball', 'item_desc','The good old fireball. Burns enemies and melts ice.','Der gute alte Feuerball. Verbrennt Gegner und schmilzt Eis.','De guet alt Füürball. Verbrennt Gegner und schmelzt Iis.','La siempre leal bola de fuego. Quema enemigos y derrite el hielo.');

-- Anti Gravity

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('sp_antigravity', 'spell', 150, 400, -1, 3);
INSERT INTO item_spell (item_id, spell_id) values ('sp_antigravity', 6);
INSERT INTO text (text_id, text_type, english, german, swiss_german, spanish) values ('sp_antigravity', 'item','Scroll - Anti Gravity','Schriftrolle - Schwerkraft umkehren','Schriftrolle - Schwerchraft umchere', 'Pergamino - Antigravedad');
INSERT INTO text (text_id, text_type, english, german, swiss_german, spanish) values ('sp_antigravity', 'item_desc','Reverses gravity for Cendric for a certain duration.','Dreht Cendrics Schwerkraft für eine gewisse Zeit um.','Drüllt em Cendric sini Schwerchraft für e gwüssi Ziit um.','Invierte la gravedad de Cendric por cierto tiempo.');
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('sp_antigravity', 0, -30, 50, 20);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('sp_antigravity', 1, 150, 400, 50, 50);

-- Summon Gargoyle

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('sp_summongargoyle', 'spell', 250, 400, -1, 3);
INSERT INTO item_spell (item_id, spell_id) values ('sp_summongargoyle', 16);
INSERT INTO text (text_id, text_type, english, german, swiss_german, spanish) values ('sp_summongargoyle', 'item','Scroll - Summon Gargoyle','Schriftrolle - Gargoyle Beschwören','Schriftrolle - Gargoyle Beschwöre', 'Pergamino - Convocación de gárgola');
INSERT INTO text (text_id, text_type, english, german, swiss_german, spanish) values ('sp_summongargoyle', 'item_desc','Cendric summons a mighty gargoyle that serves him and attacks enemies.','Cendric beschwört einen mächtigen Gargoyle, der Gegner attackiert.','De Cendric beschwört en mächtige Gargoyle wo Gegner aagrift.','Cendric convoca una poderosa gárgola a su servicio y ataca enemigos.');
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('sp_summongargoyle', 0, -30, 50, 20);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('sp_summongargoyle', 1, 500, 350, 50, 50);

-- Aureola

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('sp_aureola', 'spell', 300, 400, -1, 3);
INSERT INTO item_spell (item_id, spell_id) values ('sp_aureola', 21);
INSERT INTO text (text_id, text_type, english, german, swiss_german, spanish) values ('sp_aureola', 'item','Scroll - Aureola','Schriftrolle - Strahlenkranz','Schriftrolle - Strahlechranz', 'Pergamino - Aureola');
INSERT INTO text (text_id, text_type, english, german, swiss_german, spanish) values ('sp_aureola', 'item_desc','Sends balls of light in all directions, if they hit an enemy they will return to Cendric and heal him.','Schickt Lichtbälle in alle Richtungen, die zurückkehren und Cendric heilen, falls sie einen Gegner treffen.','Schickt Liechtbäll in all Richtige, wo zruggchömed und de Cendric heiled wenn si en Gegner troffe hend.','Arroja esferas de luz en todas direcciones. Si golpean a un enemigo, regresan a Cendric y lo curan.');
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('sp_aureola', 0, -30, 50, 20);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('sp_aureola', 1, 500, 400, 50, 50);

-- Icy Ambush

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value, rarity) values ('sp_icyambush', 'spell', 200, 400, -1, 3);
INSERT INTO item_spell (item_id, spell_id) values ('sp_icyambush', 11);
INSERT INTO text (text_id, text_type, english, german, swiss_german, spanish) values ('sp_icyambush', 'item','Scroll - Icy Ambush','Schriftrolle - Eisiger Hinterhalt','Schriftrolle - Iischalte Hinterhalt', 'Pergamino - Emboscada helada');
INSERT INTO text (text_id, text_type, english, german, swiss_german, spanish) values ('sp_icyambush', 'item_desc','Cendric throws an enchanted dagger that deals big damage when it hits an enemy, stuns it and enemies in range and teleports Cendric to its location.','Cendric wirft einen verzauberten Dolch, der eine grosse Menge Schaden verursacht, wenn er einen Gegner trifft und Cendric an dessen Stelle teleportiert. Gegner im Umkreis werden betäubt.','De Cendric wirft en verzauberte Dolch wo viel Schade amne Gegner macht und Gegner im Umkreis betäubt. De Cendric wird ad Stell vom troffene Gegner teleportiert.','Cendric lanza una daga encantada que hace gran daño cuando golpea a un enemigo, aturde a este y a los enemigos al alcance y lo teletransporta a su ubicación.');
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('sp_icyambush', 0, -30, 50, 20);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('sp_icyambush', 1, 150, 0, 50, 50);

