#include "outname.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * computeOutputFileName(const char * inputName) {
  //WRITE ME
  char * outFileName = malloc((strlen(inputName) + 8) * sizeof(*outFileName));
  strcpy(outFileName, inputName);
  strcat(outFileName, ".counts");

  return outFileName;
}
