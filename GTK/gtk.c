#include <gtk/gtk.h>
#include <stdio.h>
#include "gtk.h"
#include <stdlib.h>
#include "../SDL/tools.h"
#include "../SDL/shape.h"
#include "../SDL/filter.h"
#include "../SDL/DevTools/shared_stack.h"
#include "../CSS/css.h"

const GdkRGBA black = {.red = 0, .green = 0, .blue = 0, .alpha = 1};
GdkRGBA color = {.red = 0, .green = 0, .blue = 0, .alpha = 1};
SDL_Color sdl_color = {.r = 0, .g = 0, .b = 0};
SDL_Color grey = {.r = 127, .g = 127, .b = 127};
SDL_Color white = {.r = 255, .g = 255, .b = 255};
int crop_color = 0;
GtkColorChooser* ColorButton;
GtkWidget *window;
GtkRadioButton* brush;
GtkRadioButton* bucket;
GtkRadioButton* eraser;
GtkRadioButton* bigeraser;
GtkRadioButton* segment;
GtkRadioButton* square;
GtkRadioButton* triangle;
GtkRadioButton* circle;
GtkRadioButton* Text;
GtkRadioButton* Crop;
GtkRadioButton* Select;
GtkWidget* toolsgrid;
GtkWidget* previous;
GtkWidget* next;
GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;
GtkButton *SaveButton;
GtkWidget *dialog;
SDL_Surface* img;
SDL_Surface* img2;
SDL_Surface* pre_img;
shared_stack* before;
shared_stack* after;
GtkWidget *image;
GtkFileChooser *FileChooser;
GtkFileChooser *chooser;
GtkScale* scale;
unsigned char scale_nb = 10;
GtkComboBoxText* filtres;
GtkButton* apply;
GtkWidget* eventbox;
int pos_x = 0;
int pos_y = 0;
int old_x = 0;
int old_y = 0;
int start_x = 0;
int start_y = 0;
int end_x = 0;
int end_y = 0;
gboolean is_pressed = FALSE;
gboolean save_draw = TRUE;
GtkComboBoxText* Theme;
GtkColorChooser* ThemeColor;
char* path_perso_theme = "CSS/color-theme-light.css";

int tool_value = 1;
gboolean pre_show = FALSE;

//functions
gboolean on_Color_set(GtkColorChooser *self, gpointer user_data);
void set_tools_group(GtkGrid* toolsgrid, GtkRadioButton* brush);
gboolean on_brush(GtkRadioButton *self, gpointer user_data);
gboolean on_bucket(GtkRadioButton *self, gpointer user_data);
gboolean on_eraser(GtkRadioButton *self, gpointer user_data);
gboolean on_bigeraser(GtkRadioButton *self, gpointer user_data);
gboolean on_segment(GtkRadioButton *self, gpointer user_data);
gboolean on_square(GtkRadioButton *self, gpointer user_data);
gboolean on_triangle(GtkRadioButton *self, gpointer user_data);
gboolean on_circle(GtkRadioButton *self, gpointer user_data);
gboolean on_previous(GtkButton *self, gpointer user_data);
gboolean on_next(GtkButton *self, gpointer user_data);
gboolean on_FileChoosing_file_set(GtkFileChooserButton *f, gpointer user_data);
gboolean on_SaveButton_clicked(GtkButton *f, gpointer user_data);
gboolean on_apply_clicked(GtkButton *self, gpointer user_data);
gboolean update_scale_val(GtkScale *self, gpointer user_data);
gboolean mouse_moved(GtkWidget *widget,GdkEvent *event, gpointer user_data);
gboolean mouse_release(GtkWidget* self, GdkEvent* event, gpointer user_data);
gboolean mouse_press(GtkWidget* self, GdkEvent* event, gpointer user_data);
//GtkWidget* gtk_image_new_from_sdl_surface (SDL_Surface *surface);
gboolean draw_callback(GtkWidget* widget, cairo_t *cr, gpointer data);
gboolean on_Text(GtkRadioButton *self, gpointer user_data);
gboolean on_Select(GtkRadioButton *self, gpointer user_data);
gboolean on_Crop(GtkRadioButton *self, gpointer user_data);
void decolor_free(gpointer user_data);
gboolean on_key_press (GtkWidget *widget, GdkEventKey *event, gpointer user_data);
static void load_css(const char path[]);
gboolean theme_changed();
void CSS_rewrite();


