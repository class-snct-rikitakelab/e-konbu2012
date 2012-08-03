
echo off 

del /Q *.rxe 

//make clean 
make all 

IF EXIST *.rxe sh rxeflash.sh