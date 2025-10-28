#pragma once
#include <iostream>
#include <optional>
#include <sqlite3.h>
#include <string>

namespace SqlDB {
class DB {
public:
    explicit DB(const std::string& t_dbPath);
    ~DB();

    void init();
    std::optional<int64_t> registerPlayer(const std::string& t_username, const std::string& t_password);

    // Authenticate player; returns player id if OK, otherwise std::nullopt
    std::optional<int64_t> authenticate(const std::string& t_username, const std::string& t_password);

    // Atomically add XP and optionally log it. Returns true on success.
    bool addXP(int64_t t_playerId, int64_t t_delta);

    // Get player's current XP. Returns XP value or nullopt if not found.
    std::optional<int64_t> getXP(int64_t t_playerId);

    // Create a safe backup into the given file path using sqlite3_backup API.
    bool backup_to_file(const std::string& t_backupPath);

    bool isUserRegistered(const std::string& t_username);

private:
    std::string m_dbPath;
    sqlite3* m_db = nullptr;
    bool m_isConnected = false;

    void exec_sql(const std::string& sql);
    void createTables();
    void runMigrations();
    void logError();
    std::optional<int64_t> insert_player_stmt(const std::string& t_username, const std::string& t_pwHash);
    std::optional<std::pair<int64_t, std::string>> get_player_hash(const std::string& t_username);
};
}
