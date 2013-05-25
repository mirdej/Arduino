/*
 * TextReader.h
 *
 *  Created on: 2012/08/07
 *      Author: sin
 */

#ifndef TEXTREADER_H_
#define TEXTREADER_H_

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <Wiring.h>
#endif
#include <Stream.h>
// stdlib.h is included in Arduino.h

const char endl = '\n';
const char cr = '\r';
const char tab = '\t';

template<class T>
inline Stream &operator <<(Stream &stream, T arg) {
	stream.print(arg);
	return stream;
}

class TextReader {
	Stream & stream;

public:
    TextReader(Stream & s) : stream(s) {
	}
    ~TextReader() { }

    void resetTo(Stream & s) { stream = s; }

	void printHex(const byte b);
    void printBytes(const byte * a, const int length, const char gap, byte base);
    void printBytes(const char * s, const int length, const char gap);

    inline int available() { return stream.available(); }
    int read() { return stream.read(); }
    int readLine(char * buf = 0, size_t maxlen = 0);
    int readToken(char * buf = 0, size_t maxlen = 0);
};

#endif /* TEXTREADER_H_ */
