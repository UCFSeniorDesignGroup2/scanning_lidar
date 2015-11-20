#ifndef USB_CDC_H
#define USB_CDC_H


namespace HAL
{
	class USB_CDC
	{
	public:

		// happy compiler
		virtual ~USB_CDC() {};

		// set up cdc device
		virtual int Initialize() = 0;

		// writes `buffer_size` bytes over usb_cdc
		// returns 0 if success
		// blocking call
		virtual int WriteData(
				const unsigned char* buffer,
				unsigned int buffer_size) = 0;

		// blocks until at least 1 byte received
		// reads up to `buffer_size` bytes from usb_cdc buffer
		// returns `number of bytes read` or -1
		virtual int ReadData(
				unsigned char* buffer,
				unsigned int buffer_size) = 0;

		// get instance of USB_CDC
		static USB_CDC* GetInstance() { return mInstance; }

	protected:


		static USB_CDC* mInstance;

	};
}





#endif
