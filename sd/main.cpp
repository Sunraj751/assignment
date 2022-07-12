#include <iostream>
#include <SDL2/SDL.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <sstream>
using namespace std;

//----------------------------------------Struct-------------------------------------------
typedef struct
{
    int width;
    int height;
} framebuffer_t;

typedef struct
{
    int x;
    int y;
} point_t;

//--------------------------------------prototypes-----------------------------------------

int draw_aaline(framebuffer_t *fb, uint8_t color[], point_t *p1, point_t *p2);

//-------------------------------------Xiaolin Wu's Algorithm-------------------------------
SDL_Window *pWindow = 0;
SDL_Renderer *pRenderer = 0;

//------------------------------------ swap method --------------------------------------
/*
Function		: swap
Parameter		: pointer to variables that need to be swaped 
Return Value	: void
Description	    : swaps two numbers
*/
void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}


//------------------------------------ absolute method --------------------------------------
/*
Function		: absolute
Parameter		: float variable 
Return Value	: float
Description	    : returns absolute value of number (either -ve or +ve)
*/
float absolute(float x)
{
    if (x < 0)
        return -x;
    else
        return x;
}

//------------------------------------ iPartOfNumber --------------------------------------
/*
Function		: iPartOfNumber
Parameter		: float variable 
Return Value	: int
Description	    : returns integer part of a floating point number
*/
int iPartOfNumber(float x)
{
    return (int)x;
}

//------------------------------------ roundNumber --------------------------------------
/*
Function		: roundNumber
Parameter		: float variable 
Return Value	: int
Description	    : rounds off a number
*/
int roundNumber(float x)
{
    return iPartOfNumber(x + 0.5);
}

//------------------------------------ fPartOfNumber --------------------------------------
/*
Function		: fPartOfNumber
Parameter		: float variable
Return Value	: float
Description	    : returns fractional part of a number
*/

// returns fractional part of a number
float fPartOfNumber(float x)
{
    if (x > 0)
        return x - iPartOfNumber(x);
    else
        return x - (iPartOfNumber(x) + 1);
}

//------------------------------------ rfPartOfNumber --------------------------------------
/*
Function		: rfPartOfNumber
Parameter		: float variable
Return Value	: float
Description	    : returns fractional part of number
*/

float rfPartOfNumber(float x)
{
    return 1 - fPartOfNumber(x);
}

//------------------------------------ drawPixel --------------------------------------
/*
Function		: drawPixel
Parameter		: 1. coordinates (x,y)
                  2. brightness
                  3. varibale of type uint8_t for RGBA colors
Return Value	: void
Description	    : Draws a pixel on screen of given brightness 0<=brightness<=1.
*/

void drawPixel(int x, int y, float brightness, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
    int re = red ;
        //cout << re << " =>  Red in drawPixel-1 \n";
    int ge = green ;
        //cout << ge << " =>  green in drawPixel-1 \n";
    int be = blue ;
        //cout << be << " =>  blue in drawPixel-1 \n";
    int ae = alpha ;
        //cout << ae << " =>  ae in drawPixel-1 \n";
   
    int r = re * brightness;
       // cout << r << " =>  Red in drawPixel-2 \n";

    int g = ge * brightness;
        //cout << g << " =>  Red in drawPixel-2 \n";

    int b = be * brightness;
        //cout << b << " =>  Red in drawPixel-2 \n";
    
    int a = ae * brightness;
        //cout << a << " =>  Red in drawPixel-2 \n";

    SDL_SetRenderDrawColor(pRenderer, r, g, b, a);
   // int c = 255 * brightness;
   //SDL_SetRenderDrawColor(pRenderer, c, c, c, 255); 
    SDL_RenderDrawPoint(pRenderer, x, y);
}

//------------------------------------ drawAALine --------------------------------------
/*
Function		: drawAALine
Parameter		: 1. Coordinates for line (x0,y0,x1,y1)
                  2. Coordinates for the user's screen size (width, height)
                  3. uint_8 Color array, which holds splitted colour values in RGBA format (uint8_t color[])
Return Value	: void
Description	    : This function takes input in for of coordinates and color. It is an edited version of Xiaolin Wu's Algorithm.
                  This function calls some other functions from within that help with setting the correct brightness of the line
                  This function uses SDL library to draw a rectangle that is representing the size of screen that an 'API' would get from user, and
                  then algorithm is used to draw a line of the required color in between that box
*/

