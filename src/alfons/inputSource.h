/*
 * Based on The New Chronotext Toolkit
 * Copyright (C) 2014, Ariel Malka - All rights reserved.
 *
 * Adapted to Alfons
 * Copyright (C) 2015, Hannes Janetzek
 *
 * The following source-code is distributed under the Simplified BSD License.
 */

#pragma once

#include <string>
#include <memory>
#include <vector>
#include <functional>

namespace alfons {

using LoadSourceHandle = std::function<unsigned char*(size_t*)>;

class InputSource {
public:

    InputSource(const InputSource& _other) = delete;

    InputSource& operator=(const InputSource& _other) = delete;

    InputSource() = default;

    InputSource(const std::string& _uri)
        : m_uri(_uri) {}

    InputSource(LoadSourceHandle _loadSource)
        : m_loadSource(_loadSource) {}

    InputSource(unsigned char* _data, size_t _size)
        : m_buffer(_data), m_bufferSize(_size) {}

    ~InputSource() {
        if (m_buffer && m_bufferSize > 0) {
            free(m_buffer);
        }
    }

    const std::string& uri() const { return m_uri; }
    const unsigned char* buffer() const { return m_buffer; }
    size_t bufferSize() const { return m_bufferSize; }

    bool isUri() const { return !m_uri.empty(); }

    bool hasSourceHandler() { return bool(m_loadSource); }

    bool resolveSource() {
        if (m_buffer) {
            return true;
        }

        m_buffer = m_loadSource(&m_bufferSize);

        if (m_bufferSize == 0) {
            return false;
        }

        return true;
    }

    bool isValid() {
        return (m_buffer && m_bufferSize > 0) || !m_uri.empty() || bool(m_loadSource);
    }

protected:
    std::string m_uri = "";
    unsigned char* m_buffer = nullptr;
    size_t m_bufferSize = 0;
    LoadSourceHandle m_loadSource = nullptr;
};
}
