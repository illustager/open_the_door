#include <Arduino.h>
#include <Preferences.h>
#include <Keypad.h>
#include <AceSorting.h>

#include "manage.h"

#include <myio3.h>

// ------------------------------ OLED ------------------------------
static const byte outpins[] = {22, 21};
static const byte pinnum = 4;

// ------------------------------ NVS ------------------------------
static const char* nvsname = "manage";
static const char* nvskey = "userdata";

// ------------------------------ Password ------------------------------
static const char* password = "1";

// ------------------------------ User Data ------------------------------
static const unsigned usermax = 10;
static uint32_t userdata[usermax] PROGMEM = {0xDED0135F, 0xDEADBEEF, 0x114514};

// ------------------------------ Functions ------------------------------

// ------------------------------ Log in ------------------------------
static bool login(myio::OLEDstream* myout, Keypad* kpd) {
	myout->clear();

	myout->printf("\r\n\r\nPassword: \f");
	
	unsigned i = 0;
	const unsigned maxn = 9;
	char input[maxn+1] = {};
	char key;

	while( true ) {
		switch( key = kpd->getKey() ) {
			case NO_KEY:
				break;
			
			case '0'...'9':
				if( i < maxn ) {
					input[i++] = key;
					input[i] = '\0';
					myout->printf("*\f");
				}
				break;
			
			case '*':
				if( i > 0 ) {
					input[--i] = '\0';
					myout->printf("\b \b\f");
				}
				break;
			
			case '#':
				input[0] = '\0';
				for( i; i; --i ) {
					myout->printf("\b \b");
				}
				myout->flush();
				break;

			case 'D':
				myout->clear();
				// Serial.println(input);
				return (strcmp(input,password) == 0);
			
			default:
				break;
		}
	}
}

// ------------------------------ Others ------------------------------
static bool YorN(myio::OLEDstream* myout, Keypad* kpd, const char* msg) {
	myout->clear();

	myout->printf("\r\n\r\n%s\r\n('D'/'C' for Y/N)\f", msg);

	char key;
	while( true ) {
		switch( key = kpd->getKey() ) {
			case 'D':
				myout->clear();
				return true;
			
			case 'C':
				myout->clear();
				return false;
			
			default:
				break;
		}
	}
}

static void dataSort(uint32_t* data, unsigned maxn) {
	ace_sorting::shellSortKnuth(data, maxn, [](uint32_t a, uint32_t b) {return a > b;});
}

// ------------------------------ Edit ------------------------------
static void drawCursor4edit(myio::OLEDstream* myout, int i) {
	unsigned row = i % (usermax>>1),
			 col = i / (usermax>>1) * 11;
	
	myout->drawChr(row, col, '>');
	myout->drawChr(row, col+1, '>');
	myout->flush();
}

static void eraseCursor4edit(myio::OLEDstream* myout, int i) {
	unsigned row = i % (usermax>>1),
			 col = i / (usermax>>1) * 11;
	
	myout->drawChr(row, col, ' ');
	myout->drawChr(row, col+1, ' ');
	myout->flush();
}

static void display4edit(myio::OLEDstream* myout, const uint32_t* data) {
	myout->clear();

	unsigned half = usermax >> 1;
	for( int i = 0; i < half; ++i ) {
		myout->printf("  %08X   %08X\r\n", data[i], data[i+half]);
	}
	myout->flush();
}

static bool edit(myio::OLEDstream* myout, Keypad* kpd) {
	uint32_t userdata_cpy[usermax];
	memcpy(userdata_cpy, userdata, sizeof(userdata));

	dataSort(userdata_cpy, usermax);
	display4edit(myout, userdata_cpy);
	
	int i = 0;
	drawCursor4edit(myout, i);

	char key;
	bool sure;
	bool changed = false;
	while( true ) {
		if( Serial.available() ) {
			uint32_t uid;
			Serial.readBytes((char*)&uid, sizeof(uid));
			for( int i = 0; i < sizeof(uid)/2; ++i ) {
				byte tmp = ((byte*)&uid)[i];
				((byte*)&uid)[i] = ((byte*)&uid)[sizeof(uid)-i-1];
				((byte*)&uid)[sizeof(uid)-i-1] = tmp;
			}
			int idx = checkUserData(uid);
			Serial.println(idx);

			if( idx >= 0 ) {
				eraseCursor4edit(myout, i);
				i = idx;
			}
			else {
				bool sure = YorN(myout, kpd, "Rewrite this user?");
				if( sure ) {
					changed = true;
					userdata_cpy[i] = uid;
					dataSort(userdata_cpy, usermax);

					for( int p = 0; p < usermax; ++p ) {
						if( userdata_cpy[p] == uid ) {
							i = p;
							break;
						}
					}
				}
			}
			display4edit(myout, userdata_cpy);
			drawCursor4edit(myout, i);
		}

		switch( key = kpd->getKey() ) {
			case 'B':
				eraseCursor4edit(myout, i);
				if( ++i >= usermax ) {
					i = 0;
				}
				drawCursor4edit(myout, i);
				break;
			
			case 'A':
				eraseCursor4edit(myout, i);
				if( --i < 0 ) {
					i = usermax - 1;
				}
				drawCursor4edit(myout, i);
				break;
			
			case '#':
				sure = YorN(myout, kpd, "Delete this user?");
				if( sure ) {
					changed = true;
					userdata_cpy[i] = 0;
					dataSort(userdata_cpy, usermax);
				}
				display4edit(myout, userdata_cpy);
				drawCursor4edit(myout, i);
				break;
			
			case 'D':
				if( changed ) {
					changed = YorN(myout, kpd, "Save changes?");
					if( changed ) {
						memcpy(userdata, userdata_cpy, sizeof(userdata));
					}
				}
				myout->clear();
				return changed;
			
			default:
				break;
		}
	}
}


// ------------------------------ NVS ------------------------------
static void nvsSave(const uint32_t* data, size_t size) {
	Preferences nvs;
	nvs.begin(nvsname, false);
	nvs.putBytes(nvskey, data, size);
	nvs.end();
}

static void nvsLoad(uint32_t* data, size_t size) {
	Preferences nvs;
	nvs.begin(nvsname, true);
	nvs.getBytes(nvskey, data, size);
	nvs.end();
}


//------------------------------ Externs ------------------------------
void loadUserData() {
	nvsLoad(userdata, sizeof(userdata));
}

int checkUserData(uint32_t uid) {
	dataSort(userdata, usermax);
	for( int i = 0; i < usermax; ++i ) {
		if( userdata[i] == 0 ) {
			break;
		}
		if( userdata[i] == uid ) {
			return i;
		}
	}
	return -1;
}

void manage(Keypad* kpd) {
	myio::OLEDstream *myout = new myio::OLEDstream(pinnum, outpins);

	while( true ) {
		if( login(myout, kpd) ) {
			// Serial.println("Login successful!");
			break;
		}
		else {
			// Serial.println("Login failed!");
			myout->printf("\r\n\r\nLogin Failed!\r\n(Press 'C' to exit.)\f");

			char key;
			while( (key=kpd->getKey()) == NO_KEY ) {
				;
			}
			myout->clear();

			if( key == 'C' ) {
				goto LEV;
			}
		}
	}

	if( edit(myout, kpd) ) {
		nvsSave(userdata, sizeof(userdata));
	}


LEV:
	delete myout;
	return;
}

