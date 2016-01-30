/** @file libextract.c
 *  @brief  Contains Main Function
 *  @details  This contains the main function which triggers the extraction operation
 *  @author Vijaya Sekhar M
 *  @date Jan 2015
 *  @version  1.1
 *  @bug  No known bugs.
 *  */

#include  "unzip.h"
#include  "libextract.h"

/** Function Declaration */
int do_list(unzFile uf);
void do_help();


/** Main Function */
int main(int argc, char *argv[])
{
  int i;
  const char *apkfilename=NULL;
  char filename_try[MAXFILENAME+16] = "";
  unzFile uf=NULL;

  /** Checking the no.of args */
  if ( argc == 1 ) {
    do_help();
    return 0;
  } else 

    /** Handling multiple files */
    for ( i = 1; i < argc; i++ ){
      apkfilename = argv[i];
      printf(KBLU"\nFile Name : %s\n"RESET,apkfilename);
      /** Providing the feature to specify just name instead of name.apk */
      if (apkfilename!=NULL) {
        strncpy(filename_try, apkfilename,MAXFILENAME-1);
        /** strncpy doesnt append the trailing NULL, of the string is too long. */
        filename_try[ MAXFILENAME ] = '\0';

        /** Opening apk file using minizip library */
        uf = unzOpen(apkfilename);
        if ( uf == NULL ) {
          strcat(filename_try, ".apk");
          uf = unzOpen(filename_try);
        }
      }

      if (uf == NULL ) {
        printf(KRED"Cannot open %s or %s.apk\n"RESET,apkfilename,apkfilename);
        return 1;
      }
      /** Calling the actual extraction operation */
      do_list(uf);
    }

  /** Closing the file */
  unzCloseCurrentFile(uf);

  return 0;
}
