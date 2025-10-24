#include "DB.hpp"

#include <cstdint>

using namespace SqlDB;

DB::DB(const std::string& t_dbPath)
    : m_dbPath(t_dbPath)
{
    auto rc = sqlite3_open_v2(
        m_dbPath.c_str(),
        &m_db,
        SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_FULLMUTEX,
        nullptr);

    if (rc != SQLITE_OK) {
        std::string err = sqlite3_errmsg(m_db);
        sqlite3_close(m_db);
        m_isConnected = false;
        return;
    }
    m_isConnected = true;

    // Set busy timeout so writers wait instead of immediate error (ms)
    sqlite3_busy_timeout(m_db, 5000);
}

void DB::exec_sql(const std::string& sql)
{
    char* errMsg = nullptr;
    int rc = sqlite3_exec(m_db, sql.c_str(), nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK) {
        std::string e = errMsg ? errMsg : "unknown";
        sqlite3_free(errMsg);
        std::cout << "SQL error: " << e << std::endl;
    }
}

void DB::createTables()
{
    auto players_sql = R"sql(
        CREATE TABLE IF NOT EXISTS players (
          id INTEGER PRIMARY KEY AUTOINCREMENT,
          username TEXT NOT NULL UNIQUE,
          password_hash TEXT NOT NULL,
          xp INTEGER NOT NULL DEFAULT 0,
          level INTEGER NOT NULL DEFAULT 1,
          inventory TEXT, -- JSON string
          last_login_utc TEXT,
          created_at_utc TEXT NOT NULL DEFAULT (datetime('now')),
          updated_at_utc TEXT NOT NULL DEFAULT (datetime('now'))
        );
        CREATE INDEX IF NOT EXISTS idx_players_username ON players(username);
      )sql";

    auto migrations_sql = R"sql(
        CREATE TABLE IF NOT EXISTS schema_migrations (
          version INTEGER PRIMARY KEY,
          applied_at TEXT NOT NULL DEFAULT (datetime('now'))
        );
      )sql";

    exec_sql(players_sql);
    exec_sql(migrations_sql);
}

void DB::runMigrations()
{
    // Example: you would load and apply incremental migrations here.
    // This demo doesn't apply schema changes beyond create_tables.
    // Insert migration 1 if not present (no-op example)
    // In production, load SQL files and apply in order.
}

void DB::logError()
{
    std::cerr << "SQL error: " << sqlite3_errmsg(m_db) << std::endl;
}

void DB::init()
{
    // Set PRAGMA: WAL for concurrency, synchronous=NORMAL for performance with safety
    exec_sql("PRAGMA journal_mode = WAL;");
    exec_sql("PRAGMA synchronous = NORMAL;");
    exec_sql("PRAGMA foreign_keys = ON;");

    createTables();
    runMigrations();
}

// Insert player using prepared statement; returns lastRowid or nullopt on unique violation
std::optional<int64_t> DB::insert_player_stmt(const std::string& t_username, const std::string& t_pwHash)
{
    sqlite3_stmt* stmt = nullptr;
    const char* sql = "INSERT INTO players (username, password_hash) VALUES (?, ?);";
    int rc = sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        if (stmt)
            sqlite3_finalize(stmt);
        logError();
        return std::nullopt;
    }
    sqlite3_bind_text(stmt, 1, t_username.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, t_pwHash.c_str(), -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        int errcode = sqlite3_errcode(m_db);
        sqlite3_finalize(stmt);
        if (errcode == SQLITE_CONSTRAINT) {
            return std::nullopt;
        }
        logError();
        return std::nullopt;
    }
    int64_t id = sqlite3_last_insert_rowid(m_db);
    sqlite3_finalize(stmt);
    return id;
}

std::optional<std::pair<int64_t, std::string>> DB::get_player_hash(const std::string& t_username)
{
    sqlite3_stmt* stmt = nullptr;
    const char* sql = "SELECT id, password_hash FROM players WHERE username = ?;";
    int rc = sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        if (stmt)
            sqlite3_finalize(stmt);
        logError();
        return std::nullopt;
    }
    sqlite3_bind_text(stmt, 1, t_username.c_str(), -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        int64_t id = sqlite3_column_int64(stmt, 0);
        const unsigned char* text = sqlite3_column_text(stmt, 1);
        std::string hash = text ? reinterpret_cast<const char*>(text) : "";
        sqlite3_finalize(stmt);
        return std::make_pair(id, hash);
    }
    sqlite3_finalize(stmt);
    return std::nullopt;
}

