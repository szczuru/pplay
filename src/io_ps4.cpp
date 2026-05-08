#include "io.h"
#include <dirent.h>
#include <sys/stat.h>
#include <cstring>

namespace pplay {

std::vector<Io::File> Io::getDirList(const DeviceType &type, const std::vector<std::string> &extensions)
{
    std::vector<Io::File> files;

    DIR *dir = opendir("/");
    if (!dir) {
        dir = opendir("/data");  // fallback
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
                f.size = st.st_size;
                // Nie używamy isDir bo pole nie istnieje w tej wersji
                files.push_back(f);
            }
        }
        closedir(dir);
    }

    return files;
}

} // namespace pplay
