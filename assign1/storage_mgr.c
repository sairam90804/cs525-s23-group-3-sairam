#include "storage_mgr.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "dberror.h"
FILE *file;

//initate Storage Manager
void initStorageManager()
{
    printf("Sucessfully Initate Storage Manager\n");
    printf("Page file creation will begin now !!\n");
}


//Local function prototype declaration 
RC createFile(char *fileName);
RC validateFileHandle (SM_FileHandle *filehandle);
RC validateFile (int pageNum, SM_FileHandle *filehandle);

// create new file of one page
RC createFile(char *fileName) {

	RC code;
	file = fopen(fileName,"w");
	//Allocate memory a single page with page size as 2mb i.e 2048 bytes with calloc function
	char *ptr=(char *)calloc(PAGE_SIZE,sizeof(char));
	//Write the single page of 2mb size to a file
	if(fwrite(ptr,sizeof(char),PAGE_SIZE,file) == PAGE_SIZE) {
		//Move the file pointer to the end of the file
		fseek(file,0,SEEK_END);
		//Free the memory allocated by calloc
		free(ptr);
		//Close the file
		fclose(file);
		RC_message="File creation has been completed!!";
		code = RC_OK;
	}
	else {
		free(ptr);
		RC_message="Page Creation Failed";
		code = RC_WRITE_FAILED;
	}
	return code;
}

RC createPageFile (char *fileName)
{ 
	/*Here we are creating a new file,as per the requirement the initial 
	file size should be one page , and this single page should be filled 
	with 0 bytes. Hence we need to use calloc() version of dynamic memory 
	allocation , since by default all the blocks created are intialized to 0
	instead of garbage values
   */
  	RC code;
  	//Initialize the input variable
  	char input;
  	//Open File 
  	file = fopen(fileName,"r");
  	//Create new File 
  	if (file == NULL) {
		code = createFile(fileName);
  	}
  	//if the file is already present with the same filename
  	else {
		//Input to the user for file existence
		printf("The file already exist in the system \n");
		printf("Do you want the existing file to be overwritten? Press Y/N \n");
		scanf("%c",&input);
		if(input == 'n' || input == 'N'){
			printf("The file already exist and will not be overwritten\n");
			fclose(file);
			code = RC_OK;
		}
		//Overwrite the file again
		else if (input == 'y' || input == 'Y') {	
			code = createFile(fileName);
		}
		else {
			//If the user inputs other then y or n 
			printf("Incorrect Input given by user \n");
			RC_message="Execution is has been stopped to due incorrect user input!! \n";
			code = RC_WRITE_FAILED;
		}
  	}
	return code;
}

RC openPageFile(char *fileName,SM_FileHandle *fileHandle) {
/*This function opens a page file ans sets the properties of the file handler like 
mgmtIngo , page count and current position of the new file. The function also calculates 
the number of pages in the file*/


    //Open a a page file 
	printf("Opening a page file !!! \n");
	//Open the existing file 
    file=fopen(fileName,"r");
	//Intialize variables with  page count and filesize 
    int pageCnt=0,fileSize=0;
	//Check if the file is empty 
    if(file==NULL) {
        RC_message ="File does not exist \n";
        return RC_FILE_NOT_FOUND;
    }
	//If file is not empty then 
    else {
        //file=fopen(fileName,"r");
		//Store the file name in the file handler 
        fileHandle->fileName=fileName;
		//After freshly opening the file, set the current page pointer of the file handler to 0
        fileHandle->curPagePos=0; 
		/*To find the number of pages of the file, we need to find the size of the file 
		and then divide by the page size */
		//Take the file pointer to the end of the file
        fseek(file,0,SEEK_END);
		//Get the size of the file
        fileSize=ftell(file);
		//Get the page count
        pageCnt=fileSize/PAGE_SIZE; 
		// Sotre the page count in the file handler 
        fileHandle->totalNumPages=pageCnt;
		//Store the info of the opened file pointer in the management info 
        fileHandle->mgmtInfo=file;
        RC_message="File Opening process has executed successfully \n";
        return RC_OK;
    }
}

//Implementation of the closePageFile interface
RC closePageFile (SM_FileHandle *fileHandle)
{
/*The close page file function closes the file stream */

	printf("The file will be closed \n");
	/*mgmtInfo stored the open file pointer previously*/
	//If fclose returns 0 , then it means the file has been successfully closed
	if(fclose(fileHandle->mgmtInfo)==0) {
		printf("File closed successfully \n");
		RC_message="File closed successfully \n";
		return RC_OK;
	}
	else {
		return RC_FILE_NOT_FOUND;
	}
}


