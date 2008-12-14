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
 * port.h - specialized ports for the psp version.                             *
 ******************************************************************************/

#ifndef DOXYGEN_SKIP
#ifndef _CONSOLE_PORT_H_
#define _CONSOLE_PORT_H_

extern "C" {
#include <pspctrl.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspkernel.h>
}

#include <map>
#include <string>

namespace RPN
{
    class Port
    {
        //! Holds information on how a certain button press on the PSP maps to
        //! characters. The alternate form is accessed by pressing the
        //! right trigger.
        struct CharPair
        {
            char chr, alt_chr;

            CharPair(char chr = '\0', char alt_chr = '\0')
                : chr(chr), alt_chr(alt_chr) {}
        };

        // Executed on exit; used for cleanup.
        static int ExitCallback(int arg1, int arg2, void *common);
        // Thread to create and register callbacks.
        static int CallbackThread(SceSize args, void *argp);
        // Starts thread to create callbacks.
        static int SetupCallbacks();

        //! Gets button pushes.
        /**
         * This reports multiple button pushes, so this does not return until
         * all buttons have been released.
         *
         * @return An integer with button press flags.
         */
        static int GetButtonPushes();

        // Returns a key that matches the given buttons.
        // Simple but fast on a small map.
        static CharPair* FindPair(int buttons);

        // 64 characters is a reasonable limit for the buffer.
        const static size_t buf_size = 64;
        // To press "enter", press the triggers simultaneously.
        static int enter;
        // To cancel input, press the left trigger.
        static int cancel;
        // To switch to alternate characters, press the right trigger.
        static int alternate;
        // Don't start in alternate mode.
        static bool alternateMode;
        // Holds a map of button presses to characters.
        static std::map<int, CharPair> keyMap;

        //! Get one character; map special characters so they can be handled
        //! later.
        /**
         * @return The typed character or 0 if invalid.
         */
        static char GetCharacter();

    public:

        static bool CanRun()
        {
            return true;
        }

        static std::string GetLine();

        static void Post()
        {
            sceKernelExitGame();
        }

        static void Print(const char* str)
        {
            pspDebugScreenPrintf(str);
        }

        static void Setup();
    };
}

#endif
#endif
