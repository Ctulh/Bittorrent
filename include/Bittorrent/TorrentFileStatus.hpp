#pragma once

/*!
 * @brief statuses of the file downloaded by the BitTorrent protocol.
 */

enum class TorrentFileStatus {
    NotDownloaded = 0,
    PartiallyDownloaded = 1,
    Complete = 2,
    Paused = 3,
    Error = 4,
};
