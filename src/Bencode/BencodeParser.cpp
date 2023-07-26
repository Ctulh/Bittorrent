/**
 * @file
 * @brief Implementation of the class BencodeParser
 */

#include "BencodeParser.hpp"
#include "Exceptions/NoSuchFieldException.hpp"
#include "StringMethods.hpp"
#include <stack>
#include <numeric>
#include <algorithm>
#include <stdint.h>

/**
 * BencodeParser constructor
 * \param [in] data - text from bencode file or just bencode text to parse.
 */
BencodeParser::BencodeParser(std::string const& data): m_data(data) {}

/**
 * \param [in] fieldName - the field name of the string whose value you want to get
 * \return string value of the field or throws NoSuchFieldException
 */
std::string BencodeParser::operator[](const std::string &fieldName) {
    if(m_fieldValues.count(fieldName) == 1) {
        if(m_fieldValues[fieldName].has_value())
            return m_fieldValues[fieldName].value();
        throw NoSuchFieldException(fieldName);
    }

    auto it = m_data.find(fieldName);
    if(it == m_data.npos) {
        m_fieldValues[fieldName] = OptionalString();
        throw NoSuchFieldException(fieldName);
    }
    it += fieldName.size();
    std::string fieldValue;

    if(m_data[it] == 'i') { // 'i' means integer value
        fieldValue = readNumber(it);
    } else if(m_data[it] == 'd') { // 'd' means dictionary value
       fieldValue = readDictionary(it);
    } else if(m_data[it] == 'l') { // 'l' means list value
        StringVector strings = readList(it);
        fieldValue = std::accumulate(strings.begin(), strings.end(), std::string(),
                                                        [this](std::string const& left, std::string const& right) {
                                                            if(left.empty())
                                                                return right;
                                                            return left + this->m_listSeparator + right;
                                                        });
    } else if(std::isdigit(m_data[it])) { // no 'i', 'l', 'd' after field name means that this is string
        fieldValue = readString(it);
    }

    m_fieldValues[fieldName] = fieldValue;
    return fieldValue;
}

std::string BencodeParser::getValue(const std::string &fieldName) {
    return this->operator[](fieldName);
}

StringVector BencodeParser::getList(const std::string &fieldName) {
    const char separator = ',';

    if(m_fieldValues.count(fieldName) == 1) {
        if(m_fieldValues[fieldName].has_value())
            return StringMethods::split(m_fieldValues[fieldName].value(), separator);
        else
            throw NoSuchFieldException(fieldName);
    }

    auto it = m_data.find(fieldName);
    if(it == m_data.npos) {
        m_fieldValues[fieldName] = OptionalString();
        throw NoSuchFieldException(fieldName);
    }
    it += fieldName.size();
    StringVector strings = readList(it);
    std::string fieldValue = std::accumulate(strings.begin(), strings.end(), std::string(),
                                 [this](std::string const& left, std::string const& right) {
                                     if(left.empty())
                                         return right;
                                     return left + this->m_listSeparator + right;
                                 });
    m_fieldValues[fieldName] = fieldValue;
    return strings;
}

StringVector BencodeParser::readList(std::size_t& index) const {
    const std::size_t dataSize = m_data.size();

    if(index >= dataSize)
        return {};

    int carry = 0;

    if(m_data[index] == 'l')
        ++index;

    ++carry;

    char listElementType;
    if(index < dataSize) {
        if(m_data[index] == 'd')
            listElementType = 'd';
        else if(m_data[index] == 'l')
            listElementType = 'l';
        else if(m_data[index] == 'i')
            listElementType = 'i';
        else
            listElementType = 's';
    }
    StringVector output;
    while(index < dataSize &&carry != 0) {
        if(listElementType == 'd') {
            output.push_back(readDictionary(index));
        }
        else if(listElementType == 'i') {
            output.push_back(readNumber(index));
        }
        else if(listElementType == 's') {
            output.push_back(readString(index));
        }
        if(index < dataSize && m_data[index] == 'e') {
            --carry;
            ++index;
        }
        if(index < dataSize && m_data[index] == 'l') {
            ++carry;
            ++index;
        }
    }
    return output;
}


std::string BencodeParser::readString(std::size_t& index) const {
    const std::size_t dataSize = m_data.size();

    if(index >= dataSize)
        return "";

    if(m_data[index] == ':')
        ++index;

    std::size_t numberOfBytesInFieldValue = 0;
    while (index < dataSize && isdigit(m_data[index])) {
        numberOfBytesInFieldValue *= 10;
        numberOfBytesInFieldValue += static_cast<uint32_t>(m_data[index] - '0');
        ++index;
    }
    std::size_t begin = index+1;
    index += numberOfBytesInFieldValue;

    std::string result =  m_data.substr(begin, index - begin + 1);
    ++index; // because string don't have end symbol in bencode so we should increment index to go to another sequence
    return result;
}

std::string BencodeParser::readDictionary(std::size_t& index) const {
    const std::size_t dataSize = m_data.size();
    int carry = 1;

    std::size_t begin = index;
    if(index < dataSize && m_data[index] == 'd')
        ++index;

    while(carry != 0 && index < dataSize) {
        std::size_t dataLengthInBytes = 0;
        while(index < dataSize && std::isdigit(m_data[index])) {
             dataLengthInBytes *= 10;
             dataLengthInBytes += static_cast<uint32_t>(m_data[index] - '0');
             ++index;
        }
        ++index;

       index += dataLengthInBytes;
       if(index < dataSize) {
           if(m_data[index] == 'd') {
               ++carry;
               ++index;
           }
           else if(m_data[index] == 'i') {
               readNumber(index);
           }
           else if(m_data[index] == 'l') {
               readList(index);
           }
           while (carry != 0 && index < dataSize && m_data[index] == 'e') {
               --carry;
               ++index;
           }

       }
    }
    if (begin == std::string::npos)
        return "";
    std::string res = m_data.substr(begin, index - begin);
    return res;
}

std::string BencodeParser::readNumber(std::size_t& index) const {
    const std::size_t dataSize = m_data.size();

    if(index >= dataSize)
        return "";
    if(m_data[index] == 'i')
        ++index;
    std::string result;
    while(index < dataSize && m_data[index] != 'e') {
        result += m_data[index];
        ++index;
    }
    ++index;
    return result;
}

