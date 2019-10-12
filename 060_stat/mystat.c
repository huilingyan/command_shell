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

typedef struct stat Stat;
void report_error(const char * s) {  //error report
  fprintf(stderr, "%s", s);
  exit(EXIT_FAILURE);
}

void first_three_lines(Stat info,
                       char * pathname) {  //step1: print out the first three lines
  char * mode;
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

  printf("  File: %s\n", pathname);
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
}
void Access(Stat info) {
  char pmsn[11];
  switch (info.st_mode & S_IFMT) {  //choose file type
    case S_IFBLK:
      pmsn[0] = 'b';
      break;
    case S_IFCHR:
      pmsn[0] = 'c';
      break;
    case S_IFDIR:
      pmsn[0] = 'd';
      break;
    case S_IFIFO:
      pmsn[0] = 'p';
      break;
    case S_IFLNK:
      pmsn[0] = 'l';
      break;
    case S_IFREG:
      pmsn[0] = '-';
      break;
    case S_IFSOCK:
      pmsn[0] = 's';
      break;
    default:
      report_error("Unknown type!\n");
      break;
  }

  if (info.st_mode & S_IRUSR) {  //check if the file is readable
    pmsn[1] = 'r';
  }
  else {
    pmsn[1] = '-';
  }

  if (info.st_mode & S_IWUSR) {  //check if the file is writable
    pmsn[2] = 'w';
  }
  else {
    pmsn[2] = '-';
  }

  if (info.st_mode & S_IXUSR) {  //check if the file is executable
    pmsn[3] = 'x';
  }
  else {
    pmsn[3] = '-';
  }

  if (info.st_mode & S_IRGRP) {  //check if the file is readable for group
    pmsn[4] = 'r';
  }
  else {
    pmsn[4] = '-';
  }

  if (info.st_mode & S_IWGRP) {  //check if the file is writable for group
    pmsn[5] = 'w';
  }
  else {
    pmsn[5] = '-';
  }
  if (info.st_mode & S_IXGRP) {  //check if the file is executable for group
    pmsn[6] = 'x';
  }
  else {
    pmsn[6] = '-';
  }
  if (info.st_mode & S_IROTH) {  //check if the file is readable for other
    pmsn[7] = 'r';
  }
  else {
    pmsn[7] = '-';
  }

  if (info.st_mode & S_IWOTH) {  //check if the file is writable for other
    pmsn[8] = 'w';
  }
  else {
    pmsn[8] = '-';
  }
  if (info.st_mode & S_IXOTH) {  //check if the file is executable for other
    pmsn[9] = 'x';
  }
  else {
    pmsn[9] = '-';
  }

  pmsn[10] = '\0';  //add the null-terminator

  printf("Access: (%04o/%s)\n", info.st_mode & ~S_IFMT, pmsn);
}

int main(int argc, char ** argv) {  //implement the function of 'stat'
  Stat info;

  if (argc != 2) {
    report_error("Usage: <pathname>\n");
  }

  if (lstat(argv[1], &info) == -1) {
    report_error("Error occurs when using system call!\n");
  }
  first_three_lines(info, argv[1]);
  Access(info);

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
