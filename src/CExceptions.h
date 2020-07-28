/**
 * @file Contains declaration of classes: CException, CBMPLoadException,
 *                                        CExistFileException, CReadFileException,
 *                                        CWriteFileException, CPageException,
 *                                        CSDLInitException, CMapCreateException,
 *                                        CMapSaveException, CMapLoadException,
 *                                        CErrorMapFileException.
 */
//======================================================================================================================
#pragma once
//======================================================================================================================

//======================================================================================================================
#include <string>
//======================================================================================================================

//=============================CException===============================================================================
/**
 * Template for creating exceptions.
 */
class CException {
public:
    //------------------------------------------------------------------------------------------------------------------
    /**
     * @param exStr - the error text.
     */
    explicit CException (const std::string & exStr);

    //------------------------------------------------------------------------------------------------------------------
    /**
     * @return - the error text.
     */
    const std::string & what () const;

    //------------------------------------------------------------------------------------------------------------------
private:
    std::string m_message;
};
//======================================================================================================================

//=============================CBMPLoadException=========================================================================
/**
 * Creates an exception with text "ERROR when tried load BMP "exStr"".
 */
class CBMPLoadException : public CException{
public:
    /**
     * @param exStr - exception text.
     */
    explicit    CBMPLoadException        (const std::string & exStr);
};
//======================================================================================================================

//=============================CExistFileException======================================================================
/**
 * Creates an exception with text "ERROR file "exStr" do not exist".
 */
class CExistFileException : public CException{
public:
    /**
     * @param exStr - exception text.
     */
    explicit    CExistFileException     (const std::string & exStr);
};
//======================================================================================================================

//=============================CReadFileException=======================================================================
/**
 * Creates an exception with text "ERROR when tried read file "exStr"".
 */
class CReadFileException : public CException{
public:
    /**
     * @param exStr - exception text.
     */
    explicit    CReadFileException      (const std::string & exStr);
};
//======================================================================================================================

//=============================CWriteFileException======================================================================
/**
 * Creates an exception with text "ERROR when tried write in file "exStr"".
 */
class CWriteFileException : public CException{
public:
    /**
     * @param exStr - exception text.
     */
    explicit    CWriteFileException     (const std::string & exStr);
};
//======================================================================================================================

//=============================CPageException===========================================================================
/**
 * Creates an exception with text "ERROR opened page "pageName"".
 */
class CPageException : public CException{
public:
    /**
     * @param pageName
     */
    explicit    CPageException     (const std::string & pageName);
};
//======================================================================================================================

//=============================CSDLInit=================================================================================
/**
 * Creates an exception with text "ERROR when tried initialize SDL".
 */
class CSDLInitException : public CException{
public:
    /**
     * @param exStr - exception text.
     */
    explicit    CSDLInitException      (const std::string & exStr);
};
//======================================================================================================================

//=============================CMapCreateException======================================================================
/**
 * Creates an exception with text "ERROR when tried create new map "exStr"".
 */
class CMapCreateException : public CException{
public:
    /**
     * @param exStr - exception text.
     */
    explicit    CMapCreateException      (const std::string & exStr);
};
//======================================================================================================================

//=============================CMapSaveException========================================================================
/**
 * Creates an exception with text "ERROR when saved map".
 */
class CMapSaveException : public CException{
public:
    /**
     * @param exStr - exception text.
     */
    explicit    CMapSaveException      (const std::string & exStr);
};
//======================================================================================================================

//=============================CMapLoadException========================================================================
/**
 * Creates an exception with text "ERROR when loaded map".
 */
class CMapLoadException : public CException{
public:
    /**
     * @param exStr - exception text.
     */
    explicit    CMapLoadException      (const std::string & exStr);
};
//======================================================================================================================

//=============================CErrorMapFileException===================================================================
/**
 * Creates an exception with text "ERROR when loaded file "nameFile" context error".
 */
class CErrorMapFileException : public CException{
public:
    /**
     * @param nameFile
     */
    explicit    CErrorMapFileException      (const std::string & nameFile);
};
//======================================================================================================================

//=============================operator << for CException===============================================================
    /**
     * Write the exception to output stream
     * @param os - output stream
     * @param exception
     * @return - output stream
     */
std::ostream & operator << (std::ostream & os, const CException & exception);
//======================================================================================================================