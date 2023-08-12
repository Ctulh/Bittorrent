//
// Created by ctuh on 8/8/23.
//

#pragma once

struct PieceInfo {
    int size;
    int offset;
    int pieceId;
};


class TorrentInfo {
public:
    void commit(int operationId);
    PieceInfo getPiece();
private:
    int bytesDownloaded;
};
