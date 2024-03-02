//  keyfilter.c
//  Created by xkovacm01 on 03.10.2023.
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_POCET_PISMEN 101

void vypis(int x, char arr[]){
    printf("Enable: ");
    for (int i = x; i >= 0; i--) {
        if (isprint(arr[i])) {
            printf("%c", toupper(arr[i]));
        }
    }
    printf("\n");
}//Funkce vypisujici pole nalezenych pismen

void sorted(int x, char arr[],char arr2[]){
    char sortedArr[x];
    if (arr2[0] != 0) {
        printf("Found: ");
        printf("%s",arr2);
    }//Vypis v pripade nalezeneho celeho slova
    else{
        if (x > 0) {
                int position = 0;
                for (int i = 0; i < x; i++) {
                    position = 0;
                    for (int j = 0; j < x; j++) {
                        if (toupper(arr[i]) < toupper(arr[j])) {
                            position++;
                        }//Pozice urcena podle poctu splnene podminky
                    }
                    sortedArr[position] = arr[i];//pridani puvodniho pole do noveho, ktere bude serazene
                }
            vypis(x, sortedArr);
            }
    }//Vypis za podminky, ze jsme nenasli cele slovo
    if (x == 0) {
        printf("Not found\n");
    }// Podminka pro nenalezene pismeno
}

int main(int argc, char *argv[])
{
    char database[MAX_POCET_PISMEN] = {0}, nalezenePismeno[MAX_POCET_PISMEN] = {0}, found[MAX_POCET_PISMEN] = {0};
    int nalez = 0, pomoc = 0, duplikace = 0;//pomocne promene
    
    if (argc < 1) {
        fprintf(stderr,"Not enough arguments!\n");
        return 1;
    }// Podminka pro kontrolu poctu argumentu (situace nedostatek argumentu)

    if (argc == 1) {
        while (fgets(database, MAX_POCET_PISMEN,stdin) != NULL) {
            pomoc = 0;
            for (int i = 0; i < argc; i++) {
                for (int j = 0; j < nalez; j++) {
                    if (nalezenePismeno[j] == database[i]) {
                        pomoc++;
                    }//Pokud dojde k nalezu
                }//Konec cyklu hledani duplikaci
                if (pomoc == 0) {
                    nalezenePismeno[nalez] = database[i];
                    nalez++;
                }//Pokud nedochazi k duplikaci dojde k zapsani pismena do pole nalezenych pismen
            }
        }//Konec WHILE cyklu pro nacitani argumentu
    }// Podminka pro kontrolu poctu argumentu (zadan jeden argument)
    
    if(argc >= 2){
        while(fgets(database,MAX_POCET_PISMEN,stdin) != NULL){
            pomoc = 0;
            for (int i = 0; i < (int)strlen(argv[1]); i++) {
                if (toupper(argv[1][i]) == toupper(database[i])) {
                    pomoc++;
                }//Cyklus pro zjisteni poctu shodnych pismen z argumentu a stdin daneho radku
            }//Konec cyklu porovnávajícího jednotlivé chars
            
            if ((int)strlen(argv[1]) == pomoc) {
                for (int j = 0; j < nalez; j++) {
                    if (toupper(nalezenePismeno[j]) == toupper(database[pomoc])) {
                        duplikace++;
                    }
                }//Kontrola duplikace pismen
                
                if (duplikace == 0) {
                        nalezenePismeno[nalez] = database[pomoc];
                        nalez++;
                }//Pokud nedochazi k duplikaci dojde k zapsani pismena do pole nalezenych pismen
                
                if(nalez == 1 && duplikace == 0){
                    for (int j = 0; j < (int)strlen(database); j++) {
                        found[j] = toupper(database[j]);
                    }//Prepsani stdin do promeneho pole FOUND
                }//Podminka jedineho nalezu bez duplikace je duvod pro vypis FOUND
                if(nalez == 1 && duplikace != 0){
                    for (int i = 0; i < MAX_POCET_PISMEN; i++) {
                        found[i] = 0;
                    }//Prepsani pole na 0, abychom nemeli found
                }//Podminka jedineho nalezeneho pismena s duplikace nedojde k zapsani

                duplikace = 0;
            }//Zapsaní do arr nalezenePismeno
                if(nalez != 1){
                    for (int i = 0; i < MAX_POCET_PISMEN; i++) {
                        found[i] = 0;
                }
            }//Pokud jsem nalezl vice nez jedno pismenko, nastavim found na 0
        }//Konec WHILE cyklu pro nacitani argumentu
    }//Konec ověření 2+ argumentů
    
    sorted(nalez, nalezenePismeno, found);

    return 0;
}
