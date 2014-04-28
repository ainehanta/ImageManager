#include "DxMock.h"
#include <stdlib.h>
#include <string.h>

int LoadGraph(const char * const filename)
{
  static int count = 0;

  if(strcmp(filename,"file01.png")==0 ||
     strcmp(filename,"file02.png")==0)
    return ++count;

  return -1;
}

int DeleteGraph(const int filehandle)
{
  if(filehandle==-1)
    return -1;

  return 0;
}
