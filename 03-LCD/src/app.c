

#include "gd32vf103.h"
#include "systick.h"

/* This contains most of the LCD functions */
#include "lcd.h"

/* This contains higher level graphical functions
   currently just a graph utility but more will be added. */
#include "lcd_light_gui.h"

/* This contains an image */
#include "kth_logo.h"

#define GRAPH_HEIGHT    30

void animate_graph();

int main(void)
{
    /* Initialize the LCD for Longan nano */
    Lcd_Init();

    /* The lcd on the longan nano is a 160*80 px ips display. It has 16bit colors.
       The colors use the format RGB565 which encodes 5 bits for red, 6 bits for green and 5 for blue.
       
       The bits are arranged as such:
       | b15 | b14 | b13 | b12 | b11 | b10 | b9  | b8  | b7  | b6  | b5  | b4  | b3  | b2  | b1  | b0  |
       |-----------------------------|-----------------------------------|-----------------------------|
       | R 4 | R 3 | R 2 | R 1 | R 0|||G 5 | G 4 | G 3 | G 2 | G 1 | G 0|||B 4 | B 3 | B 2 | B 1 | B 0 |

       This gives RED = 0xF800, GREEN = 0x07E0, BLUE = 0x001F
       
       The lcd.h libary contains defines for most basic colors
       */

    /* This lcd api uses a global variable for the background color 
       colors are represented with a 16bit integer. */
    BACK_COLOR = BLACK;

    /* Use LCD_clear() to fill the entire screen with a color */
    LCD_Clear(BLACK);
    
    /* Use this to display a string */
    LCD_ShowStr(0, 0, (uint8_t*)"Hello world!", WHITE, TRANSPARENT);

    /* Draw a line from point A to point B. First two arguments are x and y for point A, and argument 3 and four point B */
    LCD_DrawLine(0, 16, 159, 16, WHITE);

    /* Draw a rectangle */
    LCD_DrawRectangle(2, 20, 31, 49, GREEN);
    
    /* Draw circle, first two arguments are origo and third the radius */
    Draw_Circle(41, 59, 20, RED);
    /* Make the circle a little clock */
    LCD_DrawLine(41, 59, 41, 43, CYAN);
    LCD_DrawLine(41, 59, 51, 66, CYAN);

    /* Draws a 3*3 point at the coordinate */
    LCD_DrawPoint_big(68, 30, MAGENTA);
    LCD_DrawPoint_big(68, 40, MAGENTA);
    LCD_DrawPoint_big(68, 50, MAGENTA);

    /* Draws a single pixel on the coordinate */
    LCD_DrawPoint(78, 30, LIGHTBLUE);
    LCD_DrawPoint(78, 40, BLUE + RED);   //BLUE + RED = MAGENTA
    LCD_DrawPoint(78, 50, BLUE + GREEN); //BLUE + GREEN = CYAN

    /* This function takes a pointer to an array containing colors to draw a picture. */
    LCD_ShowPicture(90, 20, 139, 69, (uint8_t*)&kth_logo_map);

    /* Display an integer */
    LCD_ShowNum(35,22,12, 2,RED);

    /* Display for five seconds */
    delay_1ms(5000);

    /* This function contains an example on how to use the graph UI element. */
    animate_graph();
}

void animate_graph(){


    /* One Sine wave period with amplitude 200 */
    int32_t values_to_graph[40] = { 0   ,  31,  61,  90, 117, 141, 161, 178, 190, 197, 
                                    200 , 197, 190, 178, 161, 141, 117,  90,  61,  31,
                                    0   , -31, -61, -90,-117,-141,-161,-178,-190,-197,
                                    -200,-197,-190,-178,-161,-141,-117, -90, -61, -31};

    /* This struct is used to contain settings related to the graph object. The settings below can be changed at any time */
    graph_config_t yellow_graph = {
        .color = YELLOW,                //Graph color
        .background = BLACK,            //Background color (used for erasing)
        .x_origin = 30,                 //x,y coordinates for upper left corner
        .y_origin = 20,
        .scale_setting = SCALE_TO_FIT,  //Scaling behaviour, SCALE_TO_FIT (fits the data to the window),  
                                        //                   NO_SCALING (just 1:1 maps the data),
                                        //                   USE_CUSTOM (uses .scaling as the divisor)
        .scaling = 1,
        .lower_bound = 0                //The lowest pixel on the graph maps to this number (not used by SCALE_TO_FIT)
    };
    graph_config_t cyan_graph = {
        .color = CYAN,
        .background = BLACK,
        .x_origin = 30, 
        .y_origin = 20,
        .scale_setting = USE_CUSTOM,
        .scaling = 50,
        .lower_bound = -(50*GRAPH_HEIGHT)/2  //Use this formula to get zero in the middle of the graph.
    };

    /* Initiates the graph object, width and height of the graph cannot be changed initiation. 
       Allocation of memory for the graph uses malloc() so always call gui_remove_graph() when 
       you don't need the graph anymore. */
    gui_init_graph(&yellow_graph, 100, GRAPH_HEIGHT);
    gui_init_graph(&cyan_graph, 100, GRAPH_HEIGHT);

    LCD_Clear(BACK_COLOR);

    /* Draw a rectangle around the bounding box of the graph */
    LCD_DrawRectangle(30-1, 20-1, 30 + 100+1, 20+30+1, GREEN);
    /* Coordinates: Upper corner: x_origin-1, y_origin-1. Lower corner: x_origin+graph_width+1, y_origin+graph_height+1 */

    while(1){    
        
        for(int i = 0; i < 120; i++){

            /* Add a value to the graphs. You can add multiple values in one go if needed */
            gui_append_graph(&yellow_graph, &values_to_graph[i % 40], 1);
            gui_append_graph(&cyan_graph, &values_to_graph[(i+20) % 40], 1);

            /* Render the current graph */
            gui_render_graph(&yellow_graph);
            gui_render_graph(&cyan_graph);

            /* yellow_graph uses auto-scaling so this number displays what the current divisor is */
            LCD_ShowNum(0,0,yellow_graph.scaling, 6,WHITE);

            /* Let the LCD display for a moment */
            delay_1ms(32);

            /* Erases the last drawn graph. You can append values to the graph and it will still 
               only erase the graph displayed last time. It will not erase other graphs or drawings in the 
               same area unless they are drawn on the same pixel as one of the graph values. */
            gui_erase_graph(&yellow_graph);
            gui_erase_graph(&cyan_graph);
                
        }

        /* Do some scaling of the data */
        for(int i = 0; i < 40; i++) values_to_graph[i] *= 4;
    
        /* Exactly the same as above */
        for(int i = 0; i < 120; i++){

            gui_append_graph(&yellow_graph, &values_to_graph[i % 40], 1);
            gui_append_graph(&cyan_graph, &values_to_graph[(i+20) % 40], 1);

            gui_render_graph(&yellow_graph);
            gui_render_graph(&cyan_graph);

            LCD_ShowNum(0,0,yellow_graph.scaling, 6,WHITE);

            delay_1ms(32);

            gui_erase_graph(&yellow_graph);
            gui_erase_graph(&cyan_graph);
                
        }

        /* More scaling of the sine-wave */
        for(int i = 0; i < 40; i++) values_to_graph[i] /= 2;
    
    }
}