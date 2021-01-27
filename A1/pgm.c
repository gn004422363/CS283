#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAXCOLOR 	256

typedef struct {
  unsigned int	width,height;
  unsigned int 	maxval;
  unsigned char		*pixel;
} PGM;

void load_pgm(char *filename, PGM *pgm)
{
  FILE *inf;
  char 	ch;
  char 	str[200];
  int 	i, j, value[3], val;
  unsigned char 		*ptr;

  // "rb" for open non-text file
  inf = fopen(filename, "rb");
  if (inf == NULL) {
    printf("No such file!n");
    exit(1);
  }

  // load info for image
  for (j=0; j<3; j++) {
  do {
    i=0;
    ch = fgetc(inf);
    if (ch == '#') do {
      str[i++]=fgetc(inf);
    } while (str[i-1]!= 'n');
    str[i]=0;

  } while(ch < '0' || ch > '9');

  i = 0;
  str[i++] = ch;
    do {
      ch = fgetc(inf);
      str[i++] = ch;
    } while(ch >= '0' && ch <= '9');
    str[i-1] = 0;
    value[j] = atoi(str);
    i=0;

  } // end for

  // initial value for width, height, maxval and pixel
  pgm->width  = value[0];
  pgm->height = value[1];
  pgm->maxval = value[2];
  
  // Use malloc to allocate image
  pgm->pixel  = (unsigned char *)malloc (pgm->height*pgm->width);

  if (pgm->pixel == NULL) {
   printf("MALLOC ERROR");
   exit(1);
  } // end if

    // edit height and width of pixel
    ptr=pgm->pixel;
    for (i=0; i < pgm->height; i++) {
        for (j=0; j < pgm->width; j++) {
            fscanf(inf,"%c",&ch);
	        *ptr++=ch;

        } // end for

    } // end for

  fclose(inf);

} // end load_pgm()

// save pgm info after edition
void save_pgm(char *filename, PGM pgm)
{
  FILE *outf;
  int i, j;
  unsigned char  *ptr;

  outf = fopen(filename, "wt");
   if (outf == NULL) {
    printf("No such file!n");
    exit(1);

  } // end if

  fprintf(outf, "%i %ixD", pgm.width, pgm.height);
  fprintf(outf, "%ixD", pgm.maxval);
  
  fwrite(pgm.pixel, pgm.height, pgm.width, outf);

  fclose(outf);

} // end save_pgm()

// diaplay pgm pixel info
void display_pgm(PGM pgm)
{
  int i, j;
  unsigned char* ptr;

  ptr=pgm.pixel;
  for (i=0; i < pgm.height; i++) {
    for (j=0; j < pgm.width; j++) {
      printf ("%10d",*ptr++);

    } // end for

    printf ("\n");
    
  } // end for

  printf ("\nWidth:%d \nHeight:%d \nMaxVal:%d\n", pgm.width, pgm.height, pgm.maxval);

} // end display_pgm

int main(int argc, char *argv[])
{
  PGM		pgm1;
  char		filename[256];

  // Prompt for the file name to read
  printf("\nSource PGM filename to read (Example: xxx.pgm): "); scanf("%s", filename);

  // Load the file name from input
  load_pgm(filename, &pgm1);

  // Display the data of the image
  display_pgm(pgm1);

  // Prompt user to enter output file name
  printf("\nPGM filename to write (Example: xxx.pgm): "); scanf("%s", filename);

  // Output the file with specify file name
  save_pgm(filename, pgm1);

  // Display the data of the image
  display_pgm(pgm1); 

  return EXIT_SUCCESS;

} // end main