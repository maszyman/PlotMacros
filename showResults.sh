#!/bin/bash

if [ $2 = "mult" ]; then
    root -l drawMultiplicity.C\(\"./$1\",\"$3\",0,0\);

elif [ $2 = "ypt" ]; then
    root -l drawDist.C\(\"./$1\",\"$3\",0,0\);

elif [ $2 = "pidpass" ]; then
    root -l drawPID.C\(\"./$1\",\"$3\",\"Pass\",0,0,$4\);

elif [ $2 = "dcapass" ]; then
    root -l drawDCA.C\(\"./$1\",\"$3\",\"Pass\",0,0,$4,$5,$6\);

elif [ $2 = "cf" ]; then
    root -l drawCorrFun.C\(\"./$1\",\"$3\",\"$4\",0,0,$5\);

elif [ $2 = "cfn" ]; then
    root -l drawNonID.C\(0,0,\"./$1\"\);

elif [ $2 = "comparefields" ]; then
    root -l drawCorrFunCompareFields.C\(\"./$1\",\"./$4\",\"$3\",0,0\);

elif [ $2 = "comparefieldsn" ]; then
    root -l drawNonIDcompareFields.C\(0,0,\"./$1\",\"./$3\"\);

fi