int create_window_decolor(int argc, char *argv[])
{
    gtk_init (&argc, &argv);

    GtkBuilder* Builder = gtk_builder_new_from_file("./GTK/GUI.glade");

    //tools = g_slist_alloc();
    img = load_image("./GTK/blank.png");
    img2 = load_image("./GTK/blank.png");
    pre_img = NULL;
    load_css("CSS/light-theme.css");
    
    // tools previous / next image
    before = shared_stack_new();
    after = shared_stack_new();
    
    // Getting objects
    window = GTK_WIDGET(gtk_builder_get_object(Builder, "MyWindow"));
    ColorButton = GTK_COLOR_CHOOSER(gtk_builder_get_object(Builder, "Color"));
    gtk_color_chooser_set_rgba(ColorButton,&black);
    image = GTK_WIDGET(gtk_builder_get_object(Builder, "image"));
    if (image == NULL)
        printf("%s", "image is NULL");
    gtk_widget_add_events(image, GDK_POINTER_MOTION_MASK);
    gtk_widget_add_events(image, GDK_BUTTON_PRESS_MASK);
    gtk_widget_add_events(image, GDK_BUTTON_RELEASE_MASK);
    toolsgrid = GTK_WIDGET(gtk_builder_get_object(Builder, "toolsgrid"));
    SaveButton = GTK_BUTTON(gtk_builder_get_object(Builder, "Save"));
    FileChooser = GTK_FILE_CHOOSER(gtk_builder_get_object(Builder, "FileChooser"));
    eventbox = gtk_event_box_new ();
    //gtk_container_add(GTK_CONTAINER (eventbox), image);
    
    // Tools buttons
    brush = GTK_RADIO_BUTTON(gtk_builder_get_object(Builder, "brush"));
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(brush), TRUE);
    //gtk_radio_button_set_group (brush ,tools);
    bucket = GTK_RADIO_BUTTON(gtk_builder_get_object(Builder, "bucket")); 
    eraser = GTK_RADIO_BUTTON(gtk_builder_get_object(Builder, "eraser"));
    bigeraser = GTK_RADIO_BUTTON(gtk_builder_get_object(Builder, "bigeraser"));
    segment = GTK_RADIO_BUTTON(gtk_builder_get_object(Builder, "segment"));
    square = GTK_RADIO_BUTTON(gtk_builder_get_object(Builder, "square"));
    triangle = GTK_RADIO_BUTTON(gtk_builder_get_object(Builder, "triangle"));
    circle = GTK_RADIO_BUTTON(gtk_builder_get_object(Builder, "circle"));
    //set_tools_group(toolsgrid, brush);
    scale = GTK_SCALE(gtk_builder_get_object(Builder, "Scale")); 
    filtres = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(Builder, "Filtres"));
    apply = GTK_BUTTON(gtk_builder_get_object(Builder, "Appliquer"));
    Text = GTK_RADIO_BUTTON(gtk_builder_get_object(Builder, "Text"));
    Select = GTK_RADIO_BUTTON(gtk_builder_get_object(Builder, "Select"));
    Crop = GTK_RADIO_BUTTON(gtk_builder_get_object(Builder, "Crop"));
    Theme = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(Builder, "theme"));
    ThemeColor = GTK_COLOR_CHOOSER(gtk_builder_get_object(Builder, "ThemeColor"));

    // Previous and Next
    previous = GTK_WIDGET(gtk_builder_get_object(Builder, "previous"));
    next = GTK_WIDGET(gtk_builder_get_object(Builder, "next"));


    // Handling Signals
    g_signal_connect(ColorButton, "color-set", G_CALLBACK(on_Color_set), NULL);
    g_signal_connect(brush, "toggled", G_CALLBACK(on_brush), NULL);
    g_signal_connect(bucket, "toggled", G_CALLBACK(on_bucket), NULL);
    g_signal_connect(eraser, "toggled", G_CALLBACK(on_eraser), NULL);
    g_signal_connect(bigeraser, "toggled", G_CALLBACK(on_bigeraser), NULL);
    g_signal_connect(segment, "toggled", G_CALLBACK(on_segment), NULL);
    g_signal_connect(square,"toggled", G_CALLBACK(on_square), NULL);
    g_signal_connect(triangle, "toggled", G_CALLBACK(on_triangle), NULL);
    g_signal_connect(circle, "toggled", G_CALLBACK(on_circle), NULL);
    g_signal_connect(scale, "value_changed", G_CALLBACK(update_scale_val), NULL);
    g_signal_connect(apply, "clicked", G_CALLBACK(on_apply_clicked), NULL);
    g_signal_connect(Text, "toggled", G_CALLBACK(on_Text), NULL);
    g_signal_connect(Select, "toggled", G_CALLBACK(on_Select), NULL);
    g_signal_connect(Crop, "toggled", G_CALLBACK(on_Crop), NULL);
    g_signal_connect(Theme, "changed", G_CALLBACK(theme_changed), NULL);
    g_signal_connect(ThemeColor, "color-set", G_CALLBACK(CSS_rewrite), NULL);
    g_signal_connect(previous, "clicked", G_CALLBACK(on_previous), NULL);
    g_signal_connect(next, "clicked", G_CALLBACK(on_next), NULL);
    //replace NULL by the stack containing the modifications.

    g_signal_connect (G_OBJECT (window), "key_press_event", G_CALLBACK (on_key_press), NULL);
    g_signal_connect(SaveButton, "clicked", G_CALLBACK(on_SaveButton_clicked), image);
    g_signal_connect(FileChooser, "file-set", G_CALLBACK(on_FileChoosing_file_set), image);
    g_signal_connect(window, "destroy", G_CALLBACK(decolor_free), NULL);
    
    g_signal_connect (image, "motion-notify-event", G_CALLBACK(mouse_moved), NULL);
    g_signal_connect (image, "button-press-event", G_CALLBACK(mouse_press), NULL);
    g_signal_connect (image, "button-release-event", G_CALLBACK(mouse_release), NULL);

    g_signal_connect (G_OBJECT (image), "draw", G_CALLBACK (draw_callback), NULL);

    gtk_widget_set_app_paintable(image, TRUE);
    gtk_widget_show_all(window);

    gtk_main();

    return EXIT_SUCCESS;
}

