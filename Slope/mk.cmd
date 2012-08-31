
echo off 

del /Q *.rxe 
<<<<<<< HEAD
make all
=======
make clean all
>>>>>>> 769e5601931a4effe38fc1234a8d1fe3537d0d91

IF EXIST *.rxe sh rxeflash.sh