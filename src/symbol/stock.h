#ifndef SRC_STOCK_SYMBOL_H_
#define SRC_STOCK_SYMBOL_H_

#include <unordered_map>

namespace fep::srcs::stock
{

  constexpr char kUnknown[] = "UNKNOWN";
  constexpr char kAAPL[] = "AAPL";
  constexpr char kGOOG[] = "GOOG";
  constexpr char kTSLA[] = "TSLA";

  enum class Symbol
  {
    UNKNOWN = 0,
    AAPL = 1,
    GOOG = 2,
    TSLA = 3,
  };

  const std::unordered_map<std::string, Symbol> SymbolFromString{
      {kUnknown, Symbol::UNKNOWN},
      {kAAPL, Symbol::AAPL},
      {kGOOG, Symbol::GOOG},
      {kTSLA, Symbol::TSLA}};

  const std::unordered_map<Symbol, std::string> SymbolToString{
      {Symbol::UNKNOWN, kUnknown},
      {Symbol::AAPL, kAAPL},
      {Symbol::GOOG, kGOOG},
      {Symbol::TSLA, kTSLA}};

} // namespace fep::srcs::stock

#endif