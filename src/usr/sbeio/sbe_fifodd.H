/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/usr/sbeio/sbe_fifodd.H $                                  */
/*                                                                        */
/* OpenPOWER HostBoot Project                                             */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2012,2017                        */
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

#ifndef __SBE_FIFODD_H
#define __SBE_FIFODD_H

/**
 * @file sbe_fifodd.H
 * @brief SBE service provider declaration
 */

#include <stdint.h>
#include <builtins.h>
#include <sys/time.h>

#include <errl/errlentry.H>
#include <util/singleton.H>
#include <error_info_defs.H>

namespace SBEIO
{

//-----------------------------------------------------------------------------
//  Interfaces to the SBE FIFO device driver to be used by clients
//  within this component.
//-----------------------------------------------------------------------------

/** @class SbeFifo
 *  *  @brief Class for SBE/FIFO communication
 *  */

class SbeFifo
{
    public:

        /**
         * @brief get the instance of this class
         *
         * @return the (one and only) instance of SbeFifo
         */

        static SbeFifo & getTheInstance();

        /**
         * @brief enums for SBE command class
         */
        enum fifoCommandClass
        {
            SBE_FIFO_CLASS_UNKNOWN             = 0,
            SBE_FIFO_CLASS_IPL_CONTROL         = 0xA1,
            SBE_FIFO_CLASS_SCOM_ACCESS         = 0xA2,
            SBE_FIFO_CLASS_RING_ACCESS         = 0xA3,
            SBE_FIFO_CLASS_MEMORY_ACCESS       = 0xA4,
            SBE_FIFO_CLASS_REGISTER_ACCESS     = 0xA5,
            SBE_FIFO_CLASS_ARRAY_ACCESS        = 0xA6,
            SBE_FIFO_CLASS_INSTRUCTION_CONTROL = 0xA7,
            SBE_FIFO_CLASS_GENERIC_MESSAGE     = 0xA8,
            SBE_FIFO_CLASS_MPIPL_COMMANDS      = 0xA9,
        };

        /**
         * @brief enums for FIFO SCOM Access Messages
         */
        enum fifoScomAccessMessage
        {
            SBE_FIFO_CMD_GET_SCOM            = 0x01,
            SBE_FIFO_CMD_PUT_SCOM            = 0x02,
            SBE_FIFO_CMD_MODIFY_SCOM         = 0x03,
            SBE_FIFO_CMD_PUT_SCOM_UNDER_MASK = 0x04,
        };

        /**
        * @brief enums for FIFO Generic Messages
        */
        enum fifoGenericMessage
        {
            SBE_FIFO_CMD_GET_SBE_FFDC        = 0x01,
        };

        /**
        * @brief enums for FIFO MPIPL Messages
        */
        enum fifoMpiplMessage
        {
            SBE_FIFO_CMD_ENTER_MPIPL         = 0x01,
            SBE_FIFO_CMD_CONTINUE_MPIPL      = 0x02,
            SBE_FIFO_CMD_STOP_CLOCKS         = 0x03,
        };

        /**
        * @brief Struct for FIFO Continue MPIPL request
        *
        */
        struct fifoContinueMpiplRequest
        {
            uint32_t wordCnt;
            uint16_t reserved;
            uint8_t  commandClass;
            uint8_t  command;
            fifoContinueMpiplRequest() :
            wordCnt(2),
            reserved(0)
            {
            }
        } PACKED;

        /**
        * @brief Struct for FIFO Get SBE FFDC request
        *
        */
        struct fifoGetSbeFfdcRequest
        {
            uint32_t wordCnt;
            uint16_t reserved;
            uint8_t  commandClass;
            uint8_t  command;
            fifoGetSbeFfdcRequest() :
            wordCnt(2),
            reserved(0)
            {
            }
        } PACKED;

        /**
         * @brief Struct for FIFO Get SCOM request
         *
         */
        struct fifoGetScomRequest
        {
            uint32_t wordCnt;
            uint16_t reserved;
            uint8_t  commandClass;
            uint8_t  command;
            uint64_t address;  // Register Address (0..31) + (32..63)
            fifoGetScomRequest() :
                wordCnt(4),
                reserved(0)
            {
            }
        } PACKED;

