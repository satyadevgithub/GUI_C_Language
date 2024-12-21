// gcc chatgpt_api_bot.c -o chatgpt_api_bot `pkg-config --cflags --libs gtk+-3.0` -lcurl -ljson-c

#include <gtk/gtk.h>
#include <curl/curl.h>
#include <json-c/json.h>
#include <stdlib.h>
#include <string.h>

// Struct for storing API response
typedef struct {
    char *data;
    size_t size;
} Memory;

// Callback function to handle API response data
size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t total_size = size * nmemb;
    Memory *mem = (Memory *)userp;

    char *ptr = realloc(mem->data, mem->size + total_size + 1);
    if (ptr == NULL) return 0; // Out of memory

    mem->data = ptr;
    memcpy(&(mem->data[mem->size]), contents, total_size);
    mem->size += total_size;
    mem->data[mem->size] = 0;

    return total_size;
}

// Function to query ChatGPT API
char *query_chatgpt(const char *question) {
    CURL *curl;
    CURLcode res;
    Memory chunk = {NULL, 0};

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        // OpenAI API endpoint
        const char *url = "https://api.openai.com/v1/chat/completions";

        // OpenAI API key (replace with your actual key)
        const char *api_key = "OpenAI API key";

        // JSON payload
        char post_data[1024];
        snprintf(post_data, sizeof(post_data),
                 "{\"model\": \"gpt-3.5-turbo\", \"messages\": [{\"role\": \"user\", \"content\": \"%s\"}]}", question);

        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        // Set the authorization header with your API key
        char auth_header[256];
        snprintf(auth_header, sizeof(auth_header), "Authorization: Bearer %s", api_key);
        headers = curl_slist_append(headers, auth_header);

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &chunk);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }

    curl_global_cleanup();

    return chunk.data;
}

// Callback function for the button click event
void on_submit_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *input_entry = GTK_WIDGET(g_object_get_data(G_OBJECT(button), "input_entry"));
    GtkWidget *output_label = GTK_WIDGET(g_object_get_data(G_OBJECT(button), "output_label"));

    const gchar *question = gtk_entry_get_text(GTK_ENTRY(input_entry));

    if (g_strcmp0(question, "") == 0) {
        gtk_label_set_text(GTK_LABEL(output_label), "Please enter a question!");
        return;
    }

    // Query ChatGPT and get the response
    char *response = query_chatgpt(question);
    if (response) {
        // Debugging: Print the raw response
        printf("Response: %s\n", response);

        // Parse the JSON response to extract the reply
        struct json_object *parsed_json;
        struct json_object *choices_array;
        struct json_object *choice_object;
        struct json_object *message_object;
        struct json_object *content_object;

        parsed_json = json_tokener_parse(response);
        if (parsed_json == NULL) {
            gtk_label_set_text(GTK_LABEL(output_label), "Failed to parse JSON.");
            free(response);
            return;
        }

        if (json_object_object_get_ex(parsed_json, "choices", &choices_array)) {
            choice_object = json_object_array_get_idx(choices_array, 0);
            if (json_object_object_get_ex(choice_object, "message", &message_object)) {
                if (json_object_object_get_ex(message_object, "content", &content_object)) {
                    const char *answer = json_object_get_string(content_object);
                    gtk_label_set_text(GTK_LABEL(output_label), answer);
                } else {
                    gtk_label_set_text(GTK_LABEL(output_label), "Error: No content found in message.");
                }
            } else {
                gtk_label_set_text(GTK_LABEL(output_label), "Error: No message found.");
            }
        } else {
            gtk_label_set_text(GTK_LABEL(output_label), "Error: No choices found.");
        }

        free(response);
    } else {
        gtk_label_set_text(GTK_LABEL(output_label), "Failed to fetch response. Check your internet connection or API key.");
    }
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    // Create the main window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "ChatGPT Question Answer");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 200);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    // Create a vertical box layout
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Create an entry for user input
    GtkWidget *input_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(input_entry), "Type your question here...");
    gtk_box_pack_start(GTK_BOX(vbox), input_entry, FALSE, FALSE, 0);

    // Create a button to submit the question
    GtkWidget *submit_button = gtk_button_new_with_label("Ask ChatGPT");
    gtk_box_pack_start(GTK_BOX(vbox), submit_button, FALSE, FALSE, 0);

    // Create a label to display the output
    GtkWidget *output_label = gtk_label_new("");
    gtk_label_set_xalign(GTK_LABEL(output_label), 0.0); // Align left
    gtk_box_pack_start(GTK_BOX(vbox), output_label, FALSE, FALSE, 0);

    // Connect the submit button click event to the callback
    g_object_set_data(G_OBJECT(submit_button), "input_entry", input_entry);
    g_object_set_data(G_OBJECT(submit_button), "output_label", output_label);
    g_signal_connect(submit_button, "clicked", G_CALLBACK(on_submit_clicked), NULL);

    // Connect the window destroy signal
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Show all widgets
    gtk_widget_show_all(window);

    // Run the GTK main loop
    gtk_main();

    return 0;
}