void decolor_free(gpointer user_data)
{
    if (user_data == NULL)
    {
        printf("MMH\n");
    }
    shared_stack_destroy(before);
    shared_stack_destroy(after);
    if (pre_img != NULL)
        SDL_FreeSurface(pre_img);
    SDL_FreeSurface(img);
    SDL_FreeSurface(img2);
    //cairo_destroy (cr);
    
    printf("free tout ici");
    gtk_main_quit();
}

gboolean draw_callback(GtkWidget* widget, cairo_t *cr, gpointer data)
{
    //Unused parameters :
    widget = widget;
    data = data;

    //Actual function :
    if (pre_show == TRUE)
    {
        SDL_SaveBMP(pre_img, "./GTK/tmpfile.bmp");
        SDL_FreeSurface(pre_img);
        pre_img = NULL;
        if (pre_img != NULL)
            printf("free = caca\n");
    }
    else
    {
        SDL_SaveBMP(img, "./GTK/tmpfile.bmp");
    }
    GdkPixbuf *pixbuf;

    pixbuf = gdk_pixbuf_new_from_file("./GTK/tmpfile.bmp", NULL);
    

    gdk_cairo_set_source_pixbuf(cr, pixbuf, 0, 0);
    cairo_paint(cr);
    //cairo_destroy (cr);

    if (pixbuf)
        g_object_unref(pixbuf);

    return FALSE;
}

/*
void update_gtk(int x, int y, int a, int b)
{
    gtk_widget_queue_draw_area(image,start_x, start_y, pos_x, pos_y);
}*/

gboolean on_key_press (GtkWidget *widget, GdkEventKey *event, gpointer user_data)
{

  if (widget == NULL && user_data != NULL)
      return FALSE;

  switch (event->keyval)
  {
    case GDK_KEY_z:
      if (event->type == GDK_KEY_PRESS && GDK_CONTROL_MASK)
      {
        printf("key pressed: %s\n", "ctrl + z");
        on_previous(NULL, NULL);
      }
      break;
    case GDK_KEY_y:
      if (event->type == GDK_KEY_PRESS && GDK_CONTROL_MASK)
      {
        printf("key pressed: %s\n", "ctrl + y");
        on_next(NULL, NULL);
      }
      break;

    default:
      return FALSE;
  }

  return FALSE;
}

