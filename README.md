# Extension-Assignment-Group7-HCMUT

## Project Overview
ATmega324P microcontroller project: Implementation and Proteus simulation of a handheld calculator. Features floating-point arithmetic (basic operations, square root, power), expression evaluation with parentheses, trigonometric functions, logarithms, and quadratic equation solving.

## Tools & Hardware
*   **Microcontroller:** ATmega324P (AVR Architecture).
*   **Simulation:** Proteus Professional.
*   **Programming Language:** C / Assembly.
*   **IDE:** Microchip Studio (for coding, debugging, and compiling C/Assembly).
## Display Features
The project support two types of displays:
*   **LCD (Liquid Crystal Display):** Used for the basic calculator version to display simple calculations and results on a character-based interface (16x2).
*   **OLED (SSD1306):** Used for the advanced version to display complex mathematical expressions, symbols, and high-resolution menus.

## Main Functions
*   Basic Arithmetic: +, -, *, /
*   Advanced Math: Square root, Power (x^y), Logarithms.
*   Trigonometry: Sin, Cos, Tan.
*   Equation Solver: Solving quadratic equations (ax² + bx + c = 0).
*   Expression Evaluation: Supports calculations with parentheses.

---

## How to Use

### 1. Build Source Code
1.  Open the project in **Microchip Studio**.
2.  Check the source code in `LCD_CODE` or `OLED_CODE` folders.
3.  Press **F7** (or go to **Build > Build Solution**) to compile the code.
4.  Ensure the output window shows **"Build succeeded"** with no errors. This process generates the `.hex` file needed for the next steps.

### 2. Simulation in Proteus
1.  Open the `.pdsprj` file (e.g., `OLED_BTLVXL.pdsprj`) using **Proteus Professional**.
2.  Double-click on the **ATmega324P** microcontroller in the schematic.
3.  In the **"Program File"** field, browse and select the `.hex` file you just built (usually located in the `Debug` or `Release` folder).
4.  Press the **Play** button at the bottom left to start the simulation.

### 3. Operating the Calculator
Once the simulation is running:
*   **Basic Calculations:** Use the keypad to input numbers and basic operators (+, -, *, /).
*   **Scientific Functions:** 
    *   Access **Sin, Cos, Tan**, and **Logarithms** via designated function keys.
    *   Results are displayed with floating-point precision on the **OLED/LCD**.
*   **Quadratic Equation Solver:** 
    *   Enter the Equation mode.
    *   Input coefficients **a, b, and c** when prompted to find the roots ($x_1, x_2$).
