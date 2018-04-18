#!/bin/sh
./my_otp -i messageA.txt -o mA.enc -k keyfile16K.key
if diff mA.enc messageA.enc; then : ; else echo FAILED;  exit 1 ; fi

./my_otp -i messageB.txt -o mB.enc -k secretkey
if diff mB.enc messageB.enc; then : ; else echo FAILED ; exit 1 ; fi

./my_otp -i mA.enc -o mA.txt -k secretkey
if diff m1.txt messageA.txt; then : ; else echo FAILED;  exit 1 ; fi

./my_otp -i mB.enc -o mB.txt -k secretkey
if diff mB.txt messageB.txt; then : ; else echo FAILED ; exit 1 ; fi
rm -f mA* mB* 
echo PASSED
exit 0
