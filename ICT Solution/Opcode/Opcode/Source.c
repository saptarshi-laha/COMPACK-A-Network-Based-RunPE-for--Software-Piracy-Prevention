struct encodedInstruction {
	unsigned char code[16];
	unsigned char offset;
	struct encodedInstruction* next;
	struct encodedInstruction* prev;
};

int main() {

	struct encodedInstruction* currentInstruction;
	struct encodedInstruction* instructionSequence;
	struct encodedInstruction* firstInstruction;
	struct encodedInstruction instruction;
	currentInstruction = 0xCCCCCCCC;
	instructionSequence = 0xCCCCCCCC;
	firstInstruction = 0xCCCCCCCC;
	char* instructionPointer = 0x0040101B, * nextInstructionPointer = 0x0040101B;
	char total = 0;

	//2 gap for popfd popad 9D 61
	//16 for instruction
	//2 gap for pushad pushfd 60 9C
	//5 gap for JMP
	//2 gap for popfd popad
	//16 for instruction
	//2 gap for pushad pushfd
	
	while (currentInstruction) {

		if (currentInstruction->offset == 0x10) {
			__asm {
				mov eax, 0xCCCCCCCC
				mov ebx, esp
				sub ebx, 0x500
				mov[eax], ebx
				add eax, 4
				mov ebx, ebp
				sub ebx, 0x500
				mov[eax], ebx
				mov ebp, 0x0019FF80;
				mov esp, 0x0019FF74;
			}
		}
		else {
			__asm {
				mov eax, 0xCCCCCCCC
				mov[eax], esp
				add eax, 4
				mov[eax], ebp
				mov ebp, 0x0019FF80;
				mov esp, 0x0019FF74;
				sub esp, 0x224
					popad
					popfd
					add esp, 0x200

			}
		}

		instruction = *currentInstruction;

		int prime = 0;
		for (int i = 2; i < total; i++) {
			if (total % i == 0) {
				prime = 1;
			}
		}

		if (prime == 0) {
			for (int i = 0; i < 16; i++) {
				instruction.code[i] = instruction.code[0] - total;
			}
		}
		else if (prime == 1) {
			for (int i = 0; i < 16; i++) {
				instruction.code[i] = instruction.code[0] + total;
			}
		}

		memcpy(instructionPointer, instruction.code, 16);

		total = currentInstruction->code[0] + currentInstruction->code[1] + currentInstruction->code[2] + currentInstruction->code[3] + currentInstruction->code[4] + currentInstruction->code[5] +
			currentInstruction->code[6] + currentInstruction->code[7] + currentInstruction->code[8] + currentInstruction->code[9] + currentInstruction->code[10] + currentInstruction->code[11] +
			currentInstruction->code[12] + currentInstruction->code[13] + currentInstruction->code[14] + currentInstruction->code[15];



		if (currentInstruction->code[0] == 0xE8 || currentInstruction->code[0] == 0xE9) {
			if (instructionPointer == nextInstructionPointer) {
				int adjustedAddress = currentInstruction->code[4] * 0x1000000 + currentInstruction->code[3] * 0x10000 + currentInstruction->code[2] * 0x100 + currentInstruction->code[1];
				nextInstructionPointer = instructionPointer - 0x19;
				instructionPointer[1] = adjustedAddress & 0xFF;
				instructionPointer[2] = (adjustedAddress >> 8) & 0xFF;
				instructionPointer[3] = (adjustedAddress >> 16) & 0xFF;
				instructionPointer[4] = (adjustedAddress >> 24) & 0xFF;
				instructionSequence = currentInstruction;
				if (adjustedAddress >= 0) {
					while (adjustedAddress >= 0) {
						instructionSequence = instructionSequence->next;
						adjustedAddress = adjustedAddress - 0x10;
					}
				}
				else {
					while (adjustedAddress <= 0) {
						instructionSequence = instructionSequence->prev;
						adjustedAddress = adjustedAddress + 0x10;
					}
				}
				currentInstruction = instructionSequence;
				instructionSequence = firstInstruction;
				while (instructionSequence != currentInstruction) {

					int innertotal = instructionSequence->code[0] + instructionSequence->code[1] + instructionSequence->code[2] + instructionSequence->code[3] + instructionSequence->code[4] + instructionSequence->code[5] +
						instructionSequence->code[6] + instructionSequence->code[7] + instructionSequence->code[8] + instructionSequence->code[9] + instructionSequence->code[10] + instructionSequence->code[11] +
						instructionSequence->code[12] + instructionSequence->code[13] + instructionSequence->code[14] + instructionSequence->code[15];

					int prime = 0;
					for (int i = 2; i < innertotal; i++) {
						if (total % i == 0) {
							prime = 1;
						}
					}

					instruction = *instructionSequence->next;

					if (prime == 0) {
						for (int i = 0; i < 16; i++) {
							instruction.code[i] = instruction.code[0] - total;
						}
					}
					else if (prime == 1) {
						for (int i = 0; i < 16; i++) {
							instruction.code[i] = instruction.code[0] + total;
						}
					}

					instructionSequence = instructionSequence->next;

				}
				instruction = *currentInstruction;
				int prime = 0;
				for (int i = 2; i < total; i++) {
					if (total % i == 0) {
						prime = 1;
					}
				}
				if (prime == 0) {
					for (int i = 0; i < 16; i++) {
						instruction.code[i] = instruction.code[0] - total;
					}
				}
				else if (prime == 1) {
					for (int i = 0; i < 16; i++) {
						instruction.code[i] = instruction.code[0] + total;
					}
				}
				memcpy(nextInstructionPointer, instruction.code, 16);
				total = currentInstruction->code[0] + currentInstruction->code[1] + currentInstruction->code[2] + currentInstruction->code[3] + currentInstruction->code[4] + currentInstruction->code[5] +
					currentInstruction->code[6] + currentInstruction->code[7] + currentInstruction->code[8] + currentInstruction->code[9] + currentInstruction->code[10] + currentInstruction->code[11] +
					currentInstruction->code[12] + currentInstruction->code[13] + currentInstruction->code[14] + currentInstruction->code[15];
			}
			else {
				int adjustedAddress = currentInstruction->code[4] * 0x1000000 + currentInstruction->code[3] * 0x10000 + currentInstruction->code[2] * 0x100 + currentInstruction->code[1];
				nextInstructionPointer = instructionPointer + 0x19;
				instructionPointer[1] = adjustedAddress & 0xFF;
				instructionPointer[2] = (adjustedAddress >> 8) & 0xFF;
				instructionPointer[3] = (adjustedAddress >> 16) & 0xFF;
				instructionPointer[4] = (adjustedAddress >> 24) & 0xFF;
				instructionSequence = currentInstruction;
				if (adjustedAddress >= 0) {
					while (adjustedAddress >= 0) {
						instructionSequence = instructionSequence->next;
						adjustedAddress = adjustedAddress - 0x10;
					}
				}
				else {
					while (adjustedAddress <= 0) {
						instructionSequence = instructionSequence->prev;
						adjustedAddress = adjustedAddress + 0x10;
					}
				}
				currentInstruction = instructionSequence;
				instructionSequence = firstInstruction;
				while (instructionSequence != currentInstruction) {

					int innertotal = instructionSequence->code[0] + instructionSequence->code[1] + instructionSequence->code[2] + instructionSequence->code[3] + instructionSequence->code[4] + instructionSequence->code[5] +
						instructionSequence->code[6] + instructionSequence->code[7] + instructionSequence->code[8] + instructionSequence->code[9] + instructionSequence->code[10] + instructionSequence->code[11] +
						instructionSequence->code[12] + instructionSequence->code[13] + instructionSequence->code[14] + instructionSequence->code[15];

					int prime = 0;
					for (int i = 2; i < innertotal; i++) {
						if (total % i == 0) {
							prime = 1;
						}
					}

					instruction = *instructionSequence->next;

					if (prime == 0) {
						for (int i = 0; i < 16; i++) {
							instruction.code[i] = instruction.code[0] - total;
						}
					}
					else if (prime == 1) {
						for (int i = 0; i < 16; i++) {
							instruction.code[i] = instruction.code[0] + total;
						}
					}

					instructionSequence = instructionSequence->next;

				}
				instruction = *currentInstruction;
				int prime = 0;
				for (int i = 2; i < total; i++) {
					if (total % i == 0) {
						prime = 1;
					}
				}
				if (prime == 0) {
					for (int i = 0; i < 16; i++) {
						instruction.code[i] = instruction.code[0] - total;
					}
				}
				else if (prime == 1) {
					for (int i = 0; i < 16; i++) {
						instruction.code[i] = instruction.code[0] + total;
					}
				}
				memcpy(nextInstructionPointer, instruction.code, 16);
				total = currentInstruction->code[0] + currentInstruction->code[1] + currentInstruction->code[2] + currentInstruction->code[3] + currentInstruction->code[4] + currentInstruction->code[5] +
					currentInstruction->code[6] + currentInstruction->code[7] + currentInstruction->code[8] + currentInstruction->code[9] + currentInstruction->code[10] + currentInstruction->code[11] +
					currentInstruction->code[12] + currentInstruction->code[13] + currentInstruction->code[14] + currentInstruction->code[15];
			}
		}
		else if (currentInstruction->code[0] == 0x0F && (currentInstruction->code[1] >= 0x80 && currentInstruction->code[1] <= 0x8F)) {
			if (instructionPointer == nextInstructionPointer) {
				int adjustedAddress = currentInstruction->code[5] * 0x1000000 + currentInstruction->code[4] * 0x10000 + currentInstruction->code[3] * 0x100 + currentInstruction->code[2];
				nextInstructionPointer = instructionPointer - 0x19;
				instructionPointer[2] = adjustedAddress & 0xFF;
				instructionPointer[3] = (adjustedAddress >> 8) & 0xFF;
				instructionPointer[4] = (adjustedAddress >> 16) & 0xFF;
				instructionPointer[5] = (adjustedAddress >> 24) & 0xFF;
				instructionSequence = currentInstruction;
				if (adjustedAddress >= 0) {
					while (adjustedAddress >= 0) {
						instructionSequence = instructionSequence->next;
						adjustedAddress = adjustedAddress - 0x10;
					}
				}
				else {
					while (adjustedAddress <= 0) {
						instructionSequence = instructionSequence->prev;
						adjustedAddress = adjustedAddress + 0x10;
					}
				}
				currentInstruction = instructionSequence;
				instructionSequence = firstInstruction;
				while (instructionSequence != currentInstruction) {

					int innertotal = instructionSequence->code[0] + instructionSequence->code[1] + instructionSequence->code[2] + instructionSequence->code[3] + instructionSequence->code[4] + instructionSequence->code[5] +
						instructionSequence->code[6] + instructionSequence->code[7] + instructionSequence->code[8] + instructionSequence->code[9] + instructionSequence->code[10] + instructionSequence->code[11] +
						instructionSequence->code[12] + instructionSequence->code[13] + instructionSequence->code[14] + instructionSequence->code[15];

					int prime = 0;
					for (int i = 2; i < innertotal; i++) {
						if (total % i == 0) {
							prime = 1;
						}
					}

					instruction = *instructionSequence->next;

					if (prime == 0) {
						for (int i = 0; i < 16; i++) {
							instruction.code[i] = instruction.code[0] - total;
						}
					}
					else if (prime == 1) {
						for (int i = 0; i < 16; i++) {
							instruction.code[i] = instruction.code[0] + total;
						}
					}

					instructionSequence = instructionSequence->next;

				}
				instruction = *currentInstruction;
				int prime = 0;
				for (int i = 2; i < total; i++) {
					if (total % i == 0) {
						prime = 1;
					}
				}
				if (prime == 0) {
					for (int i = 0; i < 16; i++) {
						instruction.code[i] = instruction.code[0] - total;
					}
				}
				else if (prime == 1) {
					for (int i = 0; i < 16; i++) {
						instruction.code[i] = instruction.code[0] + total;
					}
				}
				memcpy(nextInstructionPointer, instruction.code, 16);
				total = currentInstruction->code[0] + currentInstruction->code[1] + currentInstruction->code[2] + currentInstruction->code[3] + currentInstruction->code[4] + currentInstruction->code[5] +
					currentInstruction->code[6] + currentInstruction->code[7] + currentInstruction->code[8] + currentInstruction->code[9] + currentInstruction->code[10] + currentInstruction->code[11] +
					currentInstruction->code[12] + currentInstruction->code[13] + currentInstruction->code[14] + currentInstruction->code[15];
			}
			else {
				int adjustedAddress = currentInstruction->code[5] * 0x1000000 + currentInstruction->code[4] * 0x10000 + currentInstruction->code[3] * 0x100 + currentInstruction->code[2];
				nextInstructionPointer = instructionPointer + 0x19;
				instructionPointer[2] = adjustedAddress & 0xFF;
				instructionPointer[3] = (adjustedAddress >> 8) & 0xFF;
				instructionPointer[4] = (adjustedAddress >> 16) & 0xFF;
				instructionPointer[5] = (adjustedAddress >> 24) & 0xFF;
				instructionSequence = currentInstruction;
				if (adjustedAddress >= 0) {
					while (adjustedAddress >= 0) {
						instructionSequence = instructionSequence->next;
						adjustedAddress = adjustedAddress - 0x10;
					}
				}
				else {
					while (adjustedAddress <= 0) {
						instructionSequence = instructionSequence->prev;
						adjustedAddress = adjustedAddress + 0x10;
					}
				}
				currentInstruction = instructionSequence;
				instructionSequence = firstInstruction;
				while (instructionSequence != currentInstruction) {

					int innertotal = instructionSequence->code[0] + instructionSequence->code[1] + instructionSequence->code[2] + instructionSequence->code[3] + instructionSequence->code[4] + instructionSequence->code[5] +
						instructionSequence->code[6] + instructionSequence->code[7] + instructionSequence->code[8] + instructionSequence->code[9] + instructionSequence->code[10] + instructionSequence->code[11] +
						instructionSequence->code[12] + instructionSequence->code[13] + instructionSequence->code[14] + instructionSequence->code[15];

					int prime = 0;
					for (int i = 2; i < innertotal; i++) {
						if (total % i == 0) {
							prime = 1;
						}
					}

					instruction = *instructionSequence->next;

					if (prime == 0) {
						for (int i = 0; i < 16; i++) {
							instruction.code[i] = instruction.code[0] - total;
						}
					}
					else if (prime == 1) {
						for (int i = 0; i < 16; i++) {
							instruction.code[i] = instruction.code[0] + total;
						}
					}

					instructionSequence = instructionSequence->next;

				}
				instruction = *currentInstruction;
				int prime = 0;
				for (int i = 2; i < total; i++) {
					if (total % i == 0) {
						prime = 1;
					}
				}
				if (prime == 0) {
					for (int i = 0; i < 16; i++) {
						instruction.code[i] = instruction.code[0] - total;
					}
				}
				else if (prime == 1) {
					for (int i = 0; i < 16; i++) {
						instruction.code[i] = instruction.code[0] + total;
					}
				}
				memcpy(nextInstructionPointer, instruction.code, 16);
				total = currentInstruction->code[0] + currentInstruction->code[1] + currentInstruction->code[2] + currentInstruction->code[3] + currentInstruction->code[4] + currentInstruction->code[5] +
					currentInstruction->code[6] + currentInstruction->code[7] + currentInstruction->code[8] + currentInstruction->code[9] + currentInstruction->code[10] + currentInstruction->code[11] +
					currentInstruction->code[12] + currentInstruction->code[13] + currentInstruction->code[14] + currentInstruction->code[15];
			}
		}
		else if (currentInstruction->code[0] == 0x83 && currentInstruction->code[3] == 0x0F) {
			if (instructionPointer == nextInstructionPointer) {
				int adjustedAddress = currentInstruction->code[8] * 0x1000000 + currentInstruction->code[7] * 0x10000 + currentInstruction->code[6] * 0x100 + currentInstruction->code[5];
				nextInstructionPointer = instructionPointer - 0x19;
				instructionPointer[5] = adjustedAddress & 0xFF;
				instructionPointer[6] = (adjustedAddress >> 8) & 0xFF;
				instructionPointer[7] = (adjustedAddress >> 16) & 0xFF;
				instructionPointer[8] = (adjustedAddress >> 24) & 0xFF;
				instructionSequence = currentInstruction;
				if (adjustedAddress >= 0) {
					while (adjustedAddress >= 0) {
						instructionSequence = instructionSequence->next;
						adjustedAddress = adjustedAddress - 0x10;
					}
				}
				else {
					while (adjustedAddress <= 0) {
						instructionSequence = instructionSequence->prev;
						adjustedAddress = adjustedAddress + 0x10;
					}
				}
				currentInstruction = instructionSequence;
				instructionSequence = firstInstruction;
				while (instructionSequence != currentInstruction) {

					int innertotal = instructionSequence->code[0] + instructionSequence->code[1] + instructionSequence->code[2] + instructionSequence->code[3] + instructionSequence->code[4] + instructionSequence->code[5] +
						instructionSequence->code[6] + instructionSequence->code[7] + instructionSequence->code[8] + instructionSequence->code[9] + instructionSequence->code[10] + instructionSequence->code[11] +
						instructionSequence->code[12] + instructionSequence->code[13] + instructionSequence->code[14] + instructionSequence->code[15];

					int prime = 0;
					for (int i = 2; i < innertotal; i++) {
						if (total % i == 0) {
							prime = 1;
						}
					}

					instruction = *instructionSequence->next;

					if (prime == 0) {
						for (int i = 0; i < 16; i++) {
							instruction.code[i] = instruction.code[0] - total;
						}
					}
					else if (prime == 1) {
						for (int i = 0; i < 16; i++) {
							instruction.code[i] = instruction.code[0] + total;
						}
					}

					instructionSequence = instructionSequence->next;

				}
				instruction = *currentInstruction;
				int prime = 0;
				for (int i = 2; i < total; i++) {
					if (total % i == 0) {
						prime = 1;
					}
				}
				if (prime == 0) {
					for (int i = 0; i < 16; i++) {
						instruction.code[i] = instruction.code[0] - total;
					}
				}
				else if (prime == 1) {
					for (int i = 0; i < 16; i++) {
						instruction.code[i] = instruction.code[0] + total;
					}
				}
				memcpy(nextInstructionPointer, instruction.code, 16);
				total = currentInstruction->code[0] + currentInstruction->code[1] + currentInstruction->code[2] + currentInstruction->code[3] + currentInstruction->code[4] + currentInstruction->code[5] +
					currentInstruction->code[6] + currentInstruction->code[7] + currentInstruction->code[8] + currentInstruction->code[9] + currentInstruction->code[10] + currentInstruction->code[11] +
					currentInstruction->code[12] + currentInstruction->code[13] + currentInstruction->code[14] + currentInstruction->code[15];
			}
			else {
				int adjustedAddress = currentInstruction->code[8] * 0x1000000 + currentInstruction->code[7] * 0x10000 + currentInstruction->code[6] * 0x100 + currentInstruction->code[5];
				nextInstructionPointer = instructionPointer + 0x19;
				instructionPointer[5] = adjustedAddress & 0xFF;
				instructionPointer[6] = (adjustedAddress >> 8) & 0xFF;
				instructionPointer[7] = (adjustedAddress >> 16) & 0xFF;
				instructionPointer[8] = (adjustedAddress >> 24) & 0xFF;
				instructionSequence = currentInstruction;
				if (adjustedAddress >= 0) {
					while (adjustedAddress >= 0) {
						instructionSequence = instructionSequence->next;
						adjustedAddress = adjustedAddress - 0x10;
					}
				}
				else {
					while (adjustedAddress <= 0) {
						instructionSequence = instructionSequence->prev;
						adjustedAddress = adjustedAddress + 0x10;
					}
				}
				currentInstruction = instructionSequence;
				instructionSequence = firstInstruction;
				while (instructionSequence != currentInstruction) {

					int innertotal = instructionSequence->code[0] + instructionSequence->code[1] + instructionSequence->code[2] + instructionSequence->code[3] + instructionSequence->code[4] + instructionSequence->code[5] +
						instructionSequence->code[6] + instructionSequence->code[7] + instructionSequence->code[8] + instructionSequence->code[9] + instructionSequence->code[10] + instructionSequence->code[11] +
						instructionSequence->code[12] + instructionSequence->code[13] + instructionSequence->code[14] + instructionSequence->code[15];

					int prime = 0;
					for (int i = 2; i < innertotal; i++) {
						if (total % i == 0) {
							prime = 1;
						}
					}

					instruction = *instructionSequence->next;

					if (prime == 0) {
						for (int i = 0; i < 16; i++) {
							instruction.code[i] = instruction.code[0] - total;
						}
					}
					else if (prime == 1) {
						for (int i = 0; i < 16; i++) {
							instruction.code[i] = instruction.code[0] + total;
						}
					}

					instructionSequence = instructionSequence->next;

				}
				instruction = *currentInstruction;
				int prime = 0;
				for (int i = 2; i < total; i++) {
					if (total % i == 0) {
						prime = 1;
					}
				}
				if (prime == 0) {
					for (int i = 0; i < 16; i++) {
						instruction.code[i] = instruction.code[0] - total;
					}
				}
				else if (prime == 1) {
					for (int i = 0; i < 16; i++) {
						instruction.code[i] = instruction.code[0] + total;
					}
				}
				memcpy(nextInstructionPointer, instruction.code, 16);
				total = currentInstruction->code[0] + currentInstruction->code[1] + currentInstruction->code[2] + currentInstruction->code[3] + currentInstruction->code[4] + currentInstruction->code[5] +
					currentInstruction->code[6] + currentInstruction->code[7] + currentInstruction->code[8] + currentInstruction->code[9] + currentInstruction->code[10] + currentInstruction->code[11] +
					currentInstruction->code[12] + currentInstruction->code[13] + currentInstruction->code[14] + currentInstruction->code[15];
			}
		}
		else if ((currentInstruction->code[0] == 0xC3 || currentInstruction->code[0] == 0xCB) && currentInstruction->code[1] == 0x90) {
			int adjustedAddress = instructionPointer;
			instructionPointer[0] = 0xE9;
			instructionPointer[4] = adjustedAddress & 0xFF;
			instructionPointer[3] = (adjustedAddress >> 8) & 0xFF;
			instructionPointer[2] = (adjustedAddress >> 16) & 0xFF;
			instructionPointer[1] = (adjustedAddress >> 24) & 0xFF;
		}
		else {
			instructionPointer = 0x00401000;
			nextInstructionPointer = 0x00401000;
		}

		currentInstruction = currentInstruction->next;

		/**__asm {
			sub esp, 0x200
			pushfd
			pushad
			mov esp, 0x0019FF74;
			mov eax, instructionPointer
			mov ebx, 0xCCCCCCCC
			mov esp, [ebx]
			add ebx, 4
			mov ebp, [ebx]
			jmp eax
		}**/
	}


}