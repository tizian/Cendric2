-- Golden Goblet

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('mi_goldengoblet', 'item', 'Golden goblet','Goldener Pokal','Goldige Pokal');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('mi_goldengoblet', 'item_desc', 'A golden goblet, some merchants might pay a lot for this.','Ein goldener Pokal, manche Händler würden viel dafür bezahlen.','En goldige Pokal, gwüssi Händler zahled sicher vil defür.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('mi_goldengoblet', 'misc', 0, 50, 20);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('mi_goldengoblet', -10, -10, 30, 40);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('mi_goldengoblet', 1, 50, 0, 50, 50);

-- Silver Plate

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('mi_silverplate', 'item', 'Silver Plate','Silberner Teller','Silbrige Teller');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('mi_silverplate', 'item_desc', 'I can see my reflection in the glossy surface.','Ich kann mich darin spiegeln.','Ich chan mich drin spiegle.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('mi_silverplate', 'misc', 250, 200, 15);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('mi_silverplate', 0, -30, 50, 20);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('mi_silverplate', 1, 50, 150, 50, 50);

-- Candleholder

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('mi_candleholder', 'item', 'Candleholder','Kerzenleuchter','Cherzeständer');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('mi_candleholder', 'item_desc', 'Made of silver, must be worth something.','Aus Silber gefertigt, kann man sicher gut verkaufen.','Us Silber gmacht, cha me sicher guet verkaufe.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('mi_candleholder', 'misc', 300, 200, 18);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('mi_candleholder', 0, -40, 50, 65);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('mi_candleholder', 1, 300, 0, 50, 100);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('mi_candleholder', 2, 350, 0, 50, 100);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('mi_candleholder', 3, 400, 0, 50, 100);
INSERT INTO levelitem_light(item_id, light_offset_x, light_offset_y, light_radius_x, light_radius_y) values ('mi_candleholder', 25, 40, 350, 300);

-- Ice Crystal

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('mi_icecrystal', 'item', 'Ice Crystal','Eiskristall','Iiskristall');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('mi_icecrystal', 'item_desc', 'An exeptionally beautiful crystal. Maybe someone could craft something from it.','Ein aussergewöhnlich schöner Kristall. Vielleicht könnte jemand etwas daraus anfertigen.','En ussergwöhnlich schöne Kristall. Vilicht chönt öpper öppis drus herstelle.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('mi_icecrystal', 'misc', 600, 500, 30);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('mi_icecrystal', -3, -10, 44, 40);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('mi_icecrystal', 1, 300, 300, 50, 50);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('mi_icecrystal', 3, 350, 300, 50, 50);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('mi_icecrystal', 2, 450, 300, 50, 50);
INSERT INTO levelitem_light(item_id, light_offset_x, light_offset_y, light_radius_x, light_radius_y) values ('mi_icecrystal', 25, 40, 350, 300);

-- Elysia's Tail Feathers

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('mi_elysiatail', 'item', 'Elysia''s Tail Feathers','Elysia''s Schwanzfedern','Schwanzfädere vode Elysia');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('mi_elysiatail', 'item_desc', 'This tail was cut from the storm bird Elysia. Maybe someone could craft something from it.','Dieser Schwanz wurde dem Sturmvogel Elysia abgehackt. Vielleicht könnte jemand etwas daraus anfertigen.','De Schwanz isch em Sturmvogel Elysia abghackt worde. Vilicht chönt öpper öppis drus herstelle.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('mi_elysiatail', 'misc', 650, 50, -1);

-- Teeth of a Nekomata

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('mi_neko_teeth', 'item', 'Fangs of a Nekomata','Zähne einer Nekomata','Fangzää vonere Nekomata');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('mi_neko_teeth', 'item_desc', 'It is rumoured that these enormous teeth have magical powers.','Diesen gewaltigen Zähnen wird nachgesagt, dass sie magische Kräfte zu besitzen.','Me munklet dass die riesige Fangzää magischi Chräft hend.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('mi_neko_teeth', 'misc', 250, 0, 10);

-- Fur of a Nekomata

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('mi_neko_fur', 'item', 'Fur of a Nekomata','Fell einer Nekomata','Fell vonere Nekomata');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('mi_neko_fur', 'item_desc', 'This fur is worth a lot on the market as these demonic cats are rarely spotted and hard to defeat.','Dieses Fell ist eine Menge wert auf dem Markt, da die dämonischen Katzen selten anzutreffen und schwierig zu besiegen sind.','Da Fell isch recht viel wert ufem Markt well die dämonische Chatze selte und schwierig z''besiege sind.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('mi_neko_fur', 'misc', 300, 0, 30);

-- Fur of a Fire Rat

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('mi_firerat_fur', 'item', 'Fur of a Fire Rat','Fell einer Feuerratte','Fell vonere Füürratte');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('mi_firerat_fur', 'item_desc', 'This fur is exceptionally well preserved. Maybe someone can use it to craft a piece of armour out of it.','Diese Fell ist aussergewöhnlich gut erhalten. Vielleicht kann es jemand zu einer Rüstung verarbeiten.','Da Fell isch ussergwöhnlich guet erhalte. Vilicht chan öpper da drus e Rüstig mache.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('mi_firerat_fur', 'misc', 200, 350, -1);

-- Fur of a Wolf

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('mi_wolf_fur', 'item', 'Wolf Fur','Wolfsfell','Wolfspelz');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('mi_wolf_fur', 'item_desc', 'Fluffy and soft.','Flauschig und weich.','Flauschig und weich.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('mi_wolf_fur', 'misc', 400, 300, 20);

-- Pile of gargoyle dust

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('mi_gargoyle_dust', 'item', 'Pile of gargoyle dust','Häufchen von Gargoylestaub','Es Hüüfeli Gargoylestaub');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('mi_gargoyle_dust', 'item_desc', 'Once a mighty stone monster, now a pityful pile of dust.','War einmal ein mächtiges Steimonster und ist jetzt nur noch ein trauriges Häufchen Staub.','Isch mal es mächtigs Steimonster gsi und isch ez nur na es elendiglichs Hüüfeli Staub.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('mi_gargoyle_dust', 'misc', 350, 0, 20);

-- Teeth

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('mi_teeth', 'item', 'Teeth','Zähne','Zää');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('mi_teeth', 'item_desc', 'Small and pointy.','Klein und spitz.','Chlii und spitzig.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('mi_teeth', 'misc', 150, 0, 5);

-- Feather

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('mi_feather', 'item', 'Feather','Feder','Fädere');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('mi_feather', 'item_desc', 'This tail feather is long and well preserved.','Diese Schwanzfeder ist lang und gut erhalten.','Die Schwanzfädere isch lang und guet erhalte.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('mi_feather', 'misc', 450, 100, 2);

-- Pearl

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('mi_pearl', 'item', 'Pearl','Perle','Perle');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('mi_pearl', 'item_desc', 'Can be found in some shells.','Kann man manchmal in Muscheln finden.','Cha me mengsmal in Muschle finde.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('mi_pearl', 'misc', 400, 250, 15);

-- Heart of the First Guardian

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('mi_firstguardianheart', 'item', 'Heart of Khajag','Herz von Khajag','Herz vom Khajag');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('mi_firstguardianheart', 'item_desc', 'This gem is the only thing left from the giant wolf guardian of the howling caverns.','Dieses Juwel ist das Einzige, was vom riesigen Wächterwolf aus den Heulenden Höhlen noch übrig ist.','Da Juwel ischs Einzig, wo vom riesige Wächterwolf us de Hüülende Höhle übrig blibe isch.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('mi_firstguardianheart', 'misc', 450, 350, -1);

-- Stone

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('mi_stone', 'item', 'Stone','Stein','Stei');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('mi_stone', 'item_desc', 'A small stone.','Ein kleiner Stein.','En chliine Stei.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('mi_stone', 'misc', 250, 350, 4);

-- Corrupt stone (Ice)

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('mi_corrupt_stone_ice', 'item', 'Corrupted Ice Stone','Verdorbener Eisstein','Verdorbene Iisstei');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('mi_corrupt_stone_ice', 'item_desc', 'This stone has been corrupted by a Cairn Wraith and is now infused with magical powers.','Dieser Stein wurde von einem Cairn Geist verdorben und ist nun mit magischen Kräften durchzogen.','De stei isch voneme Cairn Geist verdorbe worde und isch jetzt vo magische Chräft durchzoge.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('mi_corrupt_stone_ice', 'misc', 300, 350, 28);

-- Corrupt stone (Fire)

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('mi_corrupt_stone_fire', 'item', 'Corrupted Fire Stone','Verdorbener Feuerstein','Verdorbene Füürstei');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('mi_corrupt_stone_fire', 'item_desc', 'This stone has been corrupted by a Cairn Wraith and is now infused with magical powers.','Dieser Stein wurde von einem Cairn Geist verdorben und ist nun mit magischen Kräften durchzogen.','De stei isch voneme Cairn Geist verdorbe worde und isch jetzt vo magische Chräft durchzoge.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('mi_corrupt_stone_fire', 'misc', 700, 0, 28);

-- Teleport Stone

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('mi_portstone', 'item', 'Teleport Stone','Teleportstein','Teleportstei');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('mi_portstone', 'item_desc', 'I am able to teleport to activated waypoints using this stone.','Mit diesem Stein kann ich mich zu aktivierten Wegpunkten teleportieren.','Mit dem Stei chani mich zu aktivierte Wegpunkt teleportiere.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('mi_portstone', 'misc', 700, 150, -1);

-- Pile of Magical Pearls

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('mi_pearlpile', 'item', 'Pile of Magical Pearls','Haufen Magischer Perlen','Es Hüüfli Magischi Perle');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('mi_pearlpile', 'item_desc', 'Maybe someone could use these pearls to craft something.','Vielleicht könnte jemand aus diesen Perlen etwas herstellen.','Vilicht chönt öpper us dene Perle öppis herstelle.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('mi_pearlpile', 'misc', 300, 650, 50);

-- Broken Sickles

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('mi_janusglaive', 'item', 'Broken Sickles','Zerbrochene Sicheln','Verbrocheni Sichle');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('mi_janusglaive', 'item_desc', 'The broken remains of Arastor''s weapon. Maybe someone could fix it.','Die zerbrochenen Überreste von Arastor''s Waffe. Vielleicht könnte sie jemand wieder zusammensetzen.','Di verbrochene Überreste vom Arastor sinere Waffe. Vilicht chönt si öppert wieder zemesetze.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('mi_janusglaive', 'misc', 650, 200, 10);

-- Iron Ore

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('mi_ironore', 'item', 'Iron Ore','Eisenerz','Iseerz');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('mi_ironore', 'item_desc', 'A sought-after ingredient for smithing.','Eine begehrte Schmiedezutat.','E begehrti Schmiedezuetat.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('mi_ironore', 'misc', 200, 500, 1);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('mi_ironore', 0, -35, 30, 15);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('mi_ironore', 1, 500, 50, 50, 50);

-- Shiny Stone

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('mi_shinystone', 'item', 'Shiny Stone','Funkelnder Stein','Funklige Stei');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('mi_shinystone', 'item_desc', 'What a beautiful stone. A merchant might pay a lot for this.','Welch schöner Stein. Ein Händler könnte einiges dafür bezahlen.','Wa füren schöne Stei. En Händler chönt einiges defür zahle.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('mi_shinystone', 'misc', 150, 550, 10);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('mi_shinystone', -25, -40, 10, 25);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('mi_shinystone', 1, 500, 150, 50, 50);

-- Sextant

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('mi_sextant', 'item', 'Sextant','Sextant','Sextant');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('mi_sextant', 'item_desc', 'I have no clue how this thing works, but it seems to be made from massive gold.','Ich habe keine Ahnung wie dieses Ding funktioniert, aber es scheint aus massivem Gold zu sein.','Ich ha ken Plan we das Dingens funktioniert, aber es isch glaub us massivem Gold.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('mi_sextant', 'misc', 500, 550, 100);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('mi_sextant', -5, -20, 40, 35);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('mi_sextant', 1, 550, 0, 50, 100);

-- Dragon Skull

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('mi_dragonskull', 'item', 'Dragon Skull','Drachenschädel','Dracheschädel');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('mi_dragonskull', 'item_desc', 'This one will no longer breathe fire.','Der spuckt kein Feuer mehr.','De spuckt kei Füür me.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('mi_dragonskull', 'misc', 0, 200, 10);