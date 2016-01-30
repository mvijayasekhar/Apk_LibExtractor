/** @file libextract.h
 *  @brief  Including the standard libraries headers.
 *  @details  This contains inclusion of standard headers 
 *  @author Vijaya Sekhar M
 *  @date Dec 2015
 *  @version  1.1
 *  @bug  No known bugs.
 *  */

#ifndef  __INC_HEADER__
#define __INC_HEADER__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>

#ifdef unix
# include <unistd.h>
# include <utime.h>
#else
# include <direct.h>
# include <io.h>
#endif
#include <regex.h>
#include <elf.h> 

#define MAXFILENAME (256)
#define CASESENSITIVITY (0) 
#define ELF_HEADER_SIZE (52)
#define ARM_V7 (0xa)
#define ARM_ATTR_SEC (0x70000003)
#define VER_OFFSET (25)

/** Defining Colors */
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
#define RESET "\033[0m"
#endif

