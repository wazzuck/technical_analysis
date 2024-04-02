# !/bin/bash -x
# vim:set bg=dark number: #

cd ~/vunderland/eod/scripts
./getEOD.py

rm /dev/shm/test.db

cd ~/vunderland/eod/src
./eod.exe

cd /home/neville/vunderland/eod/scripts/
./DisplayResultsBottle.py
#./archive_data.sh 
