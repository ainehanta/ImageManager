#include <stdlib.h>
#include <string.h>

#include "ImageManager.h"
#include "DxMock.h"

// 初期化関数
// _imageListを使う前に必ず呼ぶこと
int imageManagerInit(void)
{
  _imageList = (ImageList *)malloc(sizeof(ImageList));
  if(_imageList == NULL)
    return -1;

  _imageList->next = NULL;
  _imageList->prev = _imageList;
  _imageList->filehandle = -1;
  _imageList->filename = NULL;

  return 0;
}

// 削除関数
// もう_imageListを使わなくなった時に
// 全ての画像を解放後、リストを開放する
int imageManagerDestroy(void)
{
  ImageList * tmp;

  for(tmp=_imageList->next;tmp!=NULL;tmp=tmp->next)
  {
    _imageManagerDeleteImageFromList(tmp);
  }

  _imageManagerDestroyImage(_imageList);
  _imageList = NULL;
}

// 画像を取得する
// リストに検索をかけ、見つかる（読み込み済み）時
// ファイルハンドルを返す
// 見つからない時はLoadGraphする
int imageManagerGetImage(const char * const filename)
{
  ImageList * image = NULL;
  int ret = -1;

  image = _imageManagerSearchByFilename(filename);
  if(image == NULL)
  {
    ret = _imageManagerAddImageToList(filename);
  }
  else
  {
    ret = image->filehandle;
  }

  return ret;
}

// 画像をリストから削除する
int imageManagerDeleteImage(const int filehandle)
{
  int ret = -1;
  ImageList * image = NULL;

  image = _imageManagerSearchByFilehandle(filehandle);
  if(image == NULL)
    return -1;

  _imageManagerDeleteImageFromList(image);

  return 0;
}

// リストにノードを追加する
static int _imageManagerAddImageToList(const char * const filename)
{
  ImageList * image = NULL;
  ImageList * prev = NULL;

  image = _imageManagerLoadImage(filename);
  if(image == NULL)
    return -1;

  prev = _imageList->prev;
  prev->next = image;
  image->prev = prev;
  _imageList->prev = image;
  image->next = NULL;

  return image->filehandle;
}

// リストからノードを削除する
static int _imageManagerDeleteImageFromList(ImageList * const image)
{
  if(image->filename==NULL)
    return -1;

  image->prev->next = image->next;
  if(image->next!=NULL)
  {
    image->next->prev = image->prev;
  }
 
  _imageManagerDestroyImage(image);

  return 0;
}

// LoadGraphを実行し、ノードを作成する
static ImageList * _imageManagerLoadImage(const char * const filename)
{
  ImageList * image = NULL;
  int filehandle = -1;

  filehandle = LoadGraph(filename);
  if(filehandle == -1)
    return NULL;

  image = (ImageList *)malloc(sizeof(ImageList));
  if(image == NULL)
  {
    DeleteGraph(filehandle);
    return NULL;
  }

  image->filename = NULL;
  image->filename = (char *)malloc(sizeof(char)*strlen(filename));
  if(image->filename == NULL)
  {
    DeleteGraph(filehandle);
    free(image);
    return NULL;
  }

  strcpy(image->filename,filename);

  image->filehandle = filehandle;

  image->next = image->prev = NULL;

  return image;
}

// ノードを開放する
static void _imageManagerDestroyImage(ImageList * const image)
{
  if(image->filehandle!=-1)
    DeleteGraph(image->filehandle);
  free(image->filename);
  free(image);
}

// ファイル名でリストを検索する
static ImageList * _imageManagerSearchByFilename(const char * const filename)
{
  ImageList * tmp;

  for(tmp=_imageList->next;tmp!=NULL;tmp=tmp->next)
  {
    if(strcmp(tmp->filename,filename)==0)
      return tmp;
  }

  return NULL;
}

// ファイルハンドルで検索する
static ImageList *  _imageManagerSearchByFilehandle(const int filehandle)
{
  ImageList * tmp;

  for(tmp=_imageList->next;tmp!=NULL;tmp=tmp->next)
  {
    if(tmp->filehandle == filehandle)
      return tmp;
  }

  return NULL;
}
