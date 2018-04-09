#include <stdio.h>
#include <stdlib.h>
void main(int argc, char * argv[]) {
FILE * fp=stdin;
unsigned char ch;
while(1) {
ch=fgetc(fp);
if(feof(fp)) break;
printf("%02X",ch);
}

}
