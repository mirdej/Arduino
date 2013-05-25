/*
 * StringStream.h
 *
 *  Created on: 2013/05/11
 *      Author: sin
 */

#ifndef STRINGSTREAM_H_
#define STRINGSTREAM_H_

#include <Arduino.h>
#include <Stream.h>
#include <Printable.h>

class StringStream: public Stream, public Printable {
	// constants
public:
	static const char endl = '\n';
	static const char cr = '\r';

	// members
private:
	uint8_t * buffer_array;
	uint16_t buffer_size;
	uint16_t windex, rindex;
	uint16_t count;

public:
	StringStream(char buf[], uint16_t lim);
//	virtual ~StringStream() {}

	void setstring(char str[], uint16_t sz) {
		buffer_array = (uint8_t *) str;
		buffer_size = sz;
		windex = strlen(str); rindex = 0;
		count = 0;
	}
    using Print::write;
    virtual size_t write(uint8_t b);
    virtual int available();
    virtual inline int read();
    virtual inline int peek();

    size_t readLineFrom(Stream &, uint16_t);

    uint8_t is_full();

    size_t getString(char * dst, uint16_t maxlen);
    size_t getToken(char * dst, uint16_t maxlen);

    uint32_t parseHex();

    void flush();
    uint16_t writeindex() { return windex; }
    uint16_t readindex() { return rindex; }
    size_t length() { return count; }

	using Print::print;


    virtual size_t printTo(Print& p) const;

//	size_t readLine(char *buffer, size_t length);

};


#endif /* STRINGSTREAM_H_ */
