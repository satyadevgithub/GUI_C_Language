// gcc -o Linked_List_Student_Record Linked_List_Student_Record.c $(pkg-config --cflags --libs gtk+-3.0)

#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>

#define WINDOW_WIDTH 1000  // Set the initial window width
#define NODE_WIDTH 150
#define NODE_HEIGHT 100
#define SPACING 220

// Linked List Node structure
struct Node {
    char name[50];
    int id;
    char branch[50];
    char grade[5];
    int joining_year;
    struct Node* next;
};

// Structure to hold the linked list data and GTK elements
struct LinkedList {
    struct Node* head;
    GtkWidget* drawing_area;
    GtkWidget* scroll_window;
};

// Function to create a new node
struct Node* createNode(const char* name, int id, const char* branch, const char* grade, int joining_year) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    strncpy(newNode->name, name, sizeof(newNode->name) - 1);
    newNode->id = id;
    strncpy(newNode->branch, branch, sizeof(newNode->branch) - 1);
    strncpy(newNode->grade, grade, sizeof(newNode->grade) - 1);
    newNode->joining_year = joining_year;
    newNode->next = NULL;
    return newNode;
}

// Function to insert a node at a specific position in the linked list
void insertNodeAtPos(struct LinkedList* list, struct Node* newNode, int pos) {
    if (pos == 0) {
        newNode->next = list->head;
        list->head = newNode;
        return;
    }

    struct Node* temp = list->head;
    int currentPos = 0;

    while (temp != NULL && currentPos < pos - 1) {
        temp = temp->next;
        currentPos++;
    }

    if (temp == NULL) {
        free(newNode);  // Free the allocated memory if position is invalid
        return;
    }

    newNode->next = temp->next;
    temp->next = newNode;
}

// Function to delete a node by ID
void deleteNodeByID(struct LinkedList* list, int id) {
    struct Node* temp = list->head;
    struct Node* prev = NULL;

    if (temp != NULL && temp->id == id) {
        list->head = temp->next;
        free(temp);
        return;
    }

    while (temp != NULL && temp->id != id) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        return;
    }

    prev->next = temp->next;
    free(temp);
}

// Function to sort the linked list by ID
void sortLinkedListByID(struct LinkedList* list) {
    if (list->head == NULL || list->head->next == NULL) {
        return; // No need to sort if the list has 0 or 1 node
    }

    struct Node* i;
    struct Node* j;
    for (i = list->head; i != NULL; i = i->next) {
        for (j = i->next; j != NULL; j = j->next) {
            if (i->id > j->id) {
                // Swap the contents of the nodes
                struct Node temp = *i;
                *i = *j;
                *j = temp;

                // Restore the next pointers
                struct Node* tmpNext = i->next;
                i->next = j->next;
                j->next = tmpNext;
            }
        }
    }
}

// Function to draw the linked list visually
void draw_linked_list(GtkWidget* widget, cairo_t* cr, struct Node* head) {
    int x = 50;
    int y = 50;
    int total_width = 0;
    int student_count = 1;

    // Calculate total width of all nodes for horizontal scrolling
    struct Node* temp = head;
    while (temp != NULL) {
        total_width += SPACING;
        temp = temp->next;
    }

    // Set the width of the drawing area to fit all nodes
    gtk_widget_set_size_request(widget, total_width, 400);

    // Draw the nodes
    while (head != NULL) {
        cairo_rectangle(cr, x, y, NODE_WIDTH, NODE_HEIGHT);
        cairo_set_source_rgb(cr, 0.2, 0.6, 0.8); // Node color
        cairo_fill_preserve(cr);
        cairo_set_source_rgb(cr, 0, 0, 0); // Border color
        cairo_stroke(cr);
        
        // Display the heading (Student 1, Student 2, ...)
        cairo_set_source_rgb(cr, 1, 0, 0); // Text color
        cairo_move_to(cr, x + 10, y + 15);
        cairo_show_text(cr, g_strdup_printf("Student %d", student_count));
        student_count++; // Increment the count
        
        // Display node details
        cairo_set_source_rgb(cr, 1, 1, 1); // Text color
        cairo_move_to(cr, x + 10, y + 30);
        cairo_show_text(cr, g_strdup_printf("Name: %s", head->name));
        cairo_move_to(cr, x + 10, y + 45);
        cairo_show_text(cr, g_strdup_printf("ID: %d", head->id));
        cairo_move_to(cr, x + 10, y + 60);
        cairo_show_text(cr, g_strdup_printf("Branch: %s", head->branch));
        cairo_move_to(cr, x + 10, y + 75);
        cairo_show_text(cr, g_strdup_printf("Grade: %s", head->grade));
        cairo_move_to(cr, x + 10, y + 90);
        cairo_show_text(cr, g_strdup_printf("Joining Year: %d", head->joining_year));

        // Draw arrow to the next node
        if (head->next != NULL) {
            cairo_move_to(cr, x + NODE_WIDTH, y + NODE_HEIGHT / 2);
            cairo_line_to(cr, x + SPACING, y + NODE_HEIGHT / 2);
            cairo_stroke(cr);
        }

        x += SPACING;
        head = head->next;
    }
}

