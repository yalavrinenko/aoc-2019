//
// Created by yalavrinenko on 07.12.2019.
//

#ifndef _INSTRUCTION_HPP
#define _INSTRUCTION_HPP

#include <array>
#include <functional>
#include <map>
#include <memory>
#include <tuple>

enum class Mode { position = 0, immediate = 1, relative = 2 };

template <typename TInstruction, typename TMemory> struct Instruction {
  template <size_t argc>
  static std::array<TInstruction*, argc>
  operands(TMemory &mem, std::array<Mode, argc> const &mode, size_t ip) {
    std::array<TInstruction*, argc> argv;
    auto i = 1;
    for (auto &arg : argv) {
      arg = mem.get(mode[i - 1], ip + i);
      ++i;
    }
    return argv;
  }

  virtual long execute(TMemory &mem, size_t ip) = 0;
};

template <typename TInstruction, typename TMemory, size_t argc,
          typename Function>
struct InstructionTemplate : public Instruction<TInstruction, TMemory> {
  InstructionTemplate(Function func) : func_{std::move(func)} {}

  template <std::size_t... I>
  long call(TMemory &mem, std::array<TInstruction*, argc> const &operands,
              std::index_sequence<I...>) {
    return func_(mem, operands[I]...);
  }

  template <typename Index = std::make_index_sequence<argc>>
  long call(TMemory &mem, std::array<TInstruction*, argc> const &operands) {
    return call(mem, operands, Index{});
  }

  long execute(TMemory &mem, size_t ip) override {
    auto mode = mem.template operand_modes<argc>(ip);
    auto operands =
        Instruction<TInstruction, TMemory>::template operands(mem, mode, ip);
    return call(mem, operands);
  }

protected:
  Function func_;
};

template <typename TInstruction, typename TMemory> class Architecture {
public:
  static size_t exec_instruction(TMemory &mem, size_t ip) {
    auto op = mem.instruction(ip);
    return instructions()[op]->execute(mem, ip);
  }

  template <size_t argc, typename Function>
  using InstructionType =
      InstructionTemplate<TInstruction, TMemory, argc, Function>;

  using TernaryFunction = std::function<long(TMemory &, TInstruction*, TInstruction*, TInstruction*)>;
  using BinaryFunction = std::function<long(TMemory &, TInstruction*, TInstruction*)>;
  using UnaryFunction = std::function<long(TMemory &, TInstruction*)>;
  using ZeroFunction = std::function<long(TMemory &)>;

  static constexpr std::map<TInstruction,
                            std::shared_ptr<Instruction<TInstruction, TMemory>>>
  instructions();
};

template <typename TInstruction, typename TMemory>
constexpr std::map<TInstruction,
                   std::shared_ptr<Instruction<TInstruction, TMemory>>>
Architecture<TInstruction, TMemory>::instructions() {
  return {{99, std::make_shared<InstructionType<0, ZeroFunction>>(
                   [](auto &mem) { return 0; })},
          {1, std::make_shared<InstructionType<3, TernaryFunction>>(
                  [](auto &mem, TInstruction* op1, TInstruction* op2, TInstruction* op3) {
                    *op3 = *op1 + *op2;
                    return 4;
                  })},
          {2, std::make_shared<InstructionType<3, TernaryFunction>>(
                  [](auto &mem, TInstruction* op1, TInstruction* op2, TInstruction* op3) {
                    *op3 = *op1 * *op2;
                    return 4;
                  })},
          {3, std::make_shared<InstructionType<1, UnaryFunction>>(
                  [](auto &mem, TInstruction* op1) {
                    *op1 = mem.input();
                    return 2;
                  })},
          {4, std::make_shared<InstructionType<1, UnaryFunction>>(
                  [](auto &mem, TInstruction* op1) {
                    mem.output(*op1);
                    return 2;
                  })},
          {5, std::make_shared<InstructionType<2, BinaryFunction>>(
                  [](auto &mem, TInstruction* op1, TInstruction* op2) {
                    if (*op1)
                      return *op2 - mem.ip();
                    else
                      return 3l;
                  })},
          {6, std::make_shared<InstructionType<2, BinaryFunction>>(
                  [](auto &mem, TInstruction* op1, TInstruction* op2) {
                    if (*op1 == 0) {
                      return *op2 - mem.ip();
                    }
                    return 3l;
                  })},
          {7, std::make_shared<InstructionType<3, TernaryFunction>>(
                  [](auto &mem, TInstruction* op1, TInstruction* op2, TInstruction* op3) {
                    if (*op1 < *op2)
                      *op3 = 1;
                    else
                      *op3 = 0;
                    return 4;
                  })},
          {8, std::make_shared<InstructionType<3, TernaryFunction>>(
                  [](auto &mem, TInstruction* op1, TInstruction* op2, TInstruction* op3) {
                    if (*op1 == *op2)
                      *op3 = 1;
                    else
                      *op3 = 0;
                    return 4;
                  })},
          {9, std::make_shared<InstructionType<1, UnaryFunction >>(
              [](auto &mem, TInstruction* op1){
                mem.relative_base() += *op1;
                return 2;
              }
              )}};
}

#endif //_INSTRUCTION_HPP
