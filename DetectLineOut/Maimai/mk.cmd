echo off 
del /Q *.rxe 
make all

IF EXIST *.rxe sh rxeflash.sh
