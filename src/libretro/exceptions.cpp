/*
    Copyright 2023 Jesse Talavera-Greenberg

    melonDS DS is free software: you can redistribute it and/or modify it under
    the terms of the GNU General Public License as published by the Free
    Software Foundation, either version 3 of the License, or (at your option)
    any later version.

    melonDS DS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with melonDS DS. If not, see http://www.gnu.org/licenses/.
*/

#include "exceptions.hpp"
#include "format.hpp"

#include <optional>
#include <sstream>
#include <fmt/core.h>

using std::optional;
using std::string;
using std::string_view;

melonds::nds_firmware_not_bootable_exception::nds_firmware_not_bootable_exception(string_view firmwareName) noexcept
    : bios_exception(
    fmt::format(
        FMT_STRING(
            "The firmware file at \"{}\" can't be used to boot to the DS menu."
        ),
        firmwareName
    ),
    "Ensure you have native DS (not DSi) firmware in your frontend's system folder. "
    "Pick it in the core options, then restart the core. "
    "If you just want to play a DS game, try setting Boot Mode to \"Direct\" "
    "or BIOS/Firmware Mode to \"Built-In\" in the core options."
) {
}

melonds::nds_firmware_not_bootable_exception::nds_firmware_not_bootable_exception() noexcept
    : bios_exception(
    "The built-in firmware can't be used to boot to the DS menu.",
    "Ensure you have native DS (not DSi) firmware in your frontend's system folder. "
    "Pick it in the core options, then restart the core. "
    "If you just want to play a DS game, try setting Boot Mode to \"Direct\" "
    "or BIOS/Firmware Mode to \"Built-In\" in the core options."
) {
}

melonds::wrong_firmware_type_exception::wrong_firmware_type_exception(
    std::string_view firmwareName,
    melonds::ConsoleType consoleType,
    SPI_Firmware::FirmwareConsoleType firmwareConsoleType
) noexcept : bios_exception(
    fmt::format(
        FMT_STRING("The firmware file at \"{}\" is for the {}, but it can't be used in {} mode."),
        firmwareName,
        firmwareConsoleType,
        consoleType
    ),
    fmt::format(
        FMT_STRING(
            "Ensure you have a {}-compatible firmware file in your frontend's system folder (any name works). "
            "Pick it in the core options, then restart the core. "
            "If you just want to play a DS game, try disabling DSi mode in the core options."
        ),
        consoleType
    )
) {
}

melonds::dsi_no_firmware_found_exception::dsi_no_firmware_found_exception() noexcept
    : bios_exception(
    "DSi mode requires a native firmware file, but none could be found.",
    "Place your DSi firmware file in your frontend's system folder (any name works), then restart the core. "
    "If you want to play a regular DS game, try disabling DSi mode in the core options."
) {
}

melonds::firmware_missing_exception::firmware_missing_exception(std::string_view firmwareName) noexcept
    : bios_exception(
    fmt::format(
        FMT_STRING("The core is set to use the firmware file at \"{}\", but it wasn't there or it couldn't be loaded."),
        firmwareName),
    fmt::format(
        FMT_STRING(
            "Place your DSi firmware file in your frontend's system folder, name it \"{}\", then restart the core."
        ),
        firmwareName
    )
) {
}

melonds::nds_sysfiles_incomplete_exception::nds_sysfiles_incomplete_exception() noexcept
: bios_exception(
    "Booting to the native DS menu requires native DS firmware and BIOS files, "
    "but some of them were missing or couldn't be loaded.",
    "Place your DS system files in your frontend's system folder, then restart the core. "
    "If you want to play a regular DS game, try setting Boot Mode to \"Direct\" "
    "and BIOS/Firmware Mode to \"Built-In\" in the core options."
) {
}

melonds::dsi_missing_bios_exception::dsi_missing_bios_exception(melonds::BiosType bios, string_view biosName) noexcept
    : bios_exception(
    fmt::format(FMT_STRING("DSi mode requires the {} BIOS file, but none was found."), bios),
    fmt::format(
        FMT_STRING(
            "Place your {} BIOS file in your frontend's system folder, name it \"{}\", then restart the core. "
            "If you want to play a regular DS game, try disabling DSi mode in the core options."
        ),
        bios,
        biosName
    )
) {
}

melonds::dsi_no_nand_found_exception::dsi_no_nand_found_exception() noexcept
    : bios_exception(
    "DSi mode requires a NAND image, but none was found.",
    "Place your NAND file in your frontend's system folder (any name works), then restart the core. "
    "If you have multiple NAND files, you can choose one in the core options. "
    "If you want to play a regular DS game, try disabling DSi mode in the core options."
) {

}

melonds::dsi_nand_missing_exception::dsi_nand_missing_exception(string_view nandName) noexcept
    : bios_exception(
    fmt::format(
        FMT_STRING("The core is set to use the NAND file at \"{}\", but it wasn't there or it couldn't be loaded."),
        nandName),
    fmt::format(
        FMT_STRING("Place your NAND file in your frontend's system folder, name it \"{}\", then restart the core."),
        nandName
    )
) {

}

static std::string construct_missing_bios_message(const std::vector<std::string>& bios_files) {
    std::stringstream error;

    error << "Missing these BIOS files: ";
    for (size_t i = 0; i < bios_files.size(); ++i) {
        const std::string& file = bios_files[i];
        error << file;
        if (i < bios_files.size() - 1) {
            // If this isn't the last missing BIOS file...
            error << ", ";
        }
    }

    return error.str();
}

melonds::missing_bios_exception::missing_bios_exception(const std::vector<std::string>& bios_files)
    : bios_exception(construct_missing_bios_message(bios_files)) {
}