gboolean mouse_release(GtkWidget* self, GdkEvent* event, gpointer user_data)
{
    //Unused parameters :
    self = self;
    event = event;

    //Actual function :
    int oldh = 0;
    int oldw = 0;

    if(user_data == NULL)
    {
        is_pressed = FALSE;
        end_x = pos_x;
        end_y = pos_y;
        //printf("End coordinates: (%u,%u)\n", end_x, end_y);

        switch (tool_value)
        {
            case -1:
                break;

            case 5:
                // Line call
                shared_stack_push(before, img);                                    
                shared_stack_empty(after);
                pre_show = FALSE;

                // Block img is isn't free
                if (pre_img != NULL){
                    SDL_FreeSurface(pre_img);
                    pre_img = NULL;
                }

                drawline(img, sdl_color, start_x, start_y, end_x, end_y, (int)scale_nb / 3);
                gtk_widget_queue_draw_area(image,0,0,img->w,img->h);
                break;

            case 6:
                // Square call
                shared_stack_push(before, img);
                shared_stack_empty(after);
                pre_show = FALSE;

                // Block img is isn't free
                if (pre_img != NULL){
                    SDL_FreeSurface(pre_img);
                    pre_img = NULL;
                }

                make_empty_square(img, start_x, start_y, end_x, end_y, sdl_color, (int)scale_nb / 2);
                gtk_widget_queue_draw_area(image,0,0,img->w,img->h);
                break;

            case 7:
                // Triangle call
                shared_stack_push(before, img);                                    
                shared_stack_empty(after);                                    
                pre_show = FALSE;
                
                // Block img is isn't free
                if (pre_img != NULL){
                    SDL_FreeSurface(pre_img);
                    pre_img = NULL;
                }

                make_empty_triangle(img, start_x, start_y, end_x, end_y, sdl_color, (int)scale_nb / 3);
                gtk_widget_queue_draw_area(image,0,0,img->w,img->h);
                break;

            case 8:
                // Circle call
                shared_stack_push(before, img);                                    
                shared_stack_empty(after);
                pre_show = FALSE;
                
                // Block img is isn't free
                if (pre_img != NULL){
                    SDL_FreeSurface(pre_img);
                    pre_img = NULL;
                }

                bresenham_circle(img, start_x, start_y, end_x, end_y, sdl_color, (int)scale_nb / 3);
                gtk_widget_queue_draw_area(image,0,0,img->w,img->h);
                break;

            case 9:
                // Text Call
                shared_stack_push(before, img);
                shared_stack_empty(after);
                // FONCTION ICI !!!!
                gtk_widget_queue_draw_area(image,0,0,img->w,img->h);
                break;

            case 10:
                // Crop Call
                shared_stack_push(before, img);
                shared_stack_empty(after);
                pre_show = FALSE;
                
                // Block img is isn't free
                if (pre_img != NULL){
                    SDL_FreeSurface(pre_img);
                    pre_img = NULL;
                }

                oldh = img->h;
                oldw = img->w;
                img = crop(img, start_x, start_y, end_x, end_y);
                gtk_widget_queue_draw_area(image,0,0,oldw,oldh);
                break;

            case 11:
                // Select Call

                break;
        }
    }
    return FALSE;
}

gboolean mouse_press(GtkWidget* self, GdkEvent* event, gpointer user_data)
{
    //Unused parameters :
    self = self;
    event = event;

    //Actual function :
    if(user_data == NULL)
    {
        is_pressed = TRUE;
        start_x = pos_x;
        start_y = pos_y;
        //printf("Start coordinates: (%u,%u)\n", start_x, start_y);

        switch (tool_value)         
        {
            case -1:
                break;
            case 1:
                // point call
                shared_stack_push(before, img);
                shared_stack_empty(after);                                        
                point(img, sdl_color, start_x, start_y, (int)scale_nb / 3);   
                gtk_widget_queue_draw_area(image,0,0,img->w,img->h);
                break;

             case 2:
                // bucket call
                shared_stack_push(before, img);
                shared_stack_empty(after);                                        
                filling_seal(img, start_x, start_y, sdl_color, ((int)scale_nb * 255) / 100);   
                gtk_widget_queue_draw_area(image,0,0,img->w,img->h);
                break;
                
         }
    }

    return FALSE;
}

