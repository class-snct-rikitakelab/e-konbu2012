
echo off 

del /Q *.rxe 

//make clean 
make clean all 

IF EXIST *.rxe sh rxeflash.sh