/**
 * @file fs_port_fatfs.c
 * @brief File system abstraction layer (FatFs)
 *
 * @section License
 *
 * Copyright (C) 2010-2014 Oryx Embedded. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * @author Oryx Embedded (www.oryx-embedded.com)
 * @version 1.5.0
 **/

//Dependencies
#include <string.h>
#include "fs_port.h"
#include "error.h"
#include "debug.h"

//FatFs specific headers
#include "ff.h"

//File system objects
static FATFS fs;
static FIL fileTable[FS_MAX_FILES];
static DIR dirTable[FS_MAX_DIRS];

//Mutex that protects critical sections
static OsMutex fsMutex;


/**
 * @brief File system initialization
 * @return Error code
 **/

error_t fsInit(void)
{
   FRESULT res;

   //Clear file system objects
   memset(fileTable, 0, sizeof(fileTable));
   memset(dirTable, 0, sizeof(dirTable));

   //Create a mutex to protect critical sections
   if(!osCreateMutex(&fsMutex))
   {
      //Failed to create mutex
      return __ERROR_OUT_OF_RESOURCES;
   }

//Revision 0.09b or lower?
#if (_FATFS == 124 || _FATFS == 126 || _FATFS == 8085 || _FATFS == 8255 || \
   _FATFS == 8237 || _FATFS == 6502 || _FATFS == 4004 || _FATFS == 82786)
   //Mount file system
   res = f_mount(0, &fs);
//Revision 0.10 or higher?
#else
   //Mount file system
   res = f_mount(&fs, "", 1);
#endif

   //Failed to mount file system?
   if(res != FR_OK)
   {
      //Clean up side effects
      osDeleteMutex(&fsMutex);
      //Report an error
      return __ERROR_FAILURE;
   }

   //Successful processing
   return __NO_ERROR;
}


/**
 * @brief Check whether a file exists
 * @param[in] path NULL-terminated string specifying the filename
 * @return The function returns TRUE if the file exists. Otherwise FALSE is returned
 **/

bool_t fsFileExists(const char_t *path)
{
   FRESULT res;
   FILINFO fno;

//Long filename support?
#if (_USE_LFN != 0)
   fno.lfname = NULL;
   fno.lfsize = 0;
#endif

   //Make sure the pathname is valid
   if(path == NULL)
      return FALSE;

#if (_FS_REENTRANT == 0)
   //Enter critical section
   osAcquireMutex(&fsMutex);
#endif

   //Check whether the file exists
   res = f_stat(path, &fno);

#if (_FS_REENTRANT == 0)
   //Leave critical section
   osReleaseMutex(&fsMutex);
#endif

   //Any error to report?
   if(res != FR_OK)
      return FALSE;

   //Valid file?
   if(fno.fattrib & AM_DIR)
      return FALSE;
   else
      return TRUE;
}


/**
 * @brief Retrieve the size of the specified file
 * @param[in] path NULL-terminated string specifying the filename
 * @param[out] size Size of the file in bytes
 * @return Error code
 **/

error_t fsGetFileSize(const char_t *path, uint32_t *size)
{
   FRESULT res;
   FILINFO fno;

//Long filename support?
#if (_USE_LFN != 0)
   fno.lfname = NULL;
   fno.lfsize = 0;
#endif

   //Check parameters
   if(path == NULL || size == NULL)
      return __ERROR_INVALID_PARAMETER;

#if (_FS_REENTRANT == 0)
   //Enter critical section
   osAcquireMutex(&fsMutex);
#endif

   //Retrieve information about the specified file
   res = f_stat(path, &fno);

#if (_FS_REENTRANT == 0)
   //Leave critical section
   osReleaseMutex(&fsMutex);
#endif

   //Any error to report?
   if(res != FR_OK)
      return __ERROR_FAILURE;
   //Valid file?
   if(fno.fattrib & AM_DIR)
      return __ERROR_FAILURE;

   //save the size of the file
   *size = fno.fsize;

   //Successful processing
   return __NO_ERROR;
}


/**
 * @brief Rename the specified file
 * @param[in] oldPath NULL-terminated string specifying the pathname of the file to be renamed
 * @param[in] newPath NULL-terminated string specifying the new filename
 * @return Error code
 **/

