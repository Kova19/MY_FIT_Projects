// Created by Martin Kovacik (xkovacm01) on 06.11.2023.
// maze.c second project for IZP
// Tested on MacOS Sonoma and merlin.fit.vutbr.cz

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define pravaRuka 0
#define levaRuka 1
#define leftMaze -1
#define outMaze -1

typedef struct {
    int rows;
    int cols;
    unsigned char *cells;
} Map;

int getMAP(FILE *soubor, Map *map) {
    map->rows = 0;
    map->cols = 0;
    
    fscanf(soubor, "%d", &map->rows);//Naskenovani poctu radku
    fscanf(soubor, "%d", &map->cols);//Naskenovani poctu sloupcu

    if ((map->cells = (unsigned char *)malloc(map->rows * map->cols * sizeof(unsigned char))) == NULL) {
        fprintf(stderr, "Pamet nebyla prirazena!\n");
        return -1;
    }// Alokace pameti pro bunky
    for (int i = 0; i < map->rows*map->cols; i++) {
        map->cells[i] = '8';
    }//Inicializovani mapy hodnotou 8, aby cislo neopovidalo definovane hranici
    
    int i = 0, j = 0, ch = 0, count = 0;
    while (i < map->rows) {
        while (j < map->cols) {
            ch = fgetc(soubor);
            if (!isspace(ch)) {
                map->cells[i * map->cols + j] = (unsigned char)ch;
                j++;
                count++;
            }
        }
        i++;
        j = 0;
    }//Cyklus pro nacteni mapy
    
    while((ch = fgetc(soubor)) != EOF){
        if (!isspace(ch)) {
            count++;
        }
    }// cyklus, ktery po nacteni do mapy zkusi projit soubor, jestli se tam nenachazi dalsi cisla
    if (count != map->rows * map->cols) {
        return false;
    }// Overeni spravnosti mapi v souboru je tolik cisel kolik definuje r a c
    return true; //Pokud inicializace i nacteni mapy projde v poradku funkce vratie true
}// Konec funkce nacitajici a inicializujici mapu

unsigned char getCell(Map *map, int r, int c) {
    return map->cells[r * map->cols + c];
}//Funkce, ktera vrati hodnotu dotazovaneho policka

void cleanMap(Map *map){
    free(map->cells);
}//Funkce uvolnuji pamet, ktera byla alokovana pro mapu

int check_argument(char path[], char **arguments, int argc){
    if (argc < 1) {
        return false;
    }
        if((strcmp(arguments[1], "--help") == 0) && (argc == 2)){
            return 1;
        }// Kontrola argumentu --help, ktera vraci jednicku
        if((strcmp(arguments[1], "--test") == 0) && (argc == 3)){
            strcpy(path, arguments[2]);
            return 2;
        }// Kontrola argumentu --test, ktera vraci dvojku
        if((strcmp(arguments[1], "--rpath") == 0) && (atoi(arguments[2]) > 0) && (atoi(arguments[3]) > 0) && argc == 5){
            if (atoi(arguments[3])%2 == 0) {
                printf("Temito argumenty nelze vstoupist do bludiste!\n");
                return false;
            }// Kontrola souradnic vetsich nez 0 a souradnic radku, abych nevstoupil lezatym trojuhelnikem
            else{
                strcpy(path, arguments[4]);
                return 3;
            }
        }// Kontrola argumentu --rpath, ktera vraci trojku
        if((strcmp(arguments[1], "--lpath") == 0) && (atoi(arguments[2]) > 0) && (atoi(arguments[3]) > 0) && argc == 5){
            if (atoi(arguments[3])%2 == 0) {
                printf("Temito argumenty nelze vstoupist do bludiste!\n");
                return false;
            }// Kontrola souradnic vetsich nez 0 a souradnic radku, abych nevstoupil lezatym trojuhelnikem
            else{
                strcpy(path, arguments[4]);
                return 4;
            }// Kontrola argumentu --lpath, ktera vraci trojku
        }
        else{
            return false;
        }//Pokud doslo k nespecifikovane chybe vratim false
    
    return true;
}// funkce kontroluje agrumetny a v pripade pozitivniho vysledku vraci true

