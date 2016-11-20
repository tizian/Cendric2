#/bin/sh

./SQLiteShell.exe leaderboard.db '.read init_leaderboard.sql'
./SQLiteShell.exe leaderboard.db '.read insert_users.sql'

