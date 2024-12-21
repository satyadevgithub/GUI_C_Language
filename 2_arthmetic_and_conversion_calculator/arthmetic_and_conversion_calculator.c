// gcc -o calculator_window calculator_window.c $(pkg-config --cflags --libs gtk+-3.0)

#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>

// Function to convert decimal to binary
void decimal_to_binary(int n, char *binary) {
    int i = 0;
    while (n > 0) {
        binary[i++] = (n % 2) + '0';
        n = n / 2;
    }
    binary[i] = '\0';
    // Reverse the binary string
    int length = i;
    for (int j = 0; j < length / 2; j++) {
        char temp = binary[j];
        binary[j] = binary[length - j - 1];
        binary[length - j - 1] = temp;
    }
}

// Function to convert decimal to octal
void decimal_to_octal(int n, char *octal) {
    int i = 0;
    while (n > 0) {
        octal[i++] = (n % 8) + '0';
        n = n / 8;
    }
    octal[i] = '\0';
    // Reverse the octal string
    int length = i;
    for (int j = 0; j < length / 2; j++) {
        char temp = octal[j];
        octal[j] = octal[length - j - 1];
        octal[length - j - 1] = temp;
    }
}

// Function to convert decimal to hexadecimal
void decimal_to_hexadecimal(int n, char *hexadecimal) {
    int i = 0;
    while (n > 0) {
        int rem = n % 16;
        if (rem < 10)
            hexadecimal[i++] = rem + '0';
        else
            hexadecimal[i++] = rem - 10 + 'A';
        n = n / 16;
    }
    hexadecimal[i] = '\0';
    // Reverse the hexadecimal string
    int length = i;
    for (int j = 0; j < length / 2; j++) {
        char temp = hexadecimal[j];
        hexadecimal[j] = hexadecimal[length - j - 1];
        hexadecimal[length - j - 1] = temp;
    }
}

// Callback for arithmetic operations
static void calculate_arithmetic(GtkWidget *widget, gpointer user_data) {
    GtkWidget **entry_fields = (GtkWidget **)user_data;
    const gchar *num1_text = gtk_entry_get_text(GTK_ENTRY(entry_fields[0]));
    const gchar *num2_text = gtk_entry_get_text(GTK_ENTRY(entry_fields[1]));

    float num1 = atof(num1_text);
    float num2 = atof(num2_text);

    float sum = num1 + num2;
    float diff = num1 - num2;
    float prod = num1 * num2;
    float quot = (num2 != 0) ? num1 / num2 : 0;

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

// Callback for conversion operations
static void convert_decimal(GtkWidget *widget, gpointer user_data) {
    GtkWidget **entry_fields = (GtkWidget **)user_data;
    const gchar *decimal_text = gtk_entry_get_text(GTK_ENTRY(entry_fields[0]));

    int decimal = atoi(decimal_text);

    char binary[32], octal[32], hexadecimal[32];
    decimal_to_binary(decimal, binary);
    decimal_to_octal(decimal, octal);
    decimal_to_hexadecimal(decimal, hexadecimal);

    gchar result[100];

    snprintf(result, sizeof(result), "Binary: %s", binary);
    gtk_label_set_text(GTK_LABEL(entry_fields[1]), result);

    snprintf(result, sizeof(result), "Octal: %s", octal);
    gtk_label_set_text(GTK_LABEL(entry_fields[2]), result);

    snprintf(result, sizeof(result), "Hexadecimal: %s", hexadecimal);
    gtk_label_set_text(GTK_LABEL(entry_fields[3]), result);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    // Create a window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Arithmetic and Conversion");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 475);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create a vertical box
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Arithmetic section
    GtkWidget *entry1 = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(vbox), entry1, FALSE, FALSE, 0);

    GtkWidget *entry2 = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(vbox), entry2, FALSE, FALSE, 0);

    GtkWidget *label_sum = gtk_label_new("Sum: ");
    GtkWidget *label_diff = gtk_label_new("Difference: ");
    GtkWidget *label_prod = gtk_label_new("Product: ");
    GtkWidget *label_quot = gtk_label_new("Quotient: ");
    
    gtk_box_pack_start(GTK_BOX(vbox), label_sum, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), label_diff, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), label_prod, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), label_quot, FALSE, FALSE, 0);

    GtkWidget *arithmetic_button = gtk_button_new_with_label("Calculate Arithmetic");
    GtkWidget *arithmetic_fields[] = {entry1, entry2, label_sum, label_diff, label_prod, label_quot};
    g_signal_connect(arithmetic_button, "clicked", G_CALLBACK(calculate_arithmetic), arithmetic_fields);
    gtk_box_pack_start(GTK_BOX(vbox), arithmetic_button, FALSE, FALSE, 0);

    // Conversion section
    GtkWidget *entry_decimal = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(vbox), entry_decimal, FALSE, FALSE, 0);

    GtkWidget *label_binary = gtk_label_new("Binary: ");
    GtkWidget *label_octal = gtk_label_new("Octal: ");
    GtkWidget *label_hexadecimal = gtk_label_new("Hexadecimal: ");
    
    gtk_box_pack_start(GTK_BOX(vbox), label_binary, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), label_octal, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), label_hexadecimal, FALSE, FALSE, 0);

    GtkWidget *conversion_button = gtk_button_new_with_label("Convert Decimal");
    GtkWidget *conversion_fields[] = {entry_decimal, label_binary, label_octal, label_hexadecimal};
    g_signal_connect(conversion_button, "clicked", G_CALLBACK(convert_decimal), conversion_fields);
    gtk_box_pack_start(GTK_BOX(vbox), conversion_button, FALSE, FALSE, 0);

    // Show all widgets
    gtk_widget_show_all(window);

    // Run the GTK main loop
    gtk_main();

    return 0;
}