void help(void){
    printf("./maze --test soubor.txt\n");
    printf("Pro kontrolu radne definice mapy v souboru\n");
    printf("./maze --rpath R C soubor.txt\n");
    printf("Program se spusti s algoritmem pro hledani cesty pravidlem prave ruky\n");
    printf("./maze --lpath R C soubor.txt\n");
    printf("Program se spusti s algoritmem pro hledani cesty pravidlem leve ruky\n");
    printf("Hodnota R a C udava souradnice mista od ktereho se zacina hledat\n");
}// Funkce vypisujici napovedu pro spusteni programu

int check_lineUp(Map *map){
    int pozice = 0;
    
    for (int i = 0; i < map->rows; i++) {
        for (int j = 0; j < map->cols; j++) {
            pozice = i * map->cols + j; //Nastaveni aktualni pozice
            
            if (map->cells[pozice] >=48 && map->cells[pozice] <=55) {
                if (i == 0) {
                    continue;
                }// Kontrola ze cisla jsou v rozmezi 0 az 7
                
                if (map->cells[pozice] >= '0' && map->cells[pozice] <= '3') {
                    if (i%2 == j%2) { // Trojuhelnik s horni hranici (stojici na vrcholu)
                        if (map->cells[pozice-map->cols] >= '0' && map->cells[pozice-map->cols] <= '3') {
                            continue;
                        }
                        else{
                            return false;
                        }
                    }
                }//Kontrola pradznych hranic

                if (map->cells[pozice] >= '4' && map->cells[pozice] <= '7') {
                    if (i%2 == j%2) { // Trojuhelnik s horni hranici (stojici na vrcholu)
                        if (map->cells[pozice-map->cols] >= '4' && map->cells[pozice-map->cols] <= '7') {
                            continue;
                        }
                        else{
                            return false;
                        }
                    }
                }// Kontrola plnych hranic
            }
            else{
                return false;
            }
        }
    }
    return true;
}//funkce porachzejici vodorovnou hranici pri splneni vsech podminek vraci true

int check_left_line(Map *map){
    
    for (int i  = 0; i < map->rows*map->cols; i++) {
        if (map->cells[i] == '0' || map->cells[i] == '2' || map->cells[i] == '4' || map->cells[i] == '6') {//Pokud aktualni bunka ma prazdnou levou stranu
            if (i%map->cols == 0) {
                continue;
            }
            if (i%map->cols != 0) {
                if (map->cells[i-1] == '0' || map->cells[i-1] == '1' || map->cells[i-1] == '4' || map->cells[i-1] == '5') { //Kontrola policka v levo dle definic
                    continue;
                }
                else{
                    return false;
                }
            }
        } // Konec overeni leve prazdne hranice
        
        if (map->cells[i] == '1' || map->cells[i] == '3' || map->cells[i] == '5' || map->cells[i] == '7') { //Pokud aktualni bunka ma levou hranici plnou
            if (i%map->cols == 0) {
                continue;
            }
            if (i%map->cols != 0) {
                if (map->cells[i-1] == '2' || map->cells[i-1] == '3' || map->cells[i-1] == '6' || map->cells[i-1] == '7') {
                    continue;
                }
                else{
                    return false;
                }
            }
        }// Konec overeni leve plne hranice
    }
        return true;
}//funkce porachzejici levou hranici pri splneni vsech podminek vraci true
    
int test(Map *map){

    if ((map->rows > 0 && map->cols > 0)){
        if ((check_lineUp(map) == true) && (check_left_line(map) == true)) {
            printf("Valid\n");
        }// Pokud jsou hranice v poradku vrati Valid
        else{
            printf("Invalid\n");
        }//Pokud hranice nejsou v poradku vrati false
    }
    else{
        printf("Invalid\n");
    }//Pokud jsou rozmery hranic mensi jako 0 vrati false
    return 0;
}// Funkce testujici bludiste, jeho hranice a spravnost vraci stdout Valid nebo Invalid