        /**
         * @brief Struct for FIFO Put SCOM request
         *
         */
        struct fifoPutScomRequest
        {
            uint32_t wordCnt;
            uint16_t reserved;
            uint8_t  commandClass;
            uint8_t  command;
            uint64_t address;     // Register Address (0..31) + (32..63)
            uint64_t data;        // Data (0..31) + (32..63)
            fifoPutScomRequest() :
                wordCnt(6),
                reserved(0),
                data(0)
            {
            }
        } PACKED;

        /**
         * @brief Struct for FIFO Put SCOM Under Mask request
         *
         */
        struct fifoPutScomUnderMaskRequest
        {
            uint32_t wordCnt;
            uint16_t reserved;
            uint8_t  commandClass;
            uint8_t  command;
            uint64_t address;     // Register Address (0..31) + (32..63)
            uint64_t data;        // Data (0..31) + (32..63)
            uint64_t mask;        // Mask Data (0..31) + (32..63)
            fifoPutScomUnderMaskRequest() :
                wordCnt(8),
                reserved(0),
                data(0),
                mask(0)
            {
            }
        } PACKED;

        /**
        * @brief enums for FIFO Magic values
        */
        enum fifoMagic
        {
            FIFO_STATUS_MAGIC = 0xC0DE,
            FIFO_FFDC_MAGIC   = 0xFFDC,
        };

        /**
         * @brief Struct for FIFO status
         *
         */
        struct statusHeader
        {
            uint16_t  magic;     // set to 0xC0DE
            uint8_t   commandClass;
            uint8_t   command;
            uint16_t  primaryStatus;
            uint16_t  secondaryStatus;
        } PACKED;

        /**
         * @brief Struct for FIFO FFDC
         *
         */
        struct ffdcHeader
        {
            uint16_t  magic;     // set to 0xFFDC
            uint16_t  lengthWords;
            uint16_t  sequenceId;
            uint8_t   commandClass;
            uint8_t   command;
            uint32_t  returnCode;
        } PACKED;

        /**
         * @brief Struct for FIFO Put SCOM and Put SCOM under mask response
         *
         * The actual number of returned words varies based on whether there was
         * an error.
         */
        struct fifoPutScomResponse
        {
            statusHeader status;
            struct       fapi2::ffdc_struct ffdc;  // ffdc data
            uint32_t     status_distance; // distance to status
            fifoPutScomResponse() {}
        } PACKED;

        /**
         * @brief Struct for FIFO Get SCOM response
         *
         * The actual number of returned words varies based on whether there was
         * an error.
         */
        struct fifoGetScomResponse
        {
            uint64_t     data;          // Data (0..31) + (32..63)
            statusHeader status;
            struct       fapi2::ffdc_struct ffdc;  // ffdc data
            uint32_t     status_distance; // distance to status
            fifoGetScomResponse() {}
        } PACKED;

        /**
        * @brief Struct for FIFO Continue MPIPL response
        *
        * The actual number of returned words varies based on whether there was
        * an error.
        */
        struct fifoContinueMpiplResponse
        {
            statusHeader status;
            struct       fapi2::ffdc_struct ffdc;  // ffdc data
            uint32_t     status_distance; // distance to status
            fifoContinueMpiplResponse() {}
        } PACKED;

        /**
         * @Brief perform SBE FIFO chip-op
         *
         * @param[in]  i_target        Target to access
         * @param[in]  i_pFifoRequest  Pointer to request
         * @param[in]  i_pFifoResponse Pointer to response
         * @param[in]  i_responseSize  Size of response in bytes
         *
         * @return errlHndl_t Error log handle on failure.
         */
        errlHndl_t performFifoChipOp(TARGETING::Target * i_target,
                                     uint32_t * i_pFifoRequest,
                                     uint32_t * i_pFifoResponse,
                                     uint32_t   i_responseSize);

