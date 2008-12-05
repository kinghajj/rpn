/** \mainpage RPN Documentation
 *
 * \section intro Introduction
 *
 * RPN is an easy-to-use Reverse Polish Notation calculator with a fair amount
 * of useful features. It started out as a sort of pet project, but has become
 * somewhat serious to me.
 *
 * At 1.0, this was a fully-function plain calculator, with not many bells and
 * whistles. In 1.1, I added variables, and in 1.2 I ported this to the
 * PlayStation portable. In  1.3, I ported this to the Nintendo Wii, and fixed
 * some potential bugs with Valgrind.
 *
 * In the current version, 2.0.0.0-alpha2, I've re-written the program in C++.
 * The source code is now less than half of what the C code was, though the
 * compiled executable is larger. This alpha release has the same PSP and Wii
 * ports, though I've not tested them well; however, I don't expect any problems
 * with them. This alpha implements all of the old commands except "help", and
 * all of the old arguments except "--help" and "-h".
 *
 * \section features Features
 *
 * Like any calculator, RPN can calculate, so it supports many operators:
 * addition, subtraction, multiplication, division, exponentiation, bitwise
 * operations, etc. Unlike some, however, RPN has support for commands and
 * variables.
 *
 * Commands are like operators in implementation, but are separate because they
 * affect the calculator as a whole--it's stack, variables, etc. Operators only
 * affect the stack. This limitation is part of the design of the program. A
 * downside to this is that operators can only have two operands. This isn't
 * much of a problem yet, but it may be later if one wants to add a ternary
 * operator. For now, anything that can't be implemented as an operator should
 * be implemented as a command.
 *
 * Variables let you save the results of your expressions and keep shorthands
 * for commonly used numbers. There are some predefined variables like PI, E,
 * and kilobyte/kibibyte/etc. All predefined variables begin with an uppercase
 * letter, but there are currently no limitations for a variable name except
 * 1) it must have at least one non-numerical character, and 2) it cannot be the
 * same as a command or operator. This gives you a lot of freedom to choose
 * whatever name you want.
 *
 * \section about About the Program
 *
 * By default, the calculator uses "long doubles" to store values. On my 64-bit
 * Pentium D processor, this is 128 bits wide, and can hold up to about 2 **
 * 16383 before reaching "infinity"--a 4932 digit number. So, though this is not
 * an arbitrary precision calculator, it should be good enough for most people.
 * If you compile with the flag RPN_DOUBLE, however, then "doubles" will be used
 * instead.
 *
 * I'm not quite sure how portable this program is. It compiles on Ubuntu Linux,
 * so it will likely compile on any GNU/Linux system with the right libraries. I
 * believe that all the functions I use are POSIX, so hopefully this program can
 * be ported easily to just about anything. The PSP port wasn't too difficult,
 * though, and that required creating a custom input system!
 *
 * \section comp Compiling
 *
 * Hopefully, compiling can be as simple as "make; make install". However, you
 * may need to make changes to the Makefile, and may need to type, for instance,
 * "sudo make install" instead.
 *
 * Compiling for the PSP should not present any difficulties if you already have
 * a PSP SDK and toolchain installed. If not, you can find a great tutorial on
 * the Gentoo wiki. I used it to install the toolchain and SDK on Ubuntu 7.10
 * without problem. See \ref psp-spec for more information about the PSP port.
 *
 * Likewise, compiling for the Wii should be simple if you already have a
 * properly setup DevkitPPC toolchain; but that's easier said than done. See
 * \ref wii-spec for more information abouth the Wii port.
 *
 * \section using Using the Program
 *
 * When you first run the program, you're presented with a simple prompt:
 *
 * \code
 * [0]>
 * \endcode
 *
 * The number in barckets is the topmost item of the stack. To push a number to
 * it, just type it in and press enter.
 *
 * \code
 * [0]> PI
 * [3.14159]>
 * \endcode
 *
 * Great! But this is a calculator--how do you calculate? If you don't know what
 * Reverse Polish Notation is, look it up on Wikipedia. Done yet? Good. If you
 * want to multiply by five, type this.
 *
 * \code
 * [3.14159]> 5 *
 * [15.708]>
 * \endcode
 *
 * You can even do more than one operation per line.
 *
 * \code
 * [15.708]> 3 / 2 -
 * [3.23599]>
 * \endcode
 *
 * But what if you want to see the whole stack?
 *
 * \code
 * [3.23599] 2 1
 * [1]> ps
 * [ 1, 2, 3.23599, ]
 * [1]>
 * \endcode
 *
 * How about removing the top item of the stack?
 *
 * \code
 * [1]> pop
 * [2]>
 * \endcode
 *
 * How about duplicating the top item of the stack?
 *
 * \code
 * [2]> dup ps
 * [2, 2, 3.23599, ]
 * \endcode
 *
 * Can you print the stack in more detail?
 *
 * \code
 * [2]> psd
 * [ 2.000000, 2.000000, 3.235988, ]
 * \endcode
 *
 * \section thanks Thanks
 *
 * Much thanks to Troy Hanson for uthash. It made the program much better, and
 * makes implementing hash tables wonderfully simple.
 *
 * The PSP port would not have been possible without the tutorials at
 * psp-programming.com. The callback functions are almost carbon-copied form
 * them.
 *
 * The Wii port would not have been possible without the help of wiibrew.org.
 */

