/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/memory/lib/eff_config/timing.C $ */
/*                                                                        */
/* OpenPOWER HostBoot Project                                             */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2017                        */
/* [+] International Business Machines Corp.                              */
/*                                                                        */
/*                                                                        */
/* Licensed under the Apache License, Version 2.0 (the "License");        */
/* you may not use this file except in compliance with the License.       */
/* You may obtain a copy of the License at                                */
/*                                                                        */
/*     http://www.apache.org/licenses/LICENSE-2.0                         */
/*                                                                        */
/* Unless required by applicable law or agreed to in writing, software    */
/* distributed under the License is distributed on an "AS IS" BASIS,      */
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or        */
/* implied. See the License for the specific language governing           */
/* permissions and limitations under the License.                         */
/*                                                                        */
/* IBM_PROLOG_END_TAG                                                     */
// *HWP HWP Owner: Andre Marin <aamarin@us.ibm.com>
// *HWP HWP Backup: Jacob Harvey <jlharvey@us.ibm.com>
// *HWP Team: Memory
// *HWP Level: 3
// *HWP Consumed by: FSP:HB


#include <fapi2.H>
#include <mss.H>
#include <generic/memory/lib/utils/find.H>
#include <lib/eff_config/timing.H>

namespace mss
{

enum temp_mode : uint8_t
{
    NORMAL = 1,
    EXTENDED = 2,
};

// Proposed DDR4 Full spec update(79-4B)
// Item No. 1716.78C
// pg.46
// Table 24 - tREFI and tRFC parameters (in ps)
constexpr uint64_t TREFI_BASE = 7800000;

// Proposed DDR4 3DS Addendum
// Item No. 1727.58A
// pg. 69 - 71
// Table 42 - Refresh parameters by logical rank density
static const std::vector<std::pair<uint8_t, uint64_t> > TRFC_DLR1 =
{
    // { density in GBs, tRFC4(min) in picoseconds }
    {4, 90000},
    {8, 120000},
    // 16Gb - TBD
};

// Proposed DDR4 3DS Addendum
// Item No. 1727.58A
// pg. 69 - 71
// Table 42 - Refresh parameters by logical rank density
static const std::vector<std::pair<uint8_t, uint64_t> > TRFC_DLR2 =
{
    // { density in GBs, tRFC4(min) in picoseconds }
    {4, 55000},
    {8, 90000}
    // 16Gb - TBD
};

// Proposed DDR4 3DS Addendum
// Item No. 1727.58A
// pg. 69 - 71
// Table 42 - Refresh parameters by logical rank density
static const std::vector<std::pair<uint8_t, uint64_t> > TRFC_DLR4 =
{
    // { density in GBs, tRFC4(min) in picoseconds }
    {4, 40000},
    {8, 55000}
    // 16Gb - TBD
};

///
/// @brief Calculates refresh interval time
/// @param[in] i_mode fine refresh rate mode
/// @param[in] i_refresh_request_rate refresh rate
/// @param[out] o_value timing val in ps
/// @return fapi2::ReturnCode
///
fapi2::ReturnCode calc_trefi( const refresh_rate i_mode,
                              const uint8_t i_refresh_request_rate,
                              uint64_t& o_timing )
{
    uint64_t l_refresh_request = 0;
    constexpr double TEN_PERCENT_FASTER = 0.90;

    switch(i_refresh_request_rate)
    {
        case fapi2::ENUM_ATTR_MSS_MRW_REFRESH_RATE_REQUEST_SINGLE:
            l_refresh_request = TREFI_BASE;
            break;

        case fapi2::ENUM_ATTR_MSS_MRW_REFRESH_RATE_REQUEST_DOUBLE:
            // We are truncating but there is no remainder with TREFI_BASE, so we are okay
            l_refresh_request = TREFI_BASE / 2;
            break;

        case fapi2::ENUM_ATTR_MSS_MRW_REFRESH_RATE_REQUEST_SINGLE_10_PERCENT_FASTER:
            // We are truncating but there is no remainder with TREFI_BASE, so we are okay
            // 10% faster so 100% - 10% = 90%
            l_refresh_request = TREFI_BASE * TEN_PERCENT_FASTER;
            break;

        case fapi2::ENUM_ATTR_MSS_MRW_REFRESH_RATE_REQUEST_DOUBLE_10_PERCENT_FASTER:
            // We are truncating but there is no remainder with TREFI_BASE, so we are okay
            // 10% faster so 100% - 10% = 90%
            l_refresh_request = (TREFI_BASE / 2) * TEN_PERCENT_FASTER;
            break;

        default:
            // Will catch incorrect MRW value set
            FAPI_ASSERT(false,
                        fapi2::MSS_INVALID_REFRESH_RATE_REQUEST().set_REFRESH_RATE_REQUEST(i_refresh_request_rate),
                        "Incorrect refresh request rate received: %d ", i_refresh_request_rate);
            break;
    }

    o_timing =  (l_refresh_request / i_mode);

    FAPI_INF( "tREFI (ps): %d, refresh request (ps): %d, tREFI_base (ps): %d, REF%dX",
              o_timing, l_refresh_request, TREFI_BASE, i_mode );

    // FAPI_ASSERT doesn't set current_err as good
    return fapi2::FAPI2_RC_SUCCESS;

fapi_try_exit:
    return fapi2::current_err;
}

/// @brief Calculates Minimum Refresh Recovery Delay Time (different logical rank)
/// @param[in] i_mode fine refresh rate mode
/// @param[in] i_density SDRAM density
/// @param[out] o_trfc_in_ps timing val in ps
/// @return fapi2::FAPI2_RC_SUCCESS iff okay
///
fapi2::ReturnCode calc_trfc_dlr(const fapi2::Target<fapi2::TARGET_TYPE_DIMM>& i_target,
                                const uint8_t i_refresh_mode,
                                const uint8_t i_density,
                                uint64_t& o_trfc_in_ps)
{
    bool l_is_val_found = 0;

    // Selects appropriate tRFC based on fine refresh mode
    switch(i_refresh_mode)
    {
        case fapi2::ENUM_ATTR_MSS_MRW_FINE_REFRESH_MODE_NORMAL:
            l_is_val_found = find_value_from_key(TRFC_DLR1, i_density, o_trfc_in_ps);
            break;

        case fapi2::ENUM_ATTR_MSS_MRW_FINE_REFRESH_MODE_FIXED_2X:
        case fapi2::ENUM_ATTR_MSS_MRW_FINE_REFRESH_MODE_FLY_2X:
            l_is_val_found = find_value_from_key(TRFC_DLR2, i_density, o_trfc_in_ps);
            break;

        case fapi2::ENUM_ATTR_MSS_MRW_FINE_REFRESH_MODE_FIXED_4X:
        case fapi2::ENUM_ATTR_MSS_MRW_FINE_REFRESH_MODE_FLY_4X:
            l_is_val_found = find_value_from_key(TRFC_DLR4, i_density, o_trfc_in_ps);
            break;

        default:
            // Fine Refresh Mode will be a platform attribute set by the MRW,
            // which they "shouldn't" mess up as long as use "attribute" enums.
            // if openpower messes this up we can at least catch it
            FAPI_ASSERT( false,
                         fapi2::MSS_INVALID_FINE_REFRESH()
                         .set_REFRESH_MODE(i_refresh_mode),
                         "Incorrect Fine Refresh Mode received: %d ",
                         i_refresh_mode);
            break;
    }// switch

    FAPI_ASSERT( l_is_val_found,
                 fapi2::MSS_FAILED_TO_FIND_TRFC()
                 .set_SDRAM_DENSITY(i_density)
                 .set_REFRESH_MODE(i_refresh_mode)
                 .set_DIMM_TARGET(i_target),
                 "%s: Unable to find tRFC (ps) from map with SDRAM density key %d with %d refresh mode",
                 mss::c_str(i_target),
                 i_density,
                 i_refresh_mode);

    // Again, FAPI_ASSERT doesn't set current_err to good, only to bad
    return fapi2::FAPI2_RC_SUCCESS;
fapi_try_exit:
    return fapi2::current_err;
}

}// mss
