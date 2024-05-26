#include <windows.h>
#include <GL/gl.h>
#include <string>

#define FONT_CENTERED_X 0x1
#define FONT_CENTERED_Y 0x2

class GLFont
{
private:
    HFONT hFont;
    int iHeight;
    int iAscent;
    GLuint base; // Display list base for font glyphs

public:
    GLFont(const std::string &fontName, int fontSize)
    {
        LOGFONT lf;
        memset(&lf, 0, sizeof(LOGFONT));
        lf.lfHeight = fontSize;
        lf.lfWeight = FW_NORMAL;
        lf.lfCharSet = DEFAULT_CHARSET;
        lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
        lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
        lf.lfQuality = DEFAULT_QUALITY;
        lf.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
        strcpy_s(lf.lfFaceName, fontName.c_str());

        hFont = CreateFontIndirect(&lf);
        if (hFont) {
            HDC hDC = GetDC(NULL);
            SelectObject(hDC, hFont);
            TEXTMETRIC tm;
            GetTextMetrics(hDC, &tm);
            iAscent = tm.tmAscent;
            iHeight = tm.tmHeight;
            ReleaseDC(NULL, hDC);
        }
    }

    ~GLFont()
    {
        if (hFont) {
            DeleteObject(hFont);
        }

        if (base) {
            glDeleteLists(base, 256);
        }
    }

    int GetFontHeight() const
    {
        return iHeight;
    }

    int GetFontAscent() const
    {
        return iAscent;
    }

    int GetTextWidth(const std::string &text) const
    {
        HDC hDC = GetDC(NULL);
        SelectObject(hDC, hFont);
        SIZE size;
        GetTextExtentPoint32(hDC, text.c_str(), static_cast<int>(text.length()), &size);
        ReleaseDC(NULL, hDC);
        return size.cx;
    }

    void RenderText(const std::string &text, int x, int y, BYTE r, BYTE g, BYTE b, BYTE a, DWORD flags)
    {
        // Save the current OpenGL states
        glPushAttrib(GL_ALL_ATTRIB_BITS);

        // Enable blending for transparency
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Disable lighting as it's not needed for text rendering
        glDisable(GL_LIGHTING);

        // Set text color
        glColor4ub(r, g, b, a);

        y += iAscent;

        if (flags & FONT_CENTERED_X) {
            int textWidth = GetTextWidth(text);
            x -= textWidth / 2;
        }
        if (flags & FONT_CENTERED_Y) {
            y -= iHeight / 2;
        }

        // Set the raster position
        glRasterPos2i(x, y);

        // Generate font bitmaps
        if (!base) {
            // Get the device context
            HDC hDC = wglGetCurrentDC();

            // Set the font
            SelectObject(hDC, hFont);

            base = glGenLists(256); // Assuming you're using ASCII characters
            wglUseFontBitmaps(hDC, 0, 256, base);
        }

        // Set the base list
        glListBase(base);

        // Iterate through each character in the text
        for (size_t i = 0; i < text.size(); ++i) {
            char c = text[i];
            if (c == '\n') {
                // Move to the next line
                x = flags & FONT_CENTERED_X ? -(GetTextWidth(text.substr(i + 1)) / 2) : x;
                y += iHeight; // Assuming iHeight is the height of a line
                glRasterPos2i(x, y);
            } else {
                // Render the character
                glCallLists(1, GL_UNSIGNED_BYTE, &c);
            }
        }

        // Restore the previous OpenGL states
        glPopAttrib();
    }
};
