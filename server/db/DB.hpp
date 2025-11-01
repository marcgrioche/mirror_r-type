#pragma once
#include <iostream>
#include <optional>
#include <sqlite3.h>
#include <string>

#include "PasswordManager.hpp"

namespace SqlDB {
class DB {
public:
    explicit DB(const std::string& t_dbPath);
    ~DB();

    /**
     * @brief Initialize the database, create tables if they don't exist, and run migrations.
     */
    void init();

    /**
     * @brief Try to register a new player with the given username and password.
     * @param t_username The username of the player to register.
     * @param t_password The password of the player to register.
     * @return The player ID if registration is successful, otherwise std::nullopt.
     */
    std::optional<int64_t> registerPlayer(const std::string& t_username, const std::string& t_password);

    /**
     * @brief Authenticate a player with the given username and password.
     * @param t_username The username of the player to authenticate.
     * @param t_password The password of the player to authenticate.
     * @return The player ID if authentication is successful, otherwise std::nullopt.
     */
    std::optional<int64_t> authenticate(const std::string& t_username, const std::string& t_password);

    /**
     * @brief add or subtract XP from a player's total.
     * @param t_playerId The player ID to add XP to.
     * @param t_delta The amount of XP to add (can be negative).
     * @return True if the operation was successful, false otherwise.
     */
    bool addXP(int64_t t_playerId, int64_t t_delta);

    /**
     * @brief Get the current XP of a player.
     * @param t_playerId The player ID to get XP for.
     * @return The current XP if the player exists, otherwise std::nullopt.
     */
    std::optional<int64_t> getXP(int64_t t_playerId);

    /**
     * @brief Backup the database to a specified file path.
     * @param t_backupPath The file path to back up the database to.
     * @return True if the backup was successful, false otherwise.
     */
    bool backup_to_file(const std::string& t_backupPath);

    /**
     * @brief Check if a user is already registered in the database.
     * @param t_username The username to check.
     * @return True if the user is registered, false otherwise.
     */
    bool isUserRegistered(const std::string& t_username);

private:
    std::string m_dbPath;
    sqlite3* m_db = nullptr;
    bool m_isConnected = false;
    PasswordManager m_passwordManager;

    void exec_sql(const std::string& sql);
    void createTables();
    void runMigrations();
    void logError();
    std::optional<int64_t> insert_player_stmt(const std::string& t_username, const std::string& t_pwHash);
    std::optional<std::pair<int64_t, std::string>> get_player_hash(const std::string& t_username);
};
}
