
echo off 
//set path = c:\cygwin\bin\
del /Q *.rxe 

make clean 
//make all 

IF EXIST *.rxe sh rxeflash.sh