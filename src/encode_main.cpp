#include "wspr/wspr_ref_api.hpp"

#include <nlohmann/json.hpp>

#include <exception>
#include <iostream>
#include <string>

namespace
{
    using json = nlohmann::json;

    std::string cli_type_from_plan(const std::string &plan_type)
    {
        if (plan_type == "Type1Single")
            return "TYPE1";
        if (plan_type == "Type2Single")
            return "TYPE2";
        if (plan_type == "Type3Single")
            return "TYPE3";
        if (plan_type == "Type2Type3Paired")
            return "TYPE2+TYPE3";
        return "UNKNOWN";
    }
} // namespace

int main(int argc, char **argv)
{
    bool quiet = false;
    bool json_mode = false;
    int argi = 1;

    while (argi < argc)
    {
        const std::string arg = argv[argi];

        if (arg == "--quiet" || arg == "--symbols-only")
        {
            quiet = true;
            ++argi;
            continue;
        }

        if (arg == "--json")
        {
            json_mode = true;
            ++argi;
            continue;
        }

        break;
    }

    if (quiet && json_mode)
    {
        std::cerr << "Use either --quiet/--symbols-only or --json, not both.\n";
        return 1;
    }

    if ((argc - argi) != 3)
    {
        std::cerr
            << "Usage: wspr-encode [--quiet|--symbols-only|--json] "
            << "<callsign> <locator> <power_dbm>\n";
        return 1;
    }

    const std::string callsign = argv[argi];
    const std::string locator = argv[argi + 1];

    int power_dbm = 0;
    try
    {
        power_dbm = std::stoi(argv[argi + 2]);
    }
    catch (const std::exception &)
    {
        std::cerr << "Invalid power value.\n";
        return 1;
    }

    const wspr::WsprEncodeResult result =
        wspr::encode_message(callsign, locator, power_dbm);

    if (!result.ok)
    {
        std::cerr << result.error << "\n";
        return 1;
    }

    const std::string cli_type = cli_type_from_plan(result.type);

    if (json_mode)
    {
        json j;
        j["type"] = cli_type;
        j["callsign"] = result.callsign;
        j["locator"] = result.locator;
        j["power_dbm"] = result.power_dbm;
        if (result.symbols_list.size() <= 1)
        {
            j["symbols"] = result.symbols;
        }
        else
        {
            j["symbols_list"] = result.symbols_list;
        }

        std::cout << j.dump(2) << "\n";
        return 0;
    }

    if (quiet)
    {
        if (result.symbols_list.size() <= 1)
        {
            std::cout << result.symbols << "\n";
        }
        else
        {
            for (const auto &symbols : result.symbols_list)
                std::cout << symbols << "\n";
        }
        return 0;
    }

    std::cout << "Type: " << cli_type << "\n";
    std::cout << "Callsign: " << result.callsign << "\n";
    std::cout << "Locator: " << result.locator << "\n";
    std::cout << "Power: " << result.power_dbm << " dBm\n";
    if (result.symbols_list.size() <= 1)
    {
        std::cout << "Symbols: " << result.symbols << "\n";
    }
    else
    {
        std::cout << "Symbols 1: " << result.symbols_list[0] << "\n";
        std::cout << "Symbols 2: " << result.symbols_list[1] << "\n";
    }

    return 0;
}
