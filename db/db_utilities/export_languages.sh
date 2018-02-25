#/bin/sh
echo 'Exporting English'
echo 
./../SQLiteShell.exe ../game_data.db '.mode list' ".separator '\n'" '.output english.txt' 'select english from text;'
echo 
echo 'Exporting German'
echo 
./../SQLiteShell.exe ../game_data.db '.mode list' ".separator '\n'" '.output german.txt' 'select german from text;'
echo 
echo 'Exporting Swiss German'
echo 
./../SQLiteShell.exe ../game_data.db '.mode list' ".separator '\n'" '.output swiss_german.txt' 'select swiss_german from text;'
echo 
echo 'Done'

