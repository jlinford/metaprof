#!/bin/bash
#
# Generates a CSV file showing samples of /proc/meminfo
# that can be used to generate graphs in a spreadsheet program
#
# John C. Linford (jlinford@paratools.com)
# 26 Aug 2014
#

# Seconds between samples
FREQ=0.1

LOG=memstat.$$
function measure()
{
  (
  echo "Timestamp: " `date +%s.%N`
  cat /proc/meminfo 
  ) >> $LOG
}


# Use /tmp/$$ as a synchronization flag
touch /tmp/$$

measure
( $@ ; rm -f /tmp/$$ ) &

while test -f /tmp/$$ ; do 
  measure
  sleep $FREQ
done

(
echo -n "Timestamp:s.ns,"
cat /proc/meminfo | awk '{print $1 $3}' | tr '\n' ','
echo
cat $LOG | awk '{if (NR%44==0) {print $2} else {printf "%s,",$2} }'
) > memstat.$$.csv
rm $LOG