gboolean mouse_moved(GtkWidget *widget,GdkEvent *event, gpointer user_data) 
{
    //Unused parameters :
    widget = widget;

    //Actual function :
    if (event->type==GDK_MOTION_NOTIFY && user_data == NULL) 
    {
        GdkEventMotion* e =(GdkEventMotion*)event;
        old_x = pos_x;
        old_y = pos_y;
        pos_x = (guint)e->x;
        pos_y = (guint)e->y;

        //printf("Old coordinates: (%u,%u)\n", old_x, old_y);
        //printf("coordinates: (%u,%u)\n", pos_x, pos_y);

        if (tool_value == 1)
        {
            if (is_pressed)
            {
                //point(img, sdl_color, pos_x, pos_y, (int)scale_nb);
                drawline(img, sdl_color, old_x, old_y, pos_x, pos_y, (int)scale_nb / 3);
                gtk_widget_queue_draw_area(image,0,0,img->w,img->h);
            }
            // Preview POINT
            /*
            else
            {
                pre_show = TRUE;
                pre_img = copy_image(img);
                bresenham_circle(pre_img, pos_x, pos_y, pos_x + (int)(scale_nb / 4), pos_y + (int)(scale_nb / 4), grey, 1);
                gtk_widget_queue_draw_area(image,0,0,img->w,img->h);
            }
            */
        }
        else
        {
            if (tool_value == 3 && is_pressed)
            {
                if (save_draw)
                {
                    save_draw = FALSE;
                    shared_stack_push(before, img);
                    shared_stack_empty(after);
                }

                //point(img, sdl_color, pos_x, pos_y, (int)scale_nb);
                drawline(img, white, old_x, old_y, pos_x, pos_y, (int)scale_nb / 3);
                gtk_widget_queue_draw_area(image,0,0,img->w,img->h);
            }
            else
            {
                if (tool_value == 4 && is_pressed)
                {
                    if (save_draw)
                    {
                        save_draw = FALSE;
                        shared_stack_push(before, img);
                        shared_stack_empty(after);
                    }

                    //point(img, sdl_color, pos_x, pos_y, (int)scale_nb);
                    drawline_image(img, img2, old_x, old_y, pos_x, pos_y, (int)scale_nb / 3);
                    gtk_widget_queue_draw_area(image,0,0,img->w,img->h);
                }
                else
                {
                    if (tool_value == 5 && is_pressed)
                    {
                        pre_show = TRUE;
                        pre_img = copy_image(img);
                        drawline(pre_img, sdl_color, start_x, start_y, pos_x, pos_y, (int)scale_nb / 3);
                        gtk_widget_queue_draw_area(image,0,0,img->w,img->h);
                    }
                    else
                    {
                        if (tool_value == 6 && is_pressed)
                        {
                            pre_show = TRUE;
                            pre_img = copy_image(img);
                            make_empty_square(pre_img, start_x, start_y, pos_x, pos_y, sdl_color, (int)scale_nb / 2);
                            gtk_widget_queue_draw_area(image,0,0,img->w,img->h);
                        }
                        else
                        {
                            if (tool_value == 7 && is_pressed)
                            {
                                pre_show = TRUE;
                                pre_img = copy_image(img);
                                make_empty_triangle(pre_img, start_x, start_y, pos_x, pos_y, sdl_color, (int)scale_nb / 3);
                                gtk_widget_queue_draw_area(image,0,0,img->w,img->h);
                            }
                            else
                            {
                                if (tool_value == 8 && is_pressed)
                                {
                                    pre_show = TRUE;
                                    pre_img = copy_image(img);
                                    bresenham_circle(pre_img, start_x, start_y, pos_x, pos_y, sdl_color, (int)scale_nb / 3);
                                    gtk_widget_queue_draw_area(image,0,0,img->w,img->h);
                                }
                                else
                                {
                                    if (tool_value == 10 && is_pressed)
                                    {
                                        pre_show = TRUE;
                                        pre_img = copy_image(img); 
                                        make_empty_square(pre_img, start_x, start_y, pos_x, pos_y, grey, 1);
                                        gtk_widget_queue_draw_area(image,0,0,img->w,img->h);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        
        if (!is_pressed)
            save_draw = TRUE;
    }
    return TRUE;
}

gboolean on_previous(GtkButton* self, gpointer user_data)
{
    //use this fonction to revert last modification
    if (self && user_data)
        return FALSE;

    if (before->size > 0)
    {
        //printf("%li\n", before->size);
        int oldh = img->h;
        int oldw = img->w;

        shared_stack_push(after, img);
        SDL_FreeSurface(img);
        img = shared_stack_pop(before);
        if (img->h > oldh)
            oldh = img->h;
        if (img->w > oldw)
            oldw = img->w;

        gtk_widget_queue_draw_area(image,0,0,oldw,oldh);
    }

    return FALSE;
}

gboolean on_next(GtkButton* self, gpointer user_data)
{
    //use this fonction to re-do last modification that was reverted
    if (self && user_data)  
        return FALSE;

    if (after->size > 0)
    {
        //printf("%li\n", after->size);
        int oldh = img->h;
        int oldw = img->w;
        shared_stack_push(before, img);
        SDL_FreeSurface(img);
        img = shared_stack_pop(after);
        if (img->h > oldh)
            oldh = img->h;
        if (img->w > oldw)
            oldw = img->w;

        gtk_widget_queue_draw_area(image,0,0,oldw,oldh);
    }
    return FALSE;
}

gboolean on_Text(GtkRadioButton *self, gpointer user_data)
{
    if (user_data == NULL &&
        gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(self)) == TRUE)
    {
        tool_value = 9;
    }
    return FALSE;
}

gboolean on_Crop(GtkRadioButton *self, gpointer user_data)
{
    if (user_data == NULL &&
        gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(self)) == TRUE)
    {
        tool_value = 10;
    }
    return FALSE;
}

gboolean on_Select(GtkRadioButton *self, gpointer user_data)
{
    if (user_data == NULL &&
        gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(self)) == TRUE)
    {
        tool_value = 11;
    }
    return FALSE;
}

gboolean on_brush(GtkRadioButton *self, gpointer user_data)
{
    if (user_data == NULL &&
        gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(self)) == TRUE)
    {
        tool_value = 1;
    }

    return FALSE;
}

gboolean on_bucket(GtkRadioButton *self, gpointer user_data)
{
     if (user_data == NULL &&
         gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(self)) == TRUE)
     {
         tool_value = 2;
     }
     return FALSE;
}

gboolean on_eraser(GtkRadioButton *self, gpointer user_data)
{
     if (user_data == NULL &&
         gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(self)) == TRUE)
     {
         tool_value = 3;
     }
     return FALSE;
}

gboolean on_bigeraser(GtkRadioButton *self, gpointer user_data)
{
     if (user_data == NULL &&
         gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(self)) == TRUE)
     {
         tool_value = 4;
     }
     return FALSE;
}

gboolean on_segment(GtkRadioButton *self, gpointer user_data)
{
     if (user_data == NULL &&
         gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(self)) == TRUE)
     {
         tool_value = 5;
     }
     return FALSE;
}

