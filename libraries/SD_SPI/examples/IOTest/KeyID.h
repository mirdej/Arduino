/*
 * KeyID.h
 *
 *  Created on: 2013/05/26
 *      Author: sin
 */

#ifndef KEYID_H_
#define KEYID_H_

#include <string.h>
#include <Print.h>
#include <Printable.h>

struct KeyID : public Printable {
	char jobclass;
	char id[8];
	char issue;
	uint16_t exdate;

	void set(const char * src) {
		jobclass = src[0];
		strncpy(id, src+1, 8);
		issue = src[9];
		exdate = strtol(src+10, 0, 16);
	}

	bool idmatch(const char * another) {
		return jobclass == another[0] && strncmp(id, another+1, 8) == 0;
	}

	int compareTo(const KeyID & another) {
		int diff = jobclass - another.jobclass;
		if ( diff != 0 )
			return diff;
		diff = strncmp(id, another.id, 8);
		if ( diff != 0 )
			return diff;
		diff = issue - another.issue;
		return diff;
	}

	bool operator<(const KeyID & another) {
		return compareTo(another) < 0;
	}

	virtual size_t printTo(Print& p) const {
		size_t n = 0;
		n += p.print(jobclass);
		n += p.print('-');
		for(int i = 0; i < 8; i++) {
			n += p.print(id[i]);
		}
		n += p.print('-');
		n += p.print(issue);

		n += p.print(" /");
		n += p.print(exdate>>4&0x0f);
		n += p.print(exdate&0x0f);
		n += p.print('/');
		n += p.print(0x2000+(exdate>>8), HEX);

		return n;
	}
};

#endif /* KEYID_H_ */
