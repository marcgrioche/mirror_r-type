#include "PasswordManager.hpp"
#include <iostream>

PasswordManager::PasswordManager()
{
    if (sodium_init() < 0) {
        m_status = -1;
        std::cout << "Failed to initialize libsodium" << std::endl;
    }
    m_status = 1;
}

std::string PasswordManager::hashPassword(const std::string& t_password)
{
    std::vector<char> hashed(crypto_pwhash_STRBYTES);

    if (m_status < 0)
        return "";
    if (crypto_pwhash_str(
            hashed.data(),
            t_password.c_str(),
            t_password.length(),
            crypto_pwhash_OPSLIMIT_INTERACTIVE, // For interactive logins
            crypto_pwhash_MEMLIMIT_INTERACTIVE)
        != 0) {
        throw std::runtime_error("Password hashing failed (out of memory)");
    }

    return std::string(hashed.data());
}

bool PasswordManager::verifyPassword(const std::string& t_hashedPassword,
    const std::string& t_passwordAttempt)
{
    if (m_status < 0)
        return false;
    std::cout << "Verification in progress..." << std::endl;
    std::cout << "Attempt: " << t_passwordAttempt << std::endl;
    std::cout << "Real: " << t_hashedPassword << std::endl;
    return crypto_pwhash_str_verify(
               t_hashedPassword.c_str(),
               t_passwordAttempt.c_str(),
               t_passwordAttempt.length())
        == 0;
}

bool PasswordManager::needsRehash(const std::string& t_hashedPassword)
{
    if (m_status < 0)
        return false;
    return crypto_pwhash_str_needs_rehash(
               t_hashedPassword.c_str(),
               crypto_pwhash_OPSLIMIT_INTERACTIVE,
               crypto_pwhash_MEMLIMIT_INTERACTIVE)
        != 0;
}
