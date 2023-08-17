//
// Created by ctuh on 7/27/23.
//

#pragma once

#include "UrlMethods.hpp"

#include <gtest/gtest.h>

TEST(UrlMethodsTest, TestUrlGetDomainWithEmptyString) {
    std::string result;
    EXPECT_NO_THROW(result = UrlMethods::getDomainName(""));
    ASSERT_TRUE(result.empty());
}

TEST(UrlMethodsTest, TestUrlGetDomainWithOnlyDomainName) {
    const std::string domainName = "www.test.com";
    const std::string url = domainName;

    std::string result;
    EXPECT_NO_THROW(result = UrlMethods::getDomainName(url));
    EXPECT_EQ(result, domainName);
}

TEST(UrlMethodsTest, TestUrlGetDomainWithSchemeAndDomainName) {
    const std::string domainName = "www.test.com";
    const std::string scheme = "http://";
    const std::string url = scheme + domainName;

    std::string result;
    EXPECT_NO_THROW(result = UrlMethods::getDomainName(url));
    EXPECT_EQ(result, domainName);
}

TEST(UrlMethodsTest, TestUrlGetDomainWithDomainNameAndPort) {
    const std::string domainName = "www.test.com";
    const std::string port = ":80";
    const std::string url = domainName + port;

    std::string result;
    EXPECT_NO_THROW(result = UrlMethods::getDomainName(url));
    EXPECT_EQ(result, domainName);
}

TEST(UrlMethodsTest, TestUrlGetDomainWithDomainNameAndPathToTheFile) {
    const std::string domainName = "www.test.com";
    const std::string path = "/path/to/file.html";
    const std::string url = domainName + path;

    std::string result;
    EXPECT_NO_THROW(result = UrlMethods::getDomainName(url));
    EXPECT_EQ(result, domainName);
}

TEST(UrlMethodsTest, TestUrlGetDomainWithAllUrlParts) {
    const std::string scheme = "http://";
    const std::string domainName = "www.test.com";
    const std::string port = ":80";
    const std::string path = "/path/to/file.html";
    const std::string parameters = "?key1=1&key2=2";
    const std::string anchor = "#Somewhere";
    const std::string url = scheme + domainName + port + path + parameters + anchor;

    std::string result;
    EXPECT_NO_THROW(result = UrlMethods::getDomainName(url));
    EXPECT_EQ(result, domainName);
}

TEST(UrlMethodsTest, TestGetEmptyPath) {
    const std::string domainName = "www.test.com";
    const std::string path = "";
    const std::string url = domainName + path;

    std::string result;
    EXPECT_NO_THROW(result = UrlMethods::getPathToFile(url));
    ASSERT_TRUE(result.empty());
}

TEST(UrlMethodsTest, TestGetPathWithFile) {
    const std::string domainName = "www.test.com";
    const std::string path = "/file.html";
    const std::string url = domainName + path;

    std::string result;
    EXPECT_NO_THROW(result = UrlMethods::getPathToFile(url));
    EXPECT_EQ(path, result);
}

TEST(UrlMethodsTest, TestGetPathWithManyDirectories) {
    const std::string domainName = "www.test.com";
    const std::string filename = "/file.html";
    const std::size_t directoriesAmount = 10;
    std::string fullPath;

    for(std::size_t i = 1; i <= directoriesAmount; ++i) {
        fullPath += "/";
        fullPath += std::to_string(i);
    }
    fullPath += filename;

    const std::string url = domainName + fullPath;
    std::string result;

    EXPECT_NO_THROW(result = UrlMethods::getPathToFile(url));
    EXPECT_EQ(result, fullPath);
}

TEST(UrlMethodsTest, TestGetPathWithPort) {
    const std::string domainName = "www.test.com";
    const std::string port = ":80";
    const std::string path = "/file.html";
    const std::string url = domainName + port + path;

    std::string result;

    EXPECT_NO_THROW(result = UrlMethods::getPathToFile(url));
    EXPECT_EQ(result, path);
}

