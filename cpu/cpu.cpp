#include "cpu.hpp"

uint8_t memory::A{};
uint8_t memory::B{};
uint8_t memory::C{};
uint8_t memory::D{};
uint8_t memory::E{};
uint8_t memory::H{};
uint8_t memory::L{};
uint16_t memory::PC{};
uint16_t memory::SP{0xFFFF}; // stack grows from down to upper

std::stack<uint8_t> memory::stack{}; // Stack of CPU

bool cpu_handler::handle_instructions() {
	if (tuple_instructions.empty())
		exception::invalid_format(); // why would you run an empty executeable?
	for (; memory::PC <= tuple_instructions.rbegin()->first;) {
		try {
			uint16_t previous_pc = memory::PC;
			auto instruction = tuple_instructions[memory::PC];
			auto ptr = std::get<1>(instruction);
			auto param = std::get<2>(instruction);
			ptr(param);
			if (memory::PC == previous_pc)
				memory::PC += (std::get<3>(instruction) + 1);

		} catch (...) {
			exception::invalid_asm();
		}
	}
	return true;
}

bool cpu_instructions::nop() { return true; }

void cpu_instructions::mvi(uint8_t val, uint8_t &reg) { reg = val; }

void cpu_instructions::push(uint8_t &reg) {
	memory::stack.push(reg);
	memory::SP--;
	reg = 0;
}

void cpu_instructions::pop(uint8_t &reg) {
	reg = memory::stack.top();
	memory::stack.pop();
	memory::SP++;
}

void cpu_instructions::jmp(uint8_t &addr) { memory::PC = addr; }

void cpu_instructions::call(uint8_t &addr) {
	memory::stack.push(memory::PC + 3); // we dont wanna have a call loop
	memory::PC = addr;
}

void cpu_instructions::ret() {
	if (memory::stack.empty())
		return; // Issue here : function gets invoked twice, so it will result in a segfault
	memory::PC = memory::stack.top();
	memory::stack.pop();
}
