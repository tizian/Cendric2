#/bin/sh

echo '#########################'
echo '#      UNIT TEST 1      #'
echo '#   Max Choice Length   #'
echo '#########################'
echo
echo 'The following text for choices are too long:'
echo 
./SQLiteShell.exe game_data.db '.read unit_tests/UT_max_choice_length.sql'
echo 
echo 
echo '#########################'
echo '#      UNIT TEST 2      #'
echo '#     Double Spaces     #'
echo '#########################'
echo
echo 'The following texts contain double spaces:'
echo 
./SQLiteShell.exe game_data.db '.read unit_tests/UT_double_spaces.sql'
echo 
echo 
echo 
echo 'Press any key to exit'
read
