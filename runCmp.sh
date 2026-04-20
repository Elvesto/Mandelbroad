#!/bin/bash

./script.sh v1 VERS1 > version_1.csv
printf "v1 done\n"
./script.sh v2 > version_2.csv
printf "v2 done\n"
./script.sh v3 > version_3.csv
printf "v3 done\n"
./script.sh v4 > version_4.csv
printf "v4 done\n"

python3 -m venv venv

source venv/bin/activate

# pip3 install pandas matplotlib

python3 drawGraphic.py version_1.csv
python3 drawGraphic.py version_2.csv
python3 drawGraphic.py version_3.csv
python3 drawGraphic.py version_4.csv

deactivate

printf "Done\n"