/**
 * \page psp-spec Specific Information About the PSP Port
 *
 * In the current version, the PSP port is very, very simple. It works, and you
 * can actually type, though not easily. It is basically a full port, though the
 * error module is simplified. The highest calculatable value, I believe, is
 * 2 ** 1023.
 *
 * Because not many people have a PSP toolchain on hand, I've included a
 * pre-built EBOOT.PBP. Though compiled for firmware 1.50, I have successfully
 * run it under 4.00 M33-2. To install it, simply copy EBOOT.PBP to
 * ms0:/PSP/GAME/PSPRPN .
 *
 * \section psp-type How to Type on the PSP
 *
 * Typing numbers is a combination of cross, circle, and a directional button.
 * Pressing cross alone is 0; pressing cross and up simultaneously is 1; cross
 * and right 2; cross and down 3; and cross and left 4. This direction
 * order--up, right, down, and left--is consistent thoughout all typing modes.
 *
 * There is also an alternate mod, accessed by pressing the right trigger once;
 * think of it as a caps lock. Not all button combinations have alternate
 * characters, however, so they do not change.
 *
 * If you make a mistake, there is no way (currently) to undo it. If you press
 * the left trigger alone, however, any input you've entered will be discarded.
 *
 * Here is a table of typable characters.
 *
 * <TABLE>
 *   <TR>
 *     <TD><STRONG>Buttons</STRONG></TD>
 *     <TD><STRONG>Normal Character</STRONG></TD>
 *     <TD><STRONG>Alternate Character</STRONG></TD>
 *   </TR>
 *   <TR>
 *     <TD>X</TD>
 *     <TD>0</TD>
 *     <TD>0</TD>
 *   </TR>
 *   <TR>
 *     <TD>X Up</TD>
 *     <TD>1</TD>
 *     <TD>1</TD>
 *   </TR>
 *   <TR>
 *     <TD>X Right</TD>
 *     <TD>2</TD>
 *     <TD>2</TD>
 *   </TR>
 *   <TR>
 *     <TD>X Down</TD>
 *     <TD>3</TD>
 *     <TD>3</TD>
 *   </TR>
 *   <TR>
 *     <TD>X Left</TD>
 *     <TD>4</TD>
 *     <TD>4</TD>
 *   </TR>
 *   <TR>
 *     <TD>()</TD>
 *     <TD>5</TD>
 *     <TD>5</TD>
 *   </TR>
 *   <TR>
 *     <TD>() Up</TD>
 *     <TD>6</TD>
 *     <TD>6</TD>
 *   </TR>
 *   <TR>
 *     <TD>() Right</TD>
 *     <TD>7</TD>
 *     <TD>7</TD>
 *   </TR>
 *   <TR>
 *     <TD>() Down</TD>
 *     <TD>8</TD>
 *     <TD>8</TD>
 *   </TR>
 *   <TR>
 *     <TD>() Left</TD>
 *     <TD>9</TD>
 *     <TD>9</TD>
 *   </TR>
 *   <TR>
 *     <TD>[]</TD>
 *     <TD>.</TD>
 *     <TD>=</TD>
 *   </TR>
 *   <TR>
 *     <TD>[] Up</TD>
 *     <TD>+</TD>
 *     <TD>+</TD>
 *   </TR>
 *   <TR>
 *     <TD>[] Right</TD>
 *     <TD>-</TD>
 *     <TD>-</TD>
 *   </TR>
 *   <TR>
 *     <TD>[] Down</TD>
 *     <TD>*</TD>
 *     <TD>*</TD>
 *   </TR>
 *   <TR>
 *     <TD>[] Left</TD>
 *     <TD>/</TD>
 *     <TD>/</TD>
 *   </TR>
 *   <TR>
 *     <TD>^</TD>
 *     <TD>Space</TD>
 *     <TD>Space</TD>
 *   </TR>
 *   <TR>
 *     <TD>^ Up</TD>
 *     <TD>%</TD>
 *     <TD>%</TD>
 *   </TR>
 *   <TR>
 *     <TD>^ Right</TD>
 *     <TD>^</TD>
 *     <TD>^</TD>
 *   </TR>
 *   <TR>
 *     <TD>^ Down</TD>
 *     <TD>&</TD>
 *     <TD>&</TD>
 *   </TR>
 *   <TR>
 *     <TD>^ Left</TD>
 *     <TD>|</TD>
 *     <TD>|</TD>
 *   </TR>
 *   <TR>
 *     <TD>LT X</TD>
 *     <TD>a</TD>
 *     <TD>A</TD>
 *   </TR>
 *   <TR>
 *     <TD>LT X Up</TD>
 *     <TD>b</TD>
 *     <TD>B</TD>
 *   </TR>
 *   <TR>
 *     <TD>LT X Right</TD>
 *     <TD>c</TD>
 *     <TD>C</TD>
 *   </TR>
 *   <TR>
 *     <TD>LT X Down</TD>
 *     <TD>d</TD>
 *     <TD>D</TD> <!-- This is SPARTAAA!!! -->
 *   </TR>
 *   <TR>
 *     <TD>LT X Left</TD>
 *     <TD>e</TD>
 *     <TD>E</TD>
 *   </TR>
 *   <TR>
 *     <TD>LT ()</TD>
 *     <TD>f</TD>
 *     <TD>F</TD>
 *   </TR>
 *   <TR>
 *     <TD>LT () Up</TD>
 *     <TD>g</TD>
 *     <TD>G</TD>
 *   </TR>
 *   <TR>
 *     <TD>LT () Right</TD>
 *     <TD>h</TD>
 *     <TD>H</TD>
 *   </TR>
 *   <TR>
 *     <TD>LT () Down</TD>
 *     <TD>i</TD>
 *     <TD>I</TD>
 *   </TR>
 *   <TR>
 *     <TD>LT () Left</TD>
 *     <TD>j</TD>
 *     <TD>J</TD>
 *   </TR>
 *   <TR>
 *     <TD>LT []</TD>
 *     <TD>k</TD>
 *     <TD>K</TD>
 *   </TR>
 *   <TR>
 *     <TD>LT [] Up</TD>
 *     <TD>l</TD>
 *     <TD>L</TD>
 *   </TR>
 *   <TR>
 *     <TD>LT [] Right</TD>
 *     <TD>m</TD>
 *     <TD>M</TD>
 *   </TR>
 *   <TR>
 *     <TD>LT [] Down</TD>
 *     <TD>n</TD>
 *     <TD>N</TD>
 *   </TR>
 *   <TR>
 *     <TD>LT [] Left</TD>
 *     <TD>o</TD>
 *     <TD>O</TD>
 *   </TR>
 *   <TR>
 *     <TD>LT ^</TD>
 *     <TD>p</TD>
 *     <TD>P</TD>
 *   </TR>
 *   <TR>
 *     <TD>LT ^ Up</TD>
 *     <TD>q</TD>
 *     <TD>Q</TD>
 *   </TR>
 *   <TR>
 *     <TD>LT ^ Right</TD>
 *     <TD>r</TD>
 *     <TD>R</TD>
 *   </TR>
 *   <TR>
 *     <TD>LT ^ Down</TD>
 *     <TD>s</TD>
 *     <TD>S</TD>
 *   </TR>
 *   <TR>
 *     <TD>LT ^ Left</TD>
 *     <TD>t</TD>
 *     <TD>T</TD>
 *   </TR>
 *   <TR>
 *     <TD>LT RT X</TD>
 *     <TD>u</TD>
 *     <TD>U</TD>
 *   </TR>
 *   <TR>
 *     <TD>LT RT X Up</TD>
 *     <TD>v</TD>
 *     <TD>V</TD>
 *   </TR>
 *   <TR>
 *     <TD>LT RT X Right</TD>
 *     <TD>w</TD>
 *     <TD>W</TD>
 *   </TR>
 *   <TR>
 *     <TD>LT RT X Down</TD>
 *     <TD>x</TD>
 *     <TD>X</TD>
 *   </TR>
 *   <TR>
 *     <TD>LT RT X Left</TD>
 *     <TD>y</TD>
 *     <TD>Y</TD>
 *   </TR>
 *   <TR>
 *     <TD>LT RT ()</TD>
 *     <TD>z</TD>
 *     <TD>Z</TD>
 *   </TR>
 * </TABLE>
 */

