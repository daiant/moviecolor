#include "stdio.h"
#include "stdlib.h"
FILE *ifp, *ofp;
char outputname[] = "adios.txt";
int main() {
  ifp = fopen("hola.txt", "r");
  if(ifp == NULL) {
    fprintf(stderr,"El archivo no existe!\n");
    exit(1);
  }
  char ch;
  ch = fgetc(ifp); // lee el primer char
  while(ch != EOF) { // es end of file??
    printf("%c", ch);
    ch = fgetc(ifp); // lee el siguiente y así
  }
  ofp = fopen(outputname, "w");
  fprintf(ofp, "Testing for fprintf\n"); // las dos valen para meter texto
  fputs("Hasta luego cocodrilo", ofp);
  fclose(ifp);
  fclose(ofp);
  return 0;
}
