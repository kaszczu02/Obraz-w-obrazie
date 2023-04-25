//
// Created by marty on 07.04.2023.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "image_utils.h"

struct image_t* load_image_t(const char *filename, int *err_code)
{
    //Sprawdzam poprawnosc danych
    if(filename == NULL)
    {
        if(err_code != NULL)
        {
            *err_code = 1;
        }
        return NULL;
    }

    //Otwieram plik
    FILE *file;
    file = fopen(filename,"r");
    if(file == NULL)
    {
        if(err_code != NULL)
        {
            *err_code = 2;
        }
        return NULL;
    }

    //Allokuje pamiec na strukture
    struct image_t *obraz = (struct image_t*)malloc(sizeof(struct image_t));
    if(obraz == NULL)
    {
        if(err_code != NULL)
        {
            *err_code = 4;
        }
        fclose(file);
        return NULL;
    }

    //Pobieram type
    fscanf(file,"%[^\n]",obraz->type);
    if(strcmp("P2",obraz->type) != 0)
    {
        if(err_code != NULL)
        {
            *err_code = 3;
        }
        free(obraz);
        fclose(file);
        return NULL;
    }

    //Pobieram szerokosc i wysokosc
    if(fscanf(file,"%d %d",&obraz->width,&obraz->height) != 2)
    {
        if(err_code != NULL)
        {
            *err_code = 3;
        }
        free(obraz);
        fclose(file);
        return NULL;
    }

    if(obraz->width <= 0 || obraz->height <= 0)
    {
        if(err_code != NULL)
        {
            *err_code = 3;
        }
        free(obraz);
        fclose(file);
        return NULL;
    }
    //Pobieram skladowa
    int skladowa;
    if(fscanf(file,"%d",&skladowa) != 1)
    {
        if(err_code != NULL)
        {
            *err_code = 3;
        }
        free(obraz);
        fclose(file);
        return NULL;
    }

    //Allokacja pamieci na tablice

    obraz->ptr = (int**)malloc(obraz->height * sizeof(int*));
    if(obraz->ptr == NULL)
    {
        if(err_code != NULL)
        {
            *err_code = 4;
        }
        free(obraz);
        fclose(file);
        return NULL;
    }

    for(int i = 0; i < obraz->height; i++)
    {
        *(obraz->ptr + i) = (int*)malloc(obraz->width * sizeof(int));
        if(*(obraz->ptr + i) == NULL)
        {
            if(err_code != NULL)
            {
                *err_code = 4;
            }
            for(int j = 0; j < i; j++)
            {
                free(*(obraz->ptr + j));
            }
            free(obraz->ptr);
            free(obraz);
            fclose(file);
            return NULL;
        }
    }

    //Wczytuje macierz i sprawdzam jej poprawnosc

    for(int i = 0; i < obraz->height; i++)
    {
        for(int j = 0; j< obraz->width; j++)
        {
            if(fscanf(file,"%d",*(obraz->ptr + i) + j) != 1)
            {
                if(err_code != NULL)
                {
                    *err_code = 3;
                }
                for(int z = 0; z < obraz-> height ; z++)
                {
                    free(*(obraz->ptr + z));
                }
                free(obraz->ptr);
                free(obraz);
                fclose(file);
                return NULL;

            }

            if(*(*(obraz->ptr + i) + j) < 0 || *(*(obraz->ptr + i) + j) > 255)
            {
                if(err_code != NULL)
                {
                    *err_code = 3;
                }
                for(int z = 0; z < obraz-> height ; z++)
                {
                    free(*(obraz->ptr + z));
                }
                free(obraz->ptr);
                free(obraz);
                fclose(file);
                return NULL;
            }
        }
    }

    if(err_code != NULL)
    {
        *err_code = 0;
    }
    fclose(file);
    return obraz;
}

int save_image_t(const char * filename, const struct image_t *m1)
{
    //Sprawdzanie poprawnosci danych wejsciowych
    if(filename == NULL || m1 == NULL || m1->ptr == NULL || m1->height <= 0 || m1->width <= 0)
    {
        return 1;
    }

    //Tworzenie pliku
    FILE *file;
    file = fopen(filename,"w");
    if(file == NULL)
    {
        return 2;
    }

    if(fprintf(file,"%s\n",m1->type) == 0)
    {
        fclose(file);
        return 3;
    }

    if(fprintf(file,"%d %d\n",m1->width,m1->height) == 0)
    {
        fclose(file);
        return 3;
    }

    if(fprintf(file,"%d\n",255) == 0)
    {
        fclose(file);
        return 3;
    }

    for(int i = 0; i < m1->height; i++)
    {
        for(int j = 0; j < m1->width; j++)
        {
            if(fprintf(file,"%d ",*(*(m1->ptr + i) + j)) == 0)
            {
                fclose(file);
                return 3;
            }
        }

        fprintf(file,"\n");
    }
    fclose(file);
    return 0;
}

void destroy_image(struct image_t **m)
{
    if(m == NULL)
    {
        return;
    }

    for(int i = 0; i < (*m)->height; i++)
    {
        free(*((*m)->ptr + i));
    }

    free((*m)->ptr);
    (*m)->ptr = NULL;
    free(*m);
    *m = NULL;

}

const int* image_get_pixel(const struct image_t *img, int x, int y)
{
    if(img == NULL || x < 0 || y < 0 || img->ptr == NULL || y >= img->height || x >= img->width)
    {
        return NULL;
    }

    return (*(img->ptr + y) + x);
}
int* image_set_pixel(struct image_t *img, int x, int y)
{
    if(img == NULL || x < 0 || y < 0 || img->ptr == NULL || y >= img->height || x >= img->width)
    {
        return NULL;
    }

    return (*(img->ptr + y) + x);
}

int draw_image(struct image_t *img, const struct image_t *src, int destx, int desty)
{
    if(img == NULL || src == NULL || img->ptr == NULL || src->ptr == NULL || destx < 0 || desty < 0 || (src->height > img->height - desty) || (src->width > img->width - destx) || (src->height <= 0 || src->width <= 0) || (img->height <= 0 || img->width <= 0))
    {
        return 1;
    }


    for(int i = 0; i < src->height; i++)
    {
        for(int j = 0; j < src->width; j++)
        {
            *(*(img->ptr + i + desty) + j + destx) = *(*(src->ptr + i) + j);
        }
    }

    return 0;
}