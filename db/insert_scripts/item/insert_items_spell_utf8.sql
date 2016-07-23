-- Light

INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('sp_light', 'spell', 300, 400, -1);
INSERT INTO item_spell (item_id, spell_id) values ('sp_light', 17);
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('sp_light', 'item', 'Scroll - Light','Schriftrolle - Licht','Schriftrolle - Liecht');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('sp_light', 'item_desc', 'There shall be light!','Es werde Licht!','Chasch chli heller mache, bitte?');
