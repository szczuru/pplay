#include "io.h"
#include <dirent.h>
#include <sys/stat.h>

namespace pplay {

std::vector<Io::File> Io::getDirList(const DeviceType &type, const std::vector<std::string> &extensions)
{
    std::vector<Io::File> files;

    // Na PS4 ignorujemy typ i extensions - pokazujemy wszystko
    DIR *dir = opendir("/");
    if (!dir) {
        // fallback na data jeśli root nie działa
        dir = opendir("/data");
    }

    if (dir) {
        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;

            std::string fullpath = "/";
            fullpath += entry->d_name;

            struct stat st;
            if (stat(fullpath.c_str(), &st) == 0) {
                Io::File f;
                f.name = entry->d_name;
                f.path = fullpath;
                f.isDir = S_ISDIR(st.st_mode);
                f.size = st.st_size;
                files.push_back(f);
            }
        }
        closedir(dir);
    }

    return files;
}

} // namespace pplay
