../tester/xv6-edit-makefile.sh src/Makefile $1 > src/Makefile.test
cp -f $1.c src/$1.c
cd src
make -f Makefile.test clean
make -f Makefile.test xv6.img
make -f Makefile.test fs.img
cd ..