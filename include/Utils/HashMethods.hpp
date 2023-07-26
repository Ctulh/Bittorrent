//
// Created by ctuh on 7/26/23.
//

#pragma once

#include <boost/uuid/detail/sha1.hpp>
#include <array>

namespace hash {
    using Sha1Hash = std::array<std::byte, 20>;

    Sha1Hash stringHashToBytes(std::string const& hash);
    Sha1Hash sha1(std::string const& str);
    std::string urlEncodeSha1Hash(Sha1Hash const& hash);

}
