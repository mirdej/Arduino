/*
 * NFCCard.h
 *
 *  Created on: 2012/07/24
 *      Author: sin
 */

#ifndef ISO14443_H_
#define ISO14443_H_

//#include <Print.h>
#include <Printable.h>

<<<<<<< HEAD
//#include "PN532_I2C.h"
=======
namespace NFC {
static const byte BaudrateType_106kbitTypeA = 0x00;
static const byte BaudrateType_212kbitFeliCa = 0x01;
static const byte BaudrateType_424kbitFeliCa = 0x02;
static const byte BaudrateType_106kbitTypeB = 0x03;
static const byte BaudrateType_Jewel = 0x04;

static const byte Type_GenericPassiveTypeA = 0x00;
static const byte Type_GenericPassive212kbFeliCa = 0x01;
static const byte Type_GenericPassive424kbFeliCa = 0x02;
static const byte Type_PassiveTypeB = 0x03;
static const byte Type_Mifare = 0x10;
static const byte Type_FeliCa212kb = 0x11;
static const byte Type_FeliCa424kb = 0x12;
static const byte Type_DESFire = 0x20;
static const byte Type_Empty = 0xff;


static const word ATQA_MIFARE_CLASSIC1K = 0x0004; // SAK = 08
static const word ATQA_MIFARE_CLASSIC4K = 0x0002;
static const word ATQA_MIFARE_ULTRALIGHT = 0x0044;
static const word ATQA_MIFARE_DESFIRE = 0x0344;
static const word ATQA_MIFARE_DESFIRE_EV1 = 0x0344;

static const word FELICA_SYSCODE_COMMON = 0x00FE;
static const word FELICA_SYSCODE_SUICA = 0x0003;
static const word FELICA_SERVICE_SUICA = 0x090F;
static const word FELICA_SERVICE_EDY = 0x170F;
static const word FELICA_SERVICE_FCF = 0x1a8b;
};
>>>>>>> 724a7e5cb7e4feda098878587bfaf8203e4f1384

struct ISO14443 : public Printable {
	static const byte TypeA = 0x00;
	static const byte TypeF = 0x01;
	static const byte TypeF_212kb = 0x01;
	static const byte TypeF_424kb = 0x02;
	static const byte TypeB = 0x03;
	static const byte TypeJewel = 0x03;
	static const byte Mifare = 0x10;
	static const byte FeliCa212kb = 0x11;
	static const byte FeliCa424kb = 0x12;
	static const byte DESFire = 0x20;
	static const byte Type_Empty = 0xff;

	static const word FELICA_SYSCODE_COMMON = 0x00FE;
	static const word FELICA_SYSCODE_SUICA = 0x0003;
	static const word FELICA_SERVICE_SUICA = 0x090F;
	static const word FELICA_SERVICE_EDY = 0x170F;
	static const word FELICA_SERVICE_FCF = 0x1a8b;


	static const byte NFCID_size = 8;
	//
	byte type;
	byte IDLength;
	union {
		byte UID[7];
		byte NUID[7];
		byte IDm[8];
		byte ID[NFCID_size];
	};

	ISO14443() {
		clear();
	}

	ISO14443(const byte * raw) {
		set(raw);
	}

	ISO14443(const byte & card) {
		set(card);
	}

	void set(const ISO14443 & card) {
		type = card.type;
		IDLength = card.IDLength;
		switch (type) {
<<<<<<< HEAD
		case FeliCa212kb: // Felica
=======
		case NFC::Type_FeliCa212kb: // Felica
>>>>>>> 724a7e5cb7e4feda098878587bfaf8203e4f1384
			memcpy(IDm, card.IDm, IDLength);
			break;
		default: // Mifare
			memcpy(UID, card.UID, IDLength);
			break;
		}
	}

	ISO14443 & operator=(const ISO14443 & c) {
		set(c);
		return *this;
	}

	void set(const byte tp, const byte *data, const byte len) {
		type = tp;
		IDLength = len;
		memcpy(ID, data, len);
	}

