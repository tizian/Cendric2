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
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('mi_gargoyle_dust', 'item_desc', 'Once a mighty stone monster, now a pityful pile of dust.','War einmal ein mächtiges Steimonster  und ist jetzt nur noch ein trauriges Häufchen Staub.','Isch mal es mächtigs Steimonster gsi und isch ez nur na es elendiglichs Hüüfeli Staub.');
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

-- Broken Shaft (part 1 of the broken staff)

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('mi_brokenstaff1', 'item', 'Broken Shaft','Zerbrochener Schaft','Kabutte Schaft');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('mi_brokenstaff1', 'item_desc', 'This shaft seems to be a part of something bigger. Maybe I can find someone who can repair it.','Dieser Schaft scheint ein Teil von etwas grösserem zu sein. Vielleicht finde ich jemanden, der das reparieren kann.','De Schaft isch glaub en Teil vo öppis grösserem. Vilicht chani öpper finde, wo das flicke chan.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('mi_brokenstaff1', 'misc', 450, 50, -1);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('mi_brokenstaff1', 0, -20, 50, 40);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('mi_brokenstaff1', 1, 300, 150, 50, 100);

-- Broken Staff Head (part 2 of the broken staff)

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('mi_brokenstaff2', 'item', 'Broken Staff Head','Zerbrochener Stabkopf','Kabutte Stabchopf');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('mi_brokenstaff2', 'item_desc', 'Seems like a part of a staff. Maybe I can find someone who can repair it.','Scheint ein Stück eines Stabes zu sein. Vielleicht finde ich jemanden, der das reparieren kann.','Da isch glaub es Stück vomne Stab. Vilicht chani öpper finde, wo das flicke chan.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('mi_brokenstaff2', 'misc', 550, 0, -1);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('mi_brokenstaff2', 0, -20, 50, 40);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('mi_brokenstaff2', 1, 400, 250, 50, 100);

-- Tear Stone (part 3 of the broken staff)

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('mi_brokenstaff3', 'item', 'Tear Stone','Tränenstein','Tränestei');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('mi_brokenstaff3', 'item_desc', 'A tear-shaped stone which focuses magic energy around it. Would fit perfectly into a staff head.','Ein tränenförmiger Stein, der die Magie um sich herum konzentriert. Er würde perfekt in einen Stabkopf passen.','En träneförmige Stei wo d''Magie um sich konzentriert. Er wür perfekt inen Stabchopf passe.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('mi_brokenstaff3', 'misc', 550, 50, -1);

-- Heart of the First Guardian

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('mi_firstguardianheart', 'item', 'Heart of Khajag','Herz von Khajag','Herz vom Khajag');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('mi_firstguardianheart', 'item_desc', 'This gem is the only thing left from the giant wolf guardian of the howling caverns.','Dieses Juwel ist das Einzige, was vom riesigen Wächterwolf aus den Heulenden Höhlen noch übrig ist.','Da Juwel ischs Einzig, wo vom riesige Wächterwolf us de Hüülende Höhle übrig blibe isch.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('mi_firstguardianheart', 'misc', 450, 350, -1);

-- Stone

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('mi_stone', 'item', 'Stone','Stein','Stei');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('mi_stone', 'item_desc', 'A small stone.','Ein kleiner Stein.','En chline stei.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('mi_stone', 'misc', 250, 350, 4);

-- Corrupt stone

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('mi_corrupt_stone', 'item', 'Corrupted Stone','Verdorbener Stein','Verdorbene Stei');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('mi_corrupt_stone', 'item_desc', 'This stone has been corrupted by a Cairn Wraith and is now infused with magical powers.','Dieser Stein wurde von einem Cairn Geist verdorben und ist nun mit magischen Kräften durchzogen.','De stei isch voneme Cairn Geist verdorbe worde und isch jetzt vo magische Chräft durchzoge.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('mi_corrupt_stone', 'misc', 300, 350, 28);