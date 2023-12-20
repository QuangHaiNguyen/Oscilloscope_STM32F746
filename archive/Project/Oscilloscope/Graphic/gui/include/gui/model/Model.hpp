/**************************************************************************************************
* NAME: Model.hpp                                                                                 *
*                                                                                                 *
* PURPOSE: Header of the Model.cpp, this file store the data according to MVC model               *
*                                                                                                 *
* PULIC VARIABLES:                                                                                *
*                                                                                                 *
* Variable        Type            Description                                                     *
* --------        ----            -----------                                                     *
*                                                                                                 *
* PUBLIC FUNCTIONS:                                                                               *
*                                                                                                 *
* Name                   Return      Description                                                  *
* ----                   ------      -----------                                                  *
* Model                    void      Constructor for Model class                                  *
* bind                     void      Bind this model to the Presenter                             *
* tick                     void      call back function for every tick of the system              *
* GetGraphParameter         int      Return the element od Graph srtuct                           *
* SetGraphParameter        void      Set the value of elements of Graph struct                    *
*                                                                                                 *
* PRIVATE VARIABLES:                                                                              *
*                                                                                                 *
* Variable                    Type                 Description                                    *
* --------                    ----                 -----------                                    *
* graph_1                     struct Graph         Store elements of the graph i.e. offset,       *
*                                                     trigger level...                            * 
*                                                                                                 *
* PRIVATE FUNCTIONS:                                                                              *
*                                                                                                 *
* Name                    Return               Description                                        *
* ----                    ------               -----------                                        *
*                                                                                                 *
* Date        Author       Change Id Release  Description Of Change                               *
* ----        ------       --------- -------  ---------------------                               *
* 30.06.2016  Hai Nguyen          01     2.0  Original Files                                      *
*                                                                                                 *
**************************************************************************************************/
#ifndef MODEL_HPP
#define MODEL_HPP

#include <touchgfx/hal/Types.hpp>
#include <gui\common\main_header.h>
class ModelListener;

/**
 * The Model class defines the data model in the model-view-presenter paradigm.
 * The Model is a singular object used across all presenters. The currently active
 * presenter will have a pointer to the Model through deriving from ModelListener.
 *
 * The Model will typically contain UI state information that must be kept alive
 * through screen transitions. It also usually provides the interface to the rest
 * of the system (the backend). As such, the Model can receive events and data from
 * the backend and inform the current presenter of such events through the modelListener
 * pointer, which is automatically configured to point to the current presenter.
 * Conversely, the current presenter can trigger events in the backend through the Model.
 */
class Model
{
public:
	Model();

	/**
	 * Sets the modelListener to point to the currently active presenter. Called automatically
	 * when switching screen.
	 */
	void bind(ModelListener* listener)
	{
		modelListener = listener;
	}

	/**
	 * This function will be called automatically every frame. Can be used to e.g. sample hardware
	 * peripherals or read events from the surrounding system and inject events to the GUI through
	 * the ModelListener interface.
	 */
	void tick();

	void SetRawData(int channel, uint16_t *data);
	void ConvertToTriggerData(int channel);
	int * GetTriggerData(int channel);

	void SetYOffset(int channel, int value);
	int GetYOffset(int channel);

	void SetXOffset(int channel, int value);
	int GetXOffset(int channel);

	void SetTriggerValue(int channel, int value);
	int GetTriggerValue(int channel);

	void SetTrigger(int channel, bool value);
	bool GetTrigger(int channel);

	void SetTriggerType(int channel, bool value);
	bool GetTriggerType(int channel);

	void SetTimeScale(int channel, int value);
	int GetTimeScale(int channel);

	void SetVoltageScale(int channel, int value);
	int GetVoltageScale(int channel);

	float GetTimeOffset(int channel);
	int GetVoltageOffset(int channel);

protected:
	/**
	 * Pointer to the currently active presenter.
	 */
	ModelListener* modelListener;

	
	
private: 

	struct GraphData
	{
		uint16_t raw_data[1000];
		int channel;
		int trigger_data[NUMBER_OF_POINT];
		int trigger_level;
		bool triger_type;
		bool trigger_found;
		bool is_triggered;
		int voltage_scale;
		int time_scale;
		int x_offset;
		int y_offset;
	} chan1, chan2;	
};

#endif /* MODEL_HPP */
