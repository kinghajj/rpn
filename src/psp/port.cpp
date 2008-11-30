/*******************************************************************************
 * Reverse Polish Notation calculator.                                         *
 * Copyright (c) 2007-2008, Samuel Fredrickson <kinghajj@gmail.com>            *
 * All rights reserved.                                                        *
 *                                                                             *
 * Redistribution and use in source and binary forms, with or without          *
 * modification, are permitted provided that the following conditions are met: *
 *     * Redistributions of source code must retain the above copyright        *
 *       notice, this list of conditions and the following disclaimer.         *
 *     * Redistributions in binary form must reproduce the above copyright     *
 *       notice, this list of conditions and the following disclaimer in the   *
 *       documentation and/or other materials provided with the distribution.  *
 *                                                                             *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER ``AS IS'' AND ANY EXPRESS *
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED           *
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE      *
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY        *
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES  *
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR          *
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER  *
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT          *
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY   *
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH *
 * DAMAGE.                                                                     *
 ******************************************************************************/

/*******************************************************************************
 * port.cpp - specialized ports for the psp version.                           *
 ******************************************************************************/

#include "../rpn.h"
#include "port.h"
using namespace RPN;
using namespace std;

#define kprintf pspDebugScreenPrintf

map<int, Port::CharPair> Port::keyMap;
int  Port::enter         = PSP_CTRL_LTRIGGER | PSP_CTRL_RTRIGGER;
int  Port::cancel        = PSP_CTRL_LTRIGGER;
int  Port::alternate     = PSP_CTRL_RTRIGGER;
bool Port::alternateMode = false;

int Port::ExitCallback(int arg1, int arg2, void *common)
{
    sceKernelExitGame();
    return 0;
}

int Port::CallbackThread(SceSize args, void *argp)
{
    int cbid;

    cbid = sceKernelCreateCallback("Exit Callback", ExitCallback, NULL);
    sceKernelRegisterExitCallback(cbid);
    sceKernelSleepThreadCB();

    return 0;
}

int Port::SetupCallbacks()
{
    int thid;

    thid = sceKernelCreateThread("update_thread", CallbackThread, 0x11,
                                 0xFA0, 0, 0);
    if(thid >= 0)
        sceKernelStartThread(thid, 0, 0);
    
    return thid;
}

Port::CharPair* Port::FindPair(int buttons)
{
    CharPair* pair = NULL;
    std::map<int, CharPair>::iterator it = keyMap.find(buttons);
    if(it != keyMap.end())
        pair = &it->second;
    return pair;
}

int Port::GetButtonPushes()
{
    int buttons;
    SceCtrlData pad;

    // Wait for a button press.
    pad.Buttons = 0;
    while(!pad.Buttons)
        sceCtrlReadBufferPositive(&pad, 1);

    // store the buttons.
    buttons = pad.Buttons;

    // wait for all buttons to come up, store them.
    while(pad.Buttons)
    {
        sceCtrlReadBufferPositive(&pad, 1);
        buttons |= pad.Buttons;
    }

    return buttons;
}

char Port::GetCharacter()
{
    int buttons = GetButtonPushes();
    CharPair *pair = FindPair(buttons);

    // Was a valid combination pressed? The print the character,
    // then return it.
    if(pair)
    {
        kprintf("%c", alternateMode ? pair->alt_chr : pair->chr);
        return alternateMode ? pair->alt_chr : pair->chr;
    }
    // Enter?
    else if(buttons == enter)
        return '\n';
    // Cancel?
    else if(buttons == cancel)
        return '\b';
    // Switch to alternate?
    else if(buttons == alternate)
        alternateMode = alternateMode ? false : true;
    // What happened?
    return 0;
}

string Port::GetLine()
{
    size_t i;
    char c = '\0';
    bool end = false;
    string ret(buf_size, ' ');

    // Go until the buffer is filled or a newline is returned.
    for(i = 0; i < buf_size && c != '\n' && !end; ++i)
    {
        c = GetCharacter();
        // Backspace?
        if(c == '\b')
        {
            ret = "";
            end = true;
        }
        // Real character?
        else if(c)
            ret[i] = c;
        // No; stay back.
        else
            i--;
    }

    kprintf("\n");
    return ret;
}

