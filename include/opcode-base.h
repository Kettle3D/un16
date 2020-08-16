/****************************************************************
*                        opcode-base.h                          *
* This file is the main header for the UN16 opcode interpreter. *
*   It is used to interpret executeables, kernel modules etc.   *
****************************************************************/

char  *last_string  =    "";
int    last_int     =     0;
float  last_float   =     0;
char   last_byte    =     0;
bool   last_boolean = false;

char *get_string(int location, char *contents) {
	char *returns = "";
	int address = location;
	nextchar:
	switch (contents[address]) {
		case 0x04: // EOT ~> This is the end of the stream
			return returns;
			break;
		default:   // This is a piece of data
			returns += contents[address];
			break;
	}
	address += 1;
	goto nextchar;
}

int get_int(int location, char *contents) {
	int digit1 = contents[location];
	int digit2 = contents[location + 1];
	return ((digit1 * 256) + digit2);
}

char get_byte(int location, char *contents) {
	return contents[location];
}

bool get_bool(int location, char *contents) {
	int b = contents[location];
	if (b % 2 == 0) {
		return false;
	}
	return true;
}

float get_float(int location, char *contents) {
	int digit1 = contents[location];
	int digit2 = contents[location + 1];
	return ((digit2 / 256) + digit1);
}

void run(char *file, char *args) {
	File mainBin = SD.open(file, FILE_READ);
	char *contents = mainBin.read();
	int address = 0;
	nextchar:
	if (address >= sizeof(contents)) {
		goto end;
	}
	switch (contents[address]) {
		case 0x21: // ! ~> exit
			goto end;
			break;
		case 0x22: // " ~> goto
			address = (contents[address + 1] * 256) +
			(contents[address + 2]);
			goto nextchar;
			break;
		case 0x23: // # ~> goto (1 byte)
			address = contents[address + 1];
			goto nextchar;
			break;
		case 0x24: // $ ~> get_string
			last_string = get_string((contents[address + 1] * 256) +
			(contents[address + 2]), contents);
			address += 2;
			break;
		case 0x25: // % ~> get_string (1 byte)
			last_string = get_string(contents[address + 1], contents);
			address += 1;
			break;
		case 0x26: // & ~> get_int
			last_int = get_int((contents[address + 1] * 256) +
			(contents[address + 2]), contents);
			address += 2;
			break;
		case 0x27: // ' ~> get_int (1 byte)
			last_int = get_int(contents[address + 1], contents);
			address += 1;
			break;
		case 0x28: // ( ~> get_float
			last_float = get_float((contents[address + 1] * 256) +
			(contents[address + 2]), contents);
			address += 2;
			break;
		case 0x29: // ) ~> get_float (1 byte)
			last_float = get_float(contents[address + 1], contents);
			address += 1;
			break;
		case 0x2A: // * ~> get_byte
			last_byte = get_byte((contents[address + 1] * 256) +
			(contents[address + 2]), contents);
			address += 2;
			break;
		case 0x2B: // + ~> get_byte (1 byte)
			last_byte = get_byte(contents[address + 1], contents);
			address += 1;
			break;
		case 0x2C: // , ~> get_bool
			last_boolean = get_bool((contents[address + 1] * 256) +
			(contents[address + 2]), contents);
			address += 2;
			break;
		case 0x2D: // - ~> get_bool (1 byte)
			last_boolean = get_bool(contents[address + 1], contents);
			address += 1;
			break;
		case 0x2E: // . ~> log
			File log = SD.open("/tmp/console.log", FILE_WRITE);
			log.println(last_string);
			break;
		case 0x2F: // / ~> if
			if (last_boolean) {
				address = (contents[address + 1] * 256) +
				(contents[address + 2]);
				goto nextchar;
			}
			address += 2;
			break;
		case 0x30: // 0 ~> else (basically IF NOT)
			if (last_boolean) {
				address += 2;
				break;
			}
			address = (contents[address + 1] * 256) +
			(contents[address + 2]);
			goto nextchar;
			break;
		case 0x31: // 1 ~> if (1 byte)
			if (last_boolean) {
				address = contents[address + 1];
				goto nextchar;
			}
			address += 1;
			break;
		case 0x32: // 2 ~> else (1 byte)
			if (last_boolean) {
				address += 1;
				break;
			}
			address = contents[address + 1]
			goto nextchar;
			break;
		case 0x33: // 3 ~> get_cond
			last_boolean = get_cond((contents[address + 1] * 256) +
			(contents[address + 2]), contents);
			address += 2;
			break;
		case 0x34: // 4 ~> get_cond (1 byte)
			last_boolean = get_cond(contents[address + 1], contents);
	}
	address += 1;
	goto nextchar;
	end:
}
