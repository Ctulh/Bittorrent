//
// Created by ctuh on 8/22/23.
//

#pragma once

enum class TorrentFileStatus {
    NotDownloaded = 0,
    PartiallyDownloaded = 1,
    Complete = 2,
    Paused = 3,
    Error = 4,
};