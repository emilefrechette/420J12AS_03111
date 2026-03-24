/* Copyright (c) 2026. LetTheMiceFree. */
#include "db.h"

#include <string>

DatabaseClient::~DatabaseClient () { CloseDatabase (); }

Sint32
DatabaseClient::OpenDatabase ()
{
  SDL_CreateDirectory("data");
  Sint32 result = sqlite3_open ("data/scores.db", &database);
  if (result != SQLITE_OK)
    {
      SDL_LogError (1, "Cannot open database! %s", sqlite3_errmsg (database));
      sqlite3_close (database);
      database = nullptr;
      return result;
    }
  SDL_LogInfo (0, "Database opened!");
  result = CreateTable ();
  if (result != SQLITE_OK)
    {
      sqlite3_close (database);
      return result;
    }
  return SQLITE_OK;
}

Sint32
DatabaseClient::CreateTable ()
{
  std::string query
      = "CREATE TABLE IF NOT EXISTS highscores ("
        "   id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "   player_name TEXT NOT NULL,"
        "   score INTEGER NOT NULL"
        ");"
        "CREATE INDEX IF NOT EXISTS idx_score ON highscores(score DESC);";

  char *error_message = nullptr;
  const Sint32 result = sqlite3_exec (database, query.c_str (), nullptr,
                                      nullptr, &error_message);
  if (result != SQLITE_OK)
    {
      SDL_LogError (SDL_LOG_CATEGORY_ERROR, "Failed to create table: %s!",
                    error_message);
      sqlite3_free (error_message);
      return result;
    }
  SDL_LogInfo (SDL_LOG_CATEGORY_APPLICATION, "Table created successfully!");
  return SQLITE_OK;
}

Sint32
DatabaseClient::InsertScore (const ScoreRecord &score)
{
  std::string query = "INSERT INTO highscores (player_name, score) "
                      "VALUES (?, ?);";
  sqlite3_stmt *prepared_statement = nullptr;
  Sint32 result = sqlite3_prepare_v2 (database, query.c_str (), -1,
                                      &prepared_statement, nullptr);
  if (result != SQLITE_OK)
    {
      SDL_LogError (SDL_LOG_CATEGORY_ERROR,
                    "Failed to prepare statement: %s\n",
                    sqlite3_errmsg (database));
      return result;
    }

  result = sqlite3_bind_text (prepared_statement, 1,
                              score.player_name.c_str (), -1, SQLITE_STATIC);
  if (result != SQLITE_OK)
    {
      SDL_LogError (SDL_LOG_CATEGORY_ERROR, "Failed to bind player name: %s",
                    sqlite3_errmsg (database));
      sqlite3_finalize (prepared_statement);
      return result;
    }
  result = sqlite3_bind_int (prepared_statement, 2, score.value);
  if (result != SQLITE_OK)
    {
      SDL_LogError (SDL_LOG_CATEGORY_ERROR, "Failed to bind score: %s",
                    sqlite3_errmsg (database));
      sqlite3_finalize (prepared_statement);
      return result;
    }

  result = sqlite3_step (prepared_statement);
  if (result != SQLITE_DONE)
    {
      SDL_LogError (SDL_LOG_CATEGORY_ERROR, "Failed to insert record: %s\n",
                    sqlite3_errmsg (database));
      sqlite3_finalize (prepared_statement);
      return result;
    }

  SDL_LogInfo (SDL_LOG_CATEGORY_APPLICATION,
               "Score inserted successfully! Player: %s, Score: %d",
               score.player_name.c_str (), score.value);

  sqlite3_finalize (prepared_statement);
  return SQLITE_OK;
}

Sint32
DatabaseClient::GetHighScores (std::vector<ScoreRecord> &scores, Sint32 limit)
{
  std::string query = "SELECT player_name, score "
                      "FROM highscores "
                      "ORDER BY score DESC "
                      "LIMIT ?;";

  sqlite3_stmt *prepared_statement;
  Sint32 result = sqlite3_prepare_v2 (database, query.c_str (), -1,
                                      &prepared_statement, nullptr);

  if (result != SQLITE_OK)
    {
      SDL_LogError (SDL_LOG_CATEGORY_ERROR, "Failed to prepare statement: %s",
                    sqlite3_errmsg (database));
      return result;
    }

  result = sqlite3_bind_int (prepared_statement, 1, limit);
  if (result != SQLITE_OK)
    {
      SDL_LogError (SDL_LOG_CATEGORY_ERROR, "Failed to bind limit: %s",
                    sqlite3_errmsg (database));
      sqlite3_finalize (prepared_statement);
      return result;
    }

  scores.clear ();

  SDL_LogInfo (SDL_LOG_CATEGORY_APPLICATION, "Fetching top %d scores", limit);
  Sint32 rowCount = 0;
  while ((result = sqlite3_step (prepared_statement)) == SQLITE_ROW)
    {
      rowCount++;
      SDL_LogInfo (SDL_LOG_CATEGORY_APPLICATION, "Processing row %d",
                   rowCount);

      ScoreRecord score;
      const unsigned char *name = sqlite3_column_text (prepared_statement, 0);
      if (name)
        {
          score.player_name = reinterpret_cast<const char *> (name);
          SDL_LogInfo (SDL_LOG_CATEGORY_APPLICATION, "Player: %s",
                       score.player_name.c_str ());
        }
      score.value = sqlite3_column_int (prepared_statement, 1);
      SDL_LogInfo (SDL_LOG_CATEGORY_APPLICATION, "Score: %d", score.value);

      scores.push_back (score);
    }

  SDL_LogInfo (SDL_LOG_CATEGORY_APPLICATION, "Total rows retrieved: %d",
               rowCount);
  SDL_LogInfo (SDL_LOG_CATEGORY_APPLICATION, "Vector size after loop: %zu",
               scores.size ());

  if (result != SQLITE_DONE)
    {
      SDL_LogError (SDL_LOG_CATEGORY_ERROR, "Failed to fetch scores: %s",
                    sqlite3_errmsg (database));
      sqlite3_finalize (prepared_statement);
      return result;
    }

  sqlite3_finalize (prepared_statement);
  return SQLITE_OK;
}

Sint32
DatabaseClient::DeleteAllScores ()
{
  std::string query = "DELETE FROM highscores;";
  char *error_message = nullptr;

  Sint32 result
      = sqlite3_exec (database, query.c_str (), nullptr, nullptr, &error_message);

  if (result != SQLITE_OK)
    {
      SDL_LogError (SDL_LOG_CATEGORY_ERROR, "Failed to delete all scores: %s!",
                    error_message);
      sqlite3_free (error_message);
      return result;
    }

  SDL_LogInfo (SDL_LOG_CATEGORY_APPLICATION,
               "All scores deleted successfully!");
  return SQLITE_OK;
}

Sint32
DatabaseClient::CloseDatabase ()
{
  if (database)
    {
      Sint32 result = sqlite3_close (database);
      if (result == SQLITE_OK)
        {
          SDL_LogInfo (SDL_LOG_CATEGORY_APPLICATION, "Database closed!");
          database = nullptr;
        }
      else
        {
          SDL_LogError (SDL_LOG_CATEGORY_ERROR, "Failed to close database: %s",
                        sqlite3_errmsg (database));
        }
      return result;
    }
  return SQLITE_OK;
}
