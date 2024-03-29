#include "tools.h"
#include "defs.h"
#include "ioapi_mem.h"
#include <algorithm>
#include <iterator>

#include "CDirEntry.h"

#include <cstdio>
#include <iostream>

#if defined(WIN32)
#    include "tps/dirent.h"
#    include "tps/dirent.c"
#else
#    include <sys/types.h>
#    include <dirent.h>
#    include <unistd.h>
#endif /* WINDOWS */

namespace zipper {

// -----------------------------------------------------------------------------
// Calculate the CRC32 of a file because to encrypt a file, we need known the
// CRC32 of the file before.
void getFileCrc(std::istream& input_stream, std::vector<char>& buff, unsigned long& result_crc)
{
    unsigned long calculate_crc = 0;
    unsigned int size_read = 0;
    unsigned long total_read = 0;

    do
    {
        input_stream.read(buff.data(), std::streamsize(buff.size()));
        size_read = static_cast<unsigned int>(input_stream.gcount());

        if (size_read > 0)
            calculate_crc = crc32(calculate_crc, reinterpret_cast<const unsigned char*>(buff.data()), size_read);

        total_read += static_cast<unsigned long>(size_read);

    } while (size_read > 0);

    input_stream.clear();
    input_stream.seekg(0, std::ios_base::beg);
    result_crc = calculate_crc;
}

// -----------------------------------------------------------------------------
bool isLargeFile(std::istream& input_stream)
{
    std::streampos pos = 0;
    input_stream.seekg(0, std::ios::end);
    pos = input_stream.tellg();
    input_stream.seekg(0);

    return pos >= 0xffffffff;
}

// -----------------------------------------------------------------------------
bool checkFileExists(const std::string& filename)
{
    return CDirEntry::exist(filename);
}

// -----------------------------------------------------------------------------
bool makedir(const std::string& newdir)
{
    return CDirEntry::createDir(newdir);
}

// -----------------------------------------------------------------------------
void removeFolder(const std::string& foldername)
{
    if (!CDirEntry::remove(foldername))
    {
        std::vector<std::string> files = filesFromDirectory(foldername);
        std::vector<std::string>::iterator it = files.begin();
        for (; it != files.end(); ++it)
        {
            if (isDirectory(*it) && *it != foldername)
                removeFolder(*it);
            else
                std::remove(it->c_str());
        }
        CDirEntry::remove(foldername);
    }
}

// -----------------------------------------------------------------------------
bool isDirectory(const std::string& path)
{
    return CDirEntry::isDir(path);
}

// -----------------------------------------------------------------------------
std::string parentDirectory(const std::string& filepath)
{
    return CDirEntry::dirName(filepath);
}

// -----------------------------------------------------------------------------
std::string currentPath()
{
    char buffer[1024u];
    return (getcwd(buffer, sizeof(buffer)) ? std::string(buffer) : std::string(""));
}

// -----------------------------------------------------------------------------
std::vector<std::string> filesFromDirectory(const std::string& path)
{
    std::vector<std::string> files;
    DIR* dir;
    struct dirent* entry;

    dir = opendir(path.c_str());

    if (dir == NULL)
        return files;

    for (entry = readdir(dir); entry != NULL; entry = readdir(dir))
    {
        std::string filename(entry->d_name);

        if (filename == "." || filename == "..") continue;

        if (CDirEntry::isDir(path + CDirEntry::Separator + filename))
        {
            std::vector<std::string> moreFiles = filesFromDirectory(path + CDirEntry::Separator + filename);
            std::copy(moreFiles.begin(), moreFiles.end(), std::back_inserter(files));
            continue;
        }


        files.push_back(path + CDirEntry::Separator + filename);
    }

    closedir(dir);


    return files;
}

// -----------------------------------------------------------------------------
std::string fileNameFromPath(const std::string& fullPath)
{
    return CDirEntry::fileName(fullPath);
}

bool replaceString(std::string& inOutStr, const std::string& srcSubStr, const std::string& targetSubStr)
{
    bool reRes = false;
    for (std::string::size_type pos(0); pos != std::string::npos; pos += targetSubStr.length())
    {
        pos = inOutStr.find(srcSubStr, pos);
        if (pos != std::string::npos)
        {
            inOutStr.replace(pos, srcSubStr.length(), targetSubStr);
            reRes = true;
        }
        else
            break;
    }
    return reRes;
}

// -----------------------------------------------------------------------------
std::string normalPath(const std::string& path)
{
    std::string newPath = path;
    replaceString(newPath,
                  (CDirEntry::Separator == "/" ? "\\" : "/"),
                  (CDirEntry::Separator == "/" ? "/" : "\\"));
    std::string doubleSeparators = CDirEntry::Separator + CDirEntry::Separator;
    while (replaceString(newPath, doubleSeparators, CDirEntry::Separator))
    {
        /*empty*/
    }
    return newPath;
}

} // namespace zipper
