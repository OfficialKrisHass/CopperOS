#include "Print.h"

#include "Drivers/Graphics.h"
#include "Drivers/Ports.h"

#define OFFSET(col, row) 2 * (row * COLS + col)
#define OFFSET_ROW(offset) offset / (2 * COLS)
#define OFFSET_COL(offset) (offset - (OFFSET_ROW(offset) * 2 * COLS)) / 2

unsigned int PrintChar(char c, int col, int row, char attribute);

unsigned int GetCursorOffset();
void SetCursorOffset(unsigned int offset);

void Print(const char* msg, int col, int row, unsigned char color) {

    unsigned int offset;

    if(col > -1 && row > -1)
        offset = OFFSET(col, row);
    else {

        offset = GetCursorOffset();
        row = OFFSET_ROW(offset);
        col = OFFSET_COL(offset);

    }

    unsigned int i = 0;
    while (msg[i]) {

        offset = PrintChar(msg[i++], col, row, color);
        row = OFFSET_ROW(offset);
        col = OFFSET_COL(offset);

    }

}

unsigned int PrintChar(char c, int col, int row, char attribute) {

    unsigned char* vm = (unsigned char*) VIDEO_MEMORY;
    if (!attribute) attribute = WHITE_ON_BLACK;

    if (col >= COLS + 1 || row > ROWS + 1) {

        Print("Error!", COLS - 6, ROWS - 1, RED_ON_WHITE);
        return OFFSET(col, row);

    }

    unsigned int offset;
    if(col > -1 && row > - 1)
        offset = OFFSET(col, row);
    else
        offset = GetCursorOffset();
    
    if(c == '\n') {

        row = OFFSET_ROW(offset);
        offset = OFFSET(0, row + 1);

    } else {
        
        vm[offset] = c;
        vm[offset + 1] = attribute;
        offset += 2;

    }

    SetCursorOffset(offset);
    return offset;

}

unsigned int GetCursorOffset() {

    PortByteOut(CURSOR_CTRL, 14);
    unsigned int offset = PortByteIn(CURSOR_DATA) << 8;

    PortByteOut(CURSOR_CTRL, 15);
    offset += PortByteIn(CURSOR_DATA);

    return offset;

}
void SetCursorOffset(unsigned int offset) {

    offset /= 2;

    PortByteOut(CURSOR_CTRL, 14);
    PortByteOut(CURSOR_DATA, (unsigned char) (offset >> 8));

    PortByteOut(CURSOR_CTRL, 15);
    PortByteOut(CURSOR_DATA, (unsigned char) (offset & 0xff));

}