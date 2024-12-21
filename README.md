GTK install --

    sudo apt install libgtk-3-dev
CURL install --
    
    sudo apt-get install libcurl4-openssl-dev
JSON install --
    
    sudo apt-get install libjson-c-dev

GTK (GIMP Toolkit) is a free and open-source cross-platform toolkit for creating graphical user interfaces (GUIs). It is widely used for developing applications on Linux and supports other platforms like Windows and macOS. Here's an overview of GTK:
Key Features

    Cross-Platform:
        Applications built with GTK can run on multiple operating systems with little or no modification.

    Object-Oriented Design:
        GTK uses GObject, an object-oriented system in C, 
        enabling features like inheritance and polymorphism in C-based code.

    Rich Set of Widgets:
        Provides standard GUI elements like buttons, text entries, 
        labels, sliders, and advanced widgets like tree views and tables.

    Theming and Customization:
        Supports themes, allowing developers and users to change the look and feel of applications.

    High-Performance Rendering:
        Uses Cairo for rendering, providing anti-aliased graphics and modern drawing capabilities.

    Language Bindings:
        Supports multiple programming languages, including C, Python, C++, JavaScript, and more.

Core Concepts

    Widgets:
        The building blocks of GTK applications (e.g., GtkButton, GtkLabel, GtkEntry).
        Organized in a hierarchy to form the GUI layout.

    Signals:
        GTK uses an event-driven programming model.
        Signals are emitted when user actions or programmatic events occur 
        (e.g., a button click emits a "clicked" signal).

    Containers:
        Widgets are arranged using container widgets like GtkBox, GtkGrid, and GtkFixed.
        Containers manage the size and position of child widgets.

    Drawing:
        The GtkDrawingArea widget allows custom rendering using the Cairo graphics library.

GTK Workflow

    Initialization:
        Start by calling gtk_init() or gtk_application_new() to set up the toolkit.

    Widget Creation:
        Create widgets like windows, buttons, or text inputs.

    Layout:
        Arrange widgets in containers to define the interface structure.

    Signal Handling:
        Connect signals to callback functions to handle user interactions.

    Main Loop:
        Use gtk_main() or g_application_run() to start the main event loop.

Advantages

    Modern Look: GTK applications look polished and integrate well with desktop environments like GNOME.
    Community Support: Active development and a large community.
    Flexible Licensing: Licensed under LGPL, making it suitable for open-source and proprietary projects.
