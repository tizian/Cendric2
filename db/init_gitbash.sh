#/bin/sh

# game data
./SQLiteShell.exe game_data.db '.read init_game_data.sql'
for f in $(find insert_scripts -name '*.sql')
do 
	echo 'reading '$f'...'
	./SQLiteShell.exe game_data.db '.read '$f
	echo 'read '$f
done
read 