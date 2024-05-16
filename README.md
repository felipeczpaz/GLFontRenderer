# GLFontRenderer

GLFontRenderer is a C++ class for rendering text in OpenGL applications on Windows platforms. It provides functionalities to create and render text using TrueType fonts with ease in OpenGL contexts.

## Features

- Render text using TrueType fonts in OpenGL applications.
- Set text color, transparency, and alignment.
- Get text width and height.

## Usage

1. Include `GLFont.h` in your project.
2. Create an instance of `GLFont` by providing the font name and font size.
3. Use `RenderText` function to render text at specified coordinates with optional settings like color, transparency, and alignment.

## Example

```cpp
#include <windows.h>
#include <GL/gl.h>
#include <GLFont.h>

int main() {
    // Initialize OpenGL context and window

    GLFont font("Arial", 24); // Create a font instance

    // Render text
    font.RenderText("Hello, World!", 100, 100, 255, 255, 255, 255, FONT_CENTERED_X | FONT_CENTERED_Y);

    // Main rendering loop

    return 0;
}
```

## License

This project is licensed under the MIT License - see the LICENSE file for details.