bool isborder(Map *map, int r, int c, char border){
    if (getCell(map, r, c) == border) {
        return true;
    }
    else
        return false;
}//Funkce vyhodnocujici dotaz z pruchodu vraci true nebo false

int start_border(Map *map, int r, int c, int leftright){
    int right = 1;
    int left = 2;
    int down = 3;
    int up = 4;
    if (leftright == pravaRuka || leftright == levaRuka) {//Algoritmus pro pravou ruku
        if (map->cells[(r-1)*map->cols + c-1] == '0' || map->cells[(r-1)*map->cols + c-1] == '2' || map->cells[(r-1)*map->cols + c-1] == '4' || map->cells[(r-1)*map->cols + c-1] == '6') {
            if ((c == 1) && ((r%2 == 1) || (r%2 == 0))) {
                return left; // Pokud jsem prisel ze leva vratim prichozi z leva
            }
        }
        if (map->cells[(r-1)*map->cols + c-1] >= '0' && map->cells[(r-1)*map->cols + c-1] <= '3') {
            if (r == 1) {
                return up;
            }// Pokud jsem prisel ze zhora vratim prichozi smer dolu
            if (r == map->rows) {
                return down;
            }
        }
        if (map->cells[(r-1)*map->cols + c-1] == '0' || map->cells[(r-1)*map->cols + c-1] == '1' || map->cells[(r-1)*map->cols + c-1] == '4' || map->cells[(r-1)*map->cols + c-1] == '5') {
            if (c == map->cols && r%2 == 1) {
                return right;// Pokud jsem prisel z prava vratim prichozi smer z prava
            }
            if (c == map->cols && r%2 == 0) {
                return right;// Pokud jsem prisel z prava vratim prichozi smer z prava
            }
        }
    }
    return false;
}//Funkce overujici moznost vkroceni do bludiste vraceni smeru

bool tryRight(Map *map, int r, int c){
    if (isborder(map, r, c, '0') == true || isborder(map, r, c, '1') == true ||
        isborder(map, r, c, '4') == true || isborder(map, r, c, '5') == true) {
        return true;
    }
    else
        return false;
}// Funkce tazajici se na moznost posunu doprava pokud je to mozne vraci true jinak false
bool tryLeft(Map *map, int r, int c){
    if (isborder(map, r, c, '0') == true || isborder(map, r, c, '2') == true ||
        isborder(map, r, c, '4') == true || isborder(map, r, c, '6') == true) {
        return true;
    }
    else
        return  false;
}// Funkce tazajici se na moznost posunu doleva pokud je to mozne vraci true jinak false
bool tryUp(Map *map, int r, int c){
    if (isborder(map, r, c, '0') == true || isborder(map, r, c, '1') == true ||
        isborder(map, r, c, '2') == true || isborder(map, r, c, '3') == true) {
        return  true;
    }
    else
        return  false;
}// Funkce tazajici se na moznost posunu nahoru pokud je to mozne vraci true jinak false
bool tryDown(Map *map, int r, int c){
    if (isborder(map, r, c, '0') == true || isborder(map, r, c, '1') == true||
        isborder(map, r, c, '2') == true || isborder(map, r, c, '3') == true) {
        return true;
    }
    else
        return  false;
}// Funkce tazajici se na moznost posunu dolu pokud je to mozne vraci true jinak false
bool done(Map *map, int r, int c){
    if (r == map->rows || c == map->cols || r == outMaze || c == outMaze) {
        return true;
    }
    else
        return false;
}//Funkce, ktera vyhodnoti, jestli je pruchod bludistem u konce hodnotou true, pokud nejsem na konci vrati false


