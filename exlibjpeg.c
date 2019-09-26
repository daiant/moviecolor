// compilar con cc exlibjpeg.c -ljpeg -o exlibjpeg
// ejecutar ./exlibjpeg filename.jpg

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <sys/stat.h>

#include <jpeglib.h>

int main(int argc, char const *argv[]) {
  int rc, i, j;
  char *syslog_prefix = (char*) malloc(1024);
  sprintf(syslog_prefix, "%s", argv[0]);
  openlog(syslog_prefix, LOG_PERROR | LOG_PID, LOG_USER);

  if(argc != 2) {
    fprintf(stderr, "Usalo bien puto\n");
    exit(EXIT_FAILURE);
  }

  /* Setup */
  // source jpg
  struct stat file_info;
  unsigned long jpg_size;
  unsigned char *jpg_buffer;

  // decompressor
  struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;

  //para el buffer de salida
  unsigned long bmp_size;
  unsigned char *bmp_buffer;
  int row_stride, width, height, pixel_size;


  rc = stat(argv[1], &file_info);
  if(rc) {
    syslog(LOG_ERR, "FAILED to stat source jpg");
    exit(EXIT_FAILURE);
  }
  jpg_size = file_info.st_size;
  jpg_buffer = (unsigned char*) malloc(jpg_size + 100);

  int fd = open(argv[1], O_RDONLY);
  i = 0;
  while (i < jpg_size) {
    rc = read(fd, jpg_buffer + i, jpg_size - i);
    syslog(LOG_INFO, "Input: Read %d%lu bytes", rc, jpg_size-i);
    i+=rc;
  }
  close(fd);


  /* el start es ahora xdddd */

  syslog(LOG_INFO, "Proc: Create Decompress struct");
  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_decompress(&cinfo);

  syslog(LOG_INFO, "Proc: Set memory buffer as source");
  jpeg_mem_src(&cinfo, jpg_buffer, jpg_size);

  syslog(LOG_INFO, "Proc: Read the JPEG header");
  rc = jpeg_read_header(&cinfo, TRUE);

  if(rc != 1) {
    syslog(LOG_ERR, "File does not seem to be a normal jpeg");
    exit(EXIT_FAILURE);
  }

  syslog(LOG_INFO, "Proc: Initiate JPEG decompression");
  jpeg_start_decompress(&cinfo);

  width  = cinfo.output_width;
  height = cinfo.output_height;
  pixel_size = cinfo.output_components;
  syslog(LOG_INFO, "Proc: Image is %d by %d with %d components",
                    width, height, pixel_size);

  bmp_size = width * height * pixel_size;
  bmp_buffer = (unsigned char*) malloc(bmp_size);
  row_stride = width * pixel_size;

  syslog(LOG_INFO, "Proc: Start reading scanlines");
  while (cinfo.output_scanline < cinfo.output_height) {
		unsigned char *buffer_array[1];
		buffer_array[0] = bmp_buffer + \
						   (cinfo.output_scanline) * row_stride;

		jpeg_read_scanlines(&cinfo, buffer_array, 1);
  }
  syslog(LOG_INFO, "Done reading scanlines");

  jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);

  free(jpg_buffer);

  /* asegurarse de que todo ha ido bien */

  fd = open("output.ppm", O_CREAT | O_WRONLY, 0666);
  char buf[1024];
  rc = sprintf(buf, "P6 %d %d 255\n", width, height);
  write(fd, buf, rc);
  write(fd, bmp_buffer, bmp_size);
  close(fd);
  free(bmp_buffer);
  syslog(LOG_INFO, "End of decompression");
  return EXIT_SUCCESS;
}
