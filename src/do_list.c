/** @file do_list.c
 *  @brief  Performs extraction using minizip library
 *  @details  This contains the extraction operation using the minizip apis
 *  @author Vijaya Sekhar M
 *  @date Jan 2015
 *  @version  1.2
 *  @bug  No known bugs.
 *  */


#include  "libextract.h"
#include  "unzip.h"

/** Declaration of regular expressions */
const char *lib_so          =   ".*\\.so$";

int do_extract_onefile(unzFile uf, 
    const char* filename, 
    int opt_extract_without_path,
    int opt_overwrite,
    const char* password);

//! do_list() is a normal function which lists the libraries in apk
/*!                                                                             
  \param unzFile                                                                   
  \return int                                                                
  */ 
int do_list(unzFile uf)
{

  int err;
  int lib_count = 0;
  uLong i;
  unz_global_info gi;

  regex_t regex_so;

  /** Extracting the Central Structure Information */
  err = unzGetGlobalInfo (uf,&gi);
  if (err!=UNZ_OK)
    printf(KRED"error %d with zipfile in unzGetGlobalInfo \n"RESET,err);

  /** Registring the regular expressions */
  regcomp(&regex_so, lib_so, 0);

  printf (KGRN"%2s%10s%20s\n"RESET, "SN", "Lib Name", "Arch");
  printf("%s %10s%22s\n", "--","----------","-------");
  for ( i = 0; i < gi.number_entry; i++ )
  {
    char filename_inzip[256];
    unz_file_info file_info;

    /** Getting the filename from the central structure */
    err = unzGetCurrentFileInfo(uf,&file_info,filename_inzip,sizeof(filename_inzip),NULL,0,NULL,0);
    if (err != UNZ_OK) {
      printf(KRED"error %d with zipfile in unzGetCurrentFileInfo\n"RESET,err);
      break;
    }

    /** Checking for *.so files */
    if (!regexec(&regex_so, filename_inzip, 0, NULL, 0) ){
      lib_count++;

      printf(KMAG"%d"RESET" %10s",lib_count, filename_inzip);
      /** Matching the ArchType */
      do_extract_onefile(uf,filename_inzip, 0, 0, NULL); 
    }

    /** Fetching the next file */
    if (( i + 1 ) < gi.number_entry ) {
      err = unzGoToNextFile(uf);
      if (err != UNZ_OK) {
        printf(KRED"error %d with zipfile in unzGoToNextFile\n"RESET,err);
        break;
      }
    }
  }
  printf(KYEL"\nNumber of Libs: %d\n\n"RESET, lib_count);

  return 0;
}

