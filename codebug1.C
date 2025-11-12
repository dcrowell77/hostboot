/*
 Which version of generate_mmio_addr() is correct and why?
 */

typedef uint32_t XSComAddress_t;
typedef uint64_t XSComBase_t;

// Read register 0x20123456 and return the value
uint64_t read_a_scom( void )
{
	// The address of the register to read
	const uint32_t l_reg_addr = 0x20123456;
	
	// Find the specific offset of this register
	TARGETING::Target* l_chip = get_my_chip();
	uint64_t l_mmio_addr = generate_mmio_addr( l_chip, l_reg_addr );
	
	// All registers are 8-bytes long, read it by dereferencing
	//  the address as a pointer
	uint64_t l_reg_value = *((uint64_t*)l_mmio_addr);
	
	return l_reg_value;
}

// Version A
uint64_t generate_mmio_addr( TARGETING::Target* i_chip,
                             uint64_t i_scomAddr )
{
    uint64_t l_returnAddr = 0;

    // Get the target chip's physical memory mapped i/o address
    uint64_t l_XSComBaseAddr =
      i_chip->getAttr<TARGETING::ATTR_XSCOM_BASE_ADDRESS>();

    // Build the XSCom address (relative to group 0, chip 0)
    XSComP9Address l_mmioAddr(i_scomAddr);

    // Compute value relative to target chip
    l_returnAddr = l_XSComBaseAddr + l_mmioAddr;

    return l_returnAddr;
}

// Version B
uint64_t generate_mmio_addr( TARGETING::Target* i_chip,
                             uint64_t i_scomAddr )
{
    uint64_t l_returnAddr = 0;

    // Get the target chip's physical memory mapped i/o address
    uint64_t l_XSComBaseAddr =
      i_chip->getAttr<TARGETING::ATTR_XSCOM_BASE_ADDRESS>();

    // Build the XSCom address (relative to group 0, chip 0)
    XSComP9Address l_mmioAddr(i_scomAddr);

    // Get the offset
    uint64_t l_offset = l_mmioAddr.offset();

    // Compute value relative to target chip
    l_returnAddr = l_XSComBaseAddr + l_offset;

    return l_returnAddr;
}

class XSComP9Address
{
  public:
    /**
     * @brief Constructor of XSComP9Address class
     *
     * @param[in]   i_addr          PCB address of the register being accessed
     *
     * @return  None
     */
    XSComP9Address(const XSComAddress_t i_addr)
	:mMmioAddress(0)
	{
		// Relative address of Group 0, chip 0
		// The chip's groupId and chip id will be taken into account
		// when calculating its XSCOM base address
		mAddressParts.mSComAddr = i_addr;
	};

    /**
     * @brief Conversion operator
     */
    operator uint64_t() const
	{
		return mMmioAddress;
	};

    /**
     * @brief Return the address' 64-bit full offset
     *
     * @return  uint64_t
     */
    uint64_t offset(void) const
	{
		return (mMmioAddress / sizeof(uint64_t));
	};

  private:
    /**
      * @brief   Disabled copy constructor and assignment operator
      */
    XSComP9Address(const XSComP9Address& i_right);
    XSComP9Address& operator=(const XSComP9Address& right);

    // Layout of XSCOM address parts
    union
    {
        uint64_t mMmioAddress;          // mMmio address
        struct
        {
            uint64_t mReserved1:8;      // Not currently used (0:7)
            uint64_t mBaseAddrHi:7;     // Base address [8:14] (8:14)
            uint64_t mGroupId:4;        // Group where target resides (15:18)
            uint64_t mChipId:3;         // Targeted chip ID (19:21)
            uint64_t mBaseAddrLo:8;     // Base address [22:29] (22:29)
            uint64_t mSComAddr:31;      // PIB Address (30:60)
            uint64_t mAlign:3;          // Align (61:63)
        } mAddressParts;
    };

}; // End XSComP9Address class

