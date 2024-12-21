// gcc -o linked_list linked_list.c $(pkg-config --cflags --libs gtk+-3.0)

#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>

// Linked List Node structure
struct Node {
    int data;
    struct Node* next;
};

// Structure to hold the linked list data and GTK elements
struct LinkedList {
    struct Node* head;
    GtkWidget* drawing_area;
};

// Function to create a new node
struct Node* createNode(int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

// Function to insert a node at a specific position in the linked list
void insertNodeAtPos(struct LinkedList* list, int data, int pos) {
    struct Node* newNode = createNode(data);

    // If the position is at the head (first position)
    if (pos == 0) {
        newNode->next = list->head;
        list->head = newNode;
        return;
    }

    struct Node* temp = list->head;
    int currentPos = 0;

    // Traverse to the node before the desired position
    while (temp != NULL && currentPos < pos - 1) {
        temp = temp->next;
        currentPos++;
    }

    // If the position is greater than the list length, append at the end
    if (temp == NULL) {
        free(newNode);  // Free the allocated memory if position is invalid
        return;
    }

    // Insert the new node at the desired position
    newNode->next = temp->next;
    temp->next = newNode;
}

// Function to delete a node with a specific value
void deleteNode(struct LinkedList* list, int data) {
    struct Node* temp = list->head;
    struct Node* prev = NULL;

    // If the node to be deleted is the head node
    if (temp != NULL && temp->data == data) {
        list->head = temp->next;
        free(temp);
        return;
    }

    // Search for the node to be deleted
    while (temp != NULL && temp->data != data) {
        prev = temp;
        temp = temp->next;
    }

    // If the data was not found in the list
    if (temp == NULL) {
        return;
    }

    // Unlink the node from the linked list
    prev->next = temp->next;
    free(temp);
}

// Function to draw the linked list visually
void draw_linked_list(GtkWidget* widget, cairo_t* cr, struct Node* head) {
    int x = 100;
    int y = 50;
    int node_width = 40;
    int node_height = 30;
    int spacing = 70;

    while (head != NULL) {
        // Draw the node as a rectangle with text
        cairo_rectangle(cr, x, y, node_width, node_height);
        cairo_set_source_rgb(cr, 0, 0, 1); // Blue color for node
        cairo_fill_preserve(cr);
        cairo_set_source_rgb(cr, 1, 1, 1); // White color for text
        cairo_move_to(cr, x + 10, y + 20);
        cairo_show_text(cr, g_strdup_printf("%d", head->data));

        // Draw an arrow pointing to the next node
        if (head->next != NULL) {
            cairo_move_to(cr, x + node_width, y + node_height / 2);
            cairo_line_to(cr, x + spacing, y + node_height / 2);
            cairo_stroke(cr);
        }

        x += spacing;
        head = head->next;
    }
}

// GTK Draw event callback function
gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
    struct LinkedList* list = (struct LinkedList*)user_data;
    draw_linked_list(widget, cr, list->head);
    return FALSE;
}

// Function to handle "Add Node" button click
void on_add_node_click(GtkWidget *widget, gpointer user_data) {
    GtkEntry* entry = (GtkEntry*)user_data;
    const gchar* entry_text = gtk_entry_get_text(entry);
    int value = atoi(entry_text);

    if (value != 0) { // Only insert non-zero values
        struct LinkedList* list = g_object_get_data(G_OBJECT(widget), "list_data");
        insertNodeAtPos(list, value, 0); // Add node at the start (default)
        gtk_widget_queue_draw(list->drawing_area); // Redraw the list
    }
}

// Function to handle "Insert at Position" button click
void on_insert_at_pos_click(GtkWidget *widget, gpointer user_data) {
    GtkWidget **widgets = (GtkWidget **)user_data;
    GtkEntry* entry_value = GTK_ENTRY(widgets[0]);
    GtkEntry* entry_pos = GTK_ENTRY(widgets[1]);

    const gchar* entry_value_text = gtk_entry_get_text(entry_value);
    const gchar* entry_pos_text = gtk_entry_get_text(entry_pos);

    int value = atoi(entry_value_text);
    int pos = atoi(entry_pos_text);

    if (value != 0 && pos >= 0) { // Insert only valid values and positions
        struct LinkedList* list = g_object_get_data(G_OBJECT(widget), "list_data");
        insertNodeAtPos(list, value, pos); // Insert at the specified position
        gtk_widget_queue_draw(list->drawing_area); // Redraw the list
    }
}

