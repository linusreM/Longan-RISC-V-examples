#include "lcd_light_gui.h"


#define DONT_GRAPH  0x7FFFFFFF

#define NO_WRITE    0xFF


static
void minmax_int(int32_t* p_min, int32_t* p_max, graph_internals_t* graph);

int gui_init_graph(graph_config_t* graph, uint32_t width, uint32_t height){
    graph->internals = malloc(sizeof(graph_internals_t));
    graph->internals->width = width;
    graph->internals->height = height;
    graph->internals->index = 0;
    graph->internals->graph_buffer = malloc(sizeof(int32_t) * width);
    graph->internals->last_write = malloc(sizeof(uint8_t) * width);

    for(int i = 0; i < width; i++){
        graph->internals->last_write[i] = NO_WRITE;
        graph->internals->graph_buffer[i] = DONT_GRAPH;
    }

    return 1;
}

int gui_remove_graph(graph_config_t* graph){
    free(graph->internals->last_write);
    free(graph->internals->graph_buffer);
    free(graph->internals);
    return 0;
}


int gui_append_graph(graph_config_t* graph, int32_t* values, size_t append_n){

    graph_internals_t *p_graph = graph->internals;

    for(int i = 0; i < append_n; i++){
        p_graph->graph_buffer[p_graph->index] = values[i];
        p_graph->index = (p_graph->index + 1) % p_graph->width;
    }
    
    return 0;
}
int gui_render_graph(graph_config_t* graph){

    graph_internals_t *p_graph = graph->internals;  //Handle to make adressing less of a hassle
    int32_t y, x, min, max, scaling;
    
    x = graph->x_origin;

    minmax_int(&min, &max, p_graph);

    if(min == INT32_MAX) return -1; //Graph empty, exit

    if(graph->scale_setting == SCALE_TO_FIT){

        graph->scaling = (max - min) / p_graph->height;
        graph->scaling = graph->scaling == 0 ? 1 : graph->scaling;

        for(int i = 0; i < p_graph->width; x++, i++){

            if(p_graph->graph_buffer[(p_graph->index + i) % p_graph->width] != DONT_GRAPH){

                y = graph->y_origin + p_graph->height - ((p_graph->graph_buffer[(p_graph->index + i) % p_graph->width] - min) / graph->scaling);

                if(!(y < graph->y_origin || y > graph->y_origin + p_graph->height)){

                    p_graph->last_write[i] = y;
                    LCD_DrawPoint(x, y, graph->color);

                } else {
                    p_graph->last_write[i] = NO_WRITE;
                }
            } else {
                p_graph->last_write[i] = NO_WRITE;
            }
        }
    }

    if(graph->scale_setting == USE_CUSTOM || graph->scale_setting == NO_SCALING){ 

        scaling = graph->scaling;
        if(graph->scale_setting == NO_SCALING) scaling = 1;

        for(int i = 0; i < p_graph->width; x++, i++){

            if(p_graph->graph_buffer[p_graph->index + i % p_graph->width] != DONT_GRAPH){

                y = graph->y_origin + p_graph->height - ((p_graph->graph_buffer[(p_graph->index + i) % p_graph->width] - graph->lower_bound)/scaling);

                if(!(y < graph->y_origin || y > graph->y_origin + p_graph->height)){
                    
                    p_graph->last_write[i] = y;
                    LCD_DrawPoint(x, y, graph->color);
                    
                } else {
                    p_graph->last_write[i] = NO_WRITE;
                }
            } else {
                p_graph->last_write[i] = NO_WRITE;
            }
            
        }

    }

    return 1;
}

int gui_erase_graph(graph_config_t* graph){
    graph_internals_t *p_graph = graph->internals;

    for(int i = 0; i < p_graph->width; i++){
        if(p_graph->last_write[i] != NO_WRITE){
            LCD_DrawPoint(i + graph->x_origin, p_graph->last_write[i], graph->background);
        }
    }
    return 0;
}

static
void minmax_int(int32_t* p_min, int32_t* p_max, graph_internals_t* graph){
    int min = INT32_MAX;
    int max = INT32_MIN;
    int temp;
    for(int i = 0; i < graph->width; i++){
        temp = graph->graph_buffer[(graph->index + i) % graph->width];
        if(temp != DONT_GRAPH){
            min = temp < min ? temp : min;
            max = temp > max ? temp : max;
        }
        
    }
    *p_min = min;
    *p_max = max;
}