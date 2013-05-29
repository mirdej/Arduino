/*
 * TextStream.cpp
 *
 *  Created on: 2012/08/12
 *      Author: sin
 */

#include "TextStream.h"

// initialization
const char * TextStream::wendl = "\r\n";

void TextStream::printHex(const byte b) {
	stream->write( ((b & 0xf0) < 0xa0 ? '0' : 'A'-10 ) + (b>>4) );
	stream->write( ((b & 0x0f) < 0x0a ? '0' : 'A'-10 ) + (b&0x0f) );
}

size_t TextStream::print(unsigned char val, int base) {
	if ( base != HEX)
		return Print::print(val, base);
	printHex(val);
	return 2;
}

void TextStream::printBytes(const byte * a, const int length, const char * gap,
		byte base) {

	for (int i = 0; i < length;) {
		switch (base) {
		case HEX:
			printHex(a[i]);
			break;
		case BIN:
			stream->print(a[i], BIN);
			break;
		default:
			stream->print(a[i], DEC);
			break;
		}
		i++;
		if (gap && i < length)
			stream->print(gap);
	}
	return;
}

void TextStream::printBytes(const char * s, const int length, const char * gap) {
	for (int i = 0; i < length;) {
		if (isprint(s[i]))
			stream->print(s[i]);
		else {
			stream->print("$");
			stream->print((byte) s[i], HEX);
		}
		i++;
		if (gap && i < length)
			stream->print(gap);
	}
	return;
}


int TextStream::getLine(char * buf, size_t maxlen) {
	size_t i = 0;
	do {
		int c = stream->read();
		if ( c == -1 )
			break;
		if ( c == '\r' ) {
			c = stream->peek();
			if ( c == '\n' )
				stream->read();
			break;
		}
		if ( c == '\n' )
			break;
		if ( buf )
			buf[i++] = (char) c;
	} while ( maxlen == 0 or i < maxlen );
	if ( buf ) buf[i] = 0;
	return i;
}

int TextStream::getToken(char * buf, size_t maxlen) {
	size_t i = 0;
	int c;
	do {
		c = stream->read();
		if ( !isspace(c) )
			break;
	} while (1);
	if ( c == -1 )
		return 0;
	do {
		if ( buf )
			buf[i++] = (char) c;
		c = stream->read();
		if ( c == -1 )
			break;
		if ( isspace(c) ) {
			break;
		}
	} while ( maxlen == 0 or i < maxlen);
	if ( buf ) buf[i] = 0;
	return i;
}

boolean ishexdigit(const char c) {
	return isdigit(c) or ('a' <= c && c <= 'f') or ('A' <= c && c<= 'F');
}

uint32_t TextStream::parseHex() {
	uint32_t tmp = 0;
	int c;

	while ( available() && !ishexdigit(peek()) ) {
		read();
	}
	if ( !available() )
		return 0;
	while (available()) {
		c = read();
		if ( c == -1 ) // not available()
			break;
//		Serial.print((char)c);
		c = toupper(c);
		if ( c >= '0' and c <= '9' ) {
			tmp = (tmp<<4) + c - '0';
		} else if ( c >= 'A' and c <= 'F' ) {
			tmp = (tmp<<4) + c - 'A' + 10;
		} else {
			break;
		}
//		Serial.print(t);
//		Serial.print(", ");
	}
	return tmp;
}
