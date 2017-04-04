// Author: Mike Little
#include <iostream>
#include <cstring>
#include <string>
#include <cctype>
#include <cstdlib>
#include <bitset>
#include <map>
#include <vector>
#include <iomanip>

const unsigned MAXREG = 32;

struct reg {
	char name[5];
	unsigned data;
};

reg regs[MAXREG];

void init_r() {
	strcpy(regs[0].name,"$zero");
    strcpy(regs[1].name,"$at");
    strcpy(regs[2].name,"$v0");
    strcpy(regs[3].name,"$v1");
    strcpy(regs[4].name,"$a0");
    strcpy(regs[5].name,"$a1");
    strcpy(regs[6].name,"$a2");
    strcpy(regs[7].name,"$a3");
    strcpy(regs[8].name,"$t0");
    strcpy(regs[9].name,"$t1");
    strcpy(regs[10].name,"$t2");
    strcpy(regs[11].name,"$t3");
    strcpy(regs[12].name,"$t4");
    strcpy(regs[13].name,"$t5");
    strcpy(regs[14].name,"$t6");
    strcpy(regs[15].name,"$t7");
    strcpy(regs[16].name,"$s0");
    strcpy(regs[17].name,"$s1");
    strcpy(regs[18].name,"$s2");
    strcpy(regs[19].name,"$s3");
    strcpy(regs[20].name,"$s4");
    strcpy(regs[21].name,"$s5");
    strcpy(regs[22].name,"$s6");
    strcpy(regs[23].name,"$s7");
    strcpy(regs[24].name,"$t8");
    strcpy(regs[25].name,"$t9");
    strcpy(regs[26].name,"$k0");
    strcpy(regs[27].name,"$k1");
    strcpy(regs[28].name,"$gp");
    strcpy(regs[29].name,"$sp");
    strcpy(regs[30].name,"$fp");
    strcpy(regs[31].name,"$ra");

    for (int i = 0; i < MAXREG; i++) {
    	regs[i].data = 0;
    }
}

int getRegNum(char *name) {
	int i;
	bool is_num = 1;

	for (int j = 0; j < strlen(name); j++) {
		if (!isdigit(name[j])) {
			is_num = 0;
			break;
		}
	}

	if (is_num) {
		i = atoi(name);
		if (i >= 0 && i <= 31)
			return i;
	}
	else {
		for (int j = 0; j < MAXREG; j++) {
			if (!strcmp(regs[j].name, name)) {
				return j;
			}
		}
	}

	// if it gets here sometings wrong, print and return error code
	std::cout << "invalid register name\n"; 
	return -1;
}


bool isNum(char *str) {
	if (strlen(str) != 0) {
		if (str[0] == '-' || isdigit(str[0])) {
			for (int j = 1; j < strlen(str); j++) {
				if (!isdigit(str[j])) {
					return false;
				}
			}
			return true;
		} else
			return false;
	} else
		return false;
}