        /**
         * @Brief perform SBE FIFO reset
         *
         *        NOT to be used on running SBE -- only for IPL
         *
         * @param[in]  i_target        Target to access
         *
         * @return errlHndl_t Error log handle on failure.
         */
        errlHndl_t performFifoReset(TARGETING::Target * i_target);


    protected:

         /**
          * @Brief Constructor
          */

        SbeFifo();

        /**
         * @Brief Destructor
         */

        ~SbeFifo();

        /**
         * @brief populate the iv_ffdcPackageBuffer
         * @param[in]  i_data        FFDC error data
         * @param[in]  i_len         data buffer len to copy
         */

        void writeFFDCBuffer(const void * i_data, uint32_t i_len);

    private:

         /**
          * @brief Write FFDC package buffer - holds information exchanged
          * between SBE and HB
          */
        void * iv_ffdcPackageBuffer;

         /**
          * @brief FFDC package needs to be 2 pages
          */
        const uint8_t ffdcPackageSize = 2;

        /**
         * @brief zero out the FFDC package buffer
         */
        void initFFDCPackageBuffer();

        //-------------------------------------------------------------------
        // Local definitions for the device driver
        //-------------------------------------------------------------------

        /**
         * @brief send a request via SBE FIFO
         *
         * @param[in]  i_target         Target to access
         * @param[in]  i_pFifoRequest   Pointer to FIFO request.
         *                              First word has count of unit32_t words.
         * @return errlHndl_t Error log handle on failure.
         */
        errlHndl_t writeRequest(TARGETING::Target * i_target,
                                uint32_t * i_pFifoRequest);

        /**
         * @brief read the response via SBE FIFO
         *
         * @param[in]  i_target         Target to access
         * @param[in]  i_pFifoRequest   Pointer to FIFO request.
         * @param[out] o_pFifoResponse  Pointer to FIFO response.
         * @param[in]  i_responseSize   Size of response buffer in bytes.
         * @return errlHndl_t Error log handle on failure.
         */
        errlHndl_t readResponse(TARGETING::Target * i_target,
                                uint32_t * i_pFifoRequest,
                                uint32_t * o_pFifoResponse,
                                uint32_t   i_responseSize);

        /**
         * @brief poll until uplift Fifo has room to write into
         *
         * @param[in]  i_target         Target to access
         * @return errlHndl_t Error log handle on failure.
         */
        errlHndl_t waitUpFifoReady(TARGETING::Target * i_target);

        /**
         * @brief poll until downlift Fifo has a value to read
         *             or has hit EOT.
         *
         * @param[in]  i_target         Target to access
         * @param[out] o_status         Down load door bell status
         * @return errlHndl_t Error log handle on failure.
         */
        errlHndl_t waitDnFifoReady(TARGETING::Target * i_target,
                                   uint32_t          & o_status);

        /**
         * @brief encapulate FSI read
         *
         * @param[in]  i_target         Target to access
         * @param[in]  i_addr           FSI addressd
         * @param[out] o_pData          Pointer to receive data
         * @return errlHndl_t Error log handle on failure.
         */
        errlHndl_t readFsi(TARGETING::Target * i_target,
                             uint64_t   i_addr,
                             uint32_t * o_pData);
        /**
         * @brief encapulate FSI write
         *
         * @param[in]  i_target         Target to access
         * @param[in]  i_addr           FSI addressd
         * @param[in]  i_pData          Pointer to receive data
         * @return errlHndl_t Error log handle on failure.
         */
        errlHndl_t writeFsi(TARGETING::Target * i_target,
                             uint64_t   i_addr,
                             uint32_t * i_pData);

        /**
         * @brief SBE FIFO FSI register addresses
         */
        enum fifoRegisterAddress
        {
            SBE_FIFO_UPFIFO_DATA_IN    = 0x00002400,
            SBE_FIFO_UPFIFO_STATUS     = 0x00002404,
            SBE_FIFO_UPFIFO_SIG_EOT    = 0x00002408,
            SBE_FIFO_UPFIFO_REQ_RESET  = 0x0000240C,
            SBE_FIFO_DNFIFO_DATA_OUT   = 0x00002440,
            SBE_FIFO_DNFIFO_STATUS     = 0x00002444,
            SBE_FIFO_DNFIFO_RESET      = 0x00002450,
            SBE_FIFO_DNFIFO_ACK_EOT    = 0x00002454,
            SBE_FIFO_DNFIFO_MAX_TSFR   = 0x00002458,
        };

