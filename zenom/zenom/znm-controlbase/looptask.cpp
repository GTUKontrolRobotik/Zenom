/*
 * LoopTask.cpp
 *
 *  Created on: Apr 4, 2012
 *      Author: root
 */

#include <iostream>
#include "looptask.h"
#include <datarepository.h>
#include "controlbase.h"

LoopTask::LoopTask( ControlBase* pControlBase
                    ,std::chrono::duration<double> period
                    ,std::string name)
    : TaskXn(name, period)
    , mControlBase(pControlBase)

{
    std::cerr << "LoopTask constructor, period " << period.count() <<
              " seconds" << std::endl;
}

void LoopTask::run()
{
    int error = 0;
    if( mControlBase->mState != STOPPED )
    {
        if( mControlBase->mState != PAUSED )
        {
            try
            {
                error = mControlBase->doloop();			// User Function
                if( error )
                {
                    std::cerr << "The doloop() function returned non zero: "
                              << error << std::endl;
                }
            }
            catch( std::exception& e )
            {
                error = -1;
                std::cerr << "An exception occured in the doloop() function:"
                          << e.what() << std::endl;
            }
            catch (...)
            {
                error = -1;
                std::cerr << "An unknown exception occured in the doloop()"
                             " function." << std::endl;
            }


            mControlBase->logVariables( elapsedTimeSec() );
            mControlBase->syncMainHeap();
        }
	}

    if( elapsedTimeSec() > mControlBase->duration() || error )
    {
        std::cerr << "LoopTask lifetime has ended" << std::endl;
        DataRepository::instance()->sendStateRequest( R_STOP );
        this->requestPeriodicTaskTermination();
    }
}
