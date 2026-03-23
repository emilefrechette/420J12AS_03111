/* Copyright (c) 2026. LetTheMiceFree. */

#ifndef DB_MODULE_H
#define DB_MODULE_H

#include <SDL3/SDL.h>
#include <sqlite3.h>
#include <string>
#include <vector>

struct ScoreRecord
{
  std::string player_name;
  Sint32 value;
};

class DatabaseClient final
{
  sqlite3 *database;

public:
  DatabaseClient ();
  ~DatabaseClient ();

  Sint32 OpenDatabase ();
  Sint32 InsertScore (const ScoreRecord &score);
  Sint32 GetHighScores (std::vector<ScoreRecord> &scores, Sint32 limit);
  Sint32 DeleteAllScores ();

private:
  Sint32 CreateTable ();
  Sint32 CloseDatabase ();
};

#endif /* DB_MODULE_H */
