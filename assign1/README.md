Assignment 1 - Storage Manager
______________________________

 
TEAM MEMBERS (GROUP 3)
______________________

1. Vivekanand Reddy Malipatel  
2. Indrajit Ghosh    
3. Sai Ram Oduri 

------------------------
Description--->

Script Execution----
1) Go to the terminal
2) Inside project folder 
3) To build run command :make 
4) To execute run command :make run 
5) Object file can be removed using : make clean

----------------------------

Functions Implemented- 

A) createPageFile 
Here we are creating a new file,as per the requirement the initial file size should be one page , and this single page should be filled 
with 0 bytes. Hence we need to use calloc() version of dynamic memory allocation , since by default all the blocks created are intialized to 0 instead of garbage values

B) openPageFile

This function opens a page file ans sets the properties of the file handler like 
mgmtIngo , page count and current position of the new file. The function also calculates 
the number of pages in the file

C) closePageFile

The close page file function closes the file stream

D) destroyPageFile

The destroy page file functions removes the file from the file system




