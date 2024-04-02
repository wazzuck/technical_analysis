#!/bin/bash
echo "" > /dev/null <<- HELP
	If a gap in the nightly data download has occurred do the following:-
		- Download the missed data from eoddata.com
		- Run this script on the files in order to put the correct files in the right directory.
HELP

if [$1 == ""];
then
	echo "Add a source directory"
	exit 1
fi

DESTINATION_DIR="/home/neville/data"

SOURCE_DIR=$1
cd $SOURCE_DIR

while true
do
	if [ -z "$(ls -A $SOURCE_DIR)" ]; then
		echo "Empty"
        break
	else
		echo "Not Empty"

		FILES=(*)
		for FILE in ${FILES[*]}
		do
			DATE=`echo $FILE | awk -F"_" '{print  $2}' | awk -F"." '{print $1}'`
			echo $DESTINATION_DIR/$DATE
			if [ -d $DESTINATION_DIR/$DATE ]
			then
				echo $DATE exists removing
				rm -rf $DESTINATION_DIR/$DATE
			fi

			mkdir $DESTINATION_DIR/$DATE
			MOVE_CMD="mv -v $SOURCE_DIR/*_$DATE*.csv $DESTINATION_DIR/$DATE/"
			echo $MOVE_CMD
			$MOVE_CMD
			break
		done
	fi
done

echo "FINISHED"