gboolean on_square(GtkRadioButton *self, gpointer user_data)
{
     if (user_data == NULL &&
         gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(self)) == TRUE)
     {
         tool_value = 6;
     }
     return FALSE;
}

gboolean on_triangle(GtkRadioButton *self, gpointer user_data)
{
     if (user_data == NULL &&
         gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(self)) == TRUE)
     {
         tool_value = 7;
     }
     return FALSE;
}

gboolean on_circle(GtkRadioButton *self, gpointer user_data)
{
     if (user_data == NULL &&
         gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(self)) == TRUE)
     {
         tool_value = 8;
     }
     return FALSE;
}

void set_tools_group(GtkGrid* toolsgrid, GtkRadioButton* brush)
{
    for (int row = 0; row < 2; row++)
    {
        for (int col = 1-row; col < 4; col++)
        {
            gtk_radio_button_join_group(GTK_RADIO_BUTTON(gtk_grid_get_child_at(toolsgrid,col,row)),
            brush);
        }
    }
}

gboolean on_SaveButton_clicked(GtkButton *f ,gpointer user_data)
{
    //Unused parameters :
    user_data = user_data;

    //Actual function :
    dialog = gtk_file_chooser_dialog_new("Save File", GTK_WINDOW(window), action, 
    "_Cancel", GTK_RESPONSE_CANCEL, "_Save", GTK_RESPONSE_ACCEPT, NULL);

    chooser = GTK_FILE_CHOOSER(dialog);
    gtk_file_chooser_set_current_name (chooser, "Saving Image");

    int res = gtk_dialog_run (GTK_DIALOG (dialog));
    if (res == GTK_RESPONSE_ACCEPT)
    {
        char *filename;

        filename = gtk_file_chooser_get_filename (chooser);
        printf("%s", gtk_button_get_label(f));
        if (img != NULL)
            SDL_SaveBMP(img, filename);
        g_free (filename);
    }

    gtk_widget_destroy (dialog);

    return FALSE;
}


