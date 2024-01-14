#!bin/bash
flightScanner() 
{
if [ $# -eq 0 ]; then
  echo "Error: No arguments provided. Please provide one or more airport codes as arguments."
  exit 1
fi
mkdir -p flightDB
cd \flightDB
for arg in "$@"
do
    current=$(date +"%s")
    before=$((current - 86400))
    if [ ! -d $arg ]; then
        mkdir "$arg"
    fi
    response=$(curl -s -w "%{http_code}" "https://opensky-network.org/api/flights/arrival?airport=$arg&begin=$before&end=$current" -o "$arg/$arg.arv.json" )
    if [[ $response != "2"* ]]; then
        echo "Error: $arg is not a valid airport code"
       rm -rf $arg/;
        continue
    fi
    
    
    response=$(curl -s -w "%{http_code}" "https://opensky-network.org/api/flights/departure?airport=$arg&begin=$before&end=$current" -o "$arg/$arg.dep.json" )
    if [[ $response != "2"* ]]; then
        echo "Error: $arg is not a valid airport code"
       rm -rf $arg/;
        continue
    fi
    
    	grep -oE '("icao24"|"firstSeen"|"estDepartureAirport"|"lastSeen"|"estArrivalAirport"|"callsign"):[^,}]*' $arg/$arg.arv.json | cut -d: -f2 | tr '\n' ',' > $arg/$arg.arv
    	sed -i 's/"//g' $arg/$arg.arv
  	awk -F, '{ for(i=1; i<=NF; i+=6) print $(i)","$(i+1)","$(i+2)","$(i+3)","$(i+4)","$(i+5) }' "$arg/$arg.arv" > "$arg/$arg.arvN"
	mv "$arg/$arg.arvN" "$arg/$arg.arv"
	echo "icao24 firstSeen estDepartureAirport lastSeen estArrivalAirport callsign" | cat - $arg/$arg.arv > temp && mv temp $arg/$arg.arv


   grep -oE '("icao24"|"firstSeen"|"estDepartureAirport"|"lastSeen"|"estArrivalAirport"|"callsign"):[^,}]*' $arg/$arg.dep.json | cut -d: -f2 | tr '\n' ',' > $arg/$arg.dep
    sed -i 's/"//g' $arg/$arg.dep
   awk -F, '{ for(i=1; i<=NF; i+=6) print $(i)","$(i+1)","$(i+2)","$(i+3)","$(i+4)","$(i+5) }' "$arg/$arg.dep" > "$arg/$arg.depN"
mv "$arg/$arg.depN" "$arg/$arg.dep"
echo "icao24 firstSeen estDepartureAirport lastSeen estArrivalAirport callsign" | cat - $arg/$arg.dep > temp && mv temp $arg/$arg.dep

rm $arg/$arg.arv.json
rm $arg/$arg.dep.json
  
done

}

flightScanner "$@"


