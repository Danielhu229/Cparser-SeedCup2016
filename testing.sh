for case in `ls test/cases`
do
echo test $case
cp bin/SeedCup2016 test/cases/$case
cd test/cases/$case
./SeedCup2016
cd ../../../
done

