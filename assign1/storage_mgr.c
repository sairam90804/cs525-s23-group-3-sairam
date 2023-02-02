#include "storage_mgr.h"
#include<stdio.h>
#include<stdlib.h>
#include "dberror.h"
FILE *file;
int pgread;

//initate Storage Manager
void initStorageManager()
{
    printf("Sucessfully Initate Storage Manager\n");
    printf("Page file creation will begin now !!\n");
	//exit(0);
}
// create new file of one page

RC createPageFile (char *fileName)
{ 
	/*Here we are creating a new file,as per the requirement the initial 
	file size should be one page , and this single page should be filled 
	with 0 bytes. Hence we need to use calloc() version of dynamic memory 
	allocation , since by default all the blocks created are intialized to 0
	instead of garbage values

   */

  //Initialize the input variable
  char input;
  //Open File 
  file=fopen(fileName,"r");
  //Create new File 
  if (file==NULL){

	//Open the new file for writing
	file=fopen(fileName,"w");
	//Allocate memory a single page with page size as 2mb i.e 2048 bytes with calloc function
	char *ptr=(char *)calloc(PAGE_SIZE,sizeof(char));
	//Write the single page of 2mb size to a file
	fwrite(ptr,PAGE_SIZE,sizeof(char),file);
	//Move the file pointer to the end of the file
	fseek(file,0,SEEK_END);
	//Free the memory allocated by calloc
	free(ptr);
	//Close the file
	fclose(file);
	RC_message="File creation has been completed !!";
	return RC_OK;
  }
  //if the file is already present with the same filename
  if(file!=NULL){
	//Input to the user for file existence
	printf("The file already exist in the system \n");
	printf("Do you want the existing file to be overwritten? Press Y/N \n");
	scanf("%c",&input);
	if(input == 'n' || input == 'N'){
	 printf("The file already exist and will not be overwritten");
	 fclose(file);
	 return RC_FILE_EXISTS;
	}
	//Overwrite the file again
	else if (input == 'y' || input == 'Y'){
		
	//Open the new file for writing
	file=fopen(fileName,"w");
	//Allocate memory a single page with page size as 2mb i.e 2048 bytes with calloc function
	char *ptr=(char *)calloc(PAGE_SIZE,sizeof(char));
	//Write the single page of 2mb size to a file
	fwrite(ptr,PAGE_SIZE,sizeof(char),file);
	//Move the file pointer to the end of the file
	fseek(file,0,SEEK_END);
	//Free the memory allocated by calloc
	free(ptr);
	//Close the file
	fclose(file);
	RC_message="File creation has been completed !! \n";
	return RC_OK;

	}else{

		//If the user inputs other then y or n 
		printf("Incorrect Input given by user \n");
		RC_message="Execution is has been stopped to due incorrect user input!! \n";
		return RC_WRITE_FAILED;
	}

  }



}
RC openPageFile(char *fileName,SM_FileHandle *fileHandle)
{
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
    if(file==NULL)
    {
        RC_message ="File does not exist \n";
        return RC_FILE_NOT_FOUND;
    }
//If file is not empty then 
    else
    {

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
	if(fclose(fileHandle->mgmtInfo)==0)
	{
		printf("File closed successfully \n");
		RC_message="File closed successfully \n";
		return RC_OK;
	}
	else
		return RC_FILE_NOT_FOUND;
}


RC destroyPageFile (char *filename)
{	/*The destroy page file functions removes the file from the file system*/

		//Try to remove the file with the remove function from the file system
		if (remove(filename)==0)
		{
		printf("File has been destroyed \n");
		RC_message="File has been destroyed successfully.\n";
		return RC_OK;
		}
		else
			return RC_FILE_NOT_FOUND;
}
// --------------------end of part 1------------------------------//

//----------------------start part 2----------------------------//
RC readBlock (int pageNum, SM_FileHandle *filehandle, SM_PageHandle memPage)
{
	printf("Reading FIle");
	if(filehandle ==NULL)
	{
		RC_message="Requested File is not initalized";
		return RC_FILE_HANDLE_NOT_INIT;
	}
	else 
	{
		if (fopen(filehandle->fileName ,"r") ==NULL)
		{
			RC_message="File Not Found";
			return RC_FILE_NOT_FOUND;
		}
		else 
		{
			if(pageNum > filehandle->totalNumPages || pageNum < 0)
			{
				RC_message="Requested page to be read doesn't exist";
				return RC_READ_NON_EXISTING_PAGE;
			}
			else
			{
				if(memPage !=NULL)
				{
				  fseek(filehandle->mgmtInfo,(((pageNum)*PAGE_SIZE)),SEEK_SET);//seeking to requested page number.
				  fread(memPage,sizeof(char),PAGE_SIZE,filehandle->mgmtInfo);//reading to requested page.
				  filehandle->curPagePos=pageNum;//updating current page position
				  RC_message="File Read Successfull.";
				  return RC_OK;
				}		
			}
		}
	}

}
int getBlockPos (SM_FileHandle *filehandle)
{
	if(filehandle ==NULL)
	{	
		RC_message="desired file related data is not initialized";
		return RC_FILE_HANDLE_NOT_INIT;
	}
	else
	{
		if((fopen(filehandle->fileName,"r")) == NULL)
		{
			RC_message="File Not Found"; 
			return RC_FILE_NOT_FOUND;
		}
		else
		{
			printf("%d ",filehandle->curPagePos);
			return filehandle->curPagePos;
		}
	}	
}
RC readFirstBlock (SM_FileHandle *filehandle, SM_PageHandle memPage)
{
	pgread=0;
	readBlock(pgread,filehandle, memPage);
}
RC readPreviousBlock (SM_FileHandle *filehandle, SM_PageHandle memPage)
{
	pgread=getBlockPos(filehandle)-1;
	readBlock (pgread,filehandle, memPage);
}
RC readCurrentBlock (SM_FileHandle *filehandle, SM_PageHandle memPage)
{
	pgread=getBlockPos(filehandle);
	readBlock (pgread,filehandle, memPage);
	
}
RC readNextBlock (SM_FileHandle *filehandle, SM_PageHandle memPage)
{
	pgread=getBlockPos(filehandle)+1;
	readBlock (pgread,filehandle, memPage);
}
RC readLastBlock (SM_FileHandle *filehandle, SM_PageHandle memPage)
{
	pgread=(filehandle->totalNumPages)-1;
	readBlock (pgread,filehandle, memPage);
}
RC writeBlock (int pageNum, SM_FileHandle *filehandle, SM_PageHandle memPage)
{
 	if(filehandle ==NULL)
	{
		RC_message="Requested File data not found;";
		return RC_FILE_HANDLE_NOT_INIT;
	}
	else
	{
		if(fopen(filehandle->fileName,"r") == NULL)
		{
			RC_message="File not found";
			return RC_FILE_NOT_FOUND;
		}
		else
		{
			if(pageNum > (filehandle->totalNumPages) || pageNum<0)
			{
				RC_message="Requested page doesnt exists.";
				return 6;
			}
			else
			{
				if(memPage != NULL)
				{
					file = fopen(filehandle->fileName,"r+");
                            		fseek(file, pageNum*PAGE_SIZE , SEEK_SET);
					if(fwrite(memPage,PAGE_SIZE,1,file) == 1)
					{
						//file = fopen(filehandle->fileName,"r");
    						filehandle->curPagePos = (ftell(filehandle->mgmtInfo)/PAGE_SIZE);
   						 fseek(file, (filehandle->curPagePos)*PAGE_SIZE , SEEK_SET);
   						 filehandle->mgmtInfo = file;
  						 RC_message="Data write successful.";
						 return RC_OK;
					}
					else
					{
						RC_message="Write Request Failed";
						return RC_WRITE_FAILED;
					}
				}
			}
		}
	}

}

RC writeCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
 return writeBlock(fHandle->curPagePos,fHandle,memPage);
}
RC appendEmptyBlock (SM_FileHandle *fHandle)
{
SM_PageHandle new_block;
    new_block = (char *) calloc(PAGE_SIZE, sizeof(char));//allocated new empty block

    fseek(fHandle->mgmtInfo,(fHandle->totalNumPages + 1)*PAGE_SIZE*sizeof(char) , SEEK_SET);//seek to the given page
    if(fwrite(new_block, sizeof(char), PAGE_SIZE, fHandle->mgmtInfo))
    {
        fHandle->totalNumPages = fHandle->totalNumPages + 1;// increment number of pages
        fHandle->curPagePos = fHandle->totalNumPages; //update the current position
        rewind(fHandle->mgmtInfo); //seek to the first page
        fprintf(fHandle->mgmtInfo, "%d\n" , fHandle->totalNumPages); // write in file with total number of pages
        fseek(fHandle->mgmtInfo,(fHandle->totalNumPages+1)*PAGE_SIZE,SEEK_SET);//seek pointer to the last position
        free(new_block);
    }
    else
    {
        free(new_block);
        return RC_WRITE_FAILED;
    }
}
RC ensureCapacity (int numberOfPages, SM_FileHandle *fHandle)
{
 if(fHandle->totalNumPages >= numberOfPages)//checking the capacity
    {
        return RC_OK;
    }
    else
    {
        int i, numPages_toAdd;
        numPages_toAdd = numberOfPages - fHandle->totalNumPages;//calculating number of pages to add
        for(i=0; i < numPages_toAdd; i++)
        {
            appendEmptyBlock(fHandle);//call to append block
        }
        return RC_OK;
    }
}