TEST(UrlMethodsTest, TestGetPathParameters) {
    const std::string domainName = "www.test.com";
    const std::string path = "/file.html";
    const std::string parameters = "?key1=1&key2=2";
    const std::string url = domainName + path + parameters;

    std::string result;

    EXPECT_NO_THROW(result = UrlMethods::getPathToFile(url));
    EXPECT_EQ(result, path);
}

TEST(UrlMethodsTest, TestGetPathWithAllUrlParts) {
    const std::string scheme = "http://";
    const std::string domainName = "www.test.com";
    const std::string port = ":80";
    const std::string path = "/path/to/file.html";
    const std::string parameters = "?key1=1&key2=2";
    const std::string anchor = "#Somewhere";
    const std::string url = scheme + domainName + port + path + parameters + anchor;

    std::string result;
    EXPECT_NO_THROW(result = UrlMethods::getPathToFile(url));
    EXPECT_EQ(result, path);
}

TEST(UrlMethodsTest, TestGetParametersOnEmptyString) {
    const std::string url = "";
    std::string result;

    EXPECT_NO_THROW(result = UrlMethods::getParameters(url));
    ASSERT_TRUE(result.empty());
}

TEST(UrlMethodsTest, TestGetParametersOnOnlyDomain) {
    const std::string domainName = "www.test.com";
    const std::string url = domainName;
    std::string result;

    EXPECT_NO_THROW(result = UrlMethods::getParameters(url));
    ASSERT_TRUE(result.empty());
}

TEST(UrlMethodsTest, TestGetParametersOnUrlWithDomainName) {
    const std::string domainName = "www.test.com";
    const std::string path = "/file";
    const std::string parameters = "?key1=1&key2=2";
    const std::string url = domainName + path + parameters;

    std::string result;

    EXPECT_NO_THROW(result = UrlMethods::getParameters(url));
    EXPECT_EQ(result, parameters);
}

TEST(UrlMethodsTest, TestGetParametersOnUrlWithPort) {
    const std::string domainName = "www.test.com";
    const std::string path = "/file";
    const std::string port = ":80";
    const std::string parameters = "?key1=1&key2=2";
    const std::string url = domainName + port + path + parameters;

    std::string result;

    EXPECT_NO_THROW(result = UrlMethods::getParameters(url));
    EXPECT_EQ(result, parameters);
}

TEST(UrlMethodsTest, TestGetParamteresWithAllUrlParts) {
    const std::string scheme = "http://";
    const std::string domainName = "www.test.com";
    const std::string port = ":80";
    const std::string path = "/path/to/file.html";
    const std::string parameters = "?key1=1&key2=2";
    const std::string anchor = "#Somewhere";
    const std::string url = scheme + domainName + port + path + parameters + anchor;

    std::string result;
    EXPECT_NO_THROW(result = UrlMethods::getParameters(url));
    EXPECT_EQ(result, parameters);
}

TEST(UrlMethodsTest, TestGetDataAfterDomainNameWithPath) {
    const std::string domainName = "www.test.com";
    const std::string path = "/path/to/file.html";
    const std::string url = domainName + path;

    std::string result;

    EXPECT_NO_THROW(result = UrlMethods::getDataAfterDomainName(url));
    EXPECT_EQ(result, path);
}

TEST(UrlMethodsTest, TestGetDataAfterDomainNameWithPortAndPath) {
    const std::string domainName = "www.test.com";
    const std::string path = "/path/to/file.html";
    const std::string port = ":80";
    const std::string url = domainName + port + path;

    std::string result;
    EXPECT_NO_THROW(result = UrlMethods::getDataAfterDomainName(url));
    EXPECT_EQ(result, path);
}

TEST(UrlMethodsTest, TestGetDataAfterDomainNameWithPathAndParameters) {
    const std::string domainName = "www.test.com";
    const std::string path = "/path/to/file.html";
    const std::string parameters = "?key1=1&key2=2";
    const std::string url = domainName + path + parameters;

    std::string result;

    EXPECT_NO_THROW(result = UrlMethods::getDataAfterDomainName(url));
    EXPECT_EQ(result, path + parameters);
}
