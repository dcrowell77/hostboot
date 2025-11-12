/* 
 Complete the code for step 7 based on the documentation and an example.
 
 Boot Step 7
 - 7.1 > mss_freq
   - Execute the p9_mss_freq HWP against all MCS targets
   - Execute the p9_mss_freq_system HWP against all MCBIST targets   
 - 7.2 > mss_volt
   - Execute the p9_mss_volt HWP against all PMC targets
*/


/******************************************************************************/
// Includes
/******************************************************************************/
#include    <stdint.h>
#include    <trace/interface.H>
#include    <initservice/taskargs.H>
#include    <errl/errlentry.H>
#include    <isteps/hwpisteperror.H>
#include    <errl/errludtarget.H>
#include    <initservice/isteps_trace.H>
#include    <targeting/common/commontargeting.H>
#include    <targeting/common/utilFilter.H>
#include    <fapi2.H>
#include    <fapi2/plat_hwp_invoker.H>
#include    <util/utilmbox_scratch.H>
#include    <sbeif.H>
#include    <p9_mss_freq.H>
#include    <p9_mss_freq_system.H>


namespace   BOOTSTEP_07
{

using   namespace   ISTEP;
using   namespace   ISTEP_ERROR;
using   namespace   ERRORLOG;
using   namespace   TARGETING;


// Execute the mss_freq substep
void* step_mss_freq( void )
{
    TRACFCOMP( ISTEPS_TRACE::g_trac_isteps_trace, "step_mss_freq entry" );
    IStepError l_StepError;
    errlHndl_t l_err = nullptr;

	//-- Execute p9_mss_freq --
	
	// Get the list of MCS targets from the Hostboot targeting model
	TARGETING::TargetHandleList l_mcsTargetList;
	getAllChiplets(l_mcsTargetList, TYPE_MCS);

	// Loop through all of the MCS targets
	for (const auto & l_mcs_target : l_mcsTargetList)
	{
		TRACFCOMP( ISTEPS_TRACE::g_trac_isteps_trace,
                    "p9_mss_freq HWP target HUID %.8x",
                    TARGETING::get_huid(l_mcs_target));

		// Convert the HB target into the FAPI target
		fapi2::Target <fapi2::TARGET_TYPE_MCS> l_fapi_mcs_target(l_mcs_target);

		// Execute the HWP function 
		FAPI_INVOKE_HWP(l_err, p9_mss_freq, l_fapi_mcs_target);

		//  process return code.
		if ( l_err )
		{
			TRACFCOMP( ISTEPS_TRACE::g_trac_isteps_trace,
                       "ERROR 0x%.8X:  p9_mss_freq HWP on target HUID %.8x",
                       l_err->reasonCode(), TARGETING::get_huid(l_mcs_target) );

			// capture the target data in the elog
			ErrlUserDetailsTarget(l_mcs_target).addToLog( l_err );

			// Create IStep error log and cross reference to error that occurred
			l_StepError.addErrorDetails( l_err );

			// Commit Error
			errlCommit( l_err, ISTEP_COMP_ID );
		}
		else
		{
			TRACFCOMP( ISTEPS_TRACE::g_trac_isteps_trace,
                       "SUCCESS :  p9_mss_freq HWP");
		}
	} // End mcs loop


	//-- Execute p9_mss_freq_system --

	// Get the list of MCBIST targets from the Hostboot targeting model
    getAllChiplets(l_mcbistTargetList, TYPE_MCBIST);
    std::vector< fapi2::Target<fapi2::TARGET_TYPE_MCBIST> > l_fapi2_mcbistTargetList;

	// Loop through all of the MCBIST targets
	for (const auto & l_mcbist_target : l_mcbistTargetList)
	{
		TRACFCOMP( ISTEPS_TRACE::g_trac_isteps_trace,
                    "p9_mss_freq_system HWP target HUID %.8x",
                    TARGETING::get_huid(l_mcbist_target));

		// Convert the HB target into the FAPI target
		std::vector< fapi2::Target<fapi2::TARGET_TYPE_MCBIST> > l_fapi2_mcbistTargetList;

		// Execute the HWP function 
		FAPI_INVOKE_HWP(l_err, p9_mss_freq_system, l_fapi2_mcbistTargetList);

		//  process return code.
		if ( l_err )
		{
			TRACFCOMP( ISTEPS_TRACE::g_trac_isteps_trace,
                       "ERROR 0x%.8X:  p9_mss_freq_system HWP on target HUID %.8x",
                       l_err->reasonCode(), TARGETING::get_huid(l_mcbist_target) );

			// capture the target data in the elog
			ErrlUserDetailsTarget(l_mcbist_target).addToLog( l_err );

			// Create IStep error log and cross reference to error that occurred
			l_StepError.addErrorDetails( l_err );

			// Commit Error
			errlCommit( l_err, ISTEP_COMP_ID );
		}
		else
		{
			TRACFCOMP( ISTEPS_TRACE::g_trac_isteps_trace,
                       "SUCCESS :  p9_mss_freq_system HWP");
		}
	} // End mcbist loop

    TRACFCOMP( ISTEPS_TRACE::g_trac_isteps_trace, "step_mss_freq exit" );
    return l_StepError.getErrorHandle();
}


// Execute the mss_volt substep
void* step_mss_volt( void )
{
	// ... fill this in ...
}
