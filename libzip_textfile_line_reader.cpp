#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
#include <streambuf>
#include <stdexcept>
#include <zip.h>

class libzip_textfile_line_reader : public std::streambuf {
public:
    explicit libzip_textfile_line_reader(zip_file_t* fp, std::size_t buffer_size=256, std::size_t put_back=8);

private:
    int_type underflow() override;

    // copy ctor and assignment not implemented; copying not allowed
    libzip_textfile_line_reader(const libzip_textfile_line_reader&);
    libzip_textfile_line_reader& operator=(const libzip_textfile_line_reader&);

private:
    zip_file_t* fp_;
    const std::size_t put_back_;
    std::vector<char> buffer_;
};


libzip_textfile_line_reader::libzip_textfile_line_reader(zip_file_t* fp, std::size_t buffer_size, std::size_t put_back) :
    fp_(fp),
    put_back_(std::max(put_back, std::size_t(1))),
    buffer_(std::max(buffer_size, put_back_) + put_back_)
{
    // Set the back, current and end buffer pointers to be equal.
    // This will force an underflow() on the first read and hence fill the buffer.
    char* end = &buffer_.front() + buffer_.size();
    setg(end, end, end);
}

std::streambuf::int_type libzip_textfile_line_reader::underflow()
{
    if (gptr() < egptr())
        return traits_type::to_int_type(*gptr());

    char* base = &buffer_.front();
    char* start = base;

    if (eback() == base) {
        // make arrangements for putback characters
        std::memmove(base, egptr() - put_back_, put_back_);
        start += put_back_;
    }

    // start is now the start of the buffer, proper.
    // Read from fp_ in to the provided buffer.
    auto n = zip_fread(fp_, start, buffer_.size() - static_cast<zip_uint64_t>(start - base));

    if (n == 0)
        return traits_type::eof();

    // set buffer pointers
    setg(base, start, start + n);

    return traits_type::to_int_type(*gptr());
}


int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <archive>" << std::endl;
        return 1;
    }

    zip_t* archive;
    int error;

    if (!(archive = zip_open(argv[1], ZIP_RDONLY, &error)))
        throw std::runtime_error{"zip_open error: " + std::to_string(error)};

    zip_stat_t stat;
    zip_stat_init(&stat);

    auto num_entries = zip_get_num_entries(archive, 0);

    for (zip_uint64_t i = 0; i < static_cast<zip_uint64_t>(num_entries); ++i) {
        if (zip_stat_index(archive, i, 0, &stat) < 0)
            throw std::runtime_error{"zip_stat_index error"};

        std::string_view entry_name{stat.name};

        if (entry_name[entry_name.size()-1] == '/')
            continue;

        zip_file_t* fp = zip_fopen_index(archive, i, 0);

        if (!fp)
            throw std::runtime_error{"zip_fopen_index error"};

        libzip_textfile_line_reader line_reader(fp);
        std::istream in(&line_reader);
        std::string s;

        while (std::getline(in, s))
            std::cout << s << std::endl;

        zip_fclose(fp);
    }

    if (zip_close(archive) < 0)
        throw std::runtime_error{"zip_close error"};
}
