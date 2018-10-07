#!/usr/bin/env bash
echo "Build"
sh build.sh

rm -rf exdata/
echo "Removed exdata/"

rm -rf ppdata/
echo "Removed ppdata/"

echo "Start Algo"
./build/OptimisationM2AIC
python -m cocopp exdata/SA_ES_on_bbob

cd ppdata
firefox index.html
cd ..
