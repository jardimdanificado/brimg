
#include <Wire.h>        // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`
#define BUZZER D3

#define SELECT D4

#define BOTAO_VERDE D5
#define BOTAO_VERMELHO D6
#define BOTAO_DIREITA D7
#define BOTAO_ESQUERDA D8

#define PROGS 2

#define byte unsigned char

#include "src/font.h"
SSD1306Wire display(0x3c, D1, D2); 

bool redraw = true;
byte cursor_x[PROGS];
byte cursor_y[PROGS];
byte *str[96];

byte mode = 0;
short global_delay = 100;

void setup()
{
    display.init();
    display.setContrast(255);

    display.flipScreenVertically();

    pinMode(BOTAO_VERDE, INPUT);
    pinMode(BOTAO_VERMELHO, INPUT);
    pinMode(BOTAO_DIREITA, INPUT);
    pinMode(BOTAO_ESQUERDA, INPUT);
    pinMode(SELECT, INPUT);
    pinMode(BUZZER, OUTPUT);

    display.setFont(Font6x8);
    for (byte i = 0; i < PROGS; i++)
    {
        str[i] = (byte *)malloc(96);
        for (byte j = 0; j < 96; j++)
        {
            
            str[i][j] = 255;
        }

        cursor_x[i] = 0;
        cursor_y[i] = 0;
    }

}

void drawFooter(char* text)
{
    display.setColor(WHITE); // alternate colors
    display.drawLine(0, 51, display.getWidth() - 1, 51);
    display.drawString(0, 53, text);
}

void drawBoard()
{
    char buffer[16];
    for (byte y = 0; y < 6; y++)
    {
        for (byte x = 0; x < 16; x++)
        {
            display.drawStringf((x * 8)+1, y * 8, buffer, "%c", str[mode][y * 16 + x]);
        }
    }
    display.setColor(WHITE); // alternate colors
    display.fillRect(cursor_x[mode]-1, cursor_y[mode]+1, 8, 8);
    display.setColor(BLACK); // alternate colors
    display.drawStringf(cursor_x[mode], cursor_y[mode], buffer, "%c", str[mode][cursor_y[mode] / 8 * 16 + cursor_x[mode] / 8]);        
}


bool moveCursor()
{
    if (digitalRead(BOTAO_ESQUERDA) == HIGH)
    {
        if (digitalRead(BOTAO_DIREITA) == HIGH)
        {
            if (digitalRead(BOTAO_VERDE) == HIGH)
            {
                if (cursor_y[mode] < 40)
                {
                    cursor_y[mode] += 8;
                }
                else if (cursor_y[mode] == 40)
                {
                    cursor_y[mode] = 0;
                }
                redraw = true;
                return true;
            }
            else if (digitalRead(BOTAO_VERMELHO) == HIGH)
            {
                if (cursor_y[mode] > 0)
                {
                    cursor_y[mode] -= 8;
                }
                else if (cursor_y[mode] == 0)
                {
                    cursor_y[mode] = 40;
                }
                redraw = true;
                return true;
            }
        }
        else if (digitalRead(BOTAO_VERDE) == HIGH)
        {
            if (cursor_x[mode] < 120)
            {
                cursor_x[mode] += 8;
            }
            else
            {
                if (cursor_y[mode] < 40)
                {
                    cursor_y[mode] += 8;
                }
                else if (cursor_y[mode] == 40)
                {
                    cursor_y[mode] = 0;
                }
                cursor_x[mode] = 0;
            }
            redraw = true;
            return true;
        }
        else if (digitalRead(BOTAO_VERMELHO) == HIGH)
        {
            if (cursor_x[mode] > 0)
            {
                cursor_x[mode] -= 8;
            }
            else
            {
                if (cursor_y[mode] > 0)
                {
                    cursor_y[mode] -= 8;
                }
                else if (cursor_y[mode] == 0)
                {
                    cursor_y[mode] = 40;
                }
                cursor_x[mode] = 120;
            }
            redraw = true;
            return true;
        }
    }
    return false;
}

bool progChange()
{
    if (digitalRead(SELECT) == HIGH)
    {
        if (digitalRead(BOTAO_VERDE) == HIGH)
        {
            if (mode < PROGS - 1)
            {
                mode++;
            }
            else
            {
                mode = 0;
            }
            redraw = true;
            return true;
        }
        else if (digitalRead(BOTAO_VERMELHO) == HIGH)
        {
            if (mode > 0)
            {
                mode--;
            }
            else
            {
                mode = PROGS - 1;
            }
            redraw = true;
            return true;
        }
    }
    return false;
}

void editProg()
{
    if (redraw)
    {
        display.clear();
        char buffer[16];
        drawBoard();
        sprintf(buffer, "edit %i", str[mode][cursor_y[mode] / 8 * 16 + cursor_x[mode] / 8]);
        drawFooter(buffer);
        display.display();
        redraw = false;
    }
    
    if (moveCursor()) {}
    else if (progChange()) {}
    else if (digitalRead(BOTAO_VERDE) == HIGH)
    {
        if (str[mode][cursor_y[mode] / 8 * 16 + cursor_x[mode] / 8] < 255)
            str[mode][cursor_y[mode] / 8 * 16 + cursor_x[mode] / 8]++;
        else
            str[mode][cursor_y[mode] / 8 * 16 + cursor_x[mode] / 8] = 0;
        redraw = true;
    }
    else if (digitalRead(BOTAO_VERMELHO) == HIGH)
    {
        if (str[mode][cursor_y[mode] / 8 * 16 + cursor_x[mode] / 8] > 0)
            str[mode][cursor_y[mode] / 8 * 16 + cursor_x[mode] / 8]--;
        else
            str[mode][cursor_y[mode] / 8 * 16 + cursor_x[mode] / 8] = 255;
        redraw = true;
    }
}


void cmdProg()
{
    if (redraw)
    {
        display.clear();
        char buffer[16];
        drawBoard();
        sprintf(buffer, "cmd %i", str[mode][cursor_y[mode] / 8 * 16 + cursor_x[mode] / 8]);
        drawFooter(buffer);
        display.display();
        redraw = false;
    }
    
    if (moveCursor()) {}
    else if (progChange()) {}
    else if (digitalRead(BOTAO_VERDE) == HIGH)
    {
        if (str[mode][cursor_y[mode] / 8 * 16 + cursor_x[mode] / 8] < 255)
            str[mode][cursor_y[mode] / 8 * 16 + cursor_x[mode] / 8]++;
        else
            str[mode][cursor_y[mode] / 8 * 16 + cursor_x[mode] / 8] = 0;
        redraw = true;
    }
    else if (digitalRead(BOTAO_VERMELHO) == HIGH)
    {
        if (str[mode][cursor_y[mode] / 8 * 16 + cursor_x[mode] / 8] > 0)
            str[mode][cursor_y[mode] / 8 * 16 + cursor_x[mode] / 8]--;
        else
            str[mode][cursor_y[mode] / 8 * 16 + cursor_x[mode] / 8] = 255;
        redraw = true;
    }

    display.clear();
}

// func pointer array
void (*programs[PROGS])() = 
{
    editProg, 
    cmdProg
};

void loop()
{
    programs[mode]();
    delay(global_delay);
}
