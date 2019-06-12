#include <string>
#include <iostream>
#include <stdexcept>
#include <archive.h>
#include <archive_entry.h>

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <archive>" << std::endl;
        return 1;
    }

    struct archive *a;
    struct archive_entry *entry;

    if (!(a = archive_read_new()))
        throw std::runtime_error{"archive_read_new error"};

    archive_read_support_filter_all(a);
    archive_read_support_format_all(a);

    if (archive_read_open_filename(a, argv[1], 10240) != ARCHIVE_OK)
        throw std::runtime_error{"unable to read archive"};

    while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {
        if (archive_entry_filetype(entry) != AE_IFDIR) {
            std::cout << archive_entry_pathname(entry) << " = "
                      << "size (uncompressed): " << archive_entry_size(entry)
                      << std::endl;
        }
    }

    if (archive_read_free(a) != ARCHIVE_OK)
        throw std::runtime_error{"archive_read_free error"};
}
