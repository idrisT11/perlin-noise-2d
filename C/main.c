#include <stdlib.h> 
#include <stdio.h> 
#include <time.h> 
#include <math.h>

#include <gd.h>


#define SCREEN_WIDTH 500
#define SCREEN_HEIGHT 500

#define NB_COLUMN 11
#define NB_CELLS (NB_COLUMN-1)

#define CELL_WIDTH      (SCREEN_WIDTH  / NB_CELLS)
#define CELL_HEIGHT     (SCREEN_HEIGHT / NB_CELLS)


void generateRandomVectors( float* grid[NB_COLUMN][NB_COLUMN] );
float produitScalaire(float vectA[], float vectB[]);
float makePerlinNoise(int i, int j, float* grid[NB_COLUMN][NB_COLUMN] );
float interpolate( float a0,  float a1,  float w) ;

void write_png_file(float screen[][SCREEN_WIDTH], int afficheMode);

//-lgd -lpng -lz -ljpeg -lfreetype -lm


int main(int argc, char const *argv[])
{
    int affichageMode = 0;
    if (argc > 1)
    {
        if (argv[1][0] == 'f')
        {
            printf("Affichage mode Flou\n");
            affichageMode = 0;
        }
        else if (argv[1][0] == 'c')
        {
            printf("Affichage mode Courbure\n");
            affichageMode = 1;
        }
        else if (argv[1][0] == 'g')
        {
            printf("Affichage mode glow\n");
            affichageMode = 2;
        }
        
    }
    
    srand((unsigned)time(NULL)); //random init

    float  screen[ SCREEN_HEIGHT ] [SCREEN_WIDTH] = {{0}};

    float* grid[ NB_COLUMN] [ NB_COLUMN] = {{NULL}};

    generateRandomVectors( grid );


    for (int i = 0; i < SCREEN_HEIGHT; i++)
    {
        for (int j = 0; j < SCREEN_WIDTH; j++)
        {
            screen[i][j] = makePerlinNoise(i, j, grid);

        }
        
    }
    

    write_png_file(screen, affichageMode);

    return 0;
}


void generateRandomVectors( float* grid[NB_COLUMN][NB_COLUMN] ){

    for (int i = 0; i < NB_COLUMN; i++)
    {
        for (int j = 0; j < NB_COLUMN; j++)
        {
            float angle = (rand()%(31415*2)) / 10000.0;

            grid[ i ][ j ] = calloc(2, sizeof(float));
            grid[ i ][ j ][0] =  cos(angle);
            grid[ i ][ j ][1] =  sin(angle);

        }
        
    }
    
}

float makePerlinNoise(int i, int j, float* grid[NB_COLUMN][NB_COLUMN] ){
    int gridPos[2] = {
        i / CELL_WIDTH,
        j / CELL_HEIGHT
    };

    int pixelRelativePos[2] = {
        i % CELL_WIDTH,
        j % CELL_HEIGHT
    };
    
    float produitsArray[4];

    for (int i = 0; i < 4; i++)
    {
        float* cornerVector = grid[ gridPos[0] + (i&2)/2 ][ gridPos[1] + (i&1) ];//Hmm les maths

        float distanceVector[2] = {
            pixelRelativePos[0] - CELL_HEIGHT * (i&2)/2,
            pixelRelativePos[1] - CELL_WIDTH * (i&1),
        };

        produitsArray[i] = produitScalaire(
            cornerVector,
            distanceVector
        );
    }

    //)==============================================================

    float zebi_1 = interpolate( 
        produitsArray[0], 
        produitsArray[1],
        pixelRelativePos[1] * (1.0/CELL_WIDTH)
    );

    float zebi_2 = interpolate(
        produitsArray[2], 
        produitsArray[3],
        pixelRelativePos[1]  * (1.0/CELL_WIDTH)    
    );

    float zebi_total = interpolate(
        zebi_1, 
        zebi_2,
        pixelRelativePos[0]  * (1.0/CELL_HEIGHT)    
    );
    

    return zebi_total;
}

float produitScalaire(float vectA[], float vectB[]){

    return vectA[0] * vectB[1] + vectA[1] * vectB[0];

}

float interpolate( float a0,  float a1,  float w) {
    //default
    //return (1.0 - w)*a0 + w*a1;

    // Use this cubic interpolation [[Smoothstep]] instead, for a smooth appearance:
    //return (a1 - a0) * (3.0 - w * 2.0) * w * w + a0;
    
    // Use [[Smootherstep]] for an even smoother result with a second derivative equal to zero on boundaries:
    return (a1 - a0) * ((w * (w * 6.0 - 15.0) + 10.0) * w * w * w) + a0;
    
}


//==================================================================
void write_png_file(float screen[][SCREEN_WIDTH], int afficheMode) {

    /* Declare the image */
    gdImagePtr im;
    /* Declare output files */
    FILE *pngout;

    im = gdImageCreateTrueColor(SCREEN_WIDTH, SCREEN_HEIGHT);

    /* Open a file for writing. "wb" means "write binary", important
        under MSDOS, harmless under Unix. */
    pngout = fopen("./perlin.png", "wb");

    for (int i = 0; i < SCREEN_HEIGHT; i++)
    {
        for (int j = 0; j < SCREEN_WIDTH; j++)
        {
            int red;

            if (afficheMode == 0)
            {    
                red = floor( (screen[i][j]*10) + 120  );
                red = red > 255 ? 255 : red;
            }

            else if (afficheMode == 1)
            
                red = floor( (screen[i][j]*200)  );

            else if (afficheMode == 2)
            
                red = floor( (screen[i][j]*20) + 100  );

            


            gdImageSetPixel (im,
                j,
                i,

                gdImageColorAllocate(im, red, 0, 0)//flem de faire un dégradé et utiliser gdImageCreate(){256colors}
            );
            
        }


        
    }
    


    /* Output the image to the disk file in PNG format. */
    gdImagePng(im, pngout);


    /* Close the files. */
    fclose(pngout);

    /* Destroy the image in memory. */
    gdImageDestroy(im);
}
