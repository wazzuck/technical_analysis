IP=rpi
FILENAME="data_latest.tar.xz"

rm ~/data/$FILENAME

scp $IP:~/data/archive/$FILENAME ~/data

cd /

tar xJvf ~/data/$FILENAME
