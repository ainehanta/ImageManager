#include <stdio.h>
#include "ImageManager.h"
#include "DxMock.h"

int main(void)
{
  int filehandle;
  int result;

  if(imageManagerInit()!=0)
  {
    printf("Error: Can't initializing ResourceList\n");
  }

  filehandle = imageManagerGetImage("file01.png");
  printf("file01.png: filehandle is %d (Called first time)\n",filehandle);

  filehandle = imageManagerGetImage("file02.png");
  printf("file02.png: filehandle is %d (Called first time)\n",filehandle);

  filehandle = imageManagerGetImage("notfound.png");
  printf("notfound.png: filehandle is %d (Can't open file)\n",filehandle);

  filehandle = imageManagerGetImage("file02.png");
  printf("file02.png: filehandle is %d (Called second times)\n",filehandle);

  result = imageManagerDeleteImage(filehandle);
  printf("file02.png: delete successful %d\n",result);

  result = imageManagerDeleteImage(-1);
  printf("notfound: delete fail %d\n",result);

  imageManagerDestroy();

  return 0;
}
