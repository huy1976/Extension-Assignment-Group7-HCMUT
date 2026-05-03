#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <avr/pgmspace.h>
#define PI 3.14159265

void i2c_init(void);
void oled_init(void);
void oled_char(char c, uint8_t x, uint8_t y);
void oled_print(const char *s, uint8_t x, uint8_t y);
void oled_clear(void);
void oled_clear_line(uint8_t y);

char get_key(void);
uint8_t check_ac(void);
double input_value(const char* label, uint8_t y);
double expression(void);
double term(void);
double number(void);

const char *ptr;

const uint8_t font[][5] PROGMEM = {
	{0x00,0x00,0x00,0x00,0x00}, {0x3E,0x51,0x49,0x45,0x3E}, {0x00,0x42,0x7F,0x40,0x00}, {0x42,0x61,0x51,0x49,0x46},
	{0x21,0x41,0x45,0x4B,0x31}, {0x18,0x14,0x12,0x7F,0x10}, {0x27,0x45,0x45,0x45,0x39}, {0x3C,0x4A,0x49,0x49,0x30},
	{0x01,0x71,0x09,0x05,0x03}, {0x36,0x49,0x49,0x49,0x36}, {0x06,0x49,0x49,0x29,0x1E}, {0x7E,0x11,0x11,0x11,0x7E},
	{0x7F,0x49,0x49,0x49,0x36}, {0x3E,0x41,0x41,0x41,0x22}, {0x7F,0x41,0x41,0x22,0x1C}, {0x7F,0x49,0x49,0x49,0x41},
	{0x7F,0x09,0x09,0x09,0x01}, {0x3E,0x41,0x49,0x49,0x7A}, {0x7F,0x08,0x08,0x08,0x7F}, {0x00,0x41,0x7F,0x41,0x00},
	{0x20,0x40,0x41,0x3F,0x01}, {0x7F,0x08,0x14,0x22,0x41}, {0x7F,0x40,0x40,0x40,0x40}, {0x7F,0x02,0x0C,0x02,0x7F},
	{0x7F,0x04,0x08,0x10,0x7F}, {0x3E,0x41,0x41,0x41,0x3E}, {0x7F,0x09,0x09,0x09,0x06}, {0x3E,0x41,0x51,0x21,0x5E},
	{0x7F,0x09,0x19,0x29,0x46}, {0x46,0x49,0x49,0x49,0x31}, {0x01,0x01,0x7F,0x01,0x01}, {0x3F,0x40,0x40,0x40,0x3F},
	{0x1F,0x20,0x40,0x20,0x1F}, {0x3F,0x40,0x38,0x40,0x3F}, {0x63,0x14,0x08,0x14,0x63}, {0x07,0x08,0x70,0x08,0x07},
	{0x61,0x51,0x49,0x45,0x43}, {0x08,0x08,0x3E,0x08,0x08}, {0x08,0x08,0x08,0x08,0x08}, {0x14,0x08,0x3E,0x08,14},
	{0x00,0x36,0x36,0x00,0x00}, {0x00,0x60,0x60,0x00,0x00}, {0x14,0x14,0x14,0x14,0x14}, {0x00,0x36,0x36,0x00,0x00},
	{0x04,0x02,0x01,0x02,0x04}, {0x3E,0x41,0x41,0x22,0x18}, {0x48,0x54,0x54,0x54,0x24}, {0x00,0x44,0x7D,0x40,0x00},
	{0x7C,0x08,0x04,0x04,0x78}, {0x38,0x44,0x44,0x44,0x20}, {0x38,0x44,0x44,0x44,0x38}, {0x04,0x3F,0x44,0x40,0x20},
	{0x20,0x54,0x54,0x54,0x78}, {0x00,0x1C,0x22,0x41,0x00}, {0x00,0x41,0x22,0x1C,0x00}, {0x00,0x41,0x7F,0x40,0x00},
	{0x0C,0x52,0x52,0x52,0x3E}
};


