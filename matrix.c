#include <stdio.h>
#include <stdlib.h>
#define HEIGHT 3
#define WIDTH 7

int main()
{
    static int lev[HEIGHT][WIDTH];
    int  i,j;
    FILE *fp;
    if ((fp=fopen("lev.txt","r "))==NULL)
    {
        printf("Owubka co3danu9.\n");
        exit(666);
    }
    for (i=0;i<HEIGHT;i++)
    {
        for (j=0;j<WIDTH;j++)
        {
            fscanf(fp,"%1d",&lev[i][j]);
            //fprintf(fp, "%d",lev[i][j]);
        }
        fscanf(fp,"\n");
    }
    fclose(fp);
    //*******************************************output
    for (i=0;i<HEIGHT;i++)
    {
        for (j=0;j<WIDTH;j++)
        {
            printf("%d",lev[i][j]);
        }
        putchar ('\n');
    }
    //*******************************************

    return 0;
}
