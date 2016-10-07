-- Rope

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('qe_rope', 'item', 'Rope','Seil','Seil');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('qe_rope', 'item_desc', 'A strong coil of hemp rope.','Ein starkes Seil, aus Hanf gefertigt.','Es starchs Hanfseil.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('qe_rope', 'quest', 50, 0, -1);

-- Silkweed

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('qe_silkweed', 'item', 'Silkweed','Seidenkraut','Sidechrütli');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('qe_silkweed', 'item_desc', 'Will hopefully bring my memories back when brewed into a potion.','Ein Trank daraus wird hoffentlich meine Erinnerungen zurückbringen.','En Trank mit dem wird hoffentlich mini Erinnerige zruggbringe.');
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
-- Levelitem not used here, values are commented out for later use.
--INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('qe_recommendationletter', 0, -30, 50, 20);
--INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('qe_recommendationletter', 1, 150, 0, 50, 50);

-- Compass

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('qe_compass', 'item', 'Compass', 'Kompass', 'Kompass');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('qe_compass', 'item_desc', 'A golden compass I found with my old belongings. It still works.', 'Ein goldener Kompass, den ich bei meinen alten Habseligkeiten gefunden habe. Er funktioniert noch.', 'En goldige Kompass woni bi mine alte Sache gfunde ha. Er tuet na.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('qe_compass', 'quest', 500, 400, -1);

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
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('qe_fireessence', 'quest', 550, 350, -1);

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