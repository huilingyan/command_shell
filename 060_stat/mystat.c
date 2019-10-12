#include <ctype.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

void report_error(const char * s) {  //error report
  fprintf(stderr, "%s", s);
  exit(EXIT_FAILURE);
}

int main(int argc, char ** argv) {  //implement the function of 'stat'
  struct stat info;
  char * mode;

  if (argc != 2) {
    report_error("Usage: <pathname>\n");
  }

  if (lstat(argv[1], &info) == -1) {
    report_error("Error occurs when using system call!\n");
  }

  switch (info.st_mode & S_IFMT) {  //choose file type
    case S_IFBLK:
      mode = strdup("block device");
      break;
    case S_IFCHR:
      mode = strdup("character device");
      break;
    case S_IFDIR:
      mode = strdup("directory");
      break;
    case S_IFIFO:
      mode = strdup("FIFO/pipe");
      break;
    case S_IFLNK:
      mode = strdup("symlink");
      break;
    case S_IFREG:
      mode = strdup("regular file");
      break;
    case S_IFSOCK:
      mode = strdup("socket");
      break;
    default:
      mode = strdup("unknown?");
      break;
  }

  printf("  File: %s\n", argv[1]);
  printf("  Size: %-10lu\tBlocks: %-10lu IO Block: %-6lu %s\n",
         info.st_size,
         info.st_blocks,
         info.st_blksize,
         mode);
  printf("Device: %lxh/%lud\tInode: %-10lu  Links: %lu\n",
         info.st_dev,
         info.st_dev,
         info.st_ino,
         info.st_nlink);
  free(mode);  //free the memory allocated by 'strdup'
  return EXIT_SUCCESS;
}
//This function is for Step 4
char * time2str(const time_t * when, long ns) {
  char * ans = malloc(128 * sizeof(*ans));
  char temp1[64];
  char temp2[32];
  const struct tm * t = localtime(when);
  strftime(temp1, 512, "%Y-%m-%d %H:%M:%S", t);
  strftime(temp2, 32, "%z", t);
  snprintf(ans, 128, "%s.%09ld %s", temp1, ns, temp2);
  return ans;
}
