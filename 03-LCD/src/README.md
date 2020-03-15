<br>

# LCD Example


This project shows how to start and use the LCD-display included on the longan nano. It includes how to draw some basic shapes, an image and how to use the graph-utility.



## Note

The LCD library uses some shorthand types for the basic integers so:
```c
u8 == uint8_t
u16 == uint16_t
```

and so on. It also uses some slightly inconsistent capitalization, so keep that in mind if a function won't compile properly. 
<br><br>
## Important Functions Used
### LCD Iitialization
```c
void Lcd_Init(void)
```
This function initializes the SPI peripheral used for the LCD on the longan board. If using another board this function might need to be edited somewhat to work properly.
<br><br>
### LCD Drawing functions

```c
void LCD_Clear(u16 Color)
```
Use this to set the entire screen to a single color.

```c
void LCD_ShowStr(u16 x,u16 y,const u8 *p,u16 color, u8 mode)
```
This functions lets you print a string to the LCD. The first two arguments are the upper left corner coordinates of the string. After the coordinates is a pointer to the string. **color** sets the color of the text. **mode** can be TRANSPARENT or OPAQUE. Transparent will only draw the letter, while opaque will use the global variable **BACK_COLOR** to also fill in the area behind the letter.

```c
void LCD_DrawPoint(u16 x,u16 y,u16 color)

void LCD_DrawPoint_big(u16 x,u16 y,u16 color)

void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 color)

void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color)

void Draw_Circle(u16 x0,u16 y0,u8 r,u16 color)

void LCD_DrawLine(u16 x1,u16 y1,u16 x2,u16 y2,u16 color)

void LCD_ShowStr(u16 x,u16 y,const u8 *p,u16 color, u8 mode)

void LCD_ShowNum(u16 x,u16 y,u16 num,u8 len,u16 color)

void LCD_ShowNum1(u16 x,u16 y,float num,u8 len,u16 color)
```

All of these functions work on the same basic principle. Takes one or two x-y coordinates and displays the selected drawing.

```c
void LCD_ShowPicture(u16 x1, u16 y1, u16 x2, u16 y2, u8 *image)
```
This function takes a pointer to an array of bytedata encoded as RGB565. This could be generated in your program, but probably it's more useful to generate the code for a picture instead.

[littlevgl](https://littlevgl.com/image-to-c-array) has a useful converter that will take an image and convert it to a c-style array. when you have converted an image copy the array which is generated under the line:
```c
#if LV_COLOR_DEPTH == 16 && LV_COLOR_16_SWAP != 0
```
copy until #endif shows up. Then initiate a **const uint8_t array[]** with the copied values in a .h file that you include in the main file.

### Graph Objects

Since it can often be useful to graph sensor inputs there's also a utility for graphing in conjunction with the LCD library.
```c
typedef struct{
    uint16_t color;
    uint16_t background;
    uint32_t x_origin, y_origin;
    scale_state_t scale_setting;
    uint32_t scaling;
    int32_t lower_bound;
    
    /* Please don't mess with */
    graph_internals_t *internals;
}graph_config_t;
```
This struct is used as the handle for every graph function. It sets configurations for the graph such as color and scaling behaviour. It also keeps track och allocated memory associated with the object.

- uint16_t **color**
    - Color setting for the graph
- uint16_t **background**
    - Background color setting for the graph
- uint32_t **x_origin, y_origin**
    - Coordinates for the top left corner of the graph
- scale_state_t **scale_setting**
    - Sets the scaling behaviour of the graph
        - SCALE_TO_FIT - Scales values so that all values fits inside the window
        - USE_CUSTOM - Uses "scaling" variable as the divisor
        - NO_SCALING - Maps values 1:1 on the pixels in the graph
- int32_t **scaling**
    - Current divisor for the graph. USE_CUSTOM setting uses this value for scaling, while SCALE_TO_FIT updates this value to indicate current scaling.
- int32_t **lower_bound** 
    - This value sets the value that the bottom pixel corresponds to when graphing. To set an arbitrary value in the middle of the graph use: <br>lower_bound = -(graph_height * scaling)/2 + (value in middle of graph)
<br>
```c
int gui_init_graph(graph_config_t* graph, uint32_t width, uint32_t height)
```
Graph initialization, height and width in pixels. This function handles allocation of memory, so do not call repeatedly without using **gui_remove_graph()**

```c
int gui_remove_graph(graph_config_t* graph)
```
Frees allocated memory associated with the graph.
```c
int gui_append_graph(graph_config_t* graph, int32_t* values, size_t append_n)
```
Adds n values to the graph. One value is one pixel in the graph. The graph takes values as a ring buffer. So for each value added in front one is discarded in the back.

```c
int gui_render_graph(graph_config_t* graph)
```
Renders the graph to the screen.

```c
int gui_erase_graph(graph_config_t* graph)
```
Erases the last drawn graph. It remembers the last render and erases correctly even if values have been appended between render and erase.