#include <stdlib.h>
#include <string.h>

#include "ImageManager.h"
#include "DxMock.h"

// 初期化関数
// _imageListを使う前に必ず呼ぶこと
// 戻り値：0（成功）
//         -1（失敗）
int imageManagerInit(void)
{
  // 先頭要素の作成
  _imageList = (ImageList *)malloc(sizeof(ImageList));
  if(_imageList == NULL)
    return -1;

  // 先頭要素はダミー
  _imageList->next = NULL;
  _imageList->prev = _imageList;
  _imageList->filehandle = -1;
  _imageList->filename = NULL;

  return 0;
}

// リスト開放関数
// もう_imageListを使わなくなった時に
// 全ての画像を解放後、リストを開放する
void imageManagerDestroy(void)
{
  ImageList * tmp;

  // 先頭以外のimageを開放していく
  for(tmp=_imageList->next;tmp!=NULL;tmp=tmp->next)
  {
    _imageManagerDeleteImageFromList(tmp);
  }

  // 先頭を開放する
  _imageManagerDestroyImage(_imageList);
  _imageList = NULL;
}

// 画像を取得する
// リストに検索をかけ、見つかる（読み込み済み）時、ファイルハンドルを返す
// 見つからない時はLoadGraphする
// 引数　：ファイル名
// 戻り値：ファイルハンドル
int imageManagerGetImage(const char * const filename)
{
  ImageList * image = NULL;
  int filehandle = -1;

  // ファイル名をキーに検索する
  image = _imageManagerSearchByFilename(filename);
  if(image == NULL)
  {
    // メモリに読み込まれていないので新たに読み込む
    filehandle = _imageManagerAddImageToList(filename);
  }
  else
  {
    // 読み込まれていた場合はファイルハンドルを取得
    filehandle = image->filehandle;
  }

  return filehandle;
}

// 画像をリストから削除する
// 引数　：ファイルハンドル
// 戻り値：0（成功）
// 　　　　-1（失敗）
int imageManagerDeleteImage(const int filehandle)
{
  ImageList * image = NULL;

  image = _imageManagerSearchByFilehandle(filehandle);
  // 見つからない時
  if(image == NULL)
    return -1;

  _imageManagerDeleteImageFromList(image);

  return 0;
}

// リストにノードを追加する
// 引数　：ファイル名
// 戻り値：ファイルハンドル
static int _imageManagerAddImageToList(const char * const filename)
{
  ImageList * image = NULL;
  ImageList * prev = NULL;

  // ノードを作成
  image = _imageManagerLoadImage(filename);
  // 失敗した時
  if(image == NULL)
    return -1;

  // リストをつなぎ合わせる
  prev = _imageList->prev;
  prev->next = image;
  image->prev = prev;
  _imageList->prev = image;
  image->next = NULL;

  return image->filehandle;
}

// リストからノードを削除する
// 引数　：削除したいノードのポインタ
// 戻り値：0（成功）
// 　　　　-1（失敗）
static int _imageManagerDeleteImageFromList(ImageList * const image)
{
  // リストの先頭のダミーが指定された時
  if(image->filename==NULL)
    return -1;

  // リストをつなぎ合わせる
  image->prev->next = image->next;
  // 最後のノードではない時、次の要素もつなぎ合わせる
  if(image->next!=NULL)
  {
    image->next->prev = image->prev;
  }
 
  // ノードの開放
  _imageManagerDestroyImage(image);

  return 0;
}

// LoadGraphを実行し、ノードを作成する
// 引数　：ファイル名
// 戻り値：リストノード
static ImageList * _imageManagerLoadImage(const char * const filename)
{
  ImageList * image = NULL;
  int filehandle = -1;

  // 画像を読み込む
  filehandle = LoadGraph(filename);
  if(filehandle == -1)
    return NULL;

  // ノードの確保
  image = (ImageList *)malloc(sizeof(ImageList));
  //失敗した時、読み込んだ画像も開放する
  if(image == NULL)
  {
    DeleteGraph(filehandle);
    return NULL;
  }

  // ファイル名をコピーする
  image->filename = NULL;
  image->filename = (char *)malloc(sizeof(char)*strlen(filename));
  // 失敗した時、画像とノードを開放する
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
// 引数　：開放したいノードのポインタ
static void _imageManagerDestroyImage(ImageList * const image)
{
  // 画像が読み込まれている場合
  if(image->filehandle!=-1)
    DeleteGraph(image->filehandle);
  free(image->filename);
  free(image);
}

// ファイル名でリストを検索する
// 引数　：ファイル名
// 戻り値：ノードのポインタ
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
// 引数　：ファイルハンドル
// 戻り値：ノードのポインタ
// strcmpがない分、ファイル名で検索するより高速
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
