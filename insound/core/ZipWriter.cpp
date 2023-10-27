#include "ZipWriter.h"

#include <zip.h>

namespace Insound {
    ZipBuffer::~ZipBuffer()
    {
        free(m_buffer);
    }

    ZipBuffer::ZipBuffer(const ZipBuffer &buf)
        : m_buffer((char *)malloc(buf.m_size)), m_size(buf.m_size)
    {
        memcpy(m_buffer, buf.m_buffer, buf.m_size);
    }

    ZipBuffer::ZipBuffer(ZipBuffer &&buf)
        : m_buffer(buf.m_buffer), m_size(buf.m_size)
    {
        buf.m_buffer = nullptr;
        buf.m_size = 0;
    }

    ZipBuffer &ZipBuffer::operator=(const ZipBuffer &buf)
    {
        free(m_buffer);
        m_buffer = (char *)malloc(buf.m_size);
        m_size = buf.m_size;
        memcpy(m_buffer, buf.m_buffer, buf.m_size);
        return *this;
    }

    ZipBuffer &ZipBuffer::operator=(ZipBuffer &&buf)
    {
        m_buffer = buf.m_buffer;
        m_size = buf.m_size;

        buf.m_buffer = nullptr;
        buf.m_size = 0;

        return *this;
    }

    void ZipBuffer::close()
    {
        free(m_buffer);
        m_buffer = nullptr;
        m_size = 0;
    }


    ZipWriter::ZipWriter() : zip(zip_stream_open(nullptr, 0,
        ZIP_DEFAULT_COMPRESSION_LEVEL, 'w'))
    {

    }

    ZipWriter::~ZipWriter()
    {
        close();
    }

    void ZipWriter::close()
    {
        if (zip)
        {
            zip_close(zip);
            zip = nullptr;
        }
    }

    void ZipWriter::addFile(const std::string &key, const std::string &file)
    {
        // Don't add file if it has no bytes
        if (file.size() == 0) return;

        zip_entry_open(zip, key.c_str());
        {
            zip_entry_write(zip, file.c_str(), file.length());
        }
        zip_entry_close(zip);
    }

    // bool ZipWriter::deleteFiles(const std::vector<std::string> &keys)
    // {
    //     if (keys.empty()) return true; // you did delete nothing, afterall

    //     // convert string vector -> c_str vector
    //     std::vector<const char *> c_strs;
    //     c_strs.reserve(keys.size());
    //     std::transform(keys.begin(), keys.end(),
    //         std::back_inserter(c_strs), std::mem_fn(&std::string::c_str));

    //     // delete the entries
    //     auto res = zip_entries_delete(zip, (char * const *)c_strs.data(), c_strs.size());
    //     IN_LOG("Num entries deleted: {}", res);
    //     return (res == keys.size());
    // }

    ssize_t ZipWriter::numEntries() const
    {
        return zip_entries_total(zip);
    }

    ZipBuffer ZipWriter::copy() const
    {
        void *buf;
        size_t size;
        zip_stream_copy(zip, &buf, &size);

        ZipBuffer res;
        res.m_buffer = (char *)buf;
        res.m_size = size;

        return res;
    }

} // namespace Insound