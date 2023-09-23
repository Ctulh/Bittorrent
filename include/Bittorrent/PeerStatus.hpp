#pragma once

enum class PeerStatus {
    Downloading = 0,
    Seeding = 1,
    Choked = 2,
    Unchoked = 3,
    Interested = 4,
    NotInterested = 5,
};
