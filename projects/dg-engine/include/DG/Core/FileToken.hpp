/** @file DG/Core/FileToken.hpp */

#pragma once

#include <DG_Pch.hpp>

namespace dg
{

  /**
   * @brief The @a `FileTokenType` enum enumerates the different kinds of syntax tokens which can be
   *        extracted from a text file.
   */
  enum class FileTokenType
  {
    Unknown,
    Identifier,
    Boolean,
    String,
    Integer,
    FloatingPoint,
    Binary,
    Hexadecimal,
    Octal,
    Period,
    Colon,
    Semicolon,
    Comma,
    Pound,
    At,
    Equals,
    Plus,
    Minus,
    Times,
    Divide,
    Percent,
    Pipe,
    Ampersand,
    Carat,
    DollarSign,
    OpenParenthesis,
    CloseParenthesis,
    OpenSquareBracket,
    CloseSquareBracket,
    OpenCurlyBrace,
    CloseCurlyBrace,
    OpenArrowBrace,
    CloseArrowBrace,
    NewLine,
    EndOfFile
  };

  /**
   * @brief The @a `FileToken` struct describes a syntax token extracted from a text file by the
   *        @a `FileLexer` class.
   */
  struct FileToken
  {

    /**
     * @brief Indicates the type of the token which was extracted.
     */
    FileTokenType type;

    /**
     * @brief The token's string contents.
     */
    String contents;

    /**
     * @brief The path to the text file from which this token was extracted.
     */
    Path sourceFile;

    /**
     * @brief The line in the text file containing this token.
     */
    Index sourceLine;

    /**
     * @brief Retrieves the string form of this @a `Token`'s type.
     *
     * @return  The string form of this @a `Token`'s type.
     */
    const Char* typeToString () const;

  };

}