error_t fsRenameFile(const char_t *oldPath, const char_t *newPath)
{
   FRESULT res;

   //Check parameters
   if(oldPath == NULL || newPath == NULL)
      return __ERROR_INVALID_PARAMETER;

#if (_FS_REENTRANT == 0)
   //Enter critical section
   osAcquireMutex(&fsMutex);
#endif

   //Rename the specified file
   res = f_rename(oldPath, newPath);

#if (_FS_REENTRANT == 0)
   //Leave critical section
   osReleaseMutex(&fsMutex);
#endif

   //Any error to report?
   if(res != FR_OK)
      return __ERROR_FAILURE;

   //Successful processing
   return __NO_ERROR;
}


/**
 * @brief Delete a file
 * @param[in] path NULL-terminated string specifying the filename
 * @return Error code
 **/

error_t fsDeleteFile(const char_t *path)
{
   FRESULT res;

   //Make sure the pathname is valid
   if(path == NULL)
      return __ERROR_INVALID_PARAMETER;

#if (_FS_REENTRANT == 0)
   //Enter critical section
   osAcquireMutex(&fsMutex);
#endif

   //Delete the specified file
   res = f_unlink(path);

#if (_FS_REENTRANT == 0)
   //Leave critical section
   osReleaseMutex(&fsMutex);
#endif

   //Any error to report?
   if(res != FR_OK)
      return __ERROR_FAILURE;

   //Successful processing
   return __NO_ERROR;
}


/**
 * @brief Open the specified file for reading or writing
 * @param[in] path NULL-terminated string specifying the filename
 * @param[in] mode Type of access permitted (FS_FILE_MODE_READ,
 *   FS_FILE_MODE_WRITE or FS_FILE_MODE_CREATE)
 * @return File handle
 **/

FsFile *fsOpenFile(const char_t *path, uint_t mode)
{
   uint_t i;
   uint_t flags;
   FRESULT res;

   //File pointer
   FsFile *file = NULL;

   //Make sure the pathname is valid
   if(path == NULL)
      return NULL;

   //Enter critical section
   osAcquireMutex(&fsMutex);

   //Loop through the file objects
   for(i = 0; i < FS_MAX_FILES; i++)
   {
      //Unused file object found?
      if(fileTable[i].fs == NULL)
      {
         //Default access mode
         flags = 0;

         //Check access mode
         if(mode & FS_FILE_MODE_READ)
            flags |= FA_READ;
         if(mode & FS_FILE_MODE_WRITE)
            flags |= FA_WRITE;
         if(mode & FS_FILE_MODE_CREATE)
            flags |= FA_OPEN_ALWAYS;

         //Open the specified file
         res = f_open(&fileTable[i], path, flags);

         //Check status code
         if(res == FR_OK)
            file = &fileTable[i];

         //Stop immediately
         break;
      }
   }

   //Leave critical section
   osReleaseMutex(&fsMutex);
   //Return a handle to the directory
   return file;
}


/**
 * @brief Move to specified position in file
 * @param[in] file Handle that identifies the file
 * @param[in] offset Number of bytes to move from origin
 * @param[in] origin Position used as reference for the offset (FS_SEEK_SET,
 *   FS_SEEK_CUR or FS_SEEK_END)
 * @return Error code
 **/

error_t fsSeekFile(FsFile *file, int_t offset, uint_t origin)
{
   FRESULT res;

   //Make sure the file pointer is valid
   if(file == NULL)
      return __ERROR_INVALID_PARAMETER;

#if (_FS_REENTRANT == 0)
   //Enter critical section
   osAcquireMutex(&fsMutex);
#endif

   //Offset is relative to the current file pointer position?
   if(origin == FS_SEEK_CUR)
      offset += f_tell((FIL *) file);
   //Offset is relative to the end of the file?
   else if(origin == FS_SEEK_END)
      offset += f_size((FIL *) file);

   //Move read/write pointer
   res = f_lseek((FIL *) file, offset);

#if (_FS_REENTRANT == 0)
   //Leave critical section
   osReleaseMutex(&fsMutex);
#endif

   //Any error to report?
   if(res != FR_OK)
      return __ERROR_FAILURE;

   //Successful processing
   return __NO_ERROR;
}


/**
 * @brief Write data to the specified file
 * @param[in] file Handle that identifies the file to be written
 * @param[in] data Pointer to a buffer containing the data to be written
 * @param[in] length Number of data bytes to write
 * @return Error code
 **/

error_t fsWriteFile(FsFile *file, void *data, size_t length)
{
   UINT n;
   FRESULT res;

   //Make sure the file pointer is valid
   if(file == NULL)
      return __ERROR_INVALID_PARAMETER;

#if (_FS_REENTRANT == 0)
   //Enter critical section
   osAcquireMutex(&fsMutex);
#endif

   //Write data
   res = f_write((FIL *) file, data, length, &n);

#if (_FS_REENTRANT == 0)
   //Leave critical section
   osReleaseMutex(&fsMutex);
#endif

   //Any error to report?
   if(res != FR_OK)
      return __ERROR_FAILURE;

   //Sanity check
   if(n != length)
      return __ERROR_FAILURE;

   //Successful processing
   return __NO_ERROR;
}


