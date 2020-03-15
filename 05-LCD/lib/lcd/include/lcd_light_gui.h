#ifndef LCD_LIGHT_GUI_H
#define LCD_LIGHT_GUI_H

#include "lcd.h"
/* Used to select how the graph behaves with regards to scaling */
typedef enum{
    /* This setting applies no scaling to the graph, use only if data
       data is already scaled to the size of the graph window. */
    NO_SCALING,
    /* This scales the values in the graph to fit. It adapts the scale
       each time the graph gets rendered. Ignores lower bound and updates
       scaling each render */             
    SCALE_TO_FIT,  
    /* This setting uses the values in the graph object for "scaling" and
       "lower_bound" to scale the graph size. */
    USE_CUSTOM              
}scale_state_t;


/*This datatype should be considered private */
typedef struct{
    int32_t* graph_buffer;
    int32_t index;
    uint8_t* last_write;
    uint32_t width, height;
}graph_internals_t;

/** Public data structure for configuring and storing graphs. Initialize once.
   width and height are considered immutable after initialization. Always call
   gui_remove_graph() when before leaving the context of the graph. */
typedef struct{
    /** Graph color when rendered in RGB565 format */
    uint16_t color;
    /** Background color to the graph in RGB565 format */
    uint16_t background;
    /** Origin for the window that the graph will render to (bottom left pixel)*/
    uint32_t x_origin, y_origin;
    /** Setting for scaling behaviour of the graph: SCALE_TO_FIT, USE_CUSTOM, NO_SCALE */
    scale_state_t scale_setting;
    /** Scale factor */
    uint32_t scaling;
    /** The lowest pixel on the graph corresponds to this value. If you want to set the middle of the graph use ((-height*scaling) + middle_value)*/
    int32_t lower_bound;
    /* Please don't mess with */
    graph_internals_t *internals;
}graph_config_t;


/**
 * Initialization of the graph object. Handles allocation of the memory needed for the
 * buffer. If you need to change the size of the graph you need to remove the prior graph
 * and initialize a new one.
 * 
 * @param graph Graph handle and user configuration see the struct for available options
 * @param width Graph width in pixels (and values) defines the x-axis
 * @param height defines the height of the graph in pixels values are scaled with the pixels
 *               as the base unit.
 *  **/
int gui_init_graph(graph_config_t* graph, uint32_t width, uint32_t height);
/**
 * Call this before discarding a graph to avoid memory leaks. Handles freeing memory properly
 * 
 * @param graph Graph handle for the graph to be discarded
 *  **/
int gui_remove_graph(graph_config_t* graph);
/**
 * Appends n values to graph. Retains last render for erasing.
 * 
 * @param graph Graph handle
 * @param values a buffer with the values to append
 * @param append_n n values to append
 * */
int gui_append_graph(graph_config_t* graph, int32_t* values, size_t append_n);
/**
 * Render graph to the LCD. Uses the bounds set in init and the base coordinates, color, and scaling from the config struct.
 * If using autoscale mode the current scaling will be written back to the config struct.
 * 
 * @param graph Graph handle and configuration
 * */
int gui_render_graph(graph_config_t* graph);
/**
 * Erases the previously rendered graph to the background color.
 * 
 * @param graph Graph handle and configuration
 * */
int gui_erase_graph(graph_config_t* graph);

#endif /* LCD_LIGHT_GUI_H */