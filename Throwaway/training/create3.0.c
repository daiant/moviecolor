#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#define _bitsperpixel 24
#define _planes 1
#define _compression 0
#pragma pack(push,1)
typedef struct{
    uint8_t signature[2];
    uint32_t filesize;
    uint32_t reserved;
    uint32_t fileoffset_to_pixelarray;
} fileheader;
typedef struct{
    uint32_t dibheadersize;
    uint32_t width;
    uint32_t height;
    uint16_t planes;
    uint16_t bitsperpixel;
    uint32_t compression;
    uint32_t imagesize;
    uint32_t ypixelpermeter;
    uint32_t xpixelpermeter;
    uint32_t numcolorspallette;
    uint32_t mostimpcolor;
} bitmapinfoheader;
typedef struct {
    fileheader fileheader;
    bitmapinfoheader bitmapinfoheader;
} bitmap;
#pragma pack(pop)


long aux_cosa(char* a) {
  return strtol(a, NULL, 10);
}
void save_bitmap(int width, int height, int dpi, char* argv[]) {
  int pixelbytesize = width*height*_bitsperpixel/8;
  int filesize = pixelbytesize+sizeof(bitmap);
  FILE *fp = fopen("test.bmp","wb");
  bitmap *pbitmap  = (bitmap*)calloc(1,sizeof(bitmap));
  uint8_t *pixelbuffer = (uint8_t*)malloc(pixelbytesize);
  strcpy(pbitmap->fileheader.signature,"BM");
  pbitmap->fileheader.filesize = filesize;
  pbitmap->fileheader.fileoffset_to_pixelarray = sizeof(bitmap);

  pbitmap->bitmapinfoheader.dibheadersize =sizeof(bitmapinfoheader);
  pbitmap->bitmapinfoheader.width = width;
  pbitmap->bitmapinfoheader.height = height;
  pbitmap->bitmapinfoheader.planes = _planes;
  pbitmap->bitmapinfoheader.bitsperpixel = _bitsperpixel;
  pbitmap->bitmapinfoheader.compression = _compression;
  pbitmap->bitmapinfoheader.imagesize = pixelbytesize;
  pbitmap->bitmapinfoheader.ypixelpermeter = dpi*39.375;
  pbitmap->bitmapinfoheader.xpixelpermeter = dpi*39.375;
  pbitmap->bitmapinfoheader.numcolorspallette = 0;
  fwrite (pbitmap, 1, sizeof(bitmap),fp);
// El color
  int stride = ((pbitmap->bitmapinfoheader.width*pbitmap->bitmapinfoheader.bitsperpixel + 31)/32)*4;
  int size = stride * pbitmap->bitmapinfoheader.height;
  char *a = malloc(size);
  for(int y = pbitmap->bitmapinfoheader.height-1; y>=0; y--) {
    for(int x = 0; x < stride; x+=3) {
      int i = y * stride + x;
      a[i+0] = aux_cosa(argv[3]);   // B
      a[i+1] = aux_cosa(argv[2]); // G
      a[i+2] = aux_cosa(argv[1]);   // R
    }
  }
  fwrite(a,1,size,fp);
  fclose(fp);
  free(pbitmap);
  free(pixelbuffer);
}

void loadBitmapData(char *filename) {
  FILE *fp = fopen(filename, "rb");
  if(fp == NULL) {
    printf("ERROR\n");
    exit(1);
  }
  fileheader *fh;
  bitmapinfoheader *bih;
  unsigned char *pixel_data;
  fread(fh, sizeof(fh),1,fp);
  fread(bih, sizeof(bih),1,fp);
  printf("exploté\n");
  fseek(fp, fh->fileoffset_to_pixelarray, SEEK_SET);

  pixel_data = (unsigned char*)malloc(sizeof(bih->imagesize));
  fread(pixel_data, bih->imagesize,1,fp);
  printf("R: %d, G: %d, B: %d\n", pixel_data[2], pixel_data[1], pixel_data[0]);
  fclose(fp);
  exit(1);
}
int main(int argc, char const *argv[])
{
  loadBitmapData("test.bmp");
  return 0;
}