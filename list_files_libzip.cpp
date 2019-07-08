#include <string>
#include <iostream>
#include <stdexcept>
#include <zip.h>

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <archive>" << std::endl;
        return 1;
    }

    zip_t* a;
    int error;

    if (!(a = zip_open(argv[1], ZIP_RDONLY, &error)))
        throw std::runtime_error{"zip_open error: " + std::to_string(error)};

    zip_stat_t stat;
    zip_stat_init(&stat);

    auto num_entries = zip_get_num_entries(a, 0);

    for (zip_uint64_t i = 0; i < static_cast<zip_uint64_t>(num_entries); ++i) {
        if (zip_stat_index(a, i, 0, &stat) < 0)
            throw std::runtime_error{"zip_stat_index error"};

        std::string_view entry_name{stat.name};

        if (entry_name[entry_name.size()-1] != '/') {
            std::cout << stat.name << " = "
                      << "size (compressed): " << stat.comp_size << ", "
                      << "size (uncompressed): " << stat.size << ", "
                      << "crc: " << stat.crc
                      << std::endl;
        }
    }

    if (zip_close(a) < 0)
        throw std::runtime_error{"zip_close error"};
}
