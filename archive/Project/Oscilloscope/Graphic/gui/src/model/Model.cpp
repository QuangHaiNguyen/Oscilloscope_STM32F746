/**************************************************************************************************
* FILE NAME: Model.cpp                                                                            *
*                                                                                                 *
* PURPOSE:   Provide functions to read and write to the data                                      *
*                                                                                                 *
* FILE REFERENCES:                                                                                *
*                                                                                                 *
* Name I/O Description                                                                            *
* ---- --- -----------                                                                            *
*                                                                                                 *
* EXTERNAL VARIABLES:                                                                             *
* Source: < >                                                                                     *
*                                                                                                 *
* Name Type I/O Description                                                                       *
* ---- ---- --- -----------                                                                       *
*                                                                                                 *
* EXTERNAL REFERENCES:                                                                            *
*                                                                                                 *
* Name Description                                                                                *
* ---- -----------                                                                                *
*                                                                                                 *
* ABNORMAL TERMINATION CONDITIONS, ERROR AND WARNING MESSAGES:                                    *
*                                                                                                 *
* ASSUMPTIONS, CONSTRAINTS, RESTRICTIONS:                                                         *
*                                                                                                 *
* NOTES:                                                                                          *
*                                                                                                 *
* REQUIREMENTS/FUNCTIONAL SPECIFICATIONS REFERENCES:                                              *
*                                                                                                 *
* DEVELOPMENT HISTORY:                                                                            *
*                                                                                                 *
* Date         Author     Change Id Release  Description Of Change                                *
* ----         ------     --------- -------  ---------------------                                *
* 30.06.2016   Hai Nguyen        01     2.0  Original Files                                       *
*                                                                                                 *
***************************************************************************************************/
#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/***************************************************************************************************
*                                                                                                  *
* FUNCTION NAME: Model                                                                             *
*                                                                                                  *
* ARGUMENTS:                                                                                       *
*                                                                                                  *
* ARGUMENT TYPE I/O DESCRIPTION                                                                    *
* -------- ---- --- -----------                                                                    *
*                                                                                                  *
* RETURNS:                                                                                         *
*                                                                                                  *
***************************************************************************************************/
Model::Model() : modelListener(0)
{
	chan1.trigger_level = 20;
	chan2.trigger_level = 20;
	chan1.y_offset = 0;
	chan2.y_offset = 0;

	chan1.x_offset = 0;
	chan2.x_offset = 0;
	
	chan1.trigger_level =  0;
	chan2.trigger_level =  0;
	
	chan1.trigger_found = false;
	chan2.trigger_found = false;

	chan1.is_triggered = false;
	chan2.is_triggered = false;
	
	chan1.triger_type = FALLING;
	chan2.triger_type = FALLING;
}

/***************************************************************************************************
*                                                                                                  *
* FUNCTION NAME: tick                                                                              *
*                                                                                                  *
* ARGUMENTS:                                                                                       *
*                                                                                                  *
* ARGUMENT TYPE I/O DESCRIPTION                                                                    *
* -------- ---- --- -----------                                                                    *
*                                                                                                  *
* RETURNS:                                                                                         *
*                                                                                                  *
***************************************************************************************************/
void Model::tick()
{
}

void Model::SetRawData(int channel, uint16_t *data)
{
	
	int i;
	switch (channel)
	{
		case CHANNEL_1:
			for( i = 0; i < 1000; i++)
			{
				chan1.raw_data[i] = (*(data + i)) * GetVoltageOffset(CHANNEL_1) / 4096;
				//chan1.raw_data[i] =  *(data + i);
			}
			break;
		case CHANNEL_2:
			for( i = 0; i < 1000; i++)
			{
				chan2.raw_data[i] = (*(data + i)) * GetVoltageOffset(CHANNEL_2) / 4096;
				//chan2.raw_data[i] =   *(data + i);
			}
			break;
	}
}

