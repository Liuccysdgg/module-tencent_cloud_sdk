#pragma once
#include "db/mysql.h"
#include "db/sqler.h"
#include "sol/sol.hpp"
#include "basemodule.h"
#include <tuple>
namespace module
{
    class TencentCloudSDK{
    public:
        // 请求json
        std::tuple<bool, std::string> RequestJson(
            const std::string& SecretId,
            const std::string& SecretKey,
            const std::string& url,
            const std::string& service,
            const std::string& version,
            const std::string& action,
            const std::string& reqjson
        );   
    public:
        static void regist(sol::state* lua);
    };
}

