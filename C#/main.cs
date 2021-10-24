using System;
using System.Drawing;
using System.Drawing.Imaging;

namespace perlin
{
    class Program
    {
        public const int PI_2_1000 = 31415*2;


        public const int screenLen = 1000;
        public const int nbCells = 10; // On choisit ici la pecision, S = {5, 10, 25}
        public const int nbGridRows = nbCells + 1;
        public const int cellLen = screenLen / nbCells;

        static void Main(string[] args)
        {



            double[,] screen = new double[screenLen, screenLen];
            double[,,] grid = generateGrid();

            for (int i = 0; i < screenLen; i++)
            {
                for (int j = 0; j < screenLen; j++)
                {
                    screen[i, j] = makePerlinNoise(i, j, grid);
                    
                }
            }
    

            writeInPng(screen, args);

            
        }

        static double[,,] generateGrid(){
            Random rand =  new Random();

            double[,,] grid = new double[nbGridRows, nbGridRows, 2];

            for (int i = 0; i < nbGridRows; i++)
            {
                for (int j = 0; j < nbGridRows; j++)
                {
                    double angle = rand.Next(PI_2_1000) / 10000.0;
                    
                    
                    grid[ i, j, 0] =  Math.Cos(angle);
                    grid[ i, j, 1] =  Math.Sin(angle);

                }
                
            }

            return grid;
        }

        static double makePerlinNoise(int i, int j, double[,,] grid){
            int[] gridPos = new int[2] {
                i / cellLen,
                j / cellLen
            };

            int[] pixelRelativePos = new int[2] {
                i % cellLen,
                j % cellLen
            };

            
            double[] produitsArray = new double[4];

            for (int k = 0; k < 4; k++)
            {
                double[] cornerVector = new double[2]{
                    grid[ gridPos[0] + (k&2)/2 , gridPos[1] + (k&1), 1],
                    grid[ gridPos[0] + (k&2)/2 , gridPos[1] + (k&1), 0]
                };//Hmm les maths

                double[] distanceVector = new double[2]{
                    pixelRelativePos[0] - cellLen * (k&2)/2,
                    pixelRelativePos[1] - cellLen * (k&1),
                };

                produitsArray[k] = produitScalaire(
                    cornerVector,
                    distanceVector
                );


            }

            //)==============================================================

            double haja_1 = interpolate( 
                produitsArray[0], 
                produitsArray[1],
                pixelRelativePos[1] * (1.0/cellLen)
            );

            double haja_2 = interpolate(
                produitsArray[2], 
                produitsArray[3],
                pixelRelativePos[1]  * (1.0/cellLen)    
            );

            double haja_total = interpolate(
                haja_1, 
                haja_2,
                pixelRelativePos[0]  * (1.0/cellLen)    
            );
            
            //Console.WriteLine(haja_1 +" "+ haja_2 + " " + haja_total);
            return haja_total;
        }

        static double produitScalaire(double[] vectA, double[] vectB){

            return vectA[0] * vectB[1] + vectA[1] * vectB[0];

        }

        static double interpolate( double a0,  double a1,  double w) {
            //default
            //return (1.0 - w)*a0 + w*a1;

            // Use this cubic interpolation [[Smoothstep]] instead, for a smooth appearance:
            return (a1 - a0) * (3.0 - w * 2.0) * w * w + a0;
            
            // Use [[Smootherstep]] for an even smoother result with a second derivative equal to zero on boundaries:
            //return (a1 - a0) * ((w * (w * 6.0 - 15.0) + 10.0) * w * w * w) + a0;
            
        }


        static void writeInPng(double[,] screen, string[] args){
                                
                

            Bitmap bitmap = new Bitmap(screenLen, screenLen, System.Drawing.Imaging.PixelFormat.Format32bppPArgb);
            
            int mode = 0;
            int red = 0;



            if(args.Length > 0)
            {
                if(args[0] == "-f")
                    mode = 0;

                if(args[0] == "-c")
                    mode = 1;

                if(args[0] == "-g")
                    mode = 2;
            }

            

            for (int i = 0; i < screenLen; i++)
            
                for (int j = 0; j < screenLen; j++)
                {
                    
                    red = getColorGradiant(mode, screen[i, j]);

                    bitmap.SetPixel(i, j, Color.FromArgb(red, 0, 0) );
                    
                }
            bitmap.Save(@"perlinas.png");        //, ImageFormat.Png    
        }

        static int getColorGradiant(int mode, double intessite){
            int val = 0;

            switch(mode){
                case 0:
                    val = (int)Math.Floor( ( intessite * 15 ) + 120  );

                    if (val > 255) val = 254;
                    if (val < 0) val = 0;//Math.Abs(val);
                    
                    break;

                case 1:
                    val = (int)Math.Floor( ( intessite * 200 )  );
                    break;

                case 2:
                    val = (int)Math.Floor( ( intessite * 20 ) + 100  );
                    if (val < 0) val = 0;
                    break;

                default:
                    val = (int)Math.Floor( ( intessite * 10 ) + 120  );
                    if (val > 255) val = 255;
                    if (val < 0) val = Math.Abs(val);
                    
                    break;

            }
            val %= 255;
            

            

            return val;

        }
    
    }
}
