-- Rope

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('qe_rope', 'item', 'Rope','Seil','Seil');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('qe_rope', 'item_desc', 'A strong coil of hemp rope.','Ein starkes Seil, aus Hanf gefertigt.','Es starchs Hanfseil.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('qe_rope', 'quest', 50, 0, -1);

-- Tower Key 1 (Part)

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('qe_tower_1', 'item', 'Green Key Head','Grüner Schlüsselkopf','Grüene Schlüsselchopf');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('qe_tower_1', 'item_desc', 'A part of the key is missing.','Ein Teil des Schlüssels fehlt.','En Teil vom Schlüssel fehlt.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('qe_tower_1', 'quest', 300, 700, -1);

-- Tower Key 2 (Part)

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('qe_tower_2', 'item', 'Yellow Key Head','Gelber Schlüsselkopf','Gääle Schlüsselchopf');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('qe_tower_2', 'item_desc', 'A part of the key is missing.','Ein Teil des Schlüssels fehlt.','En Teil vom Schlüssel fehlt.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('qe_tower_2', 'quest', 350, 700, -1);

-- Tower Key 3 (Part)

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('qe_tower_3', 'item', 'Orange Key Head','Oranger Schlüsselkopf','Orange Schlüsselchopf');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('qe_tower_3', 'item_desc', 'A part of the key is missing.','Ein Teil des Schlüssels fehlt.','En Teil vom Schlüssel fehlt.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('qe_tower_3', 'quest', 400, 700, -1);

-- Tower Key 4 (Part)

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('qe_tower_4', 'item', 'Red Key Head','Roter Schlüsselkopf','Rote Schlüsselchopf');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('qe_tower_4', 'item_desc', 'A part of the key is missing.','Ein Teil des Schlüssels fehlt.','En Teil vom Schlüssel fehlt.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('qe_tower_4', 'quest', 450, 700, -1);

-- Silkweed

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('qe_silkweed', 'item', 'Silkweed','Seidenkraut','Sidechrütli');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('qe_silkweed', 'item_desc', 'Will hopefully bring my memories back when brewed into a potion.','Ein Trank daraus wird hoffentlich meine Erinnerungen zurückbringen.','En Trank mit dem Chrütli wird hoffentlich mini Erinnerige zruggbringe.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('qe_silkweed', 'quest', 150, 250, -1);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('qe_silkweed', -5, -25, 40, 70);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('qe_silkweed', 1, 100, 150, 50, 100);

-- Spoiled Feudal Fire

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('qe_spoiledfeudalfire', 'item', 'Feudal Fire (Spoiled)', 'Feudales Feuer (Verdorben)', 'Feudals Füür (Verdorbe)');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('qe_spoiledfeudalfire', 'item_desc', 'Vincent has mixed something in there. It doesn''t seem safe to drink anymore.', 'Vincent hat etwas hinein gemischt. Es ist vermutlich nicht mehr sicher, das zu trinken.', 'De Vincent het da öppis ine gmischlet. Es isch vermuetlich nümme sicher, das z''trinke.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('qe_spoiledfeudalfire', 'quest', 350, 250, -1);

-- Proof of the Monster's Death

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('qe_proofmonsterdeath', 'item', 'Proof of the Monster''s Death','Beweis für den Tod des Monsters','Bewiis für de Tod vom Monster');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('qe_proofmonsterdeath', 'item_desc', 'The monster''s heart - A better proof that the enormous wolf was slain doesn''t exist.','Das Herz des Monsters - Es gibt keinen besseren Beweis für das Ableben des gigantischen Wolfs.','Es Härz vom Monster - Es git ken bessere Bewiis defür, dasi de riisig Wolf abgmurkst han.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('qe_proofmonsterdeath', 'quest', 300, 300, -1);

-- Letter of Recommendation

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('qe_recommendationletter', 'item', 'Letter of Recommendation','Empfehlungsschreiben','Empfehligsschriibe');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('qe_recommendationletter', 'item_desc', 'A letter of recommendation from the High Priestess Inina. It is sealed.','Ein Empfehlungsschreiben der Hohepriesterin Inina. Es ist versiegelt.','Es Empfehligsschriibe vode Hohepriesterin Inina. Es isch versiglet.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('qe_recommendationletter', 'quest', 200, 0, -1);

-- Compass

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('qe_compass', 'item', 'Compass', 'Kompass', 'Kompass');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('qe_compass', 'item_desc', 'A golden compass. It still works.', 'Ein goldener Kompass. Er funktioniert noch.', 'En goldige Kompass. Er tuet na.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('qe_compass', 'quest', 500, 400, -1);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('qe_compass', -15, -35, 20, 15);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('qe_compass', 1, 550, 250, 50, 50);

-- Broken Shaft (part 1 of the broken staff)

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('qe_brokenstaff1', 'item', 'Broken Shaft','Zerbrochener Schaft','Kabutte Schaft');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('qe_brokenstaff1', 'item_desc', 'This shaft seems to be a part of something bigger. Maybe I can find someone who can repair it.','Dieser Schaft scheint ein Teil von etwas grösserem zu sein. Vielleicht finde ich jemanden, der das reparieren kann.','De Schaft isch glaub en Teil vo öppis grösserem. Vilicht chani öpper finde, wo das flicke chan.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('qe_brokenstaff1', 'quest', 450, 50, -1);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('qe_brokenstaff1', 0, -20, 50, 40);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('qe_brokenstaff1', 1, 300, 150, 50, 100);

-- Broken Staff Head (part 2 of the broken staff)

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('qe_brokenstaff2', 'item', 'Broken Staff Head','Zerbrochener Stabkopf','Kabutte Stabchopf');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('qe_brokenstaff2', 'item_desc', 'Seems like a part of a staff. Maybe I can find someone who can repair it.','Scheint ein Stück eines Stabes zu sein. Vielleicht finde ich jemanden, der das reparieren kann.','Da isch glaub es Stück vomne Stab. Vilicht chani öpper finde, wo das flicke chan.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('qe_brokenstaff2', 'quest', 550, 0, -1);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('qe_brokenstaff2', 0, -20, 50, 40);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('qe_brokenstaff2', 1, 400, 250, 50, 100);

-- Tear Stone (part 3 of the broken staff)

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('qe_brokenstaff3', 'item', 'Tear Stone','Tränenstein','Tränestei');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('qe_brokenstaff3', 'item_desc', 'A tear shaped stone which focuses magic energy around it. Would fit perfectly into a staff head.','Ein tränenförmiger Stein, der die Magie um sich herum konzentriert. Er würde perfekt in einen Stabkopf passen.','En träneförmige Stei wo d''Magie um sich konzentriert. Er wür perfekt inen Stabchopf passe.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('qe_brokenstaff3', 'quest', 550, 50, -1);

-- Essence of Fire

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('qe_fireessence', 'item', 'Essence of Fire','Essenz des Feuers','Füüressenz');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('qe_fireessence', 'item_desc', 'From the lungs of a fire rat.','Aus den Lungen einer Feuerratte.','Us de Lunge vonere Füürratte');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('qe_fireessence', 'quest', 350, 550, -1);

-- Heart of a Fire Rat

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('qe_fireratheart', 'item', 'Heart of a Fire Rat','Feuerrattenherz','Füürratteherz');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('qe_fireratheart', 'item_desc', 'Eww!','Igitt!','Wää!');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('qe_fireratheart', 'quest', 300, 300, -1);

-- Blood of a Bat

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('qe_batblood', 'item', 'Blood of a Bat','Fledermausblut','Fledermuusbluet');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('qe_batblood', 'item_desc', 'A small vial containing a few drops of blood.','Eine kleine Phiole mit ein paar Tropfen Blut darin.','E chliini Phiole mit paar Tropfe Bluet drin.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('qe_batblood', 'quest', 400, 550, -1);

-- Bone

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('qe_bone', 'item', 'Bone','Knochen','Chnoche');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('qe_bone', 'item_desc', 'I really don''t want to know what kind of bone it is.','Ich will wirklich nicht wissen was das für ein Knochen ist.','Ich wett würkli gar nid wüsse wa da füren Chnoche isch.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('qe_bone', 'quest', 450, 550, -1);

-- Cinderbloom

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('qe_cinderbloom', 'item', 'Cinderbloom','Aschenblüte','Zündelblueme');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('qe_cinderbloom', 'item_desc', 'It only prospers on the grave of someone that died in a fire.','Sie gedeiht nur auf dem Grab von jemandem, der im Feuer verendet ist.','Si wachst nur uf em Grab vo öpperem vo imne Füür gstorbe isch.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('qe_cinderbloom', 'quest', 150, 600, -1);

-- Recipe: Invisibility Potion 

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('qe_invisrecipe', 'item', 'Recipe: Invisibility Potion','Rezept: Unsichtbarkeitstrank','Rezept: Unsichtbarkeitstrank');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('qe_invisrecipe', 'item_desc', 'I stole this recipe from Syrah''s basement.','Dieses Rezept habe ich aus Syrah''s Keller entwendet.','Da Rezept hani us de Syrah ihrem Chäller klaut.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('qe_invisrecipe', 'quest', 600, 0, -1);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('qe_invisrecipe', 0, -30, 50, 20);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('qe_invisrecipe', 1, 150, 0, 50, 50);

-- Graham's Dulcimer 

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('qe_dulcimer', 'item', 'Graham''s Dulcimer','Graham''s Dulcimer', 'Em Graham sini Dulcimer');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('qe_dulcimer', 'item_desc', 'What a beautiful instrument! It belongs to the bard Graham.','Was für ein schönes Instrument! Es gehört dem Barden Graham.','Was füres schöns Instrument! Es ghört em Bard Graham.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('qe_dulcimer', 'quest', 800, 200, -1);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('qe_dulcimer', 0, 0, 50, 50);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('qe_dulcimer', 1, 550, 350, 50, 50);

-- Frying Pan

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('qe_jackpan', 'item', 'Frying Pan','Bratpfanne','Bratpfanne');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('qe_jackpan', 'item_desc', 'Jack''s beloved frying pan.','Jack''s geliebte Bratpfanne.', 'Em Jack sini gliebt Bratpfanne.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('qe_jackpan', 'quest', 600, 650, -1);

-- Helena's Special Chicken

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('qe_leeroychicken', 'item', 'Helena''s Special Chicken','Helena''s Hühnchen Spezial','Helena''s Spezialgüggeli');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('qe_leeroychicken', 'item_desc', 'Helena''s famous grilled chicken.','Helena''s berühmtes Brathühnchen.', 'De Helena ihres berüemt Güggeli.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('qe_leeroychicken', 'quest', 650, 250, -1);

-- Precious Candleholder

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('qe_divinecandleholder', 'item', 'Precious Candleholder','Wertvoller Kerzenständer','Wertvolle Cherzeständer');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('qe_divinecandleholder', 'item_desc', 'An extraordinary candleholder, endowed with precious jewels.','Ein aussergewöhnlicher Kerzenständer, mit wertvollen Juwelen besetzt.', 'En ussergwöhnliche Cherzeständer, mit wertvolle Juwele gschmückt.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('qe_divinecandleholder', 'quest', 650, 300, -1);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('qe_divinecandleholder', -4, -7, 37, 43);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('qe_divinecandleholder', 1, 600, 00, 50, 50);

-- Necrotic Grimoire

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('qe_necrobook', 'item', 'Necrotic Grimoire','Nekrotisches Zauberbuch','Nekrotischs Zauberbuech');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('qe_necrobook', 'item_desc', 'I hope it won''t come to live again.','Ich hoffe, es erwacht nicht wieder zum Leben.', 'Ich hoff, es wird nid wieder lebendig.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('qe_necrobook', 'quest', 650, 350, -1);

-- Inactive Teleport Stone

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('qe_portstone_inactive', 'item', 'Teleport Stone (Inactive)','Teleportstein (Inaktiv)','Teleportstei (Inaktiv)');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('qe_portstone_inactive', 'item_desc', 'This stone has to be activated with the magic of a Cairn Wraith in order to be used.','Dieser Stein muss zuerst mit der Magie eines Cairn Geistes aktiviert werden, damit ich ihn benutzen kann.','De Stei mu zersch mit de Magie vomne Cairn Geist aktiviert werde dasin bruche chan.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('qe_portstone_inactive', 'quest', 700, 350, -1);

-- Robert's Teleport Stone

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('qe_portstone_bob', 'item', 'Robert''s Teleport Stone','Robert''s Teleportstein','Em Robert sin Teleportstei');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('qe_portstone_bob', 'item_desc', 'A magical teleport stone. I can''t use it as it''s bound to the mage Robert.','Ein magischer Teleportstein. Ich kann ihn nicht benutzen, da er an den Magier Robert gebunden ist.','En magische Teleportstei. Ich chanen nid bruche weler an Magier Robert bunde isch.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('qe_portstone_bob', 'quest', 700, 300, -1);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('qe_portstone_bob', -15, -35, 20, 15);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('qe_portstone_bob', 1, 650, 250, 50, 50);

-- Heart of Thunder

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('qe_thunderheart', 'item', 'Heart of Thunder','Donnerherz','Donnerherz');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('qe_thunderheart', 'item_desc', 'A precious stone which holds magical powers.','Ein wertvoller Stein mit magischen Kräften.','En wertvolle Stei mit magische Chräft.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('qe_thunderheart', 'quest', 700, 650, -1);

-- Yaslaw's Mask

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('qe_yashamask', 'item', 'Yaslaw''s Mask','Yaslaw''s Maske','Maske vom Yaslaw');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('qe_yashamask', 'item_desc', 'This mask granted the necromancer Yaslaw special powers.','Diese Maske hat dem Nekromanten Yaslaw spezielle Kräfte verliehen.','Die Maske het em Yaslaw spezielli Chräft gä.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('qe_yashamask', 'quest', 750, 0, -1);

-- Yasha's Essence

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('qe_yashaessence', 'item', 'Yasha''s Essence','Yasha''s Essenz','Yasha''s Essenz');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('qe_yashaessence', 'item_desc', 'The fiery essence of the powerful demon Yasha.','Die feurige Essenz der mächtigen Dämonin Yasha.','Di füürig Essenz vode mächtige Dämonin Yasha.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('qe_yashaessence', 'quest', 350, 550, -1);