int main(void) {
	char buf[64] = ""; uint8_t app = 0, shift_h = 0;
	oled_init(); oled_clear();
	DDRA = 0x0F; PORTA = 0xF0; 
	DDRB &= ~(1 << PB2); PORTB |= (1 << PB2); 
	DDRD &= ~((1 << PD1) | (1 << PD3)); PORTD |= (1 << PD1) | (1 << PD3); 
	DDRB &= ~(1 << PB0); PORTB |= (1 << PB0); 

	while (1) {
		if (check_ac()) { app = 0; buf[0] = '\0'; shift_h = 0; oled_clear(); _delay_ms(200); continue; }

		if (app == 0) {
			oled_print("CHOOSE MODE:", 0, 0); oled_print("1. SCI CALC", 0, 2); oled_print("2. PT BAC 2", 0, 3);
			char key = get_key();
			if (key == '1') { app = 1; oled_clear(); } else if (key == '2') { app = 2; oled_clear(); }
		}
		else if (app == 1) {
			oled_print("MODE 1: CALC", 0, 0);
			if (!(PIND & (1 << PD3))) { _delay_ms(150); shift_h = !shift_h; oled_char(shift_h ? 'H' : ' ', 115, 0); while(!(PIND & (1 << PD3))); }
			if (!(PINB & (1 << PB0))) { strcat(buf, "q"); oled_clear_line(2); oled_print(buf, 0, 2); while(!(PINB & (1 << PB0))); }
			if (!(PIND & (1 << PD1))) { strcat(buf, "^"); oled_clear_line(2); oled_print(buf, 0, 2); while(!(PIND & (1 << PD1))); }

			char key = get_key();
			if (key) {
				if (key == '=') {
					ptr = buf; double res = expression();
					char s[20]; dtostrf(res, 7, 3, s);
					oled_clear_line(5); oled_print("Ans:", 0, 5); oled_print(s, 30, 5);
					} else {
					if (shift_h) {
						if (key == '+') strcat(buf, "sin");
						else if (key == '-') strcat(buf, "cos");
						else if (key == '*') strcat(buf, "tan");
						else if (key == '/') strcat(buf, "cot");
						else if (key == '.') strcat(buf, "log");
						else if (key == '0') strcat(buf, "(");
						else if (key == '1') strcat(buf, ")");
						else { char t[2] = {key, '\0'}; strcat(buf, t); }
						shift_h = 0; oled_char(' ', 115, 0);
						} else { char t[2] = {key, '\0'}; strcat(buf, t); }
						oled_clear_line(2); oled_print(buf, 0, 2);
					}
				}
			}
			else if (app == 2) {
				oled_print("PT: AX^2 + BX + C = 0", 0, 0);
				double a = input_value("A:", 2); if (isnan(a)) { app = 0; oled_clear(); continue; }
				double b = input_value("B:", 3); if (isnan(b)) { app = 0; oled_clear(); continue; }
				double c = input_value("C:", 4); if (isnan(c)) { app = 0; oled_clear(); continue; }
				
				char s1[16], s2[16];
				oled_clear_line(6);

				if (a == 0) {
					oled_print("A=0 -> PT Bac 1", 0, 5);
					if (b == 0) {
						if (c == 0) oled_print("VO SO NGHIEM !", 0, 6);
						else oled_print("VO NGHIEM !", 0, 6);
						} else {
						dtostrf(-c/b, 6, 2, s1);
						oled_print("x =", 0, 6); oled_print(s1, 25, 6);
					}
				}
				else {
					double d = b*b - 4*a*c;
					if (d < 0) {
						oled_print("VO NGHIEM !", 0, 6);
					}
					else if (d == 0) {
						dtostrf(-b/(2*a), 6, 2, s1);
						oled_print("x1=x2=", 0, 6); oled_print(s1, 40, 6);
					}
					else {
						dtostrf((-b + sqrt(d))/(2*a), 5, 2, s1);
						dtostrf((-b - sqrt(d))/(2*a), 5, 2, s2);
						oled_print("x1:", 0, 6); oled_print(s1, 20, 6);
						oled_print("x2:", 65, 6); oled_print(s2, 85, 6);
					}
				}
				oled_print("PRESS AC TO BACK", 0, 7);
				while(1) { if (check_ac()) { app = 0; oled_clear(); break; } }
			}
		}
		return 0;
	}


	double number() {
		double r = 0;
		if (*ptr == '(') { ptr++; r = expression(); if (*ptr == ')') ptr++; return r; }
		if (strncmp(ptr, "sin", 3) == 0) { ptr += 3; return sin(number() * PI / 180.0); }
		if (strncmp(ptr, "cos", 3) == 0) { ptr += 3; return cos(number() * PI / 180.0); }
		if (strncmp(ptr, "tan", 3) == 0) { ptr += 3; return tan(number() * PI / 180.0); }
		if (strncmp(ptr, "cot", 3) == 0) { ptr += 3; return 1.0 / tan(number() * PI / 180.0); }
		if (strncmp(ptr, "log", 3) == 0) { ptr += 3; return log10(number()); }
		if (*ptr == 'q') { ptr++; return sqrt(number()); }
		int sign = 1; if (*ptr == '-') { sign = -1; ptr++; }
		while (*ptr >= '0' && *ptr <= '9') r = r * 10 + (*ptr++ - '0');
		if (*ptr == '.') { ptr++; double w = 0.1; while (*ptr >= '0' && *ptr <= '9') { r += (*ptr++ - '0') * w; w /= 10; } }
		return r * sign;
	}

	double term() {
		double r = number();
		while (*ptr == '*' || *ptr == '/' || *ptr == '^') {
			char o = *ptr++;
			if (o == '*') r *= number(); else if (o == '/') r /= number(); else if (o == '^') r = pow(r, number());
		}
		return r;
	}

	double expression() {
		double r = term();
		while (*ptr == '+' || *ptr == '-') {
			char o = *ptr++;
			if (o == '+') r += term(); else r -= term();
		}
		return r;
	}

	void i2c_init() { TWBR = 32; }
	void i2c_start() { TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN); uint16_t t = 1000; while (!(TWCR & (1<<TWINT)) && t--); }
	void i2c_stop() { TWCR = (1<<TWINT) | (1<<TWSTO) | (1<<TWEN); }
	void i2c_write(uint8_t d) { TWDR = d; TWCR = (1<<TWINT) | (1<<TWEN); uint16_t t = 1000; while (!(TWCR & (1<<TWINT)) && t--); }
	void oled_cmd(uint8_t c) { i2c_start(); i2c_write(0x78); i2c_write(0x00); i2c_write(c); i2c_stop(); }
	void oled_data(uint8_t d) { i2c_start(); i2c_write(0x78); i2c_write(0x40); i2c_write(d); i2c_stop(); }

	void oled_init() {
		uint8_t cmds[] = {0xAE, 0x20, 0x10, 0xB0, 0xC8, 0x00, 0x10, 0x40, 0x81, 0xFF, 0xA1, 0xA6, 0xA8, 0x3F, 0xA4, 0xD3, 0x00, 0xD5, 0xF0, 0xD9, 0x22, 0xDA, 0x12, 0xDB, 0x20, 0x8D, 0x14, 0xAF};
		i2c_init(); _delay_ms(100);
		for(uint8_t i=0; i<28; i++) oled_cmd(cmds[i]);
	}

	void oled_char(char c, uint8_t x, uint8_t y) {
		uint8_t idx = 0;
		if (c == ' ') idx = 0;
		else if (c >= '0' && c <= '9') idx = c - '0' + 1;
		else if (c >= 'A' && c <= 'Z') idx = c - 'A' + 11;
		else if (c == '+') idx = 37; else if (c == '-') idx = 38; else if (c == '*') idx = 39;
		else if (c == '/') idx = 40; else if (c == '.') idx = 41; else if (c == '=') idx = 42;
		else if (c == ':') idx = 43; else if (c == '^') idx = 44; else if (c == 'q') idx = 45;
		else if (c == 's') idx = 46; else if (c == 'i') idx = 47; else if (c == 'n') idx = 48;
		else if (c == 'c') idx = 49; else if (c == 'o') idx = 50; else if (c == 't') idx = 51;
		else if (c == 'a') idx = 52; else if (c == '(') idx = 53; else if (c == ')') idx = 54;
		else if (c == 'l') idx = 55; else if (c == 'g') idx = 56;
		
		oled_cmd(0xB0 + y); oled_cmd(0x00 + (x & 0x0F)); oled_cmd(0x10 + ((x >> 4) & 0x0F));
		for (int i = 0; i < 5; i++) {
			oled_data(pgm_read_byte(&(font[idx][i])));
		}
		oled_data(0x00);
	}

	void oled_print(const char *s, uint8_t x, uint8_t y) { while(*s) { oled_char(*s++, x, y); x+=6; } }
	void oled_clear() { for(uint8_t i=0; i<8; i++) { oled_cmd(0xB0+i); oled_cmd(0x00); oled_cmd(0x10); for(int j=0; j<128; j++) oled_data(0x00); } }
	void oled_clear_line(uint8_t y) { oled_cmd(0xB0+y); oled_cmd(0x00); oled_cmd(0x10); for(int j=0; j<128; j++) oled_data(0x00); }

	char get_key() {
		char k[4][4] = {{'.','/','*','='},{'8','9','+','-'},{'4','5','6','7'},{'0','1','2','3'}};
		for (uint8_t r = 0; r < 4; r++) {
			DDRA = (1 << r); PORTA = ~(1 << r); _delay_us(5);
			for (uint8_t c = 0; c < 4; c++) {
				if (!(PINA & (1 << (c + 4)))) { _delay_ms(20); while (!(PINA & (1 << (c + 4)))); return k[r][c]; }
			}
		}
		return 0;
	}

	uint8_t check_ac() {
		if (!(PINB & (1 << PB2))) {
			_delay_ms(20);
			if (!(PINB & (1 << PB2))) {
				while (!(PINB & (1 << PB2)));
				return 1;
			}
		}
		return 0;
	}

	double input_value(const char* label, uint8_t y) {
		char buf[16] = ""; uint8_t p = 0; oled_print(label, 0, y);
		while(1) {
			if (check_ac()) return NAN;
			char k = get_key(); if (k == '=') break;
			if (k && p < 10) { if ((k >= '0' && k <= '9') || k == '.' || k == '-') { buf[p++] = k; buf[p] = '\0'; oled_print(buf, 20, y); } }
		}
		return (p > 0) ? atof(buf) : 0;
	}
