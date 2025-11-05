# Generating Documentation

This library uses [Doxygen](http://www.doxygen.nl/) to generate API documentation from the source code comments.

## Prerequisites

Install Doxygen on your system:

- **Windows**: Download from [Doxygen Downloads](http://www.doxygen.nl/download.html) or use `choco install doxygen`
- **macOS**: `brew install doxygen`
- **Linux**: `sudo apt-get install doxygen` (Debian/Ubuntu) or `sudo yum install doxygen` (RedHat/CentOS)

## Generating Documentation

1. Open a terminal/command prompt in the library root directory (`MAX7219_CH32/`)

2. Run Doxygen:
   ```bash
   doxygen Doxyfile
   ```

3. The documentation will be generated in the `docs/html/` directory

4. Open `docs/html/index.html` in your web browser to view the documentation

## Documentation Features

The generated documentation includes:

- **API Reference**: Complete documentation of all classes, methods, and constants
- **Class Hierarchy**: Visual representation of class relationships
- **File List**: Overview of all source files
- **Examples**: Included example code from the `examples/` folder
- **Search Functionality**: Searchable index of all documented elements

## Customization

You can customize the documentation by editing the `Doxyfile`. Key settings include:

- `PROJECT_NAME`: Library name
- `PROJECT_NUMBER`: Version number
- `OUTPUT_DIRECTORY`: Where documentation is generated (default: `docs`)
- `HAVE_DOT`: Enable/disable graph generation (requires Graphviz)
- `GENERATE_LATEX`: Enable LaTeX output

## Adding Documentation

To add or update documentation:

1. Use Doxygen-style comments in the source code:
   ```cpp
   /**
    * @brief Brief description
    * @param paramName Parameter description
    * @return Return value description
    */
   ```

2. Regenerate the documentation using `doxygen Doxyfile`

## Online Documentation

For online hosting, you can:
- Upload the `docs/html/` folder to GitHub Pages
- Use services like Read the Docs
- Host on your own web server

