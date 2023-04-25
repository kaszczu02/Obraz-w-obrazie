#include <stdio.h>
#include <stdlib.h>
#include "image_utils.h"
#include <string.h>

int main()
{
    //Allokacja pamieci na plik i wczytywanie nazwy
    char *nazwa_pliku = (char*)malloc(40 * sizeof(char));
    if(nazwa_pliku == NULL)
    {
        printf("Failed to allocate memory");
        return 8;
    }
    printf("Podaj nazwe pliku:");
    scanf("%39[^\n]",nazwa_pliku);
    scanf("%*[^\n]s");
    scanf("%*c");

    //Wczytywanie obrazu
    int err_code = 0;
    struct image_t* obraz = load_image_t(nazwa_pliku,&err_code);
    if(err_code == 2)
    {
        printf("Couldn't open file");
        free(nazwa_pliku);
        return 4;
    }
    else if(err_code == 3)
    {
        printf("File corrupted");
        free(nazwa_pliku);
        return 6;
    }
    else if(err_code == 4)
    {
        printf("Failed to allocate memory");
        free(nazwa_pliku);
        return 8;
    }


    //Ile obrazow chce zapisac uzytkownik

    int liczba_obrazow;
    printf("Ile obrazow chcesz zapisac ?: ");
    if(scanf("%d",&liczba_obrazow) != 1)
    {
        printf("Incorrect input");
        destroy_image(&obraz);
        free(nazwa_pliku);
        return 1;
    }
    scanf("%*[^\n]s");
    scanf("%*c");

    if(liczba_obrazow <= 0)
    {
        printf("Incorrect input data");
        destroy_image(&obraz);
        free(nazwa_pliku);
        return 2;
    }

    //Allokacja pamieci i wczytywanie drugiej nazwy pliku
    char *nazwa_pliku2 = (char*)malloc(40 * sizeof(char));
    if(nazwa_pliku2 == NULL)
    {
        printf("Failed to allocate memory");
        destroy_image(&obraz);
        free(nazwa_pliku);
        return 8;
    }

    //Wczytywanie obrazow
    int err_code2 = 0;
    struct image_t *obraz2;
    int x,y,wynik_draw_image;
    for(int i = 0; i < liczba_obrazow; i++)
    {
        printf("Podaj nazwe pliku:");
        scanf("%39[^\n]",nazwa_pliku2);
        scanf("%*[^\n]s");
        scanf("%*c");
        obraz2 = load_image_t(nazwa_pliku2,&err_code2);
        if(err_code2 == 2)
        {
            if(obraz2 !=NULL)
            {
                destroy_image(&obraz2);
            }
            printf("Couldn't open file\n");
            continue;
        }
        else if(err_code2 == 3)
        {
            if(obraz2 !=NULL)
            {
                destroy_image(&obraz2);
            }
            printf("File corrupted\n");
            continue;
        }
        else if(err_code2 == 4)
        {
            printf("Failed to allocate memory");
            destroy_image(&obraz);
            free(nazwa_pliku);
            free(nazwa_pliku2);
            return 8;
        }
        printf("Podaj koordynaty: ");
        if(scanf("%d %d",&x,&y) != 2)
        {
            printf("Incorrect input");
            if(obraz2 !=NULL)
            {
                destroy_image(&obraz2);
            }
            destroy_image(&obraz);
            free(nazwa_pliku);
            free(nazwa_pliku2);
            return 1;
        }
        scanf("%*[^\n]s");
        scanf("%*c");
        wynik_draw_image = draw_image(obraz,obraz2,x,y);
        if(wynik_draw_image == 1)
        {
            printf("Incorrect input data\n");
            if(obraz2 !=NULL)
            {
                destroy_image(&obraz2);
            }
            continue;
        }
        destroy_image(&obraz2);

    }
    free(nazwa_pliku2);
    char *temp;
    if(strcmp(strrchr(nazwa_pliku, '.'), ".txt") == 0)
    {
        int dlugosc_nazwa_pliku = strlen(nazwa_pliku);
        nazwa_pliku = realloc(nazwa_pliku,(dlugosc_nazwa_pliku + 10) * sizeof(char));
        temp = strrchr(nazwa_pliku,'.');
        *temp = '\0';
        strcat(nazwa_pliku,"_modified.txt");
    }
    else if(strcmp(strrchr(nazwa_pliku, '.'), ".bin") == 0)
    {
        int dlugosc_nazwa_pliku = strlen(nazwa_pliku);
        nazwa_pliku = realloc(nazwa_pliku,(dlugosc_nazwa_pliku + 10) * sizeof(char));
        temp = strrchr(nazwa_pliku,'.');
        *temp = '\0';
        strcat(nazwa_pliku,"_modified.bin");
    }

    int wynik_save_image_t = save_image_t(nazwa_pliku,obraz);
    if(wynik_save_image_t == 2)
    {
        printf("Couldn't create file");
        destroy_image(&obraz);
        free(nazwa_pliku);
        return 5;
    }
    if(wynik_save_image_t == 3)
    {
        printf("Incorrect input data");
        destroy_image(&obraz);
        free(nazwa_pliku);
        return 2;
    }
    printf("File saved");
    destroy_image(&obraz);
    free(nazwa_pliku);
    return 0;
}

