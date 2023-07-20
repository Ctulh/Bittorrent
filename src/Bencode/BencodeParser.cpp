/**
 * @file
 * @brief Implementation of the class BencodeParser
 */

#include "BencodeParser.hpp"
#include "Exceptions/NoSuchFieldException.hpp"
#include "StringMethods.hpp"
#include <stack>
#include <numeric>

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
    int numberOfBytesInFieldValue = 0;
    const int strSize = m_data.size();

    while(it < strSize && m_data[it] != ':') {
        numberOfBytesInFieldValue *= 10;
        numberOfBytesInFieldValue += m_data[it] - '0';
        ++it;
    }
    std::size_t beginOfTheFieldValue = it+1;

    std::string fieldValue = m_data.substr(beginOfTheFieldValue, numberOfBytesInFieldValue);
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

    int carry = 0;
    it += fieldName.size();

    const int dataSize = m_data.size();

    if(it < dataSize && m_data[it] != 'l') {
        return {getValue(fieldName)};
    }

    ++it;
    ++carry;

    StringVector output;
    while(carry != 0) {
        int listElementBytesLength = 0;
        char ch = m_data[it];
        while(it < dataSize && isdigit(m_data[it])) {
            listElementBytesLength *= 10;
            listElementBytesLength += m_data[it] - '0';
            ++it;
        }
        ++it;
        output.push_back(m_data.substr(it, listElementBytesLength));
        it += listElementBytesLength;
        if(it < dataSize && m_data[it] == 'e') {
            --carry;
            ++it;
        }
        if(it < dataSize && m_data[it] == 'l') {
            ++carry;
            ++it;
        }
    }
    std::string accumulatorString = std::accumulate(output.begin(), output.end(), std::string(),
                                [separator](std::string const& left, std::string const& right) {
                                                    if(left.empty())
                                                        return right;
                                                    return left + separator + right;
                                          });

    m_fieldValues[fieldName] = accumulatorString;
    return output;
}


