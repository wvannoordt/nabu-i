#include <string>
#include <filesystem>
#include <print.h>
#include "nbi.h"

static std::string get_asset_path(int argc, char** argv, bool& failure, std::string& fail_message)
{
    failure = false;
    fail_message = "no error";
    std::string nbi_path = "NOPATH";
    if (argc>1)
    {
        nbi_path = std::string(argv[1]);
    }
    else
    {
        failure = true;
        fail_message = "Did not specify program root path";
        return "ERR";
    }
    
    std::filesystem::path asset_path(nbi_path);
    std::string asset_base_dir = "assets";
    std::string asset_dir      = "default";
    if (argc>2)
    {
        asset_dir = std::string(argv[2]);
        print(argc, asset_dir);
    }
    asset_path /= asset_base_dir;
    asset_path /= "profiles";
    asset_path /= asset_dir;
    if (!std::filesystem::exists(asset_path))
    {
        failure = true;
        fail_message = "Cannot find path " + std::string(asset_path);
    }
    return std::string(asset_path);
}

int main(int argc, char** argv)
{
    bool failure;
    std::string er_msg;
    std::string full_asset_path = get_asset_path(argc, argv, failure, er_msg);
    if (failure)
    {
        print(er_msg);
        return 1;
    }
    nbi::assets_t nbi_assets(full_asset_path);
    nbi::root_window_t app(nbi_assets);
    app.run();
    return 0;
}