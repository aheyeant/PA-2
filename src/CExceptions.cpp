/**
 * @file Implementation of classes: CException, CBMPLoadException,
 *                                  CExistFileException, CReadFileException,
 *                                  CWriteFileException, CPageException,
 *                                  CSDLInitException, CMapCreateException,
 *                                  CMapSaveException, CMapLoadException,
 *                                  CErrorMapFileException.
 */
//======================================================================================================================
#include "CExceptions.h"
//======================================================================================================================

//=============================CException===============================================================================
/**
 * @param exStr - the error text.
 */
CException::CException (const std::string &exStr) : m_message(exStr) {}

//----------------------------------------------------------------------------------------------------------------------
/**
 * @return - the error text.
 */
const std::string &CException::what () const {
    return m_message;
}
//======================================================================================================================

//=============================CBMPLoadException========================================================================
/**
 * @param exStr - exception text.
 */
CBMPLoadException::CBMPLoadException (const std::string &exStr) : CException(
        "ERROR when tried load BMP \"" + exStr + "\".\n") {}
//======================================================================================================================

//=============================CExistFileException======================================================================
/**
 * @param exStr - exception text.
 */
CExistFileException::CExistFileException (const std::string &exStr) : CException(
        "ERROR file \"" + exStr + "\" do not exist.\n") {}
//======================================================================================================================

//=============================CReadFileException=======================================================================
/**
 * @param exStr - exception text.
 */
CReadFileException::CReadFileException (const std::string &exStr) : CException(
        "ERROR when tried read file \"" + exStr + "\".\n") {}
//======================================================================================================================

//=============================CWriteFileException======================================================================
/**
 * @param exStr - exception text.
 */
CWriteFileException::CWriteFileException (const std::string &exStr) : CException(
        "ERROR when tried write in file \"" + exStr + "\".\n") {}
//======================================================================================================================

//=============================CPageException===========================================================================
/**
 * @param pageName
 */
CPageException::CPageException (const std::string &pageName) : CException(
        "ERROR opened page \"" + pageName + "\"\n") {}
//======================================================================================================================

//=============================CSDLInitException========================================================================
/**
 * @param exStr - exception text.
 */
CSDLInitException::CSDLInitException (const std::string &exStr) : CException(
        "ERROR when tried initialize SDL" + exStr + ".\n") {}
//======================================================================================================================

//=============================CMapCreateException======================================================================
/**
 * @param exStr - exception text.
 */
CMapCreateException::CMapCreateException (const std::string &exStr) : CException(
        "ERROR when tried create new map " + exStr + ".\n") {}
//======================================================================================================================

//=============================CMapSaveException========================================================================
/**
 * @param exStr - exception text.
 */
CMapSaveException::CMapSaveException (const std::string &exStr) : CException(
        "ERROR when saved map" + exStr + ".\n") {}
//======================================================================================================================

//=============================CMapLoadException========================================================================
/**
 * @param exStr - exception text.
 */
CMapLoadException::CMapLoadException (const std::string &exStr) : CException(
        "ERROR when loaded map" + exStr + ".\n") {}
//======================================================================================================================

//=============================CErrorMapFileException===================================================================
/**
 * @param nameFile
 */
CErrorMapFileException::CErrorMapFileException (const std::string &nameFile) : CException(
        "ERROR when loaded file \"" + nameFile + "\" context error.\n") {}
//======================================================================================================================

//=============================operator << for CException===============================================================
/**
 * Write the exception to output stream
 * @param os - output stream
 * @param exception
 * @return - output stream
 */
std::ostream &operator<< (std::ostream &os, const CException &exception) {
    os << exception.what();
    return os;
}
//======================================================================================================================