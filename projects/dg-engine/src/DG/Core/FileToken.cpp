/** @file DG/Core/FileToken.cpp */

#include <DG/Core/FileToken.hpp>

namespace dg
{

  const Char* FileToken::typeToString () const
  {
    switch (type) {
      case FileTokenType::Identifier: return "Identifier";
      case FileTokenType::Boolean:  return "Boolean";
      case FileTokenType::String: return "String";
      case FileTokenType::Integer:  return "Integer";
      case FileTokenType::FloatingPoint:  return "Floating Point";
      case FileTokenType::Binary: return "Binary";
      case FileTokenType::Hexadecimal:  return "Hexadecimal";
      case FileTokenType::Octal:  return "Octal";
      case FileTokenType::Period: return "Period";
      case FileTokenType::Colon: return "Colon";
      case FileTokenType::Semicolon: return "Semicolon";
      case FileTokenType::Comma:  return "Comma";
      case FileTokenType::Pound:  return "Pound";
      case FileTokenType::At: return "At";
      case FileTokenType::Equals: return "Equals";
      case FileTokenType::Plus: return "Plus";
      case FileTokenType::Minus:  return "Minus";
      case FileTokenType::Times:  return "Times";
      case FileTokenType::Divide: return "Divide";
      case FileTokenType::Percent:  return "Percent";
      case FileTokenType::Pipe: return "Pipe";
      case FileTokenType::Ampersand:  return "Ampersand";
      case FileTokenType::Carat:  return "Carat";
      case FileTokenType::DollarSign:  return "Dollar Sign";
      case FileTokenType::OpenParenthesis:  return "Opening Parenthesis";
      case FileTokenType::CloseParenthesis: return "Closing Parenthesis";
      case FileTokenType::OpenSquareBracket:  return "Opening Square Bracket";
      case FileTokenType::CloseSquareBracket: return "Closing Square Bracket";
      case FileTokenType::OpenCurlyBrace: return "Opening Curly Brace";
      case FileTokenType::CloseCurlyBrace:  return "Closing Curly Brace";
      case FileTokenType::OpenArrowBrace: return "Opening Arrow Brace";
      case FileTokenType::CloseArrowBrace:  return "Closing Arrow Brace";
      case FileTokenType::NewLine:  return "New Line";
      case FileTokenType::EndOfFile:  return "End Of File";
      default: return "Unknown Token";
    }
  }

}
