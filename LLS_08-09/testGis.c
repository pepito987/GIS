/* Author Giuseppe Quartarone mat.: 408661
 */
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "gis.h"

#define ABROT_1	  "Impossibile aprire il file "
#define ABROT_2	  "Errore nella lettura della richiesta "
#define MACRO(x,y,z) if((x) == y){fprintf(stderr,"%s \n",z); exit(EXIT_FAILURE);}

int main(int argc, char *argv[]){
   int i;
   FILE *fp;
   gis_t *gis;
   int giorno,mese,anno,ore,min, gsettim;
   char stpart[25], starrivo[25],buff[100];
   struct tm *t;
   time_t tmp;

   
   gis = initializegis();
   MACRO(gis,NULL,"initializegis\n");
   printf("Caricamento dei file in corso...\n");
   MACRO(argc,1,"Argomenti mancatani")
   for(i=1; i < argc; i++){
      if((fp=fopen((argv[i]),"r")) == NULL)
	 MACRO(fp,NULL,ABROT_1);
      readconf(fp,gis);
      fclose(fp);
   }

   printf("Caricamento completato.\n");
   printf("Inserisci la tua richiesta:\n gg/mm/aa | hh:mm | stazione partenza | stazione arrivo\ndigita Q per terminare.\n");
   
   while((fgets(buff,(sizeof(buff)-2),stdin) != NULL) && (buff[0] != 'Q')){
      if((i=sscanf(buff,"%d/%d/%d|%d:%d|%[^|]|%[^\n]",&giorno,&mese,&anno,&ore,&min,stpart,starrivo)) != 7){
	 printf("Errore di sintassi nella richiesta, riprova:\ngg/mm/aa | hh:mm | stazione partenza | stazione arrivo%d\n",i);
	 continue;
      }
printf("%02d/%02d/%02d %02d:%02d ---%s---%s---\n",giorno,mese,anno,ore,min,stpart,starrivo);
      if(!( (giorno >=1) && (giorno <=31) && (mese >= 1) && (mese <=12) && (min >= 0) && (ore >=0) )){
	 printf("Errore di sintassi nella richiesta, riprova");
	 continue;
      }
      t = localtime(&tmp);
      t->tm_year = (anno-1900);
      t->tm_mon = (mese-1);
      t->tm_mday = giorno;
      t->tm_hour = ore;
      t->tm_min = min;
      t->tm_sec = 00;
      t->tm_isdst = -1;
      tmp = mktime(t);
      t = localtime(&tmp);
      gsettim = t->tm_wday+1;
      min += (ore*60);
      cammino(gis,min,gsettim,stpart,starrivo);
      printf("Inserisci la tua richiesta:\n gg/mm/aa | hh:mm | stazione partenza | stazione arrivo\ndigita Q per terminare.\n");
   }
}