void Port::Setup()
{
    pspDebugScreenInit();
    SetupCallbacks();

    // Numbers
    keyMap[PSP_CTRL_CROSS]                  = CharPair('0', '0');
    keyMap[PSP_CTRL_CROSS | PSP_CTRL_UP]    = CharPair('1', '1');
    keyMap[PSP_CTRL_CROSS | PSP_CTRL_RIGHT] = CharPair('2', '2');
    keyMap[PSP_CTRL_CROSS | PSP_CTRL_DOWN]  = CharPair('3', '3');
    keyMap[PSP_CTRL_CROSS | PSP_CTRL_LEFT]  = CharPair('4', '4');

    keyMap[PSP_CTRL_CIRCLE]                  = CharPair('5', '5');
    keyMap[PSP_CTRL_CIRCLE | PSP_CTRL_UP]    = CharPair('6', '6');
    keyMap[PSP_CTRL_CIRCLE | PSP_CTRL_RIGHT] = CharPair('7', '7');
    keyMap[PSP_CTRL_CIRCLE | PSP_CTRL_DOWN]  = CharPair('8', '8');
    keyMap[PSP_CTRL_CIRCLE | PSP_CTRL_LEFT]  = CharPair('9', '9');

    // Special characters
    keyMap[PSP_CTRL_SQUARE]                  = CharPair('.', '=');
    keyMap[PSP_CTRL_SQUARE | PSP_CTRL_UP]    = CharPair( '+', '+');
    keyMap[PSP_CTRL_SQUARE | PSP_CTRL_RIGHT] = CharPair('-', '-');
    keyMap[PSP_CTRL_SQUARE | PSP_CTRL_DOWN]  = CharPair('*', '*');
    keyMap[PSP_CTRL_SQUARE | PSP_CTRL_LEFT]  = CharPair('/', '/');

    keyMap[PSP_CTRL_TRIANGLE]                  = CharPair(' ', ' ');
    keyMap[PSP_CTRL_TRIANGLE | PSP_CTRL_UP]    = CharPair('%', '%');
    keyMap[PSP_CTRL_TRIANGLE | PSP_CTRL_RIGHT] = CharPair('^', '^');
    keyMap[PSP_CTRL_TRIANGLE | PSP_CTRL_DOWN]  = CharPair('&', '&');
    keyMap[PSP_CTRL_TRIANGLE | PSP_CTRL_LEFT]  = CharPair('|', '|');

    // The alphabet
    keyMap[PSP_CTRL_LTRIGGER | PSP_CTRL_CROSS]
        = CharPair('a', 'A');
    keyMap[PSP_CTRL_LTRIGGER | PSP_CTRL_CROSS | PSP_CTRL_UP]
        = CharPair('b', 'B');
    keyMap[PSP_CTRL_LTRIGGER | PSP_CTRL_CROSS | PSP_CTRL_RIGHT]
        = CharPair('c', 'C');
    keyMap[PSP_CTRL_LTRIGGER | PSP_CTRL_CROSS | PSP_CTRL_DOWN]
        = CharPair('d', 'D');
    keyMap[PSP_CTRL_LTRIGGER | PSP_CTRL_CROSS | PSP_CTRL_LEFT]
        = CharPair('e', 'E');

    keyMap[PSP_CTRL_LTRIGGER | PSP_CTRL_CIRCLE]
        = CharPair('f', 'F');
    keyMap[PSP_CTRL_LTRIGGER | PSP_CTRL_CIRCLE | PSP_CTRL_UP]
        = CharPair('g', 'G');
    keyMap[PSP_CTRL_LTRIGGER | PSP_CTRL_CIRCLE | PSP_CTRL_RIGHT]
        = CharPair('h', 'H');
    keyMap[PSP_CTRL_LTRIGGER | PSP_CTRL_CIRCLE | PSP_CTRL_DOWN]
        = CharPair('i', 'I');
    keyMap[PSP_CTRL_LTRIGGER | PSP_CTRL_CIRCLE | PSP_CTRL_LEFT]
        = CharPair('j', 'J');

    keyMap[PSP_CTRL_LTRIGGER | PSP_CTRL_SQUARE]
        = CharPair('k', 'K');
    keyMap[PSP_CTRL_LTRIGGER | PSP_CTRL_SQUARE | PSP_CTRL_UP]
        = CharPair('l', 'L');
    keyMap[PSP_CTRL_LTRIGGER | PSP_CTRL_SQUARE | PSP_CTRL_RIGHT]
        = CharPair('m', 'M');
    keyMap[PSP_CTRL_LTRIGGER | PSP_CTRL_SQUARE | PSP_CTRL_DOWN]
        = CharPair('n', 'N');
    keyMap[PSP_CTRL_LTRIGGER | PSP_CTRL_SQUARE | PSP_CTRL_LEFT]
        = CharPair('o', 'O');

    keyMap[PSP_CTRL_LTRIGGER | PSP_CTRL_TRIANGLE]
        = CharPair('p', 'P');
    keyMap[PSP_CTRL_LTRIGGER | PSP_CTRL_TRIANGLE | PSP_CTRL_UP]
        = CharPair('q', 'Q');
    keyMap[PSP_CTRL_LTRIGGER | PSP_CTRL_TRIANGLE | PSP_CTRL_RIGHT]
        = CharPair('r', 'R');
    keyMap[PSP_CTRL_LTRIGGER | PSP_CTRL_TRIANGLE | PSP_CTRL_DOWN]
        = CharPair('s', 'S');
    keyMap[PSP_CTRL_LTRIGGER | PSP_CTRL_TRIANGLE | PSP_CTRL_LEFT]
        = CharPair('t', 'T');

    keyMap[PSP_CTRL_LTRIGGER | PSP_CTRL_RTRIGGER | PSP_CTRL_CROSS]
        = CharPair('u', 'U');
    keyMap[PSP_CTRL_LTRIGGER | PSP_CTRL_RTRIGGER | PSP_CTRL_CROSS |
           PSP_CTRL_UP] = CharPair('v', 'V');
    keyMap[PSP_CTRL_LTRIGGER | PSP_CTRL_RTRIGGER | PSP_CTRL_CROSS |
           PSP_CTRL_RIGHT] = CharPair('w', 'W');
    keyMap[PSP_CTRL_LTRIGGER | PSP_CTRL_RTRIGGER | PSP_CTRL_CROSS |
           PSP_CTRL_DOWN] = CharPair('x', 'X');
    keyMap[PSP_CTRL_LTRIGGER | PSP_CTRL_RTRIGGER | PSP_CTRL_CROSS |
           PSP_CTRL_LEFT] = CharPair('y', 'Y');

    keyMap[PSP_CTRL_LTRIGGER | PSP_CTRL_RTRIGGER | PSP_CTRL_CIRCLE]
            = CharPair('z', 'Z');
}