/**
 * @brief Read data from the specified file
 * @param[in] file Handle that identifies the file to be read
 * @param[in] data Pointer to the buffer where to copy the data
 * @param[in] size Size of the buffer, in bytes
 * @param[in] length Number of data bytes to read
 * @return Error code
 **/

error_t fsReadFile(FsFile *file, void *data, size_t size, size_t *length)
{
   UINT n;
   FRESULT res;

   //No data has been read yet
   *length = 0;

   //Make sure the file pointer is valid
   if(file == NULL)
      return __ERROR_INVALID_PARAMETER;

#if (_FS_REENTRANT == 0)
   //Enter critical section
   osAcquireMutex(&fsMutex);
#endif

   //Read data
   res = f_read((FIL *) file, data, size, &n);

#if (_FS_REENTRANT == 0)
   //Leave critical section
   osReleaseMutex(&fsMutex);
#endif

   //Any error to report?
   if(res != FR_OK)
      return __ERROR_FAILURE;

   //End of the directory stream?
   if(!n)
      return __ERROR_END_OF_STREAM;

   //Total number of data that have been read
   *length = n;
   //Successful processing
   return __NO_ERROR;
}


/**
 * @brief Close a file
 * @param[in] file Handle that identifies the file to be closed
 **/

void fsCloseFile(FsFile *file)
{
   //Make sure the file pointer is valid
   if(file != NULL)
   {
      //Enter critical section
      osAcquireMutex(&fsMutex);

      //Close the specified file
      f_close((FIL *) file);
      //Mark the corresponding entry as free
      ((FIL *) file)->fs = NULL;

      //Leave critical section
      osReleaseMutex(&fsMutex);
   }
}


/**
 * @brief Check whether a directory exists
 * @param[in] path NULL-terminated string specifying the directory path
 * @return The function returns TRUE if the directory exists. Otherwise FALSE is returned
 **/

bool_t fsDirExists(const char_t *path)
{
   FRESULT res;
   FILINFO fno;

//Long filename support?
#if (_USE_LFN != 0)
   fno.lfname = NULL;
   fno.lfsize = 0;
#endif

   //Make sure the pathname is valid
   if(path == NULL)
      return FALSE;

   //Root directory?
   if(!strcmp(path, "/"))
      return TRUE;

#if (_FS_REENTRANT == 0)
   //Enter critical section
   osAcquireMutex(&fsMutex);
#endif

   //Check whether the file exists
   res = f_stat(path, &fno);

#if (_FS_REENTRANT == 0)
   //Leave critical section
   osReleaseMutex(&fsMutex);
#endif

   //Any error to report?
   if(res != FR_OK)
      return FALSE;

   //Valid directory?
   if(fno.fattrib & AM_DIR)
      return TRUE;
   else
      return FALSE;
}


/**
 * @brief Create a directory
 * @param[in] path NULL-terminated string specifying the directory path
 * @return Error code
 **/

error_t fsCreateDir(const char_t *path)
{
   FRESULT res;

   //Make sure the pathname is valid
   if(path == NULL)
      return __ERROR_INVALID_PARAMETER;

#if (_FS_REENTRANT == 0)
   //Enter critical section
   osAcquireMutex(&fsMutex);
#endif

   //Create a new directory
   res = f_mkdir(path);

#if (_FS_REENTRANT == 0)
   //Leave critical section
   osReleaseMutex(&fsMutex);
#endif

   //Any error to report?
   if(res != FR_OK)
      return __ERROR_FAILURE;

   //Successful processing
   return __NO_ERROR;
}


/**
 * @brief Remove a directory
 * @param[in] path NULL-terminated string specifying the directory path
 * @return Error code
 **/

error_t fsRemoveDir(const char_t *path)
{
   FRESULT res;

   //Make sure the pathname is valid
   if(path == NULL)
      return __ERROR_INVALID_PARAMETER;

#if (_FS_REENTRANT == 0)
   //Enter critical section
   osAcquireMutex(&fsMutex);
#endif

   //Remove the specified directory
   res = f_unlink(path);

#if (_FS_REENTRANT == 0)
   //Leave critical section
   osReleaseMutex(&fsMutex);
#endif

   //Any error to report?
   if(res != FR_OK)
      return __ERROR_FAILURE;

   //Successful processing
   return __NO_ERROR;
}


