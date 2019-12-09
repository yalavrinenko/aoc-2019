//
// Created by yalavrinenko on 07.12.2019.
//

#ifndef _INTCOMPUTER_HPP
#define _INTCOMPUTER_HPP

#include "instruction.hpp"
#include <vector>
#include <sstream>
#include <queue>

long convert(std::string const &s){
  return std::stol(s);
}

template <typename InstructionType>
class Memory: public std::unordered_map<InstructionType, InstructionType>{
public:
  [[nodiscard]] std::string operator_string(size_t ip) const { return std::to_string(this->at(ip)); }

  template <size_t argc>
  std::array<Mode, argc> operand_modes(size_t ip) const {
    std::string sop = operator_string(ip);
    std::array<Mode, argc> modes{};
    if (sop.size() > 2){
      auto j = 0;
      for (int i = sop.size() - 3; i >= 0; --i){
        modes[j++] = Mode(sop[i] - 0x30);
      }
    }

    return modes;
  }

  [[nodiscard]] InstructionType instruction(size_t ip) const {
    ip_ = ip;
    auto sop = operator_string(ip);
    if (sop.size() > 2)
      sop = sop.substr(sop.size() - 2);
    return convert(sop);
  }

  InstructionType* get(Mode mode, size_t ip){
    if (mode == Mode::immediate)
      return &(this->operator[](ip));
    if (mode == Mode::position)
      return &(this->operator[](this->operator[](ip)));
    if (mode == Mode::relative)
      return &(this->operator[](relative_base() + this->operator[](ip)));
    return nullptr;
  }

  InstructionType input() { auto v = input_.front(); input_.pop(); return v; }

  void input(long v) { input_.push(v); }

  InstructionType output() { auto v = output_.front(); output_.pop(); return v; }

  void output(long v) { output_.push(v); if (this->output_cb_) output_cb_(v); }

  bool have_out() const { return !output_.empty(); }

  long ip() const { return ip_; }

  long& relative_base() {
    return relative_base_;
  }

  std::function<void(long)> output_cb_ = nullptr;
protected:
  long relative_base_ = 0;
  std::queue<InstructionType> input_{};
  std::queue<InstructionType> output_{};
  mutable long ip_{0};
};

class IntComputer{
public:
  explicit IntComputer(std::string const &code){
    std::istringstream iss(code);
    std::string op;
    auto addr = 0;
    while (std::getline(iss, op, ',')){
      memory_[addr] = std::stol(op);
      ++addr;
    }
  }

  Memory<long>& mem() { return memory_; }

  int run() {
    auto ip = 0;
    auto code = 0;
    do{
      code = next_instruction();
    } while (code != -1);

    return code;
  }

  int next_instruction(){
    auto inc = Architecture<long, Memory<long>>::exec_instruction(memory_, ip_);
    if (inc == 0)
      return -1;

    ip_ += inc;
    return 0;
  }

protected:
  Memory<long> memory_;
  long ip_{0};
};

#endif //_INTCOMPUTER_HPP
