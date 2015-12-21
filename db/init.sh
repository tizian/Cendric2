#/bin/sh

SQLiteShell.exe cendricdb.db '.read initdb.sql'
for f in $(find insert_scripts -name '*.sql')
do 
	echo 'reading '$f'...'
	SQLiteShell.exe cendricdb.db '.read '$f
	echo 'read '$f
done