char rpath(Map *map, int r, int c){
    int lastR = 0;
    int lastC = 0;
    
    if (start_border(map, r, c, pravaRuka) == 1) {
        lastR = r-1;
        lastC = c;
    }// pokud jsem dosel z prava nastavim adekvatni predeslou souradnici
    if (start_border(map, r, c, pravaRuka) == 2) {
        lastR = r-1;
        lastC = c-2;
    }// pokud jsem dosel z leva nastavim adekvatni predeslou souradnici
    if (start_border(map, r, c, pravaRuka) == 3) {
        lastR = r;
        lastC = c-1;
    }// pokud jsem dosel z dola nastavim adekvatni predeslou souradnici
    if (start_border(map, r, c, pravaRuka) == 4) {
        lastR = r-2;
        lastC = c-1;
    }// pokud jsem dosel ze zhora nastavim adekvatni predeslou souradnici
    if (start_border(map, r, c, pravaRuka) == false) {
        
        printf("Temito souradnicemi nelze vstoupit.\n");
        return -1;
    }//Pokud nelze vstoupit zadanymi souradnicemi vratim -1 a dam vedet uzivateli
    r--; //Dekrementace za ucelem vyrovnani hodnot v indexaci pole
    c--; // Tedy souradnice 1,1 = 0,0 (Tak jak je pole v pameti indexovano)

    
    while (done(map,r,c) != true) {
        printf("%d,%d\n",r+1, c+1); //Vytisknuti aktualni hranice
        
        if (lastC - c == -1) { // Prisel jsem z leva
            if (r%2 == c%2) { // Trojuhelnik s horni hranici (stojici na vrcholu)
                if (tryRight(map, r, c)==true) {
                    c++;
                    lastR = r;
                    lastC = c - 1;
                    continue;
                }// Funkce vyzkousi policko v pravo a pokud tryRight je spravne posune se tam a nastavi novou minulou pozici
                
                if (tryUp(map, r, c) == true) {
                    r--;
                    lastR = r+1;
                    lastC = c;
                    continue;
                }// Funkce vyzkousi policko v nad nim a pokud tryUp je spravne posune se tam a nastavi novou minulou pozici
                
                if (tryLeft(map, r, c) == true) {
                    c--;
                    lastR = r;
                    lastC = c+1;
                    continue;
                }// Funkce vyzkousi policko v levo a pokud tryLeft je spravne posune se tam a nastavi novou minulou pozici
            }
            
            if (r%2 != c%2) {// Trojuhelnik s dolni hranici (stojici na podstave)
                
                if (tryDown(map, r, c) == true) {
                    r++;
                    lastR = r-1;
                    lastC = c;
                    continue;
                }// Funkce vyzkousi policko dolu a pokud tryDown je spravne posune se tam a nastavi novou minulou pozici
                
                if (tryRight(map, r, c) == true) {
                    c++;
                    lastR = r;
                    lastC = c-1;
                    continue;
                }// Funkce vyzkousi policko v pravo a pokud tryRight je spravne posune se tam a nastavi novou minulou pozici
                
                if (tryLeft(map, r, c) == true) {
                    c--;
                    lastR = r;
                    lastC = c+1;
                    continue;
                }// Funkce vyzkousi policko v levo a pokud tryLeft je spravne posune se tam a nastavi novou minulou pozici
            }
        }
            
        if (lastC - c == 1) { // Prisel jsem z prava
            if (r%2 == c%2) {// Trojuhelnik s horni hranici (stojici na vrcholu)
                if (tryUp(map, r, c) == true) {
                    r--;
                    lastR = r+1;
                    lastC = c;
                    continue;
                }// Funkce vyzkousi policko v nad nim a pokud tryUp je spravne posune se tam a nastavi novou minulou pozici
                if (tryLeft(map, r, c) == true) {
                    c--;
                    lastR = r;
                    lastC = c+1;
                    continue;
                }// Funkce vyzkousi policko v levo a pokud tryLeft je spravne posune se tam a nastavi novou minulou pozici
                if (tryRight(map, r, c) == true) {
                    c++;
                    lastR = r;
                    lastC = c-1;
                    continue;
                }// Funkce vyzkousi policko v pravo a pokud tryRight je spravne posune se tam a nastavi novou minulou pozici
            }
            
            if (r%2 != c%2) {// Trojuhelnik s dolni hranici (stojici na podstave)
                if (tryLeft(map, r, c) == true) {
                    c--;
                    lastR = r;
                    lastC = c+1;
                    continue;
                }// Funkce vyzkousi policko v levo a pokud tryLeft je spravne posune se tam a nastavi novou minulou pozici
                if (tryDown(map, r, c) == true) {
                    r++;
                    lastR = r-1;
                    lastC = c;
                    continue;
                }// Funkce vyzkousi policko dolu a pokud tryDown je spravne posune se tam a nastavi novou minulou pozici
                if (tryRight(map, r, c) == true) {
                    c++;
                    lastR = r;
                    lastC = c-1;
                    continue;
                }// Funkce vyzkousi policko v pravo a pokud tryRight je spravne posune se tam a nastavi novou minulou pozici
            }
        }
        
        if (lastR - r == -1) {// Prisel jsem z dola
            if (tryLeft(map, r, c) == true) {
                c--;
                lastR = r;
                lastC = c+1;
                continue;
            }// Funkce vyzkousi policko v levo a pokud tryLeft je spravne posune se tam a nastavi novou minulou pozici
            if (tryRight(map, r, c) == true ) {
                c++;
                lastR = r;
                lastC = c-1;
                continue;
            }// Funkce vyzkousi policko v pravo a pokud tryRight je spravne posune se tam a nastavi novou minulou pozici
            if (tryUp(map, r, c) == true) {
                r--;
                lastR = r+1;
                lastC = c;
                continue;
            }// Funkce vyzkousi policko v nad nim a pokud tryUp je spravne posune se tam a nastavi novou minulou pozici
        }
        
            if (lastR - r == 1) {// Prisel jsem z hora
                if (tryRight(map, r, c) == true ) {
                    c++;
                    lastR = r;
                    lastC = c-1;
                    continue;
                }// Funkce vyzkousi policko v pravo a pokud tryRight je spravne posune se tam a nastavi novou minulou pozici
                if (tryLeft(map, r, c) == true) {
                    c--;
                    lastR = r;
                    lastC = c+1;
                    continue;
                }// Funkce vyzkousi policko v levo a pokud tryLeft je spravne posune se tam a nastavi novou minulou pozici
                if (tryDown(map, r, c) == true) {
                    r++;
                    lastR = r-1;
                    lastC = c;
                    continue;
                }// Funkce vyzkousi policko dolu a pokud tryDown je spravne posune se tam a nastavi novou minulou pozici
            }
    }//Konec cyklu prochazejici bludistem
    return 0;
}//Konec funkce pruchodem dle pravidla prave ruky

