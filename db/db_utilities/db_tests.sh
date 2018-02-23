#/bin/sh

echo '#########################'
echo '#       DB TEST 1       #'
echo '#   Max Choice Length   #'
echo '#########################'
echo
echo 'The following text for choices are too long:'
echo 
./../SQLiteShell.exe game_data.db '.read ../db_tests/DT_max_choice_length.sql'
echo 
echo 
echo '#########################'
echo '#       DB TEST 2       #'
echo '#     Double Spaces     #'
echo '#########################'
echo
echo 'The following texts contain double spaces:'
echo 
./../SQLiteShell.exe game_data.db '.read ../db_tests/DT_double_spaces.sql'
echo 
echo '#########################'
echo '#       DB TEST 3       #'
echo '#     German Klingt     #'
echo '#########################'
echo
echo 'The following texts contain tönt:'
echo 
./../SQLiteShell.exe game_data.db '.read ../db_tests/DT_german_klingt.sql'
echo 
echo '#########################'
echo '#       DB TEST 4       #'
echo '#     Common Typos      #'
echo '#########################'
echo
echo 'The following texts contain compass direction lowercase, suprise or milita:'
echo 
./../SQLiteShell.exe game_data.db '.read ../db_tests/DT_north_suprised_milita.sql'
echo 
echo '#########################'
echo '#       DB TEST 5       #'
echo '#     Translations      #'
echo '#########################'
echo
echo 'The following items don''t have translations or translations which are empty, ? or ...:'
echo 
./../SQLiteShell.exe game_data.db '.read ../db_tests/DT_item_translations.sql'
echo 
echo '#########################'
echo '#       DB TEST 5       #'
echo '#     Translations      #'
echo '#########################'
echo
echo 'The following dialogue translation ids do have the wrong format:'
echo 
./../SQLiteShell.exe game_data.db '.read ../db_tests/DT_dialogue_translations.sql'
echo 
echo 'Press any key to exit'
read