/**
 * @brief Open a directory stream
 * @param[in] path NULL-terminated string specifying the directory path
 * @return Directory handle
 **/

FsDir *fsOpenDir(const char_t *path)
{
   uint_t i;
   FRESULT res;

   //Directory pointer
   FsDir *dir = NULL;

   //Make sure the pathname is valid
   if(path == NULL)
      return NULL;

   //Enter critical section
   osAcquireMutex(&fsMutex);

   //Loop through the directory objects
   for(i = 0; i < FS_MAX_DIRS; i++)
   {
      //Unused directory object found?
      if(dirTable[i].fs == NULL)
      {
         //Open the specified directory
         res = f_opendir(&dirTable[i], path);

         //Check status code
         if(res == FR_OK)
            dir = &dirTable[i];

         //Stop immediately
         break;
      }
   }

   //Leave critical section
   osReleaseMutex(&fsMutex);
   //Return a handle to the directory
   return dir;
}


/**
 * @brief Read an entry from the specified directory stream
 * @param[in] dir Handle that identifies the directory
 * @param[out] dirEntry Pointer to a directory entry
 * @return Error code
 **/

error_t fsReadDir(FsDir *dir, FsDirEntry *dirEntry)
{
   FRESULT res;
   FILINFO fno;
   char_t *fn;
   size_t n;

//Long filename support?
#if (_USE_LFN != 0)
   char_t lfn[_MAX_LFN + 1];
   fno.lfname = lfn;
   fno.lfsize = sizeof(lfn);
#endif

   //Make sure the directory pointer is valid
   if(dir == NULL)
      return __ERROR_INVALID_PARAMETER;

#if (_FS_REENTRANT == 0)
   //Enter critical section
   osAcquireMutex(&fsMutex);
#endif

   //Read the specified directory
   res = f_readdir((DIR *) dir, &fno);

#if (_FS_REENTRANT == 0)
   //Leave critical section
   osReleaseMutex(&fsMutex);
#endif

   //Any error to report?
   if(res != FR_OK)
      return __ERROR_FAILURE;

   //End of the directory stream?
   if(fno.fname[0] == '\0')
      return __ERROR_END_OF_STREAM;

//Long filename support?
#if (_USE_LFN != 0)
   fn = (*fno.lfname != '\0') ? fno.lfname : fno.fname;
#else
   fn = fno.fname;
#endif

   //File attributes
   dirEntry->attributes = fno.fattrib;
   //File size
   dirEntry->size = fno.fsize;
   //Last modified date
   dirEntry->modified.year = 1980 + ((fno.fdate >> 9) & 0x7F);
   dirEntry->modified.month = (fno.fdate >> 5) & 0x0F;
   dirEntry->modified.day = fno.fdate & 0x1F;
   dirEntry->modified.dayOfWeek = 0;
   //Last modified time
   dirEntry->modified.hours = (fno.ftime >> 11) & 0x1F;
   dirEntry->modified.minutes = (fno.ftime >> 5) & 0x3F;
   dirEntry->modified.seconds = (fno.ftime & 0x1F) * 2;
   dirEntry->modified.milliseconds = 0;

   //Make sure the date is valid
   dirEntry->modified.month = MAX(dirEntry->modified.month, 1);
   dirEntry->modified.month = MIN(dirEntry->modified.month, 12);
   dirEntry->modified.day = MAX(dirEntry->modified.day, 1);
   dirEntry->modified.day = MIN(dirEntry->modified.day, 31);

   //Retrieve the length of the file name
   n = strlen(fn);
   //Limit the number of characters to be copied
   n = MIN(n, FS_MAX_NAME_LEN);

   //Copy file name
   strncpy(dirEntry->name, fn, n);
   //Properly terminate the string with a NULL character
   dirEntry->name[n] = '\0';

   //Successful processing
   return __NO_ERROR;
}


/**
 * @brief Close a directory stream
 * @param[in] dir Handle that identifies the directory to be closed
 **/

void fsCloseDir(FsDir *dir)
{
   //Make sure the directory pointer is valid
   if(dir != NULL)
   {
      //Enter critical section
      osAcquireMutex(&fsMutex);

#if (_FATFS == 80960)
      //Close the specified directory
      f_closedir((DIR *) dir);
#endif
      //Mark the corresponding entry as free
      ((DIR *) dir)->fs = NULL;

      //Leave critical section
      osReleaseMutex(&fsMutex);
   }
}
