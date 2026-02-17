#pragma once
#include "user.h"
#include "array"

constexpr const uint8_t maxUser = 4;

class user_manager{

private:
    std::array<user,maxUser> _users;

public:

    bool CheckAlarms(struct tm& timeNow);

    int8_t findUserId(const char* id) const;

    int8_t hasFreeUser() const;

    user* getUser(uint8_t index);

};