void drawAALine(int x0, int y0, int x1, int y1, uint8_t color[], int width, int height)
{
    uint8_t red = color[0];
       // cout << red << " => this is Red in drawAALine \n";

    uint8_t green = color[1];
       // cout << green << " => this is green in drawAALine \n";

    uint8_t blue = color[2];
       // cout << blue << " => this is blue in drawAALine \n";

    uint8_t alpha = color[3];
       // cout << alpha << " => this is alpha in drawAALine \n";

    int steep = absolute(y1 - y0) > absolute(x1 - x0);

    // swap the co-ordinates if slope > 1 or we
    // draw backwards
    if (steep)
    {
        swap(&x0, &y0);
        swap(&x1, &y1);
    }
    if (x0 > x1)
    {
        swap(&x0, &x1);
        swap(&y0, &y1);
    }

    // compute the slope
    float dx = x1 - x0;
    float dy = y1 - y0;
    float gradient = dy / dx;
    if (dx == 0.0)
    {
        gradient = 1;
    }

    int xpxl1 = x0;
    int xpxl2 = x1;
    float intersectY = y0;

    SDL_RenderClear(pRenderer);
    SDL_Rect rectangle;

    // main loop
    if (steep)
    {
        rectangle.x = dy;
        rectangle.y = x0;
        rectangle.w = width;  // 630
        rectangle.h = height; // 350
        SDL_SetRenderDrawColor(pRenderer, 63, 69, 106, 255);
        SDL_RenderDrawRect(pRenderer, &rectangle);

        int x;
        for (x = xpxl1; x <= xpxl2; x++)
        {
            // pixel coverage is determined by fractional
            // part of y co-ordinate
            drawPixel(iPartOfNumber(intersectY), x, rfPartOfNumber(intersectY), red, blue, green, alpha);
            drawPixel(iPartOfNumber(intersectY) - 1, x, fPartOfNumber(intersectY), red, blue, green, alpha);
            intersectY += gradient;
        }
    }
    else
    {
        rectangle.x = dy;
        rectangle.y = x0;
        rectangle.w = width;  // 630
        rectangle.h = height; // 350
        SDL_SetRenderDrawColor(pRenderer, 63, 69, 106, 255);
        SDL_RenderDrawRect(pRenderer, &rectangle);

        int x;
        for (x = xpxl1; x <= xpxl2; x++)
        {
            // pixel coverage is determined by fractional
            // part of y co-ordinate
            drawPixel(x, iPartOfNumber(intersectY), rfPartOfNumber(intersectY), red, blue, green, alpha);
            drawPixel(x, iPartOfNumber(intersectY) - 1, fPartOfNumber(intersectY), red, blue, green, alpha);
            intersectY += gradient;
        }
    }
}

//------------------------------------ draw_aaline --------------------------------------
/*
Function		: draw_aaline
Parameter		: 1. framebuffer_t *fb (pointer to the struct)
                  2. uint8_t color[] (unsigned color array)
                  3. point_t *p1, point_t *p2 (pointer to the coordinates)
Return Value	: int
Description	    : Giving coordinates to the variables to pass them to darwAAline function. Instantiation for SDL is also done in this function
*/

int draw_aaline(framebuffer_t *fb, uint8_t color[], point_t *p1, point_t *p2)
{
    int width = fb->width;
    int height = fb->height;

    int x0 = p1->x;
    int y0 = p1->y;

    int x1 = p2->x;
    int y1 = p2->y;

    SDL_Event event;
    // initialize SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) >= 0)
    {
        // if succeeded create our window
        pWindow = SDL_CreateWindow("Anti-Aliased Line ", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1800, 1800, SDL_WINDOW_SHOWN);

        // if the window creation succeeded create our renderer
        if (pWindow != 0)
        {
            pRenderer = SDL_CreateRenderer(pWindow, -1, 0);
        }
    }
    else
    {
        return 1; // sdl could not initialize
    }

    while (1)
    {
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
        {
            break;
        }

        // Sets background color to white
        SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, 255);

        SDL_RenderClear(pRenderer);

        // drawAALine(100, 280, 550, 250);
    
        drawAALine(x0, y0, x1, y1, color, width, height); 

        // show the window
        SDL_RenderPresent(pRenderer);
    }

    // clean up SDL
    SDL_Quit();
    return 0;
}

//------------------------------------ main --------------------------------------
// Driver code
int main(int argc, char *args[])
{
    int screen_width = 0;
    int screen_height = 0;
    string color1 = " ";

    int r = 0;
    int g = 0;
    int b = 0;
    int a = 0;

    int x_0_coordinate = 0;
    int y_0_coordinate = 0;
    int y_1_coordinate = 0;
    int x_1_coordinate = 0;

    char hex[3] = {" "};    // temporary array
    uint8_t color[4] = {0}; // passed into a function

    cout << "1. Enter the WIDTH of the framebuffer: ";
    cin >> screen_width;

    cout << "2. Enter the HEIGHT of the framebuffer: ";
    cin >> screen_height;

    cout << "3. Enter color in Hex (in this format (4 pairs of 2 digit numbers) 00000000): ";
    cin >> color1;

    string strR = color1.substr(0, 2);
    strcpy(hex, strR.c_str());
    r = (int)strtol(hex, NULL, 16);
    color[0] = r;
 
    string strG = color1.substr(2, 2);
    strcpy(hex, strG.c_str());
    g = (int)strtol(hex, NULL, 16);
    color[1] = g;

    string strB = color1.substr(4, 2);
    strcpy(hex, strB.c_str());
    b = (int)strtol(hex, NULL, 16);
    color[2] = b;

    string strA = color1.substr(6, 2);
    strcpy(hex, strA.c_str());
    a = (int)strtol(hex, NULL, 16);
    color[3] = a;


    cout << "4. First coordinate's x-axis (X0): ";
    cin >> x_0_coordinate;

    cout << "5. First coordinate's y-axis (Y0): ";
    cin >> y_0_coordinate;

    cout << "6. Second coordinate's x-axis (X1): ";
    cin >> x_1_coordinate;

    cout << "7. Second coordinate's y-axis (Y1): ";
    cin >> y_1_coordinate;

    // -----------------User input---------------------
    framebuffer_t pfb;
    framebuffer_t *fb = NULL;
    fb = &pfb;
    fb->width = screen_width;
    fb->height = screen_height;
    // cout << fb->width;
    // cout << fb->height;

    point_t one;
    point_t *p1 = &one;
    p1->x = x_0_coordinate;
    p1->y = y_0_coordinate;
    // cout << p1->x;
    // cout << p1->y;

    point_t two;
    point_t *p2 = &two;
    p2->x = x_1_coordinate;
    p2->y = y_1_coordinate;

    // cout << p2->x;
    // cout << p2->y;

    draw_aaline(fb, color, p1, p2);
    
    return 0;
}