void Model::ConvertToTriggerData(int channel)
{
	int trigg_position;
	                                                                       
	int i;
	int j;

	switch (channel)
	{
		case CHANNEL_1:
			trigg_position = 0;
			chan1.trigger_found = false;
			if (chan1.is_triggered == true)
			{
				if (chan1.triger_type == RISING)
				{
					for (i = 99 ; i < 1000 - 305; i++)
					{
						if ((chan1.raw_data[i]  < chan1.trigger_level) &&
							(chan1.raw_data[i + 1] > chan1.trigger_level) &&
							(chan1.raw_data[i + 2] > chan1.trigger_level) &&
							(chan1.raw_data[i + 3] > chan1.trigger_level))
						{
							trigg_position = i + 1 + chan1.x_offset;
							chan1.trigger_found = true;
							for (j = 0; j < NUMBER_OF_POINT; j++)
							{
								chan1.trigger_data[j] = GetVoltageOffset(CHANNEL_1) - (chan1.raw_data[trigg_position]);
								trigg_position++;
							}
							break;
						}
					}
					if (chan1.trigger_found == false)
					{
						for (i = 0 + chan1.x_offset; i < NUMBER_OF_POINT; i++)
						{
							chan1.trigger_data[i] = GetVoltageOffset(CHANNEL_1) - (chan1.raw_data[99 + i]);
						}
					}
				}
				else
				{
					for (i = 99 ; i < 1000 - 305 ; i++)
					{
						if ((chan1.raw_data[i]  > chan1.trigger_level) &&
							(chan1.raw_data[i + 1] < chan1.trigger_level) &&
							(chan1.raw_data[i + 2] < chan1.trigger_level) &&
							(chan1.raw_data[i + 3] < chan1.trigger_level))
						{
							trigg_position = i + 1 + chan1.x_offset ;
							chan1.trigger_found = true;
							for (j = 0; j < NUMBER_OF_POINT; j++)
							{
								chan1.trigger_data[j] = GetVoltageOffset(CHANNEL_1) - (chan1.raw_data[trigg_position]);
								trigg_position++;
							}
							break;
						}
					}
					if (chan1.trigger_found == false)
					{
						for (i = 0 + chan1.x_offset; i < NUMBER_OF_POINT; i++)
						{
							chan1.trigger_data[i] = GetVoltageOffset(CHANNEL_1) - (chan1.raw_data[99 + i]);
						}
					}
				}
				
			}
			else
			{
				for (i = 0 + chan1.x_offset; i < NUMBER_OF_POINT; i++)
				{
					chan1.trigger_data[i] = GetVoltageOffset(CHANNEL_1) - (chan1.raw_data[99 + i]);
				}
			}
			
		break;

		case CHANNEL_2:
			trigg_position = 0;
			chan2.trigger_found = false;
			if (chan2.is_triggered == true)
			{
				if (chan2.triger_type == RISING)
				{
					for (i = 99; i < 1000 - 305; i++)
					{
						if ((chan2.raw_data[i]< chan2.trigger_level) &&
							(chan2.raw_data[i + 1] > chan2.trigger_level) &&
							(chan2.raw_data[i + 2]  > chan2.trigger_level) &&
							(chan2.raw_data[i + 3]  > chan2.trigger_level))
						{
							chan2.trigger_found = true;
							trigg_position = i + 1 + chan2.x_offset;
							for (j = 0; j < NUMBER_OF_POINT; j++)
							{
								chan2.trigger_data[j] = GetVoltageOffset(CHANNEL_2) - (chan2.raw_data[trigg_position]);
								trigg_position++;
							}
							break;
						}
					}
					if (chan2.trigger_found == false)
					{
						for (i = 0 + chan2.x_offset; i < NUMBER_OF_POINT; i++)
						{
							chan2.trigger_data[i] = GetVoltageOffset(CHANNEL_2) - (chan2.raw_data[99 + i]);
						}
					}
				}
				else
				{
					for (i = 99 ; i < 1000 - 305; i++)
					{
						if ((chan2.raw_data[i]> chan2.trigger_level) &&
							(chan2.raw_data[i + 1] < chan2.trigger_level) &&
							(chan2.raw_data[i + 2]  < chan2.trigger_level) &&
							(chan2.raw_data[i + 3]  < chan2.trigger_level))
						{
							chan2.trigger_found = true;
							trigg_position = i + 1 + chan2.x_offset;
							for (j = 0; j < NUMBER_OF_POINT; j++)
							{
								chan2.trigger_data[j] = GetVoltageOffset(CHANNEL_2) - (chan2.raw_data[trigg_position]);
								trigg_position++;
							}
							break;
						}
					}
					if (chan2.trigger_found == false)
					{
						for (i = 0 + chan2.x_offset; i < NUMBER_OF_POINT; i++)
						{
							chan2.trigger_data[i] = GetVoltageOffset(CHANNEL_2) - (chan2.raw_data[99 + i]);
						}
					}
				}
			}
			else
			{
				for (i = 0 + chan2.x_offset; i < NUMBER_OF_POINT; i++)
				{
					chan2.trigger_data[i] = GetVoltageOffset(CHANNEL_2) - (chan2.raw_data[99 + i]);
				}
			}
			
		break;
	}
}

