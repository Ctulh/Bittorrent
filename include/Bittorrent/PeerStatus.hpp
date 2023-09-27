#pragma once

enum class PeerStatus {
    NONE = -1,
    Downloading = 0,
    Seeding = 1,
    Choked = 2,
    Unchoked = 3,
    Interested = 4,
    NotInterested = 5,
};
