/** @file DG/Core/FileIo.hpp */

#pragma once

#include <DG_Pch.hpp>

namespace dg
{

  using LineFunction = LFunction<Bool, StringView, Size>;

  /**
   * @brief The @a `FileIo` class is a static helper class used for reading and writing files.
   */
  class FileIo
  {
  public:
    /**
     * @brief Retrieves the absolute, lexically normal form of the given relative file path.
     * 
     * @param path  The relative filepath to be resolved.
     *  
     * @return  The resolved, absolute, lexically-normal file path. 
     */
    static Path getAbsolute (const Path& path);

    /**
     * @brief Attempts to load a text file with the given filename.
     * 
     * @param filename      The name of the text file to load.
     * @param lineFunction  A function to call on each line in the file.
     * 
     * @return  @a `true` if the file is loaded and processed successfully; 
     *          @a `false` otherwise.
     */
    static Bool loadTextFile (const String& filename, const LineFunction& lineFunction);

    /**
     * @brief Attempts to load a text file with the given filename.
     * 
     * @param filename  The name of the text file to load.
     * @param contents  A handle to to a string which will contain the file's
     * contents.
     * 
     * @return  @a `true` if the file's contents are loaded successfully;
     *          @a `false` otherwise.
     */
    static Bool loadTextFile (const String& filename, String& contents);

    /**
     * @brief Attempts to save the given string contents to a text file with
     *        the given filename.
     * 
     * @param filename  The name of the text file to save.
     * @param contents  The string contents to be saved.
     * 
     * @return  @a `true` if the string contents are saved successfully;
     *          @a `false` otherwise.
     */
    static Bool saveTextFile (const String& filename, const String& contents);

  };

}
