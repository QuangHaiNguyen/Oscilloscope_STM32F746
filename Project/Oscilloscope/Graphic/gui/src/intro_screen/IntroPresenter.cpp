/**************************************************************************************************
* FILE NAME: MainPresenter.cpp                                                                    *
*                                                                                                 *
* PURPOSE:   Provide Function to manipulate the data stored in Model                              *
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
* Date          Author       Change Id   Release   Description Of Change                          *
* ----          ------       ---------   -------   ---------------------                          *
* 30.06.2016    Hai Nguyen          01       2.0   Original                                       *
*                                                                                                 *
***************************************************************************************************/
#include <gui/intro_screen/IntroPresenter.hpp>
#include <gui/intro_screen/IntroView.hpp>
#include <gui/model/Model.hpp>
#include <gui\common\main_header.h>


/***************************************************************************************************
*                                                                                                  *
* FUNCTION NAME:  MainPresenter                                                                    *
*                                                                                                  *
* ARGUMENTS:                                                                                       *
*                                                                                                  *
* ARGUMENT TYPE I/O DESCRIPTION                                                                    *
* -------- ---- --- -----------                                                                    *
*                                                                                                  *
* RETURNS:                                                                                         *
*                                                                                                  *
***************************************************************************************************/
IntroPresenter::IntroPresenter(IntroView& v)
    : view(v)
{
}

/***************************************************************************************************
*                                                                                                  *
* FUNCTION NAME:  activate                                                                         *
*                                                                                                  *
* ARGUMENTS:                                                                                       *
*                                                                                                  *
* ARGUMENT TYPE I/O DESCRIPTION                                                                    *
* -------- ---- --- -----------                                                                    *
*                                                                                                  *
* RETURNS:                                                                                         *
*                                                                                                  *
***************************************************************************************************/
void IntroPresenter::activate()
{
    // Set initial value on main screen
}

/***************************************************************************************************
*                                                                                                  *
* FUNCTION NAME:  deactivate                                                                       *
*                                                                                                  *
* ARGUMENTS:                                                                                       *
*                                                                                                  *
* ARGUMENT TYPE I/O DESCRIPTION                                                                    *
* -------- ---- --- -----------                                                                    *
*                                                                                                  *
* RETURNS:                                                                                         *
*                                                                                                  *
***************************************************************************************************/
void IntroPresenter::deactivate()
{

}