char lpath(Map *map, int r, int c){
    int lastR = 0;
    int lastC = 0;
    
    if (start_border(map, r, c, pravaRuka) == 1) {
        lastR = r-1;
        lastC = c;
    }// pokud jsem dosel z prava nastavim adekvatni predeslou souradnici
    if (start_border(map, r, c, pravaRuka) == 2) {
        lastR = r-1;
        lastC = c-2;
    }// pokud jsem dosel z leva nastavim adekvatni predeslou souradnici
    if (start_border(map, r, c, pravaRuka) == 3) {
        lastR = r;
        lastC = c-1;
    }// pokud jsem dosel z dola nastavim adekvatni predeslou souradnici
    if (start_border(map, r, c, pravaRuka) == 4) {
        lastR = r-2;
        lastC = c-1;
    }// pokud jsem dosel ze zhora nastavim adekvatni predeslou souradnici
    if (start_border(map, r, c, pravaRuka) == false) {
        
        printf("Temito souradnicemi nelze vstoupit.\n");
        return -1;
    }//Pokud nelze vstoupit zadanymi souradnicemi vratim -1 a dam vedet uzivateli
    r--; //Dekrementace za ucelem vyrovnani hodnot v indexaci pole
    c--; // Tedy souradnice 1,1 = 0,0 (Tak jak je pole v pameti indexovano)

    
    while (done(map,r,c) != true) {
        
        printf("%d,%d\n",r+1, c+1); //Vytisknuti aktualni pozice na stdout
        
        if (lastC - c == -1) {// Prisel jsem z leva
            if (r%2 == c%2) {// Trojuhelnik s horni hranici (stojici na vrcholu)
                if (tryUp(map, r, c) == true) {
                    r--;
                    lastR = r+1;
                    lastC = c;
                    continue;
                }// Funkce vyzkousi policko v nad nim a pokud tryUp je spravne posune se tam a nastavi novou minulou pozici
                if (tryRight(map, r, c)==true) {
                    c++;
                    lastR = r;
                    lastC = c - 1;
                    continue;
                }// Funkce vyzkousi policko v pravo a pokud tryRight je spravne posune se tam a nastavi novou minulou pozici
                if (tryLeft(map, r, c) == true) {
                    c--;
                    lastR = r;
                    lastC = c+1;
                    continue;
                }// Funkce vyzkousi policko v levo a pokud tryLeft je spravne posune se tam a nastavi novou minulou pozici
            }
            
            if (r%2 != c%2) {// Trojuhelnik s dolni hranici (stojici na podstave)
                if (tryRight(map, r, c) == true) {
                    c++;
                    lastR = r;
                    lastC = c-1;
                    continue;
                }// Funkce vyzkousi policko v pravo a pokud tryRight je spravne posune se tam a nastavi novou minulou pozici
                if (tryDown(map, r, c) == true) {
                    r++;
                    lastR = r-1;
                    lastC = c;
                    continue;
                }// Funkce vyzkousi policko dolu a pokud tryDown je spravne posune se tam a nastavi novou minulou pozici
                if (tryLeft(map, r, c) == true) {
                    c--;
                    lastR = r;
                    lastC = c+1;
                    continue;
                }// Funkce vyzkousi policko v levo a pokud tryLeft je spravne posune se tam a nastavi novou minulou pozici
            }
        }
            
        if (lastC - c == 1) {// Prisel jsem z prava
            if (r%2 == c%2) {// Trojuhelnik s horni hranici (stojici na vrcholu)
                if (tryLeft(map, r, c) == true) {
                    c--;
                    lastR = r;
                    lastC = c+1;
                    continue;
                }// Funkce vyzkousi policko v levo a pokud tryLeft je spravne posune se tam a nastavi novou minulou pozici
                if (tryUp(map, r, c) == true) {
                    r--;
                    lastR = r+1;
                    lastC = c;
                    continue;
                }// Funkce vyzkousi policko v nad nim a pokud tryUp je spravne posune se tam a nastavi novou minulou pozici
                if (tryRight(map, r, c) == true) {
                    c++;
                    lastR = r;
                    lastC = c-1;
                    continue;
                }// Funkce vyzkousi policko v pravo a pokud tryRight je spravne posune se tam a nastavi novou minulou pozici
            }
            
            if (r%2 != c%2) {// Trojuhelnik s dolni hranici (stojici na podstave)
                if (tryDown(map, r, c) == true) {
                    r++;
                    lastR = r-1;
                    lastC = c;
                    continue;
                }// Funkce vyzkousi policko dolu a pokud tryDown je spravne posune se tam a nastavi novou minulou pozici
                if (tryLeft(map, r, c) == true) {
                    c--;
                    lastR = r;
                    lastC = c+1;
                    continue;
                }// Funkce vyzkousi policko v levo a pokud tryLeft je spravne posune se tam a nastavi novou minulou pozici
                if (tryRight(map, r, c) == true) {
                    c++;
                    lastR = r;
                    lastC = c-1;
                    continue;
                }// Funkce vyzkousi policko v pravo a pokud tryRight je spravne posune se tam a nastavi novou minulou pozici
            }
        }
        
        if (lastR - r == -1) {// Prisel jsem z dola
            if (tryRight(map, r, c) == true ) {
                c++;
                lastR = r;
                lastC = c-1;
                continue;
            }// Funkce vyzkousi policko v pravo a pokud tryRight je spravne posune se tam a nastavi novou minulou pozici
            if (tryLeft(map, r, c) == true) {
                c--;
                lastR = r;
                lastC = c+1;
                continue;
            }// Funkce vyzkousi policko v levo a pokud tryLeft je spravne posune se tam a nastavi novou minulou pozici
            if (tryUp(map, r, c) == true) {
                r--;
                lastR = r+1;
                lastC = c;
                continue;
            }// Funkce vyzkousi policko v nad nim a pokud tryUp je spravne posune se tam a nastavi novou minulou pozici
        }
        
            if (lastR - r == 1) { // Prisel jsem z hora
                if (tryLeft(map, r, c) == true) {
                    c--;
                    lastR = r;
                    lastC = c+1;
                    continue;
                }// Funkce vyzkousi policko v levo a pokud tryLeft je spravne posune se tam a nastavi novou minulou pozici
                if (tryRight(map, r, c) == true ) {
                    c++;
                    lastR = r;
                    lastC = c-1;
                    continue;
                }// Funkce vyzkousi policko v pravo a pokud tryRight je spravne posune se tam a nastavi novou minulou pozici
                if (tryDown(map, r, c) == true) {
                    r++;
                    lastR = r-1;
                    lastC = c;
                    continue;
                }// Funkce vyzkousi policko dolu a pokud tryDown je spravne posune se tam a nastavi novou minulou pozici
            }
    }//Konec pruchodu bludistem
    
    return 0;
}//Konec funkce pruchodem dle pravidla leve ruky

