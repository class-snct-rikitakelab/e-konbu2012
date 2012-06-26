
echo off 

del /Q *.rxe 
//make clean all 
make all 

IF EXIST *.rxe sh rxeflash.sh