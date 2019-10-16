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
      mode = strdup("block special file");
      break;
    case S_IFCHR:
      mode = strdup("character special file");
      break;
    case S_IFDIR:
      mode = strdup("directory");
      break;
    case S_IFIFO:
      mode = strdup("fifo");
      break;
    case S_IFLNK:
      mode = strdup("symbolic link");
      break;
    case S_IFREG:
      mode = strdup("regular file");
      break;
    case S_IFSOCK:
      mode = strdup("socket");
      break;
    default:
      mode = strdup("Unknown");
      break;
  }
  //get the target of the linktarget
  char linktarget[256];
  ssize_t len = readlink(pathname, linktarget, 256);

  if (len <= 255) {  //if the size is smaller than 255, add the null-terminator at the end
    linktarget[len] = '\0';
  }
  else {  //if the size is 255, we have to change the last element to '\0'
    linktarget[255] = '\0';
  }

  //print out info
  if (S_ISLNK(info.st_mode)) {  //if the file is a symbolic link, print out the target
    printf("  File: %s -> %s\n", pathname, linktarget);
  }
  else {  //otherwise juse print out the filename
    printf("  File: %s\n", pathname);
  }
  printf("  Size: %-10lu\tBlocks: %-10lu IO Block: %-6lu %s\n",
         info.st_size,
         info.st_blocks,
         info.st_blksize,
         mode);

  if ((S_ISCHR(info.st_mode)) ||
      (S_ISBLK(info.st_mode))) {  //if the file is a device, print the device type
    printf("Device: %lxh/%lud\tInode: %-10lu  Links: %-5lu Device type: %d,%d\n",
           info.st_dev,
           info.st_dev,
           info.st_ino,
           info.st_nlink,
           major(info.st_rdev),
           minor(info.st_rdev));
  }
  else {  //otherwise just print out by default method
    printf("Device: %lxh/%lud\tInode: %-10lu  Links: %lu\n",
           info.st_dev,
           info.st_dev,
           info.st_ino,
           info.st_nlink);
  }
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

  printf("Access: (%04o/%s)  ", info.st_mode & ~S_IFMT, pmsn);
}

void user_info(Stat info) {  //print out the user information
  struct passwd * pwd;
  struct group * gwd;
  pwd = getpwuid(info.st_uid);
  gwd = getgrgid(info.st_gid);

  printf("Uid: (%5d/%8s)   Gid: (%5d/%8s)\n",
         info.st_uid,
         pwd->pw_name,
         info.st_gid,
         gwd->gr_name);
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

void times(Stat info) {  //print out the last four lines, i.e. times
  char * atime = time2str(&info.st_atime, info.st_atim.tv_nsec);  //access time
  char * mtime = time2str(&info.st_mtime, info.st_mtim.tv_nsec);  //modify time
  char * ctime = time2str(&info.st_ctime, info.st_ctim.tv_nsec);  //change time

  printf("Access: %s\n", atime);
  printf("Modify: %s\n", mtime);
  printf("Change: %s\n", ctime);
  printf(" Birth: -\n");

  //free the memory allocated by time2str
  free(atime);
  free(mtime);
  free(ctime);
}

int main(int argc, char ** argv) {  //implement the function of 'stat'
  Stat info;

  if (argc <= 1) {  //if no arguments
    report_error("Usage: <pathname>\n");
  }

  for (int i = 1; i < argc; i++) {  //for each input
    if (lstat(argv[i], &info) != 0) {
      report_error("Error occurs when calling 'lstat'!\n");
    }

    first_three_lines(info, argv[i]);  //print out the first three lines of stat
    Access(info);                      //print out the accessibility information
    user_info(info);                   //print out the user information
    times(info);                       //print out the files' manipulation time
  }

  return EXIT_SUCCESS;
}
