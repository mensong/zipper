#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <memory>
#include <ctime>

namespace zipper {

// *************************************************************************
//! \brief Zip archive compressor.
// *************************************************************************
class Zipper
{
public:

    // -------------------------------------------------------------------------
    //! \brief Archive flags.
    // -------------------------------------------------------------------------
    enum openFlags
    {
        //! \brief Minizip options/params: -o  Overwrite existing file.zip
        Overwrite = 0x01,
        //! \brief Minizip options/params: -a  Append to existing file.zip
        Append = 0x02,
        //! \brief Minizip options/params: -j  Exclude path. store only the file name.
        //TODO NoPaths = 0x20,
    };

    // -------------------------------------------------------------------------
    //! \brief Compression options for files.
    // -------------------------------------------------------------------------
    enum zipFlags
    {
        //! \brief Minizip options/params: -0  Store only
        Store = 0x00,
        //! \brief Minizip options/params: -1  Compress faster
        Faster = 0x01,
        //! \brief Minizip options/params: -5  Compress faster
        Medium = 0x05,
        //! \brief Minizip options/params: -9  Compress better
        Better = 0x09,
        //! \brief ???
        SaveHierarchy = 0x40
    };

    // -------------------------------------------------------------------------
    //! \brief Regular zip compression 
    // -------------------------------------------------------------------------
    Zipper();

    // -------------------------------------------------------------------------
    //! \brief Open zip compression (inside a disk zip archive file) with a
    //! password.
    //!
    //! \param[in] zipname: the path where to create your zip file.
    //! \param[in] password: optional password (set empty for not using password).
    //! \param[in] flags: Overwrite (default) or append existing zip file (zipname).
    //! \throw std::runtime_error if something odd happened.
    // -------------------------------------------------------------------------
    bool open(const std::string& zipname, const std::string& password,
           Zipper::openFlags flags = Zipper::openFlags::Overwrite);

    // -------------------------------------------------------------------------
    //! \brief In-memory zip compression (storage inside std::iostream).
    //!
    //! \param[in] buffer: the stream in which to store zipped files.
    //! \param[in] password: optional password (set empty for not using password).
    //! \throw std::runtime_error if something odd happened.
    // -------------------------------------------------------------------------
    bool open(std::iostream& buffer, const std::string& password = std::string());

    // -------------------------------------------------------------------------
    //! \brief In-memory zip compression (storage inside std::vector).
    //!
    //! \param[in] buffer: the vector in which to store zipped files.
    //! \param[in] password: optional password (set empty for not using password).
    //! \throw std::runtime_error if something odd happened.
    // -------------------------------------------------------------------------
    bool open(std::vector<unsigned char>& buffer, 
        const std::string& password = std::string());

    // -------------------------------------------------------------------------
    //! \brief Call close().
    // -------------------------------------------------------------------------
    ~Zipper();

    // -------------------------------------------------------------------------
    //! \brief Compress data \c source with a given timestamp in the archive
    //! with the given name \c nameInZip.
    //!
    //! \param[in,out] source: data to compress.
    //! \param[in] timestamp: the desired timestamp.
    //! \param[in] nameInZip: the desired name for \c source inside the archive.
    //! \param[in] flags: compression options (faster, better ...).
    //! \return true on success, else return false.
    //! \throw std::runtime_error if something odd happened.
    // -------------------------------------------------------------------------
    bool add(std::istream& source, const std::tm& timestamp, const std::string& nameInZip,
             Zipper::zipFlags flags = Zipper::zipFlags::Better);
    bool add(const std::string& sourceFile, const std::tm& timestamp, const std::string& nameInZip,
             Zipper::zipFlags flags = Zipper::zipFlags::Better);

    // -------------------------------------------------------------------------
    //! \brief Compress data \c source in the archive with the given name \c
    //! nameInZip. No timestamp will be stored.
    //!
    //! \param[in,out] source: data to compress.
    //! \param[in] nameInZip: the desired name for \c source inside the archive.
    //! \param[in] flags: compression options (faster, better ...).
    //! \return true on success, else return false.
    //! \throw std::runtime_error if something odd happened.
    // -------------------------------------------------------------------------
    bool add(std::istream& source, const std::string& nameInZip,
             Zipper::zipFlags flags = Zipper::zipFlags::Better);
    bool add(const std::string& sourceFile, const std::string& nameInZip,
             Zipper::zipFlags flags = Zipper::zipFlags::Better);

    // -------------------------------------------------------------------------
    //! \brief Compress a folder or a file in the archive.
    //!
    //! \param[in,out] fileOrFolderPath: file or folder to compress.
    //! \param[in] flags: compression options (faster, better ...).
    //! \return true on success, else return false.
    //! \throw std::runtime_error if something odd happened.
    // -------------------------------------------------------------------------
    bool addFolder(const std::string& folderPath, const std::string& folderInZip = std::string(),
             Zipper::zipFlags flags = Zipper::zipFlags::Better);

    // -------------------------------------------------------------------------
    //! \brief Depending on your selection of constructor, this method will do
    //! some actions such as closing the access to the zip file, flushing in the
    //! stream, releasing memory ...
    //! \note this method is called by the destructor.
    // -------------------------------------------------------------------------
    void close();

    // -------------------------------------------------------------------------
    //! \brief To be called after a close(). Depending on your selection of
    //! constructor, this method will do some actions such as opening the zip
    //! file, reserve buffers.
    //! \param[in] flags: Overwrite or append (default) existing zip file (zipname).
    //! \note this method is not called by the constructor.
    // -------------------------------------------------------------------------
    bool reopen(Zipper::openFlags flags = Zipper::openFlags::Append);

private:

    void release();

private:

    std::iostream* m_obuffer;
    std::vector<unsigned char>* m_vecbuffer;
    std::string m_zipname;
    std::string m_password;
    bool m_open;

    struct Impl;
    Impl* m_impl;
};

} // namespace zipper