gboolean on_FileChoosing_file_set(GtkFileChooserButton *f, gpointer user_data)
{
    //GdkPixbuf *pixbuf;
    GtkWidget* image = user_data;
    char *filename;
    int oldh = 0;
    int oldw = 0;


    filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(f));
    oldh = img->h;
    oldw = img->w;
    shared_stack_push(before, img);
    shared_stack_empty(after);
    
    if (img != NULL)
    {
        SDL_FreeSurface(img);
        img = NULL;
    }
    if (img2 != NULL)
    {
        SDL_FreeSurface(img2);
        img2 = NULL;
    }

    img = load_image(filename);
    img2 = load_image(filename);
    
    //image = gtk_image_new_from_sdl_surface(img);
    
    gtk_widget_queue_draw_area(image,0,0,oldw,oldh);

    int w = 1310;
    int h = 903;
    if(img->w > 1080)
    {
        w = img->w+230;
        gtk_widget_set_margin_start(GTK_WIDGET(image), 0);
    }
    else
        gtk_widget_set_margin_start(GTK_WIDGET(image), (w-230-img->w)/2);

    if(img->h > 850)
    {
        h = img->h+53;
        gtk_widget_set_margin_top(GTK_WIDGET(image), 0);
    }
    else
        gtk_widget_set_margin_top(GTK_WIDGET(image), (h-53-img->h)/2);

    gtk_window_resize(GTK_WINDOW(window), w, h);     
    
    //pixbuf = gdk_pixbuf_new_from_file(filename, NULL);

    g_free(filename);
    /*
    gtk_image_set_from_pixbuf(GTK_IMAGE(image), pixbuf);
    if (pixbuf)
        g_object_unref(pixbuf);

    gtk_widget_show(image);*/

    return FALSE;
}

gboolean on_Color_set(GtkColorChooser *self, gpointer user_data)
{

    if (user_data != NULL)
        return FALSE;
    
    gtk_color_chooser_get_rgba(self,&color);
    //printf("R:%f, G:%f, B:%f\n", color.red, color.green, color.blue);
    sdl_color.r = (Uint8) (color.red * 255);
    sdl_color.g = (Uint8) (color.green * 255);
    sdl_color.b = (Uint8) (color.blue * 255);
    //sdl_color->a = (int) (color->alpha * 255);
    return FALSE;
}


gboolean update_scale_val(GtkScale *self, gpointer user_data)
{
    //Unused parameters :
    user_data = user_data;

    //Actual function :
    scale_nb = gtk_range_get_value(&(self->range));
    //printf("Scale number : %i\n", scale_nb);
    
    return FALSE;
}