// GTK Draw event callback function
gboolean on_draw_event(GtkWidget* widget, cairo_t* cr, gpointer user_data) {
    struct LinkedList* list = (struct LinkedList*)user_data;
    draw_linked_list(widget, cr, list->head);
    return FALSE;
}

// Function to handle "Add Node" button click
void on_add_node_click(GtkWidget *widget, gpointer user_data) {
    GtkWidget** entries = (GtkWidget**)user_data;

    const gchar* name = gtk_entry_get_text(GTK_ENTRY(entries[0]));
    int id = atoi(gtk_entry_get_text(GTK_ENTRY(entries[1])));
    const gchar* branch = gtk_entry_get_text(GTK_ENTRY(entries[2]));
    const gchar* grade = gtk_entry_get_text(GTK_ENTRY(entries[3]));
    int year = atoi(gtk_entry_get_text(GTK_ENTRY(entries[4])));

    if (id > 0 && year > 0) {  // Validate inputs
        struct LinkedList* list = g_object_get_data(G_OBJECT(widget), "list_data");
        struct Node* newNode = createNode(name, id, branch, grade, year);
        insertNodeAtPos(list, newNode, 0); // Add at the head
        gtk_widget_queue_draw(list->drawing_area);
    }
}

// Function to handle "Delete Node" button click
void on_delete_node_click(GtkWidget *widget, gpointer user_data) {
    GtkEntry* entry = (GtkEntry*)user_data;
    int id = atoi(gtk_entry_get_text(entry));

    if (id > 0) {
        struct LinkedList* list = g_object_get_data(G_OBJECT(widget), "list_data");
        deleteNodeByID(list, id);
        gtk_widget_queue_draw(list->drawing_area);
    }
}

// Function to handle "Sort List" button click
void on_sort_list_click(GtkWidget *widget, gpointer user_data) {
    struct LinkedList* list = g_object_get_data(G_OBJECT(widget), "list_data");
    sortLinkedListByID(list);
    gtk_widget_queue_draw(list->drawing_area);
}

// Main function to create the GUI
int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Linked List Student Record");
    gtk_window_set_default_size(GTK_WINDOW(window), WINDOW_WIDTH, 600);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    struct LinkedList list;
    list.head = NULL;

    list.drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(list.drawing_area, WINDOW_WIDTH, 400);

    list.scroll_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(list.scroll_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start(GTK_BOX(vbox), list.scroll_window, TRUE, TRUE, 0);
    gtk_container_add(GTK_CONTAINER(list.scroll_window), list.drawing_area);

    g_signal_connect(list.drawing_area, "draw", G_CALLBACK(on_draw_event), &list);

    GtkWidget *entry_name = gtk_entry_new();
    GtkWidget *entry_id = gtk_entry_new();
    GtkWidget *entry_branch = gtk_entry_new();
    GtkWidget *entry_grade = gtk_entry_new();
    GtkWidget *entry_year = gtk_entry_new();
    
    GtkWidget *add_button = gtk_button_new_with_label("Add Node");
    GtkWidget *delete_button = gtk_button_new_with_label("Delete Node");
    GtkWidget *sort_button = gtk_button_new_with_label("Sort List");

    GtkWidget* entries[] = {entry_name, entry_id, entry_branch, entry_grade, entry_year};

    GtkWidget *label_name = gtk_label_new("Name:");
    GtkWidget *label_id = gtk_label_new("ID:");
    GtkWidget *label_branch = gtk_label_new("Branch:");
    GtkWidget *label_grade = gtk_label_new("Grade:");
    GtkWidget *label_year = gtk_label_new("Joining Year:");

    GtkWidget *hbox_form = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(hbox_form), label_name, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(hbox_form), entry_name, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(hbox_form), label_id, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(hbox_form), entry_id, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(hbox_form), label_branch, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(hbox_form), entry_branch, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(hbox_form), label_grade, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(hbox_form), entry_grade, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(hbox_form), label_year, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(hbox_form), entry_year, FALSE, FALSE, 5);

    gtk_box_pack_start(GTK_BOX(vbox), hbox_form, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), add_button, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), delete_button, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), sort_button, FALSE, FALSE, 5);

    g_object_set_data(G_OBJECT(add_button), "list_data", &list);
    g_signal_connect(add_button, "clicked", G_CALLBACK(on_add_node_click), entries);

    g_object_set_data(G_OBJECT(delete_button), "list_data", &list);
    g_signal_connect(delete_button, "clicked", G_CALLBACK(on_delete_node_click), entry_id);

    g_object_set_data(G_OBJECT(sort_button), "list_data", &list);
    g_signal_connect(sort_button, "clicked", G_CALLBACK(on_sort_list_click), NULL);

    gtk_widget_show_all(window);

    gtk_main();
    
    return 0;
}

