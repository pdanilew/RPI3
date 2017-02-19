#!/bin/bash

if [ ! -z "$1" ]
then
  if [[ "$1" =~ ^-?[0-9]*[.,]?[0-9]*[eE]?-?[0-9]+$ ]]
  then
    LIMIT=$1
    interval=0.5
  else
    LIMIT=1
    interval=0
  fi
else
  LIMIT=1
  interval=0
fi

function convert_to_MHz {
    let value=$1/1000
    echo "$value"
}

printf "Temp       [°C] Volt        [V] Min Freq  [MHz] Max Freq  [MHz] Curr Freq [MHz] Governor       \n"
for ((i=0; i<LIMIT; i++))
do
  temp=`/opt/vc/bin/vcgencmd measure_temp | grep -E -o -- '-?[0-9]*[.,]?[0-9]*[eE]?-?[0-9]+'`

  volts=$(vcgencmd measure_volts)
  volts=${volts:5:4}

  minFreq=$(cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_min_freq)
  minFreq=$(convert_to_MHz $minFreq)

  maxFreq=$(cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq)
  maxFreq=$(convert_to_MHz $maxFreq)

  freq=$(cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq)
  freq=$(convert_to_MHz $freq)

  governor=$(cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor)

  printf "%15.2f %15.2f %15i %15i %15i %s\n" $temp $volts $minFreq $maxFreq $freq $governor
  sleep $interval
done

exit 0
