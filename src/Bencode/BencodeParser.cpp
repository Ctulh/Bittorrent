/**
 * @file
 * @brief Implementation of the class BencodeParser.
 */

#include "BencodeParser.hpp"
#include "Exceptions/NoSuchFieldException.hpp"
#include "StringMethods.hpp"
#include <numeric>
#include <algorithm>
#include <stdint.h>

BencodeParser::BencodeParser(std::string const& bencodeData): m_bencodeData(bencodeData) {}

char BencodeParser::listSeparator() {
    return ',';
}

constexpr char BencodeParser::listIdentifier() {
    return 'l';
}

constexpr char BencodeParser::numberIdentifier() {
    return 'i';
}

constexpr char BencodeParser::stringIdentifier() {
    return 's';
}

constexpr char BencodeParser::dictionaryIdentifier() {
    return 'd';
}

constexpr char BencodeParser::endIdentifier() {
    return 'e';
}

std::string BencodeParser::operator[](const std::string &fieldName) {
    if(m_fieldValues.count(fieldName) == 1) {
        if(m_fieldValues[fieldName].has_value())
            return m_fieldValues[fieldName].value();
        throw NoSuchFieldException(fieldName);
    }

    auto it = m_bencodeData.find(fieldName);
    if(it == m_bencodeData.npos) {
        m_fieldValues[fieldName] = OptionalString();
        throw NoSuchFieldException(fieldName);
    }
    it += fieldName.size();
    std::string fieldValue;

    if(isSame<numberIdentifier()>(m_bencodeData[it])) {
        fieldValue = readNumber(it);
    } else if(isSame<dictionaryIdentifier()>(m_bencodeData[it])) {
       fieldValue = readDictionary(it);
    } else if(isSame<listIdentifier()>(m_bencodeData[it])) {
        StringVector strings = readList(it);
        fieldValue = std::accumulate(strings.begin(), strings.end(), std::string(),
                                                        [this](std::string const& left, std::string const& right) {
                                                            if(left.empty())
                                                                return right;
                                                            return left + listSeparator() + right;
                                                        });
    } else if(std::isdigit(m_bencodeData[it])) { // no 'i', 'l', 'd' after field name means that this is string
        fieldValue = readString(it);
    }

    m_fieldValues[fieldName] = fieldValue;
    return fieldValue;
}

std::string BencodeParser::getValue(const std::string &fieldName) {
    return this->operator[](fieldName);
}

StringVector BencodeParser::getList(const std::string &fieldName) {
    if(m_fieldValues.count(fieldName) == 1) {
        if(m_fieldValues[fieldName].has_value())
            return StringMethods::split(m_fieldValues[fieldName].value(), listSeparator());
        else
            throw NoSuchFieldException(fieldName);
    }

    auto it = m_bencodeData.find(fieldName);
    if(it == m_bencodeData.npos) {
        m_fieldValues[fieldName] = OptionalString();
        throw NoSuchFieldException(fieldName);
    }
    it += fieldName.size();
    StringVector strings = readList(it);
    std::string fieldValue = std::accumulate(strings.begin(), strings.end(), std::string(),
                                 [this](std::string const& left, std::string const& right) {
                                     if(left.empty())
                                         return right;
                                     return left + listSeparator() + right;
                                 });
    m_fieldValues[fieldName] = fieldValue;
    return strings;
}

StringVector BencodeParser::readList(std::size_t& index) const {
    const std::size_t dataSize = m_bencodeData.size();

    if(index >= dataSize)
        return {};

    int carry = 0;

    if(isSame<listIdentifier()>(m_bencodeData[index]))
        ++index;

    ++carry;

    char listElementType;
    if(index < dataSize) {
        if(isSame<dictionaryIdentifier()>(m_bencodeData[index]) ||
           isSame<listIdentifier()>(m_bencodeData[index])       ||
           isSame<numberIdentifier()>(m_bencodeData[index]))  {
            listElementType = m_bencodeData[index];
        }
        else {
            listElementType = stringIdentifier();
        }
    }

    StringVector output;
    while(index < dataSize && carry != 0) {
        if(isSame<dictionaryIdentifier()>(listElementType)) {
            output.push_back(readDictionary(index));
        }
        else if(isSame<numberIdentifier()>(listElementType)) {
            output.push_back(readNumber(index));
        }
        else if(isSame<stringIdentifier()>(listElementType)) {
            output.push_back(readString(index));
        }
        if(index < dataSize && isSame<endIdentifier()>(m_bencodeData[index])) {
            --carry;
            ++index;
        }
        if(index < dataSize && isSame<listIdentifier()>(m_bencodeData[index])) {
            ++carry;
            ++index;
        }
    }
    return output;
}


std::string BencodeParser::readString(std::size_t& index) const {
    const std::size_t dataSize = m_bencodeData.size();

    if(index >= dataSize)
        return {};

    if(m_bencodeData[index] == ':')
        ++index;

    std::size_t numberOfBytesInFieldValue = 0;
    while (index < dataSize && isdigit(m_bencodeData[index])) {
        numberOfBytesInFieldValue *= 10;
        numberOfBytesInFieldValue += static_cast<uint32_t>(m_bencodeData[index] - '0');
        ++index;
    }
    std::size_t begin = index+1;
    index += numberOfBytesInFieldValue;

    std::string result =  m_bencodeData.substr(begin, index - begin + 1);
    ++index; // because string don't have end symbol in bencode, so we should increment index to go to another sequence
    return result;
}

std::string BencodeParser::readDictionary(std::size_t& index) const {
    const std::size_t dataSize = m_bencodeData.size();
    int carry = 1;

    std::size_t begin = index;
    if(index < dataSize && isSame<dictionaryIdentifier()>(m_bencodeData[index]))
        ++index;

    while(carry != 0 && index < dataSize) {
        std::size_t dataLengthInBytes = 0;
        while(index < dataSize && std::isdigit(m_bencodeData[index])) {
             dataLengthInBytes *= 10;
             dataLengthInBytes += static_cast<uint32_t>(m_bencodeData[index] - '0');
             ++index;
        }
        ++index;

       index += dataLengthInBytes;
       if(index < dataSize) {
           if(isSame<dictionaryIdentifier()>(m_bencodeData[index])) {
               ++carry;
               ++index;
           }
           else if(isSame<numberIdentifier()>(m_bencodeData[index])) {
               readNumber(index);
           }
           else if(isSame<listIdentifier()>(m_bencodeData[index])) {
               readList(index);
           }
           while (carry != 0 && index < dataSize && isSame<endIdentifier()>(m_bencodeData[index])) {
               --carry;
               ++index;
           }

       }
    }
    if (begin == std::string::npos)
        return "";
    std::string res = m_bencodeData.substr(begin, index - begin);
    return res;
}

std::string BencodeParser::readNumber(std::size_t& index) const {
    const std::size_t dataSize = m_bencodeData.size();

    if(index >= dataSize)
        return "";
    if(isSame<numberIdentifier()>(m_bencodeData[index]))
        ++index;
    std::string result;
    while(index < dataSize && !isSame<endIdentifier()>(m_bencodeData[index])) {
        result += m_bencodeData[index];
        ++index;
    }
    ++index;
    return result;
}

