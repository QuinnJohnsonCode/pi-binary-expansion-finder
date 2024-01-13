#include <gtk/gtk.h>
#include <string.h>
#include "binary-find-pi.h"

// Will store the binary number to pass to the finder
static int bin[16] = {0};

static void
toggle_button (GtkWidget *widget,
             gpointer   data)
{
    // Retrieve the id from the data stored in the widget
    const char *id = g_object_get_data(G_OBJECT(widget), "id");

    // Convert the id to an index
    int index = atoi(id);

    // Update the binary number at the index specified
    bin[index] = !bin[index];
}

static void
find_pi(GtkWidget *widget,
             gpointer   data)
{
    // Get the text object from the data
    const GtkWidget *find_text = g_object_get_data(G_OBJECT(widget), "text");

    // Update the finding text
    gtk_label_set_text(GTK_LABEL(find_text), "Finding...");
    
    // Create the string based on the int array
    char bin_string[17];
    for (int i = 0; i < 16; ++i)
        bin_string[i] = bin[i] ? '1' : '0';
    bin_string[16] = '\0';
    
    // Find the position of the binary string within pi
    unsigned long int position = find_position_in_pi(bin_string);

    // Convert the unsigned long int to a string
    char output_string[100] = "Hex Position: ";
    char hex_position_string[20];
    char bin_position_string[20];
    snprintf(hex_position_string, sizeof(hex_position_string), "%lu", position);
    snprintf(bin_position_string, sizeof(bin_position_string), "%lu", position * 4);

    // Concatenate the output
    strcat(output_string, hex_position_string);
    strcat(output_string, " Bin Position: ");
    strcat(output_string, bin_position_string);

    // Update the text
    gtk_label_set_text(GTK_LABEL(find_text), output_string);
}

static void 
widget_destroyed(GtkWidget *widget, 
                gpointer data) 
{
    // Free the memory associated with the "id" key
    const char *id = g_object_get_data(G_OBJECT(widget), "id");
    g_free((gpointer) id);
}

static void
activate (GtkApplication *app,
          gpointer        user_data)
{
    GtkWidget *window;
    GtkWidget *button;
    GtkWidget *grid;
    GtkWidget *find_button;
    GtkWidget *find_text;

    // Setup window
    window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "PI Finder");
    gtk_window_set_default_size (GTK_WINDOW (window), 600, 600);
    gtk_window_set_resizable (GTK_WINDOW(window), FALSE);

    // Setup grid
    grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grid), 0);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 0);

    // Create find text
    find_text = gtk_label_new("");
    gtk_grid_attach(GTK_GRID(grid), find_text, 0, 5, 4, 1);

    // Create find button
    find_button = gtk_button_new_with_label("Find 4x4 Image in PI");
    gtk_grid_attach(GTK_GRID(grid), find_button, 0, 0, 4, 1);
    gtk_widget_set_vexpand(find_button, TRUE);
    g_signal_connect(find_button, "clicked", G_CALLBACK(find_pi), NULL);
    g_object_set_data(G_OBJECT(find_button), "text", (gpointer)find_text); // g_strdup (string duplicate)

    // Button information
    char num_buffer[10];
    int id = 0;

    // Setup buttons
    for (int row = 1; row < 5; ++row)
    {
        for (int col = 0; col < 4; ++col)
        {
            // Converts the index to a string (label)
            snprintf(num_buffer, 10, "%d", id); 

            // Add the button to the grid
            button = gtk_toggle_button_new();
            gtk_grid_attach(GTK_GRID(grid), button, col, row, 1, 1);

            // Add the object information
            g_object_set_data(G_OBJECT(button), "id", g_strdup(num_buffer)); // g_strdup (string duplicate)

            // Add the callback for memory freeing
            g_signal_connect(button, "destroy", G_CALLBACK(widget_destroyed), NULL); // g_strdup dynamically allocates more memory

            // Add the callback to the button
            g_signal_connect(button, "clicked", G_CALLBACK(toggle_button), NULL);
            
            // Allow the buttons to expand
            gtk_widget_set_hexpand(button, TRUE);
            gtk_widget_set_vexpand(button, TRUE);

            id++;
        }

    }

    gtk_window_set_child(GTK_WINDOW(window), grid);
    gtk_window_present (GTK_WINDOW (window));
}

int
main (int    argc,
      char **argv)
{
    GtkApplication *app;
    int status;

    app = gtk_application_new ("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    return status;
}