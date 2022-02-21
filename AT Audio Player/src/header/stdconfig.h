#pragma once

//This header contains the standard settings for ATAP. These settings will be written to "/home/<user>/.config/atap/settings.conf" upon first start
namespace stdconf {
	const unsigned short VOLUME_MAX = 10;		//0 is muted, VOLUME_MAX specifies the highest value
										//Making this value higher won't allow you to increase the
										//total volume, it only makes the range bigger
	const unsigned short SKIP_FORWARD_AMOUNT = 10;
	const unsigned short SKIP_BACKWARD_AMOUNT = 10;

	const unsigned short MENUWIN_MIN_WIDTH = 10;
	const unsigned short MENUWIN_MAX_WIDTH = 0;
}
