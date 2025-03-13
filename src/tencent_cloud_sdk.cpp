#include "tencent_cloud_sdk.h"
#include "util/time.h"
#include "dll_interface.h"
#include "util/json.h"

#include <tencentcloud/core/TencentCloud.h>
#include <tencentcloud/core/profile/HttpProfile.h>
#include <tencentcloud/core/profile/ClientProfile.h>
#include <tencentcloud/core/Credential.h>
#include <tencentcloud/core/NetworkProxy.h>
#include <tencentcloud/core/AsyncCallerContext.h>
#include <tencentcloud/core/CommonClient.h>
#include <iostream>
#include <string>


using namespace TencentCloud;
using namespace std;


extern "C" {
#ifdef _WIN32
    DLL_EXPORT
#endif
        int fastweb_module_regist(void* sol2, void* lua)
    {
        sol::state* state = static_cast<sol::state*>(sol2);
        module::TencentCloudSDK::regist(state);
        return 0;
    }
}


void module::TencentCloudSDK::regist(sol::state* lua)
{
    lua->new_usertype<module::TencentCloudSDK>("fw_tencent_cloud_sdk",
        "RequestJson", &module::TencentCloudSDK::RequestJson
    );
}

std::tuple<bool, std::string> module::TencentCloudSDK::RequestJson(
    const std::string& SecretId,
    const std::string& SecretKey,
    const std::string& url,
    const std::string& service,
    const std::string& version,
    const std::string& action,
    const std::string& reqjson
)
{

    TencentCloud::InitAPI();

    // 实例化一个认证对象，入参需要传入腾讯云账户 SecretId 和 SecretKey，此处还需注意密钥对的保密
    // 代码泄露可能会导致 SecretId 和 SecretKey 泄露，并威胁账号下所有资源的安全性。以下代码示例仅供参考，建议采用更安全的方式来使用密钥，请参见：https://cloud.tencent.com/document/product/1278/85305
    // 密钥可前往官网控制台 https://console.cloud.tencent.com/cam/capi 进行获取
    Credential cred = Credential(SecretId, SecretKey);

    HttpProfile httpProfile = HttpProfile();
    httpProfile.SetEndpoint(url);

    ClientProfile clientProfile = ClientProfile(httpProfile);
    CommonClient common_client = CommonClient(service, version, cred, "", clientProfile);

    auto outcome = common_client.MakeRequestJson(action, reqjson);

    std::tuple<bool, std::string> result;
    if (outcome.IsSuccess())
    {
        auto r = outcome.GetResult();
        string payload = string(r.Body(), r.BodySize());
        result = std::make_tuple(true, payload);
    }
    else
    {
        result = std::make_tuple(false, outcome.GetError().PrintAll());
    }
    TencentCloud::ShutdownAPI();
    return result;
}
