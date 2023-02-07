Assignment 1 - Storage Manager
______________________________

 
TEAM MEMBERS (GROUP 3)
______________________

1. Vivekanand Reddy Malipatel - A20524971
2. Indrajit Ghosh - A20525793
3. Sai Ram Oduri - A20522183

------------------------
Description----->

Script Execution---->

1) Go to the terminal
2) Inside project folder 
3) To build run command :make 
4) To execute run command :make run 
5) Object file can be removed using : make clean

----------------------------

Functions Implemented- 


--------- IMPLEMENTED BY INDRAJIT GHOSH ------------------------------------------

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

--------- IMPLEMENTED BY VIVEKANAND REDDY MALIPATEL ------------------------------------------

E) readBlock

Reading the File with the given pagenumber, filehandle and Buffer
First it Starts Reading the Page. Next,Move the file pointer of the File handle from the starting of the page (SEEK_SET) to the end of the page.
Read the Page content and store the data in the buffer (memPage), next Update the current page position of the FileHandler to the pagenumber.

F) getBlockPos

This Function will return the integer containing the current page position of the file handle. It gets the Current page position from the file handle/

G) readFirstBlock

This function sets the page number to zero and call the readBlock

H) readPreviousBlock

This function sets the page number to a page before the current page position of the file and call the readBlock

I) readCurrentBlock

This function sets the page number to a the current page position of the file and call the readBlock

J) readNextBlock

This function sets the page number to a page after the current page position of the file and call the readBlock

K) readLastBlock

This function sets the page number to a last page position of the file and call the readBlock

--------- IMPLEMENTED BY SAI RAM ODURI ------------------------------------------

L) writeBlock

This Function write the input data to the page file.
First Check whether the buffer storage pointer is not null, next Ensure whether the current file has the free space to store the input data, else add required additional pages to the file.
Move the file pointer of the File handle from the starting of the page (SEEK_SET) to the end of the page and write data from mempage to file and returns the total number of elements successfully returned.
If Write is successfull, Set the current page position of the file by diving the total size of file after moving the file pointer at the end of the file with the PAGE_SIZE.
Move the file pointer of the File handle from the starting of the page (SEEK_SET) to the end of the last page of the file and assign the File handle mgmtInfo to the file.

M) writeCurrentBlock

Set the Page number to the current page position in the file and write to the page

N) appendEmptyBlock

This Function is used to append and empty page to the file. 

Allocate memory to the pages and Check if the Empty page is written successfully to the file. Then increment number of pages in the file, update the current position, seek to the first page, write in file with total number of pages and  seek pointer to the last position.

O) ensureCapacity

This ensures that the File has capacity to store the input data. Or Add new pages to the file, as requried.

-------------------------------------------------------------------------------------------------------------------------------------------

New Testcase Implemented :- 

We have added a new testcase which will test the ensure capacity funcion by trying to write the data when the number of input pages is greater than the Total number of pages in the current file.