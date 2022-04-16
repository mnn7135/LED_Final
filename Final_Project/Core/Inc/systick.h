typedef struct
{
	uint32_t CSR ;		// control and status register
	uint32_t RVR ;		// reload value register
	uint32_t CVR ;		// current value register
	const uint32_t CALIB ;	// unused

} systick_registers_t;