int main() {
	init_r();
	
	char input[100];
	unsigned PC = 0;
	unsigned hi = 0, lo = 0;

	std::cout << "Welcome!\nType 'regs' to view the contents of the registers.\nCurrently only arithmatic operations are supported.\n";

	while(1)
	{

		std::vector<char *> tokens;
		std::cout << "TEXT:0x" << std::hex << (PC*4 + 0x00400000) << std::dec << " > ";
		std::cin.getline(input, 100);

		char * token;

		token = strtok(input, " \t,");

		// get tokens
		while ((token != NULL)) {
			char *temp = new char[strlen(token) + 1];
			memcpy(temp, token, strlen(token) + 1);
			tokens.push_back(temp);

			token = strtok(NULL, " \t,");
		}

		// parse instruction and compute
		if ((!strcmp(tokens[0], "add") || !strcmp(tokens[0], "sub") || !strcmp(tokens[0], "div") ||
			 !strcmp(tokens[0], "mul")) && tokens.size() == 4) {
			unsigned rd, rs, rt;

			if ((rd = getRegNum(tokens[1])) != -1 && (rs = getRegNum(tokens[2])) != -1
				&& (rt = getRegNum(tokens[3])) != -1) {
				if (!strcmp(tokens[0], "add")) {
					regs[rd].data = regs[rs].data + regs[rt].data;
					PC++;
				}
				else if (!strcmp(tokens[0], "sub")) {
					regs[rd].data = regs[rs].data - regs[rt].data;
					PC++;
				}
				else if (!strcmp(tokens[0], "div")) {
					regs[rd].data = regs[rs].data / regs[rt].data;
					PC++;
				}
				else if (!strcmp(tokens[0], "mul")) {
					regs[rd].data = regs[rs].data * regs[rt].data;
					PC++;
				}
			}
			else {
				std::cout << "Oops! Your sentax was incorrect.  Try again.\n";
			}
		} else if (!strcmp(tokens[0], "addi") || !strcmp(tokens[0], "subi") && tokens.size() == 4) {
			unsigned rd, rs, rt;

			if ((rd = getRegNum(tokens[1])) != -1 && (rs = getRegNum(tokens[2])) != -1
				&& isNum(tokens[3])) {
				if (!strcmp(tokens[0], "addi")) {
					regs[rd].data = regs[rs].data + atoi(tokens[3]);
					PC++;
				}
				else if (!strcmp(tokens[0], "subi")) {
					regs[rd].data = regs[rs].data - atoi(tokens[3]);
					PC++;
				}
			} else {
				std::cout << "Oops! Your sentax was incorrect.  Try again.\n";
			}
		} else if (!strcmp(tokens[0], "mult") || !strcmp(tokens[0], "div") 
				|| !strcmp(tokens[0], "move") && tokens.size() == 3) {
			unsigned rs, rt;

			if ((rs = getRegNum(tokens[1])) != -1 && (rt = getRegNum(tokens[2])) != -1) {
				if (!strcmp(tokens[0], "mult")) {
					lo = regs[rs].data * regs[rt].data;
					std::cout << hi << ' ' << lo << '\n';
					PC++;
				} else if (!strcmp(tokens[0], "div")) {
					lo = regs[rs].data / regs[rt].data;
					hi = regs[rs].data % regs[rt].data;
					PC++;
				} else if (!strcmp(tokens[0], "move")) {
					regs[rs].data = regs[rt].data;
					PC++;
				}
			} else {
				std::cout << "Oops! Your sentax was incorrect.  Try again.\n";
			}
		} else if (!strcmp(tokens[0], "mflo") || !strcmp(tokens[0], "mfhi") && tokens.size() == 2) {
			unsigned rd;

			if ((rd = getRegNum(tokens[1])) != -1) {
				if (!strcmp(tokens[0], "mflo")) {
					regs[rd].data = lo;
					PC++;
				} else if (!strcmp(tokens[0], "mfhi")) {
					regs[rd].data = hi;
					PC++;
				}
			} else {
				std::cout << "Oops! Your sentax was incorrect.  Try again.\n";
			}
		} else if (!strcmp(tokens[0], "li") && tokens.size() == 3) {
			unsigned rd, rs;

			if ((rd = getRegNum(tokens[1])) != -1 && isNum(tokens[2])) {
				if (!strcmp(tokens[0], "li")) {
					regs[rd].data = atoi(tokens[2]);
					PC++;
				}
			} else {
				std::cout << "Oops! Your sentax was incorrect.  Try again.\n";
			}
		} else if (!strcmp(tokens[0], "regs") && tokens.size() == 1) {
			std::cout << "REGS: (reg # : data\n";
			for (int i = 0; i < MAXREG; i += 4) {
				std::cout << std::setw(2) << std::dec << i << " : " << regs[i].data << " | " << std::setw(2) << (i + 1) 
						  << " : " << regs[i + 1].data << " | " << std::setw(2) << (i + 2) 
						  << " : " << regs[i + 2].data << " | " << std::setw(2) << (i + 3) 
						  << " : " << regs[i + 3].data <<'\n';
			}
		}

		for (int i = 0; i < tokens.size(); i++)
			delete tokens[i];
	}

}