gboolean on_apply_clicked(GtkButton *self, gpointer user_data)
{
    // Just for avoiding warnings you can change if you want
    if (self == NULL && user_data != NULL)
        return FALSE;


    char* fil = gtk_combo_box_text_get_active_text(filtres);
    
    if(!fil)
    {
        printf("Nothing\n");
        return FALSE;
    }

    switch(fil[0])
    {
        case 'N':
        {
            printf("Noir et Blanc\n");
	        shared_stack_push(before, img);
            shared_stack_empty(after);
	        grayscale(img);
	        gtk_widget_queue_draw_area(image,0,0,img->w,img->h);
            return FALSE; //Filtre noir et blanc
        }

        case 'I':
        {
            printf("Inversion\n");
	        shared_stack_push(before, img);
            shared_stack_empty(after);
            negative(img);
            gtk_widget_queue_draw_area(image,0,0,img->w,img->h);
            return FALSE; //Filtre d'inversion de couleur
        }

        case 'C':
        {
            printf("Contraste\n");
	        shared_stack_push(before, img);
            shared_stack_empty(after);
            color_filter(img, 1, 0, 0, 0);
            grayscale(img);
            gtk_widget_queue_draw_area(image,0,0,img->w,img->h);
            return FALSE; //Filtre de Contraste
        }

        case 'L':
        {
            printf("Luminosité\n");
	        shared_stack_push(before, img);
            shared_stack_empty(after);
            color_filter(img, 1, 0, 0, 255);
            grayscale(img);
            gtk_widget_queue_draw_area(image,0,0,img->w,img->h);
            return FALSE; //Filtre de Luminosité
        }

        case 'S':
        {
            if(fil[3] == 'p') // 'é'= two chars
            {
                printf("Sépia\n");
		        shared_stack_push(before, img);
                shared_stack_empty(after);
                color_filter(img, 1, 0, 0, 0);
                gtk_widget_queue_draw_area(image,0,0,img->w,img->h);
                return FALSE; //Filtre Sépia
            }

            else
            {
                printf("Saturation\n");
		        shared_stack_push(before, img);
                shared_stack_empty(after);
                color_filter(img, 1, 0, 0, 255);
                gtk_widget_queue_draw_area(image,0,0,img->w,img->h);
                return FALSE; //Filtre Saturation
            }
        } 

        default :
        {
            printf("Nothing\n");
            return FALSE; //Aucun filtre
        }
    }

    free(fil);
    return FALSE;
}

static void load_css(const char path[])
{
  GtkCssProvider* provider;
  GdkDisplay* display;
  GdkScreen* screen;

  //const char* css_style_file = path;
  GFile* css_fp = g_file_new_for_path(path);
  GError* err = 0;

  provider = gtk_css_provider_new();
  display = gdk_display_get_default();
  screen = gdk_display_get_default_screen(display);

  gtk_style_context_add_provider_for_screen(screen,
    GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
  gtk_css_provider_load_from_file(provider, css_fp, &err);

  if(err)
    errx(EXIT_FAILURE, "Load css... failed");

  g_object_unref(provider);
}

gboolean theme_changed(void)
{
    char* th = gtk_combo_box_text_get_active_text(Theme);
    char* path = calloc(sizeof(char), 19);

    switch(th[2])
    {
        case 'a':
        {
            path = "CSS/light-theme.css";
            break;
        }

        case 'm':
        {
            path = "CSS/dark-theme.css";
            break;
        }

        case 'u':
        {
            path = path_perso_theme;
            break;
        }

        default :
        {
            if(th[12] == 'c')
                path = "CSS/light-hc-theme.css";

            else
                path = "CSS/dark-hc-theme.css";
        }
    }

    load_css(path);
    free(th);
    return FALSE;
}

void CSS_rewrite()
{
    //Get initial color
    GdkRGBA col;
    gtk_color_chooser_get_rgba(ThemeColor,&col);
    char* hex;
    char color[] = "FFFFFF";
    int light_or_dark = 76*col.red + 150*col.green + 28*col.blue;

    hex = ints_to_hexas((int) (255*col.red));
    color[0] = hex[0]; color[1] = hex[1];

    hex = ints_to_hexas((int) (255*col.green));
    color[2] = hex[0]; color[3] = hex[1];

    hex = ints_to_hexas((int) (255*col.blue));
    color[4] = hex[0]; color[5] = hex[1];
    
    free(hex);

    if(light_or_dark>85)
    {
        CSS_rewrite_light(color);
        path_perso_theme = "CSS/color-theme-light.css";
    }

    else
    {
        CSS_rewrite_dark(color);
        path_perso_theme = "CSS/color-theme-dark.css\0";
    }
    
    gtk_combo_box_set_active(GTK_COMBO_BOX(Theme), 4);
    load_css(path_perso_theme);
}
