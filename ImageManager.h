#ifndef __IMAGEMANAGER_H
#define __IMAGEMANAGER_H

struct _ImageList
{
  struct _ImageList * next;
  struct _ImageList * prev;
  char * filename;
  int filehandle;
};

typedef struct _ImageList ImageList;

static ImageList * _imageList;

int imageManagerInit(void);
void imageManagerDestroy(void);
int imageManagerGetImage(const char * const filename);
int imageManagerDeleteImage(const int filehandle);

static int _imageManagerAddImageToList(const char * const filename);
static int _imageManagerDeleteImageFromList(ImageList * const image);
static ImageList * _imageManagerLoadImage(const char * const filename);
static void _imageManagerDestroyImage(ImageList * const image);
static ImageList * _imageManagerSearchByFilename(const char * const filename);
static ImageList *  _imageManagerSearchByFilehandle(const int filehandle);
#endif