void Model::SetXOffset(int channel, int value)
{
	switch (channel)
	{
	case CHANNEL_1:
		chan1.x_offset = value;
		break;
	case CHANNEL_2:
		chan2.x_offset = value;
		break;
	}
}

int Model::GetXOffset(int channel)
{
	int value;
	switch (channel)
	{
	case CHANNEL_1:
		value = chan1.x_offset;
		break;
	case CHANNEL_2:
		value = chan2.x_offset;
		break;
	}
	return value;
}

void Model::SetTriggerValue(int channel, int value)
{
	switch (channel)
	{
	case CHANNEL_1:
		chan1.trigger_level = value;
		break;
	case CHANNEL_2:
		chan2.trigger_level = value;
		break;
	}
}

int Model::GetTriggerValue(int channel)
{
	int value;
	switch (channel)
	{
	case CHANNEL_1:
		value = chan1.trigger_level;
		break;
	case CHANNEL_2:
		value = chan2.trigger_level;
		break;
	}
	return value;
}

void Model::SetTrigger(int channel, bool value)
{
	
	switch (channel)
	{
	case CHANNEL_1:
		chan1.is_triggered = value;
		break;
	case CHANNEL_2:
		chan2.is_triggered = value;
		break;
	}
}

bool Model::GetTrigger(int channel)
{
	bool value;
	switch (channel)
	{
	case CHANNEL_1:
		value = chan1.is_triggered;
		break;
	case CHANNEL_2:
		value = chan2.is_triggered;
		break;
	}
	return value;
}

void Model::SetTriggerType(int channel, bool value)
{
	switch (channel)
	{
	case CHANNEL_1:
		chan1.triger_type = value;
		break;
	case CHANNEL_2:
		chan2.triger_type = value;
		break;
	}
}

bool Model::GetTriggerType(int channel)
{
	bool value;
	switch (channel)
	{
	case CHANNEL_1:
		value = chan1.triger_type;
		break;
	case CHANNEL_2:
		value = chan2.triger_type;
		break;
	}
	return value;
}

void Model::SetTimeScale(int channel, int value)
{
	switch (channel)
	{
		case CHANNEL_1:
			chan1.time_scale = value;
			break;
		case CHANNEL_2:
			chan2.time_scale = value;
			break;
	}
}

int Model::GetTimeScale(int channel)
{
	int temp_value;
	switch (channel)
	{
		case CHANNEL_1:
			temp_value = chan1.time_scale;
			break;
		case CHANNEL_2:
			temp_value = chan2.time_scale;
			break;
	}
	return temp_value;
}

void Model::SetVoltageScale(int channel, int value)
{
	switch (channel)
	{
		case CHANNEL_1:
			chan1.voltage_scale = value;
			break;
		case CHANNEL_2:
			chan2.voltage_scale = value;
			break;
	}
}

int Model::GetVoltageScale(int channel)
{

	int temp_value;
	switch (channel)
	{
	case CHANNEL_1:
		temp_value = chan1.voltage_scale;
		break;
	case CHANNEL_2:
		temp_value = chan2.voltage_scale;
		break;
	}
	return temp_value;
}

