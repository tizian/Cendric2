-- Note (Paladin Marcus)

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('do_notemarcus', 'item', 'Note','Notiz','Notiz');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('do_notemarcus', 'item_desc', 'I''ve found this note nailed to a post in the Howling Caverns.','Diese Notiz habe ich in den Heulenden Höhlen gefunden, an einen Pfosten genagelt.','Die Notiz hani anen Pfoste gnaglet gfunde, ide Hüülende Höhle.');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('do_notemarcus', 'item_text', 'To whoever reads this: Beware!

These caverns are full of traps, I only made it with a lot of luck to where I am now. The five levers are designed to kill you if you pull the wrong one. Use the last one first and the first one last and leave the others untouched.

I hope I''ll find a way out of this cave...

-- Paladin Marcus','Wer das auch immer liest: Nehmt Euch in Acht!

Diese Höhlen sind gspickt mit Fallen, ich habe es nur mit viel Glück da hin geschafft, wo ich jetzt bin. Die fünf Hebel werden Euch töten wenn Ihr den falschen zieht. Benutzt den letzten als erstes und den ersten als letztes und berührt keinen der anderen.

Ich hoffe, ich komme irgendwie wieder aus diesen Höhlen hinaus...

-- Paladin Marcus','Wer das auch immer lese tuet: Passed uuf!

Die Höhle sind voll mit Fallene, ich has nur mit viel Glück det ane gschafft, woni ez bin. Di füf Hebel werded jede töte, wo am falsche ziet. Druck de letzt als ersts und de erst als letzts und langed di andere ja nid ah!

Ich hoff, ich chum irgendwie wider us dene Höhlene use...

-- Paladin Marcus');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('do_notemarcus', 'document', 400, 400, -1);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('do_notemarcus', -25, 0, 50, 50);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('do_notemarcus', 1, 450, 0, 100, 50);

-- Bloodstained Note (Paladin Marcus)

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('do_bloodnotemarcus', 'item', 'Bloodstained Note','Blutverschmierte Notiz','Bluetverschmierti Notiz');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('do_bloodnotemarcus', 'item_desc', 'Paladin Marcus must have scribbled those words on this sheet of paper before he was killed.','Paladin Marcus muss das hingekritzelt haben, bevor er getötet wurde.','De Paladin Marcus mun das anekritzlet ha bevor er töted worde isch.');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('do_bloodnotemarcus', 'item_text', 'The beast! Transforms...

 Humans into wolves
 
 Look away!!
 They keep coming
		surrounded, I''ll fight.','Das Biest!
Verwandelt...

 Menschen in Wölfe
 
	Schau weg!!
	Es kommen immer mehr
		eingekreist, Ich muss kämpfen.'
 ,'Es Biest!
 Verwandlet...
 
 Mensche i Wölf

	Lueg weg!!
	Es chömed immer me
		umzingled, ich mun kämpfe.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('do_bloodnotemarcus', 'document', 450, 400, -1);

-- Letter (Elementalists Letter)

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('do_eleletter', 'item', 'Letter','Brief','Brief');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('do_eleletter', 'item_desc', 'I found this letter in the old travel bag that belonged to me once.','Ich habe diesen Brief in dem alten Reiserucksack gefunden, der einmal mir gehört hat.','Ich ha de Brief im alte Reiserucksack gfunde, wo mal mir ghört het.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('do_eleletter', 'document', 500, 300, -1);

-- Map (Elementalists Map)

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('do_elemap', 'item', 'Map','Karte','Charte');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('do_elemap', 'item_desc', '...','...','...');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('do_elemap', 'document', 550, 400, -1);

