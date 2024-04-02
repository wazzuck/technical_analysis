# !/bin/bash -x
# vim:set bg=dark number: #

echo "" > /dev/null << COMMENT
"""Checks for missing data downloads"""
COMMENT

DATA=/home/neville/data/

STARTDATE=20200101
WEEKDAY=2

for i in {0..365}; do 
	# custom format using +
	DATE=`date +%Y%m%d -d "$STARTDATE +$i days"`

    ((WEEKDAY = $WEEKDAY+1))

	if ! { [ $WEEKDAY == 6 ] || [ $WEEKDAY == 7 ]; }; 
	then
		FILE=$DATA${DATE}/LSE_${DATE}.txt
		
		if ! [ -f $FILE ]
		then
			echo $FILE not found
		fi
	fi

	if [ "$WEEKDAY" == 7 ];
	then
		WEEKDAY=0
	fi
done