// Function to handle "Delete Node" button click
void on_delete_node_click(GtkWidget *widget, gpointer user_data) {
    GtkEntry* entry = (GtkEntry*)user_data;
    const gchar* entry_text = gtk_entry_get_text(entry);
    int value = atoi(entry_text);

    if (value != 0) { // Only delete non-zero values
        struct LinkedList* list = g_object_get_data(G_OBJECT(widget), "list_data");
        deleteNode(list, value);
        gtk_widget_queue_draw(list->drawing_area); // Redraw the list
    }
}

// Main function to create the GUI
int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    // Create the main window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Linked List Visualizer");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 400);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create a vertical box container for input and linked list display
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Linked List data structure
    struct LinkedList list;
    list.head = NULL;

    // Create a drawing area to display the linked list
    list.drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(list.drawing_area, 800, 200);
    gtk_box_pack_start(GTK_BOX(vbox), list.drawing_area, TRUE, TRUE, 0);
    g_signal_connect(list.drawing_area, "draw", G_CALLBACK(on_draw_event), &list);

    // Create a label for value and position inputs
    GtkWidget *label_value = gtk_label_new("Enter Node Value:");
    gtk_box_pack_start(GTK_BOX(vbox), label_value, FALSE, FALSE, 0);

    // Create a text box (entry widget) for node value input
    GtkWidget *entry_value = gtk_entry_new();
    gtk_widget_set_size_request(entry_value, 200, -1); // Set width to 200, height will remain unchanged
    gtk_widget_set_hexpand(entry_value, TRUE); // Allow it to expand horizontally
    gtk_box_pack_start(GTK_BOX(vbox), entry_value, FALSE, FALSE, 0);

    // Create a label for position input
    GtkWidget *label_pos = gtk_label_new("Enter Position:");
    gtk_box_pack_start(GTK_BOX(vbox), label_pos, FALSE, FALSE, 0);

    // Create a text box for position input
    GtkWidget *entry_pos = gtk_entry_new();
    gtk_widget_set_size_request(entry_pos, 200, -1);   // Set width to 200, height will remain unchanged
    gtk_widget_set_hexpand(entry_pos, TRUE); // Allow it to expand horizontally
    gtk_box_pack_start(GTK_BOX(vbox), entry_pos, FALSE, FALSE, 0);

    // Button to add a node to the linked list
    GtkWidget *add_button = gtk_button_new_with_label("Add Node");
    g_object_set_data(G_OBJECT(add_button), "list_data", &list);  // Set list data as user_data
    g_signal_connect(add_button, "clicked", G_CALLBACK(on_add_node_click), entry_value);
    gtk_widget_set_size_request(add_button, 200, -1);  // Set width to 200, height will remain unchanged
    gtk_widget_set_hexpand(add_button, TRUE); // Allow button to expand horizontally
    gtk_box_pack_start(GTK_BOX(vbox), add_button, FALSE, FALSE, 0);

    // Button to insert a node at a specific position
    GtkWidget *insert_at_pos_button = gtk_button_new_with_label("Insert at Position");
    GtkWidget *widgets[] = {entry_value, entry_pos};
    g_object_set_data(G_OBJECT(insert_at_pos_button), "list_data", &list);  // Set list data as user_data
    g_signal_connect(insert_at_pos_button, "clicked", G_CALLBACK(on_insert_at_pos_click), widgets);
    gtk_widget_set_size_request(insert_at_pos_button, 200, -1);  // Set width to 200, height will remain unchanged
    gtk_widget_set_hexpand(insert_at_pos_button, TRUE); // Allow button to expand horizontally
    gtk_box_pack_start(GTK_BOX(vbox), insert_at_pos_button, FALSE, FALSE, 0);

    // Button to delete a node from the linked list
    GtkWidget *delete_button = gtk_button_new_with_label("Delete Node");
    g_object_set_data(G_OBJECT(delete_button), "list_data", &list);  // Set list data as user_data
    g_signal_connect(delete_button, "clicked", G_CALLBACK(on_delete_node_click), entry_value);
    gtk_widget_set_size_request(delete_button, 200, -1);  // Set width to 200, height will remain unchanged
    gtk_widget_set_hexpand(delete_button, TRUE); // Allow button to expand horizontally
    gtk_box_pack_start(GTK_BOX(vbox), delete_button, FALSE, FALSE, 0);

    // Show all widgets
    gtk_widget_show_all(window);

    // Run the GTK main loop
    gtk_main();

    return 0;
}

