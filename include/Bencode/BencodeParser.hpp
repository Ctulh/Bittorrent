#pragma once

#include <string>
#include <unordered_map>
#include <optional>
#include <vector>

using OptionalString = std::optional<std::string>;
using StringVector = std::vector<std::string>;


class BencodeParser {
public:
/**
 * @param data - raw bencode data
*/
    BencodeParser(std::string const& data);

public:
/**
 * @brief overload of operator [].
 * @param [in] fieldName - the field name of the string whose value you want to get.
 * @return string value of the field.
 * @throw NoSuchFieldException If in m_bencodeData no such fieldName.
*/
    std::string operator[](std::string const& fieldName);

/**
 * @brief analog of 'operator []' overload
 * @param [in] fieldName - the field name of the string whose value you want to get.
 * @return string value of the field or throws NoSuchFieldException.
 * @throw NoSuchFieldException If in m_bencodeData no such fieldName.
*/
    std::string getValue(std::string const& fieldName);

/*!
 * @brief Method for getting list value in bencode data.
 * @param [in] fieldName - the field name of the string whose value you want to get.
 * @return A vector of strings that represent the elements of a list. If fieldName is not a list will return vector with single string.
 * @throw NoSuchFieldException If in m_bencodeData no such fieldName.
*/
    StringVector getList(std::string const& fieldName);

private:
/*!
 *
 * @param index - index in 'm_bencodeData' string where is bencode number field is located.
 * @return number which can be used in std::to_string().
*/
    std::string readNumber(std::size_t& index) const;

/*!
 *
 * @param index - index in 'm_bencodeData' string where is bencode dictionary field is located.
 * @return a bencode string with a dictionary that can be parsed further using BencodeParser.
*/
    std::string readDictionary(std::size_t& index) const;

/*!
 *
 * @param index - index in 'm_bencodeData' string where is bencode string field is located.
 * @return string for given a field name.
*/
    std::string readString(std::size_t& index) const;

/*!
 * @param index - index in 'm_bencodeData' string where is bencode list field is located.
 * @return a bencode string with a list that can be parsed further using BencodeParser.
*/
    StringVector readList(std::size_t& index) const;

private:
/*!
 * @return symbol which uses as separator when vector of strings concatenating for caching in 'm_fieldValues',
 *         and when string are splitting into vector of strings if fieldName already cached.
*/
    static char listSeparator();

/*!
 * @return character for list type in bencode format.
*/
    static constexpr char listIdentifier();

/*!
 * @return character for number type in bencode format.
*/
    static constexpr char numberIdentifier();

/*!
 * @return character for number type in bencode format. Actually string hasn't identifier in bencode format, but for specifying a string type this method exist.
*/
    static constexpr char stringIdentifier();

/*!
* @return character for dictionary type in bencode format.
*/
    static constexpr char dictionaryIdentifier();

/*!
 * @return character which represents end of any type in bencode format.
*/
    static constexpr char endIdentifier();

    template<char firstSymbol>
    static bool isSame(char secondSymbol) {
        return firstSymbol == secondSymbol;
    }

private:
    std::unordered_map<std::string, OptionalString> m_fieldValues;
    std::string m_bencodeData;
};