        enum sbeFifoUpstreamStatus
        {
            // Reserved
            UPFIFO_STATUS_RESERVED0 =0xC0000000,
            // Parity Error: dequeuing operation has detected a data parity error
            UPFIFO_STATUS_PARITY_ERROR =0x20000000,
            // Reserved
            UPFIFO_STATUS_RESERVED3 =0x1C000000,
            // External Service Processor (SP) is requesting a FIFO reset
            UPFIFO_STATUS_REQ_RESET_FR_SP =0x02000000,
            // SBE is requesting a FIFO reset through downstream path
            UPFIFO_STATUS_REQ_RESET_FR_SBE =0x01000000,
            // A fifo entry has been dequeued with set EOT flag
            UPFIFO_STATUS_DEQUEUED_EOT_FLAG =0x00800000,
            // Reserved
            UPFIFO_STATUS_RESERVED9 =0x00400000,
            // Upstream FIFO is full
            UPFIFO_STATUS_FIFO_FULL =0x00200000,
            // Upstream FIFO is empty
            UPFIFO_STATUS_FIFO_EMPTY =0x00100000,
            // Number of entries currently in FIFO
            UPFIFO_STATUS_FIFO_ENTRY_COUNT =0x000F0000,
            // Valid flags of entries currently in FIFO
            UPFIFO_STATUS_FIFO_VALID_FLAGS =0x0000FF00,
            // EOT flags of entries currently in FIFO
            UPFIFO_STATUS_FIFO_EOT_FLAGS =0x000000FF,
        };

        enum sbeUpstreamEot
        {
            FSB_UPFIFO_SIG_EOT =0x80000000,
        };

        enum sbeFifoReqUpstreamFifoReset
        {
            FSB_UPFIFO_REQ_RESET =0x80000000,
        };

        enum sbeFifoDownstreamStatus
        {
            // Reserved
            DNFIFO_STATUS_RESERVED0 =0xC0000000,
            // Parity Error: dequeuing operation has detected a data parity error
            DNFIFO_STATUS_PARITY_ERROR =0x20000000,
            // Reserved
            DNFIFO_STATUS_RESERVED3 =0x1C000000,
            // SBE is requesting a FIFO reset
            DNFIFO_STATUS_REQ_RESET_FR_SP =0x02000000,
            // Service Processor (SP) is requesting a FIFO reset
            // through downstream path
            DNFIFO_STATUS_REQ_RESET_FR_SBE =0x01000000,
            // A fifo entry has been dequeued with set EOT flag
            DNFIFO_STATUS_DEQUEUED_EOT_FLAG =0x00800000,
            // Reserved
            DNFIFO_STATUS_RESERVED9 =0x00400000,
            // Downstream FIFO is full
            DNFIFO_STATUS_FIFO_FULL =0x00200000,
            // Downstream FIFO is empty
            DNFIFO_STATUS_FIFO_EMPTY =0x00100000,
            // Number of currently hold entries
            DNFIFO_STATUS_FIFO_ENTRY_COUNT =0x000F0000,
            // Valid flags of ALL currently hold entries
            DNFIFO_STATUS_FIFO_VALID_FLAGS =0x0000FF00,
            // EOT flags of ALL currently hold entries
            DNFIFO_STATUS_FIFO_EOT_FLAGS =0x000000FF,
        };

        enum sbeFifoReqDownstreamFifoReset
        {
            FSB_DNFIFO_REQ_RESET =0x80000000,
        };

        enum sbeDownstreamEot
        {
            FSB_DNFIFO_ACK_EOT =0x80000000,
        };

        enum { MAX_UP_FIFO_TIMEOUT_NS = 10*NS_PER_MSEC }; //=10ms

}; // End of classSbeFifo

}

#endif
