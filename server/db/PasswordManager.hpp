#pragma once

#include <sodium.h>
#include <string>
#include <vector>

class PasswordManager {
public:
    PasswordManager();
    ~PasswordManager() = default;

    /**
     * @brief This function hashes a given password using the libsodium library.
     * @param t_password The password to hash.
     * @return returns The hashed password as a string.
     */
    std::string hashPassword(const std::string& t_password);

    /**
     * @brief This function verifies a password attempt against a stored hashed password.
     * @param t_hashedPassword The stored hashed password.
     * @param t_passwordAttempt The password attempt to verify.
     * @return bool True if the password attempt matches the hashed password, false otherwise.
     */
    bool verifyPassword(const std::string& t_hashedPassword, const std::string& t_passwordAttempt);

    /**
     * @brief This function checks if a hashed password needs to be rehashed based on current security parameters.
     * @param t_hashedPassword The hashed password to check.
     * @return bool True if the hashed password needs rehashing, false otherwise.
     */
    bool needsRehash(const std::string& t_hashedPassword);

private:
    int m_status = 0;
};
