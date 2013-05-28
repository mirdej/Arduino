/*
 * TextStream.h
 *
 *  Created on: 2012/08/07
 *      Author: sin
 */

#ifndef TEXTSTREAM_H_
#define TEXTSTREAM_H_

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <Wiring.h>
#endif
#include <Stream.h>
// stdlib.h is included in Arduino.h

template<class T>
inline Stream &operator <<(Stream &stream, T arg) {
	stream.print(arg);
	return stream;
}

class TextStream : public Stream {
	Stream * stream;

public:
	static const char endl = '\n';
	static const char cr = '\r';
	static const char * wendl;
	static const char tab = '\t';

	TextStream(void) : stream(0){}
    TextStream(Stream & s) : stream(&s) {
	}
    ~TextStream() { }

    void resetTo(Stream & s) { stream = &s; }

    virtual int available() { return stream->available(); }
    virtual int read() { return stream->read(); }
    virtual int peek() { return stream->peek(); }
    virtual size_t write(uint8_t c) { return stream->write(c); }
    virtual void flush() { stream->flush(); }

    int getLine(char * buf, size_t maxlen);
    int getToken(char * buf, size_t maxlen);
    inline void skipLine() { getLine(0, 0); }
    inline void skipToken() { getToken(0,0); }

    uint32_t parseHex();

	void printHex(const byte b);
    void printBytes(const byte * a, const int length, const char gap, byte base);
    void printBytes(const char * s, const int length, const char gap);

};

#endif /* TextStream_H_ */