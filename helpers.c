#include <math.h>
#include "helpers.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int b = image[i][j].rgbtBlue;
            int g = image[i][j].rgbtGreen;
            int r = image[i][j].rgbtRed;

            int av = round(((float) b + g + r) / 3);

            image[i][j].rgbtBlue = av;
            image[i][j].rgbtGreen = av;
            image[i][j].rgbtRed = av;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            int b = image[i][j].rgbtBlue;
            int g = image[i][j].rgbtGreen;
            int r = image[i][j].rgbtRed;

            image[i][j].rgbtBlue = image[i][width - j - 1].rgbtBlue;
            image[i][j].rgbtGreen = image[i][width - j - 1].rgbtGreen;
            image[i][j].rgbtRed =  image[i][width - j - 1].rgbtRed;

            image[i][width - j - 1].rgbtBlue = b;
            image[i][width - j - 1].rgbtGreen = g;
            image[i][width - j - 1].rgbtRed = r;
        }
    }
    return;
}

void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE image1[height][width];        //To store the new image

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int avr = 0;
            int avg = 0;
            int avb = 0;
            int count = 0;

            for (int k = -1; k < 2; k++)
            {
                int r = i + k;

                if (r < 0 || r >= height)
                {
                    continue;
                }

                for (int l = -1; l < 2; l++)
                {
                    int c = j + l;

                    if (c < 0 || c >= width)
                    {
                        continue;
                    }

                    avr += image[r][c].rgbtRed;
                    avg += image[r][c].rgbtGreen;
                    avb += image[r][c].rgbtBlue;
                    count++;

                }

            }

            image1[i][j].rgbtRed = round((float) avr / count);
            image1[i][j].rgbtBlue = round((float) avb / count);
            image1[i][j].rgbtGreen = round((float) avg / count);
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = image1[i][j].rgbtRed;
            image[i][j].rgbtBlue = image1[i][j].rgbtBlue;
            image[i][j].rgbtGreen = image1[i][j].rgbtGreen;
        }
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE image1[height][width];                //To store new image

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
            int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

            int r[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}}; //initialize with 0 so that when no pixel is available it will beblack
            int g[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
            int b[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};

            //for i-1
            if (!(i - 1 < 0))               //Storing rgb values of box in respective 2D arrays
            {
                if (!(j - 1 < 0))           //When if is false then it is a corner or edge as no values are assigned they will stay black
                {
                    r[0][0] += image[i - 1][j - 1].rgbtRed;
                    g[0][0] += image[i - 1][j - 1].rgbtGreen;
                    b[0][0] += image[i - 1][j - 1].rgbtBlue;
                }

                r[0][1] += image[i - 1][j].rgbtRed; //As j is always present no if
                g[0][1] += image[i - 1][j].rgbtGreen;
                b[0][1] += image[i - 1][j].rgbtBlue;

                if (!(j + 1 >= width))
                {
                    r[0][2] += image[i - 1][j + 1].rgbtRed;
                    g[0][2] += image[i - 1][j + 1].rgbtGreen;
                    b[0][2] += image[i - 1][j + 1].rgbtBlue;
                }
            }

            //for i
            if (!(j - 1 < 0))                               //same as above for diff index numbers
            {
                r[1][0] += image[i][j - 1].rgbtRed;
                g[1][0] += image[i][j - 1].rgbtGreen;
                b[1][0] += image[i][j - 1].rgbtBlue;
            }

            r[1][1] += image[i][j].rgbtRed;
            g[1][1] += image[i][j].rgbtGreen;
            b[1][1] += image[i][j].rgbtBlue;

            if (!(j + 1 >= width))
            {
                r[1][2] += image[i][j + 1].rgbtRed;
                g[1][2] += image[i][j + 1].rgbtGreen;
                b[1][2] += image[i][j + 1].rgbtBlue;
            }

            //for i+1
            if (!(i + 1 >= height))
            {
                if (!(j - 1 < 0))
                {
                    r[2][0] += image[i + 1][j - 1].rgbtRed;
                    g[2][0] += image[i + 1][j - 1].rgbtGreen;
                    b[2][0] += image[i + 1][j - 1].rgbtBlue;
                }

                r[2][1] += image[i + 1][j].rgbtRed;
                g[2][1] += image[i + 1][j].rgbtGreen;
                b[2][1] += image[i + 1][j].rgbtBlue;

                if (!(j + 1 >= width))
                {
                    r[2][2] += image[i + 1][j + 1].rgbtRed;
                    g[2][2] += image[i + 1][j + 1].rgbtGreen;
                    b[2][2] += image[i + 1][j + 1].rgbtBlue;
                }
            }

            int xr = 0, xg = 0, xb = 0, yr = 0, yg = 0, yb = 0; //Variables to store

            for (int k = 0; k < 3; k++)
            {
                for (int l = 0; l < 3; l++)
                {
                    xr += r[k][l] * Gx[k][l];       //Sobel Operation
                    xg += g[k][l] * Gx[k][l];
                    xb += b[k][l] * Gx[k][l];
                    yr += r[k][l] * Gy[k][l];
                    yg += g[k][l] * Gy[k][l];
                    yb += b[k][l] * Gy[k][l];
                }
            }

            int rr = round(sqrt(xr * xr + yr * yr)); //Gx^2 + Gy^2
            int gg = round(sqrt(xg * xg + yg * yg));
            int bb = round(sqrt(xb * xb + yb * yb));

            if (rr > 255)   //if greater than 255
            {
                rr = 255;
            }

            if (gg > 255)   //if greater than 255
            {
                gg = 255;
            }

            if (bb > 255)   //if greater than 255
            {
                bb = 255;
            }

            image1[i][j].rgbtRed = rr;
            image1[i][j].rgbtGreen = gg;
            image1[i][j].rgbtBlue = bb;

        }
    }

    for (int i = 0; i < height; i++)            //Replacing the original image
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = image1[i][j].rgbtRed;
            image[i][j].rgbtBlue = image1[i][j].rgbtBlue;
            image[i][j].rgbtGreen = image1[i][j].rgbtGreen;
        }
    }

    return;
}
