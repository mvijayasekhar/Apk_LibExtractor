/** @file do_extract.c
 *  @brief  Performs extraction using minizip library
 *  @details  This contains the extraction operation using the minizip apis
 *  @author Vijaya Sekhar M
 *  @date Jan 2015
 *  @version  1.2
 *  @bug  No known bugs.
 *  */

#include  "libextract.h"
#include  "unzip.h"

/** Declaration ELF structures */
Elf32_Ehdr *elf_head;
Elf32_Shdr *elf_shdr; 

//! do_extract_currentfile() is a function which extracts ELF format from a file
/*! 
	\param unzFile, const int*, int*, const char*
	\return int
	*/ 
int do_extract_currentfile(uf,popt_extract_without_path,popt_overwrite,password)
	unzFile uf;
	const int* popt_extract_without_path;
	int* popt_overwrite;
	const char* password;
{
  int err=UNZ_OK;
  char* buf = NULL;
  char elf_hdr[ELF_HEADER_SIZE];
  int sec_index;
  uInt size_buf;

  /** Opening current file */
  err = unzOpenCurrentFilePassword(uf,password);
  if (err!=UNZ_OK)
  {
    printf("error %d with zipfile in unzOpenCurrentFilePassword\n",err);
  }

  /** Reading ELF header in current file */
  err = unzReadCurrentFile(uf,elf_hdr, ELF_HEADER_SIZE);
  if (err<0)
  {
    printf("error %d with zipfile in unzReadCurrentFile\n",err);
  }
  if (err>0) {
    elf_head = (Elf32_Ehdr *)elf_hdr;

    /** Printing architechture based on machine type in elf header */
    switch (elf_head->e_machine) {
      case EM_NONE:
        printf(KRED"%10s\n"RESET, "Unknown");
        break;
      case EM_386:
        printf(KCYN"%10s\n"RESET, "x86");
        break;
      case EM_X86_64:
        printf(KCYN"%10s\n"RESET, "x86_64");
        break;
      case EM_MIPS:
        printf(KCYN"%10s\n"RESET, "MIPS");
        break;
      case EM_ARM:
        /** In ARM case need to cheack arm attributes section for
         * version*/
        size_buf = elf_head->e_shoff +
          ((elf_head->e_shstrndx) * elf_head->e_shentsize);
        if ((buf = (void*)malloc(size_buf)) == NULL)
        {
          printf("Error allocating memory\n");
          return UNZ_INTERNALERROR;
        }

        /** Reading contents from file */
        err = unzReadCurrentFile(uf, buf, size_buf);
        for (sec_index = elf_head->e_shnum; sec_index > 0 ; sec_index--) {

          elf_shdr = (Elf32_Shdr *)(buf + elf_head->e_shoff + 
              ((sec_index * elf_head->e_shentsize) - ELF_HEADER_SIZE));

          /** Checking for the ARM Attribute Section */
          if ( elf_shdr->sh_type == ARM_ATTR_SEC) {
            /** Checking the version */
            if ( *(buf + ((elf_shdr->sh_offset + 
                      VER_OFFSET)) - ELF_HEADER_SIZE) == ARM_V7) {
              printf(KCYN"%10s\n"RESET, "armeabi-v7"); 
            }
            else
              printf(KCYN"%10s\n"RESET, "armeabi"); 
            break;
          }
        }
        break; 
      case EM_AARCH64:
        printf(KCYN"%10s\n"RESET, "arm64-v8"); 
        break; 
    }
  }

  unzCloseCurrentFile(uf);
  if (buf)
    free(buf);
  return err;
}



//! do_extract_onefile() is a normal function which extracts a file in apk
/*! 
	\param unzFile, const int*, int, int, const char*
	\return int
	*/ 
int do_extract_onefile(uf,filename,opt_extract_without_path,opt_overwrite,password)
	unzFile uf;
	const char* filename;
	int opt_extract_without_path;
	int opt_overwrite;
	const char* password;
{
	if (unzLocateFile(uf,filename,CASESENSITIVITY)!=UNZ_OK)
	{
		printf("file %s not found in the zipfile\n",filename);
		return 2;
	}

	if (do_extract_currentfile(uf,&opt_extract_without_path,
				&opt_overwrite,
				password) == UNZ_OK)
		return 0;
	else
		return 1; 
} 