RC destroyPageFile (char *filename) {	
	/*The destroy page file functions removes the file from the file system*/

	//Try to remove the file with the remove function from the file system
	if (remove(filename)==0) {
		printf("File has been destroyed \n");
		RC_message="File has been destroyed successfully.\n";
		return RC_OK;
	}
	else {
		return RC_FILE_NOT_FOUND;
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////

//Validate File handle
RC validateFileHandle (SM_FileHandle *filehandle){

	//Initialising the return code to OK
    RC code = RC_OK;
	//Checking if FileHandle is null
    if(filehandle == NULL) {
		//So The File handle given is null
		//Setting the RC message as "Requested File is not initalized - filehandle == NULL"
		RC_message = "Requested File is not initalized - filehandle == NULL";
		//Setting return code to RC_FILE_HANDLE_NOT_INIT
		code = RC_FILE_HANDLE_NOT_INIT;
	}
    //Check for file existance
    else if (fopen(filehandle->fileName ,"r") == NULL) {
		//So The File is not created or there in the database
		RC_message = "File Not Found";
		//Setting return code to RC_FILE_NOT_FOUND
		code = RC_FILE_NOT_FOUND;
	}
	//Returning the code
    return code;
}

//This Funtion validates the given File Handle and Page number to read
RC validateFile (int pageNum, SM_FileHandle *filehandle){

    // Initialising the Return code to validate the input file handle
	RC code = validateFileHandle(filehandle);
	//If the code from "validateFileHandle(filehandle)" function is RC_OK, Then check whether the given page exists in the File
    if ((code == RC_OK) && (pageNum > filehandle->totalNumPages || pageNum < 0)) {
		//So The Pagenumber doesn't exist in the File
		RC_message = "Requested page to be read doesn't exist";
		//Setting the return code to RC_READ_NON_EXISTING_PAGE
		code = RC_READ_NON_EXISTING_PAGE;
	}
	//Returning the code
    return code;
}

//Reading the File with the given pagenumber, filehandle and Buffer
RC readBlock (int pageNum, SM_FileHandle *filehandle, SM_PageHandle memPage){

    //Initialise the Return code variable to Validate the page number and File handle
    RC code = validateFile(pageNum, filehandle);
    //If the validation returns RC_OK, Then check whether Pagehandler is null
	if((code == RC_OK) && (memPage !=NULL)) {
		//Start Reading the Page
        //Move the file pointer of the File handle from the starting of the page (SEEK_SET) to the end of the page
		fseek(filehandle->mgmtInfo,(((pageNum)*PAGE_SIZE)),SEEK_SET);
        //Read the Page content and store the data in the buffer (memPage)
		fread(memPage,sizeof(char),PAGE_SIZE,filehandle->mgmtInfo);
        //Update the current page position of the FileHandler to the pagenumber
		filehandle->curPagePos=pageNum;
		//Set Return Code Message and no need to set the code to RC_OK as its already set to RC_OK.
		RC_message="File Read Successfull.";
	}	
	//Returning the code
	return code;
}

//This Function will return the integer containing the current page position of the file handle
int getBlockPos (SM_FileHandle *filehandle){

	//Initialising the page position variable
	int pos;
	//Validate the given filehandle, If Validation returns RC_OK then check the current page position
	if(validateFileHandle(filehandle) == RC_OK){
        //Print the Position in the File and return the integer
	    printf("%d \n",filehandle->curPagePos);
	    pos = filehandle->curPagePos;
    }
	//Return the Position
	return pos;
}

//Read the First Page in the File
RC readFirstBlock (SM_FileHandle *filehandle, SM_PageHandle memPage){

    //Initializing page number to first page of the fuke handle
	int pageNum = 0;
	return readBlock(pageNum,filehandle, memPage);
}

//Read Previous page in the File
RC readPreviousBlock (SM_FileHandle *filehandle, SM_PageHandle memPage) {

    //Initializing page number to current page position of the file handle - 1
	int pageNum = getBlockPos(filehandle)-1;
	return readBlock (pageNum,filehandle, memPage);
}

//Read current page in the File
RC readCurrentBlock (SM_FileHandle *filehandle, SM_PageHandle memPage){

    //Initializing page number to current page position of the file handle
	int pageNum = getBlockPos(filehandle);
	return readBlock (pageNum,filehandle, memPage);
}

//Read Next page in the File
RC readNextBlock (SM_FileHandle *filehandle, SM_PageHandle memPage){

    //Initializing page number to current page position of the file handle + 1
	int pageNum = getBlockPos(filehandle)+1;
	return readBlock (pageNum,filehandle, memPage);
}

//Read last page of the file
RC readLastBlock (SM_FileHandle *filehandle, SM_PageHandle memPage){

    //Initializing page number to last page position of the file handle
	int pageNum = (filehandle->totalNumPages)-1;
	return readBlock (pageNum,filehandle, memPage);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

RC writeBlock (int pageNum, SM_FileHandle *filehandle, SM_PageHandle memPage) {

	// Initialising the Return code to validate the input file handle and the page number
	RC code = validateFile(pageNum, filehandle);
	//If the Validation returns RC_OK, Then Check whether the buffer storage pointer is not null
	if((code == RC_OK) && (memPage !=NULL)){
		//Ensure whether the current file has the free space to store the input data, else add required additional pages to the file
		ensureCapacity(strlen(memPage)/PAGE_SIZE,filehandle);
		//Opens a file to update both reading and writing and store it in the file variable
		file = fopen(filehandle->fileName,"r+");
		//Move the file pointer of the File handle from the starting of the page (SEEK_SET) to the end of the page
        fseek(file, pageNum*PAGE_SIZE , SEEK_SET);
		//writes data from mempage to file and returns the total number of elements successfully returned
		if(fwrite(memPage,sizeof(char),PAGE_SIZE,file) == PAGE_SIZE) {
			//Set the current page position of the file by diving the total size of file after moving the file pointer at the end of the file with the PAGE_SIZE
    		filehandle->curPagePos = (ftell(filehandle->mgmtInfo)/PAGE_SIZE);
			//Move the file pointer of the File handle from the starting of the page (SEEK_SET) to the end of the last page of the file
   			fseek(file, (filehandle->curPagePos)*PAGE_SIZE , SEEK_SET);
			// assign the File handle mgmtInfo to the file
   			filehandle->mgmtInfo = file;
			//Set the RC_message and return code to RC_OK
  			RC_message="Data write successful.";
			code = RC_OK;
		}
		else {
		//If the write operation fails, Set the return code to RC_WRITE_FAILED
		RC_message="Write Request Failed";
		code = RC_WRITE_FAILED;
		}
	}
	return code;
}

RC writeCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage) {

	//Set the Page number to the current page position in the file and write to the page
	return writeBlock(fHandle->curPagePos,fHandle,memPage);
}

RC appendEmptyBlock (SM_FileHandle *fHandle) {

	//Initialise the variable to store return code and a new memory pointer for the additonal pages
	RC code;
	SM_PageHandle new_block;
	//Allocate memory to the pages
    new_block = (char *) calloc(PAGE_SIZE, sizeof(char));
    fseek(fHandle->mgmtInfo,(fHandle->totalNumPages+1)*PAGE_SIZE*sizeof(char),SEEK_SET);
	//Check if the Empty page is written successfully to the file
    if(fwrite(new_block, sizeof(char), PAGE_SIZE, fHandle->mgmtInfo) == PAGE_SIZE) {
        fHandle->totalNumPages = fHandle->totalNumPages + 1;// increment number of pages
        fHandle->curPagePos = fHandle->totalNumPages; //update the current position
        rewind(fHandle->mgmtInfo); //seek to the first page
        fprintf(fHandle->mgmtInfo, "%d\n" , fHandle->totalNumPages); // write in file with total number of pages
        fseek(fHandle->mgmtInfo,(fHandle->totalNumPages)*PAGE_SIZE,SEEK_SET);//seek pointer to the last position
        free(new_block);//Free the memory pointer
		//Set the return code to RC_OK
		code = RC_OK;
    }
	//If the Write fail return RC_WRITE_FAILED
	else {
        free(new_block);
        code = RC_WRITE_FAILED;
    }
	return code;
}

RC ensureCapacity (int numberOfPages, SM_FileHandle *fHandle) {

	printf("Running Ensure Capacity to check whether the number of input pages is greater than the total number of pages in the current file \n");
 	//Initialise the variable to store return code
	RC code;
	//Check whether the file has the capacity to accomodate the input number of pages
	if(fHandle->totalNumPages >= numberOfPages) {
		//If the file can accomodate the data set the Return code to RC_OK and exit the function
        code = RC_OK;
    }
    else {
		printf("Number of input pages is greater than the Total Number of pages in the file. Appending more pages \n");
    	int i, numPages_toAdd;
		//Calculating the number of pages to be added
        numPages_toAdd = numberOfPages - fHandle->totalNumPages;
		//Add the required number of pages
        for(i=0; i < numPages_toAdd; i++){
			//For each iteration, a page will be added to the file by calling the below function
			//Set the return code as per the return value of the append function
            code = appendEmptyBlock(fHandle);
        }
    }
	return code;
}