/**
 * \page wii-spec Specific Information about the Wii Port
 *
 * The Wii port, like the PSP port, is very simple. You can "type," and in a
 * simpler way than the PSP, but it's still slow to use. In the future, I'll
 * update the input module to be simpler to use, but for now it will suffice.
 *
 * \section wii-type How to Type on the Wii
 *
 * The input buffer is limited to 64 characters, and starts out as all spaces.
 * Use the left and right buttons to navigate through the buffer, and the up and
 * down buttons to change the character at that position of the buffer. Press A
 * when done to evaluate the input.
 */

/**
 * \page spec Specific Information About the Program
 *
 * This section conveys the basic structure of the program so that you can get
 * an idea of what the hell I was thinking when writing this program. Not
 * everything here applies to the PSP port; see \ref psp-spec to read more about
 * the port.
 *
 * \section exec Execution
 *
 * On startup, the program processes arguments. Some arguments, like "-v" and
 * "-e", exit the program after completion.
 *
 * After this, a loop is entered that is controlled by the calculator's status.
 * As long as the status equals RPN_STATUS_CONTINUE, the loop executes. Commands
 * should exit the program by setting the status to RPN_STATUS_EXIT, unless some
 * severe error occurs, in which case one should use exit() or a similar
 * function.
 *
 * In the loop, the user is presented with a peek at the stack and a prompt to
 * evaluate a string. Upon pressing Enter, the string is sent to RPN_eval, which
 * uses RPN_splitString to get a list of tokens. It then iterates through the
 * tokens, pushing numeric ones to the stack and delegating others to
 * RPN_evalToken.
 *
 * RPN_evalToken tries to find an operator equal to the token; if it can't, it
 * sends the token to RPN_executeCommand, and if that cannot, the token is added
 * to the variable table and its value set to the value of the topmost stack
 * item.
 *
 * \section structs Structures
 *
 * The quintessential Reverse Polish Notation calculator structure is the stack.
 * My stack is RPNStack, which holds only a pointer to the first node of the
 * stack. The node structure, RPNNode, holds the value of the node and a pointer
 * to the next node in the stack.
 *
 * An RPNOperator holds the name and function of an operator. A callback
 * function for an operator looks like this:
 *
 * \code
 * void customOperator(RPNValue a, RPNValue b)
 * {
 * }
 * \endcode
 *
 * RPNOperators is a hash table, implemented with UTHash. It contains just a
 * pointer to the UTHash table.
 *
 * RPNCommand and RPNCommands are almost exactly like RPNOperator and
 * RPNOperators, respectfully, except that command callbacks are passed the
 * entire calculator structure to give them more power. Here is what it looks
 * like:
 *
 * \code
 * void customCommand(RPNCalculator *calculator)
 * {
 * }
 * \endcode
 *
 * RPNVariable and RPNVariables are also similar, except they have values
 * instead of function pointers.
 */
