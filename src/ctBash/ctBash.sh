#!/bin/bash

TIME=`ct_client -m b`
tput sc
ISFINISHED="0"
while [ 1 ]
do
	sleep 1
	TIME=`ct_client -m c`
	STATUS=`ct_client -m s`
	tput el1
	tput rc
	echo -n $TIME $STATUS
	if [[ ("$STATUS" == "CT_STATE_Finished") && ($ISFINISHED == "0") ]]
	then
		zenity --info --title="Time's Up" --text="${*:-BING}"
		ISFINISHED="1"
	fi
done
