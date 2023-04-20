#include "cpu.hpp"
	

uint8_t memory::A{};
uint8_t memory::B{};
uint8_t memory::C{};
uint8_t memory::D{};
uint8_t memory::E{};
uint8_t memory::H{};
uint8_t memory::L{};
uint16_t memory::PC{};
uint16_t memory::SP{}; 

std::stack<uint8_t> memory::stack{}; // Stack of CPU

bool cpu_handler::handle_instructions() {
	for (auto instruction : tuple_instructions) {
		try {
			auto ptr = std::get<1>(instruction);
			auto param = std::get<2>(instruction);
			ptr(param);

		} catch (...) {
			exception::invalid_asm();
		}
	}
	return true;
}

bool cpu_instructions::nop() { return true; }

void cpu_instructions::mvi(uint8_t val, uint8_t &reg) {
	reg = val;
}