int main(int argc, char * argv[]) {
    char soubor_path[101] = {0}; //Promenna do ktere ukladam cestu k souboru
    FILE *fr; // Ukazatel na soubor
    Map map;
    
    if (check_argument(soubor_path, argv, argc) == false) {
        printf("Spatne si spustil program zkontroluj argumenty!\n");
    }//Overovaci funkce pro spatne zadane argumenty
    if (check_argument(soubor_path, argv, argc)==1) {
        help();
    }// Volani funkce help
    
    if ((fr = fopen(soubor_path,"r")) == NULL){
        fprintf(stderr, "Soubor s mapou se nepodarilo otevrit.\n");
        return -1;
    }// Otevreni souboru a overeni ze ten soubor lze otevrit
    
    if (check_argument(soubor_path,argv, argc)==2) {
        if(getMAP(fr, &map) == true){
            test(&map);
        }// Overeni ze ve funkci getMAP probehlo vse v poradku
        else{
            printf("Invalid\n");
        }
    }//Volani funkce test
    
    if (check_argument(soubor_path, argv, argc)==3) {
        getMAP(fr, &map);
        rpath(&map, atoi(argv[2]), atoi(argv[3]));
    }// Volani funkce pro pruchod pomoci prave ruky
    
    if (check_argument(soubor_path, argv, argc) == 4) {
        getMAP(fr, &map);
        lpath(&map, atoi(argv[2]), atoi(argv[3]));
    }// Volani funkce pro pruchod pomoci leve ruky

    cleanMap(&map);// Uvolneni alokovane pameti
    if(fclose(fr) == EOF){
        fprintf(stderr,"Soubor s mapou se napodarilo zavrit.\n");
        return -1;
    }//Zavreni souboru a kontrola zda se to povedlo
    
    return 0;
}