	void set(const byte tp, const byte * raw) {
		//PN532::printHexString(raw, 16);
		type = tp;
		byte len;
		switch (type) {
<<<<<<< HEAD
		case FeliCa212kb:
		case FeliCa424kb:
=======
		case NFC::Type_FeliCa212kb:
		case NFC::Type_FeliCa424kb:
>>>>>>> 724a7e5cb7e4feda098878587bfaf8203e4f1384
			IDLength = 8;
			len = raw[1];
			memcpy(IDm, raw + 3, 8);
//			memcpy(PMm, raw + 11, 8);
//			if (len == 20)
//				memcpy(SysCode, raw + 19, 2);
			break;
<<<<<<< HEAD
		case Mifare:
=======
		case NFC::Type_Mifare:
>>>>>>> 724a7e5cb7e4feda098878587bfaf8203e4f1384
		default: // Mifare 106k TypeA
			IDLength = raw[4];
			memcpy(UID, raw + 5, IDLength);
			break;
		}
<<<<<<< HEAD
=======
		atqa = 0;
		sak = 0;
	}

	void setPassiveTarget(const byte * raw) {
		// raw[0] ... number
		type = NFC::Type_GenericPassiveTypeA;
		atqa = raw[1]<<8 | raw[2];
		sak = raw[3];
		IDLength = raw[4];
		memcpy(ID, raw+5, IDLength);
		switch (atqa) {
		case NFC::ATQA_MIFARE_CLASSIC1K:
		case NFC::ATQA_MIFARE_CLASSIC4K:
		case NFC::ATQA_MIFARE_ULTRALIGHT:
		case NFC::ATQA_MIFARE_DESFIRE:
			type = NFC::Type_Mifare;
			break;
		}
>>>>>>> 724a7e5cb7e4feda098878587bfaf8203e4f1384
	}

	virtual size_t printTo(Print & pr) const {
		int cnt = 0;
		switch(type) {
<<<<<<< HEAD
		case Mifare:
			cnt += pr.print("Mifare");
			break;
		case FeliCa212kb:
			cnt += pr.print("FeliCa212kb");
			break;
		case FeliCa424kb:
			cnt += pr.print("FeliCa424kb");
			break;
		case DESFire:
=======
		case NFC::Type_Mifare:
			cnt += pr.print("Mifare");
			if ( atqa == NFC::ATQA_MIFARE_ULTRALIGHT )
				cnt += pr.print(" Ultralight");
			else if ( atqa == NFC::ATQA_MIFARE_CLASSIC1K )
				cnt += pr.print(" Classic 1k");
			else if ( atqa == NFC::ATQA_MIFARE_CLASSIC4K )
				cnt += pr.print(" Classic 4k");
			break;
		case NFC::Type_FeliCa212kb:
			cnt += pr.print("FeliCa212kb");
			break;
		case NFC::Type_FeliCa424kb:
			cnt += pr.print("FeliCa424kb");
			break;
		case NFC::Type_DESFire:
>>>>>>> 724a7e5cb7e4feda098878587bfaf8203e4f1384
			cnt += pr.print("Mifare DESFire");
			break;
		case NFC::Type_Empty:
			cnt += pr.print("Empty");
			break;
		default:
			cnt += pr.print("Unknown (");
			cnt += pr.print((int)type, DEC);
			cnt += pr.print(")");
			break;
		}
		for(int i = 0; i < IDLength; i++) {
			pr.print(' ');
			pr.print(ID[i]>>4, HEX);
			pr.print(ID[i]&0x0f, HEX);
			cnt += 3;
		}
		return cnt;
	}

	void clear() {
		type = NFC::Type_Empty;
		IDLength = 0;
		memset(ID, 0, 8);
	}

	const boolean operator==(const ISO14443 & c) const {
		if (type == c.type && IDLength == c.IDLength) {
			return memcmp(ID, c.ID, IDLength) == 0;
		}
		return false;
	}

	inline const boolean operator!=(const ISO14443 & c) const {
		return !(operator==(c));
	}
};

#endif /* NFCCARD_H_ */