float Model::GetTimeOffset(int channel)
{
	float temp_value;
	if (channel == CHANNEL_1)
	{
		switch (chan1.time_scale)
		{
			//div 50us
			case 0:
				temp_value = 1.316f;
				break;
			//div 100us 
			case 1:
				temp_value = 2.632f;
				break;
			//div 200us 
			case 2:
				temp_value = 5.263f;
				break;
			//div 500us 
			case 3:
				temp_value = 13.158f;
				break;
			//div 1ms
			case 4:
				temp_value = 26.316f;
				break;
			//div 2ms
			case 5:
				temp_value = 52.632f;
				break;
			//div 5ms
			case 6:
				temp_value = 131.579f;
				break;
			//div 10ms
			case 7:
				temp_value = 263.158f;
				break;
			//div 20ms
			case 8:
				temp_value = 526.318f;
				break;
			//div 50ms
			case 9:
				temp_value = 1315.789f;
				break;
			//div 100ms
			case 10:
				temp_value = 2631.579f;
				break;
			//div 200ms
			case 11:
				temp_value = 5263.158f;
				break;
			//div 500ms
			case 12:
				temp_value = 13157.894f;
				break;
		}
	}
	else
	{
		switch (chan2.time_scale)
		{
			//div 50us
		case 0:
			temp_value = 1.316f;
			break;
			//div 100us 
		case 1:
			temp_value = 2.632f;
			break;
			//div 200us 
		case 2:
			temp_value = 5.263f;
			break;
			//div 500us 
		case 3:
			temp_value = 13.158f;
			break;
			//div 1ms
		case 4:
			temp_value = 26.316f;
			break;
			//div 2ms
		case 5:
			temp_value = 52.632f;
			break;
			//div 5ms
		case 6:
			temp_value = 131.579f;
			break;
			//div 10ms
		case 7:
			temp_value = 263.158f;
			break;
			//div 20ms
		case 8:
			temp_value = 526.318f;
			break;
			//div 50ms
		case 9:
			temp_value = 1315.789f;
			break;
			//div 100ms
		case 10:
			temp_value = 2631.579f;
			break;
			//div 200ms
		case 11:
			temp_value = 5263.158f;
			break;
			//div 500ms
		case 12:
			temp_value = 13157.894f;
			break;
		}
	}
	return temp_value;
}

int Model::GetVoltageOffset(int channel)
{
	int temp_value = 0;

	if (channel == CHANNEL_1)
	{
		switch (chan1.voltage_scale)
		{
			//div 2V
			case 7:
				temp_value = 63;
				break;

			//div 1V
			case 6:
				temp_value = 125;
				break;
			//div 500mV
			case 5:
				temp_value = 251;
				break;
			//div 200mV
			case 4:
				temp_value = 627;
				break;
			// div 100mV
			case 3:
				temp_value = 1254;
			//div 50 mV
			case 2:
				temp_value = 2508;
				break;
			//div 20mv
			case 1:
				temp_value = 6270;
				break;
			//div 10 mV
			case 0:
				temp_value = 63;
				break;
		}
	}
	else
	{
		switch (chan2.voltage_scale)
		{
			//div 2V
			case 7:
				temp_value = 63;
				break;

				//div 1V
			case 6:
				temp_value = 125;
				break;
				//div 500mV
			case 5:
				temp_value = 251;
				break;
				//div 200mV
			case 4:
				temp_value = 627;
				break;
				// div 100mV
			case 3:
				temp_value = 1254;
				//div 50 mV
			case 2:
				temp_value = 2508;
				break;
				//div 20mv
			case 1:
				temp_value = 6270;
				break;
				//div 10 mV
			case 0:
				temp_value = 12540;
				break;
		}
	}

	return temp_value;
}

int * Model::GetTriggerData(int channel)
{
	int * p_temp;
	switch (channel)
	{
		case CHANNEL_1:
			p_temp = chan1.trigger_data;
			break;
		case CHANNEL_2:
			p_temp = chan2.trigger_data;
			break;
	}
	return p_temp;
}

void Model::SetYOffset(int channel, int value)
{
	switch (channel)
	{
	case CHANNEL_1:
		chan1.y_offset = value;
		break;
	case CHANNEL_2:
		chan2.y_offset = value;
		break;
	}
}

int Model::GetYOffset(int channel)
{
	int temp;
	switch (channel)
	{
	case CHANNEL_1:
		temp = chan1.y_offset;
		break;
	case CHANNEL_2:
		temp = chan2.y_offset;
		break;
	}
	return temp;
}








