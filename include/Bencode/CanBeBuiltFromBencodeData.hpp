#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <concepts>
#include "Bencode/BencodeParser.hpp"
#include "Logger/Logger.hpp"

/*!
 * @brief CRTP class. Means that T must override 'getFieldsToFill' method which will return vector of fieldNames that will parser from 'bencodeData'
 *        and saved to map 'm_bencodeValues'.
 */
template <typename T>
class CanBeBuiltFromBencodeData {
private:
    CanBeBuiltFromBencodeData() = default;
public:
    CanBeBuiltFromBencodeData(std::string const& bencodeData) {
        auto fieldsToFill = T().getFieldsToFill();
        BencodeParser parser(bencodeData);
        for(auto&& [fieldName, fieldType]: fieldsToFill) {
            if(fieldType == 'l') {
                m_bencodeLists[fieldName] = parser.getList(fieldName);
            }
            else {
                m_bencodeValues[fieldName] = parser.getValue(fieldName);
            }
        }
    }

friend T;
public:
    virtual std::vector<std::pair<std::string, char>> getFieldsToFill() const = 0;

protected:
    std::unordered_map<std::string, std::string> m_bencodeValues;
    std::unordered_map<std::string, std::vector<std::string>> m_bencodeLists;
};

template<typename T>
concept CanConstructedFromBencodeData = std::is_base_of_v<CanBeBuiltFromBencodeData<T>, T>;
