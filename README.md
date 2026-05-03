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

### 1. Simulation in Proteus
1.  Open the `.pdsprj` file (e.g., `OLED_BTLVXL.pdsprj`) using **Proteus Professional**.
2.  Double-click on the **ATmega324P** microcontroller.
3.  In the "Program File" field, browse and select the `.hex` file (found in the `Debug` or `Release` folder of your Microchip Studio project).
4.  Press the **Play** button to start the simulation.

### 2. Operating the Calculator
*   **Basic Calculations:** Use the 4x4 Keypad to input numbers and operators (+, -, *, /).
*   **Parentheses:** Use the designated keys to wrap expressions for correct order of operations.
*   **Scientific Functions:** 
    *   Press the mode/function key to access **Sin, Cos, Tan**, and **Logarithms**.
    *   Results will be displayed on the **OLED/LCD** screen with floating-point precision.
*   **Quadratic Equation Solver:** 
    *   Select the Equation mode.
    *   Input coefficients **a, b, and c** when prompted.
    *   The system will calculate the discriminant ($\Delta$) and display the roots ($x_1, x_2$).

### 3. Hardware Deployment
1.  Open the source code in **Microchip Studio**.
2.  Build the project (**F7**) to ensure there are no errors.
3.  Connect your **ATmega324P kit** to the computer via an ISP programmer (e.g., USBASP).
4.  Flash the `.hex` file to the chip.
5.  Ensure the **OLED/LCD** and **Keypad** are wired correctly according to the pin definitions in the source code.
