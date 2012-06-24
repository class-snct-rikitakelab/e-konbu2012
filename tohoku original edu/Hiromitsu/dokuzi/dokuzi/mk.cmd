echo off 
set path=C:\cygwin\bin
del /Q *.rxe 
make all

IF EXIST *.rxe sh rxeflash.sh 
