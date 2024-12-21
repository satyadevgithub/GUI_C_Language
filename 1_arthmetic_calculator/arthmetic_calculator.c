// gcc -o arthmetic_calculator arthmetic_calculator.c $(pkg-config --cflags --libs gtk+-3.0)

#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>

static void calculate(GtkWidget *widget, gpointer user_data) {
    // Retrieve the values from the entry fields
    GtkWidget **entry_fields = (GtkWidget **)user_data;
    const gchar *num1_text = gtk_entry_get_text(GTK_ENTRY(entry_fields[0]));
    const gchar *num2_text = gtk_entry_get_text(GTK_ENTRY(entry_fields[1]));

    // Convert the input to floats
    float num1 = atof(num1_text);
    float num2 = atof(num2_text);
    
    // Perform the arithmetic operations
    float sum = num1 + num2;
    float diff = num1 - num2;
    float prod = num1 * num2;
    float quot = (num2 != 0) ? num1 / num2 : 0;  // Check for division by zero

    // Display the results in the labels
    gchar result[100];
    
    snprintf(result, sizeof(result), "Sum: %.2f", sum);
    gtk_label_set_text(GTK_LABEL(entry_fields[2]), result);
    
    snprintf(result, sizeof(result), "Difference: %.2f", diff);
    gtk_label_set_text(GTK_LABEL(entry_fields[3]), result);
    
    snprintf(result, sizeof(result), "Product: %.2f", prod);
    gtk_label_set_text(GTK_LABEL(entry_fields[4]), result);
    
    if (num2 != 0) {
        snprintf(result, sizeof(result), "Quotient: %.2f", quot);
    } else {
        snprintf(result, sizeof(result), "Quotient: Undefined (Div by 0)");
    }
    gtk_label_set_text(GTK_LABEL(entry_fields[5]), result);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    // Create a window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Satya Calculator");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 250);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create a vertical box to pack widgets
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Create entry widgets for two numbers
    GtkWidget *entry1 = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(vbox), entry1, FALSE, FALSE, 0);

    GtkWidget *entry2 = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(vbox), entry2, FALSE, FALSE, 0);

    // Create labels for the results
    GtkWidget *label_sum = gtk_label_new("Addition: ");
    GtkWidget *label_diff = gtk_label_new("Substration: ");
    GtkWidget *label_prod = gtk_label_new("Multiplication: ");
    GtkWidget *label_quot = gtk_label_new("Division: ");
    
    gtk_box_pack_start(GTK_BOX(vbox), label_sum, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), label_diff, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), label_prod, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), label_quot, FALSE, FALSE, 0);

    // Create a button to perform calculations
    GtkWidget *button = gtk_button_new_with_label("Calculate");
    
    // Pass the entries and labels to the callback function
    GtkWidget *entry_fields[] = {entry1, entry2, label_sum, label_diff, label_prod, label_quot};
    g_signal_connect(button, "clicked", G_CALLBACK(calculate), entry_fields);
    gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);

    // Show all widgets
    gtk_widget_show_all(window);

    // Run the GTK+ main loop
    gtk_main();

    return 0;
}

