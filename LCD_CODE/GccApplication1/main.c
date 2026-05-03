#define F_CPU 8000000UL 
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define RS PC0
#define EN PC1

void lcd_cmd(char cmd) {
	PORTC = (PORTC & 0x0F) | (cmd & 0xF0); 
	PORTC &= ~(1 << RS);
	PORTC |= (1 << EN);
	_delay_us(1);
	PORTC &= ~(1 << EN);
	_delay_us(200);

	PORTC = (PORTC & 0x0F) | ((cmd << 4) & 0xF0); 
	PORTC |= (1 << EN);
	_delay_us(1);
	PORTC &= ~(1 << EN);
	_delay_ms(2);
}

void lcd_data(char data) {
	PORTC = (PORTC & 0x0F) | (data & 0xF0);
	PORTC |= (1 << RS);
	PORTC |= (1 << EN);
	_delay_us(1);
	PORTC &= ~(1 << EN);
	_delay_us(200);

	PORTC = (PORTC & 0x0F) | ((data << 4) & 0xF0);
	PORTC |= (1 << EN);
	_delay_us(1);
	PORTC &= ~(1 << EN);
	_delay_ms(2);
}

void lcd_init() {
	DDRC = 0xFF; 
	_delay_ms(20);
	lcd_cmd(0x02); 
	lcd_cmd(0x28); 
	lcd_cmd(0x0C); 
	lcd_cmd(0x01); 
	_delay_ms(2);
}

void lcd_print(char *str) {
	while(*str) {
		lcd_data(*str++);
	}
}

char keys[4][4] = {
	{'.', '/', '*', '='},  
	{'8', '9', '+', '-'},  
	{'4', '5', '6', '7'},  
	{'0', '1', '2', '3'}   
};

void keypad_init() {
	DDRA = 0x0F;  
	PORTA = 0xFF; 
}

char get_key() {
	for (int r = 0; r < 4; r++) {
		PORTA = 0xFF & ~(1 << r); 
		_delay_us(10);
		for (int c = 0; c < 4; c++) {
			if (!(PINA & (1 << (c + 4)))) { 
				while (!(PINA & (1 << (c + 4)))); 
				_delay_ms(20);
				return keys[r][c];
			}
		}
	}
	return 0; 
}

void float_to_lcd(float num) {
	char str[16];
	int int_part = (int)num;
	int frac_part = (int)(fabs(num - int_part) * 1000); 
	sprintf(str, "%d.%03d", int_part, frac_part);
	lcd_print(str);
}

void extra_buttons_init() {
	DDRB &= ~((1 << PB0) | (1 << PB1) | (1 << PB2)); 
	PORTB |= (1 << PB0) | (1 << PB1) | (1 << PB2);   
}

int main() {
	lcd_init();
	keypad_init();
	extra_buttons_init(); 
	
	char key;
	char buffer[16] = "";
	int buf_idx = 0;
	
	float num1 = 0, num2 = 0, result = 0;
	char operator = 0;

	lcd_print("Ready...");
	_delay_ms(1000);
	lcd_cmd(0x01); 

	while(1) {
		if (!(PINB & (1 << PB2))) {
			while (!(PINB & (1 << PB2))); 
			_delay_ms(20);
			num1 = 0; num2 = 0; result = 0; operator = 0;
			buf_idx = 0; buffer[0] = '\0';
			
			lcd_cmd(0x01); 
			lcd_print("Ready...");
			_delay_ms(500); 
			lcd_cmd(0x01);
		}

		if (!(PINB & (1 << PB0))) {
			while (!(PINB & (1 << PB0)));
			_delay_ms(20);

			if (buffer[0] != '\0') {
				num1 = atof(buffer);
			}
			
			if (num1 < 0) {
				lcd_cmd(0x01);
				lcd_print("Math Error");
				_delay_ms(1500);
				lcd_cmd(0x01);
				} else {
				result = sqrt(num1); 
				lcd_cmd(0xC0);
				lcd_print("Ans= ");
				float_to_lcd(result);
				num1 = result;
			}
			buf_idx = 0; buffer[0] = '\0'; 
		}
		if (!(PINB & (1 << PB1))) {
			while (!(PINB & (1 << PB1))); 
			_delay_ms(20);
			if (buffer[0] != '\0') { num1 = atof(buffer); }
	
			operator = '^'; 
			buf_idx = 0; buffer[0] = '\0';
			
			lcd_cmd(0x01);
			lcd_data('^'); 
		}
		key = get_key();
		if (key) {
			if ((key >= '0' && key <= '9') || key == '.') {
				if (buf_idx < 15) {
					buffer[buf_idx++] = key;
					buffer[buf_idx] = '\0';
					lcd_cmd(0x01);
					lcd_print(buffer);
				}
			}
			else if (key == '+' || key == '-' || key == '*' || key == '/') {
				if (buffer[0] != '\0') { num1 = atof(buffer); }
				
				operator = key;
				buf_idx = 0; buffer[0] = '\0';
				
				lcd_cmd(0x01);
				lcd_data(operator);
			}
			else if (key == '=') {
				if (buffer[0] != '\0') { num2 = atof(buffer); }
				
				switch(operator) {
					case '+': result = num1 + num2; break;
					case '-': result = num1 - num2; break;
					case '*': result = num1 * num2; break;
					case '/':
					if (num2 == 0) {
						lcd_cmd(0x01); lcd_print("Math Error"); _delay_ms(1500); lcd_cmd(0x01);
						buf_idx = 0; buffer[0] = '\0';
						continue;
						} else {
						result = num1 / num2;
					}
					break;
					case '^':
					result = pow(num1, num2); 
					break;
				}
				lcd_cmd(0xC0);
				lcd_print("Ans= ");
				float_to_lcd(result);
				
				num1 = result; 
				buf_idx = 0; buffer[0] = '\0';
			}
		}
	}
}