bool DB::isUserRegistered(const std::string& t_username)
{
    if (get_player_hash(t_username)) {
        return true;
    }
    return false;
}

std::optional<int64_t> DB::registerPlayer(const std::string& t_username, const std::string& t_password)
{
    // Hash password afterward
    auto inserted = insert_player_stmt(t_username, t_password);
    return inserted;
}

std::optional<int64_t> DB::authenticate(const std::string& t_username, const std::string& t_password)
{
    auto row = get_player_hash(t_username);
    if (!row)
        return std::nullopt;
    int64_t id = row->first;
    const std::string& stored = row->second;

    // Verify
    if (stored == t_password) {
        // success - update last_login_utc
        sqlite3_stmt* stmt = nullptr;
        const char* sql = "UPDATE players SET last_login_utc = datetime('now') WHERE id = ?;";
        int rc = sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr);
        if (rc == SQLITE_OK) {
            sqlite3_bind_int64(stmt, 1, id);
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        } else if (stmt) {
            sqlite3_finalize(stmt);
        }
        return id;
    }
    return std::nullopt;
}

bool DB::addXP(const int64_t t_playerId, const int64_t t_delta)
{
    // Use a small transaction to ensure atomicity and consistent updated_at stamp
    char* errmsg = nullptr;
    int rc = sqlite3_exec(m_db, "BEGIN IMMEDIATE;", nullptr, nullptr, &errmsg);
    if (rc != SQLITE_OK) {
        if (errmsg)
            sqlite3_free(errmsg);
        return false;
    }
    sqlite3_stmt* stmt = nullptr;
    const char* sql = "UPDATE players SET xp = xp + ?, updated_at_utc = datetime('now') WHERE id = ?;";
    rc = sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        if (stmt)
            sqlite3_finalize(stmt);
        sqlite3_exec(m_db, "ROLLBACK;", nullptr, nullptr, nullptr);
        return false;
    }
    sqlite3_bind_int64(stmt, 1, t_delta);
    sqlite3_bind_int64(stmt, 2, t_playerId);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if (rc != SQLITE_DONE) {
        sqlite3_exec(m_db, "ROLLBACK;", nullptr, nullptr, nullptr);
        return false;
    }
    rc = sqlite3_exec(m_db, "COMMIT;", nullptr, nullptr, &errmsg);
    if (rc != SQLITE_OK) {
        if (errmsg)
            sqlite3_free(errmsg);
        sqlite3_exec(m_db, "ROLLBACK;", nullptr, nullptr, nullptr);
        return false;
    }
    return true;
}

bool DB::backup_to_file(const std::string& backup_path)
{
    sqlite3* dest = nullptr;
    int rc = sqlite3_open_v2(backup_path.c_str(), &dest, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    if (rc != SQLITE_OK) {
        if (dest)
            sqlite3_close(dest);
        std::cerr << "Failed to open backup target: " << sqlite3_errmsg(dest) << "\n";
        return false;
    }
    sqlite3_backup* pBackup = sqlite3_backup_init(dest, "main", m_db, "main");
    if (!pBackup) {
        std::cerr << "Backup init failed: " << sqlite3_errmsg(dest) << "\n";
        sqlite3_close(dest);
        return false;
    }
    rc = sqlite3_backup_step(pBackup, -1); // copy all pages
    if (rc != SQLITE_DONE) {
        std::cerr << "Backup step failed: " << rc << " " << sqlite3_errmsg(dest) << "\n";
        sqlite3_backup_finish(pBackup);
        sqlite3_close(dest);
        return false;
    }
    sqlite3_backup_finish(pBackup);
    sqlite3_close(dest);
    return true;
}

DB::~DB()
{
    if (m_db) {
        sqlite3_close(m_db);
    }
}
