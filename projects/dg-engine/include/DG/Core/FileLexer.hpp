/** @file DG/Core/FileLexer.hpp */

#pragma once

#include <DG/Core/FileToken.hpp>

namespace dg
{

  /**
   * @brief The @a `FileLexer` class is responsible for extracting syntax tokens from text files.
   */
  class FileLexer
  {
  public:

    /**
     * @brief Constructs a @a `FileLexer` with no tokens extracted.
     */
    FileLexer ();

    /**
     * @brief Constructs a @a `FileLexer` with tokens extracted from a text file at the given path.
     * 
     * @param filepath  The path to the text file at the given path. 
     */
    FileLexer (const Path& filepath);

    /**
     * @brief Extracts syntax tokens from a text file at the given path.
     *
     * @param filepath      The path to the text file to extract tokens from.
     *
     * @return  @a `true` if the file is loaded, and tokens extracted, successfully;
     *          @a `false` otherwise.
     */
    Bool loadFromFile (const Path& filepath);  

    /**
     * @brief Indicates whether or not the @a `FileLexer`'s token pointer is pointing to a token or
     *        the end of the tokens collection.
     * 
     * @return  @a `true` if there are more tokens to retrieve;
     *          @a `false` otherwise. 
     */
    Bool hasMoreTokens () const;

    /**
     * @brief Retrieves the next token in this @a `FileLexer`'s tokens collection.
     * 
     * @return  The next @a `FileToken`. 
     * 
     * @note  This method can also be used to skip the next token as well. Just don't use its return
     *        value.
     */
    const FileToken& getNextToken () const;

    /**
     * @brief Moves the @a `FileLexer`'s token pointer back to the start of the tokens collection.
     */
    void rewind () const;

  private:

    /**
     * @brief A collection of files which have been processed by this @a `FileLexer`.
     */
    Collection<Path> m_includes;

    /**
     * @brief The collection of extracted syntax tokens.
     */
    Collection<FileToken> m_tokens;

    /**
     * @brief An index pointing to the next token to be retrieved.
     */
    mutable Index m_tokenPointer = 0;

  };

}
