#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "cephes.h"

double threshold3 = 0.0;
int debug=0;
int is_hex=0;

int char_2_dec(char *binstring) // converting binary string to decimal. 
{
  
    int i;
    int sum = 0;
    int len = strlen(binstring);
    int num = 0;
    for(i=0;i<len;i++)
    {
        num=(int)binstring[i] - '0';
        sum=sum + (pow(2, len-i-1)*num);
    }
    return sum;
}


char *char_2_bin3(char input) // char to binary string. 
{
    
    int i = 0;
    int bit = 0;
    char * output = calloc(256, 1);

    for(i=0;i<8;++i)
    {
        bit=((input>>i)&1);
        output[7-i]=bit + '0';  
    }
    return output;
                
}

int poker(char * filename,uint8_t m)
{

    char *original_str=(char*)malloc(2222220*sizeof(char));
    memset(original_str,0,2222220);

    FILE * fp;
    if(filename==NULL)
        fp = stdin;
    else
        fp = fopen(filename, "rb");

       
    if(fp==NULL)
    {
        fprintf(stderr,"File does not exist.\n");
        exit(0);
    }

    int n,i;
    char *ch = calloc(1, 1),ch1;
    int filesize = 0;
    int ctr = 0;
    char *temp = calloc(8, 1);

    while(!(feof(fp)))
    {
      // memset(temp, NULL, 8);
      temp[0] = '\0';
      if(is_hex){
	ch1=(char)fgetc(fp);
	if(ch1>='0' && ch1<='9') ch[0]=ch1-'0';
	else if(ch1>='A' && ch1 <='F') ch[0]=ch1-'A'+10;
	else if(ch1>='a' && ch1 <='f') ch[0]=ch1-'a'+10;
	else continue;
	ch[0]=ch[0]<<4;
	while(!feof(fp)) {
	  ch1=(char)fgetc(fp);
	  if(ch1>='0' && ch1<='9') ch[0]|=ch1-'0';
	  else if(ch1>='A' && ch1 <='F') ch[0]|=ch1-'A'+10;
	  else if(ch1>='a' && ch1 <='f') ch[0]|=ch1-'a'+10;
	  else continue;
	  break;
	}
      }
      else
	ch[0]=fgetc(fp);

      filesize++;


      temp = char_2_bin3(ch[0]);
if(ctr>= 2222220) break;
      strncat(&original_str[ctr], temp, 8);
      ctr=ctr+8;
      if(feof(fp))
	break;

    }

if(is_hex == 0)    filesize--;

    n = filesize*8;
    int k;
    k = floor(n/m);
    if(debug) fprintf(stderr,"\nn = %d\nm = %d\nk = %d\n",n,m,k);



    int loop_limit =n-(n%m);
   
    char *temp_cpy = calloc(m, 1);
    
    //filesize = 5;
    int ctr_array_size = pow(2, m);
    int ctr_array[ctr_array_size];

    // initialise to zero;
    for(i=0;i<ctr_array_size;i++)
        ctr_array[i] = 0;
    

    int num=0;
    for(i=0;i<filesize*8;i+=m)
    {
        memset(temp_cpy, 0, m);
        if(i<loop_limit) // ignore nits after the multiple
        {   
	    
            strncpy(temp_cpy, &original_str[i], m);
            num = char_2_dec(temp_cpy);
            ctr_array[num] +=1;
            temp_cpy[0]='\0';
        }
        else
            break;  

    }
    if(debug) {
      for(i=0;i<ctr_array_size;i++)
      {
	fprintf(stderr,"n[%d] = %d\n" ,i, ctr_array[i]);
      }
    }

    double pow_val = 0.0;
    for(i=0;i<ctr_array_size;i++)
    {
        pow_val+= pow(ctr_array[i], 2);
    }


    double pow_div=0.0;
    pow_div=((pow(2,m)*pow_val)/(double)k);
    double X3=0.0,P_value;
    X3=pow_div-k;
    P_value=cephes_igamc((pow(2,m)-1)/2,X3/2);
    if(debug)
      fprintf(stderr,"x3 = %6f\n",X3,P_value);
    if(P_value>0.01) {
        fprintf(stdout,"P-value=%f (>0.01)\npass\n",P_value);
        return 0;
    }else{
        fprintf(stdout,"P-value=%f (<=0.01)\nfail\n",P_value);
	return -1;
    }

}


int main(int argc, char * argv[]) {

  int i,file_index=0,m=2;
  for(i=1;i<argc;i++) {
    if(argv[i][0]=='-') {
      switch(argv[i][1]) {
	case 'm':
	case 'M':
	  m=atoi(&argv[i][2]);
	  break;
	case 'v':
	case 'V':
	case 'd':
	case 'D':
	  debug=1;
	  break;
	case 'x':
	case 'X':
          is_hex=1;
          break;
        case 'h':
        case 'H':
          printf("-h help\n-d debug mode\n-m[2-16] set m=2~16\n-x hex str input\n%s [option] [file]\n",argv[0]);
          return 0; 
      }
    }
    else 
      file_index=i; 
  }
if(m>16) m=16;
 
  if(file_index>0) {
    return poker(argv[file_index],m);
  }else
    return poker(NULL,m);
}
