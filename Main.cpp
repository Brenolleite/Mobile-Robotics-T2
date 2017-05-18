#include <QCoreApplication>
#include "Robot.h"
#include "Simulator.h"
#include <iostream>
#include <unistd.h>

extern "C" {
   #include "extApi.h"
    #include "v_repLib.h"
}

int main(int argc, char *argv[])
{
    Robot *robot;
    Simulator *vrep = new Simulator("127.0.0.1", 25000);
    if (vrep->connect() ==-1){
        std::cout << "Failed to Connect" << std::endl;
        return 0;
    }
    vrep->setSynchronous();
    vrep->startSimulation();
    robot = new Robot(vrep, "Pioneer_p3dx");

    // Start Fuzzy controller to be used later
    robot->initFuzzyController();
    
    for (int i=0; i<3000; ++i)
    {        
        if (i==4)
            robot->initOdometry();
        if (i>4)
        {
            robot->update();
            robot->writeOdom();
            robot->writeGT();
            robot->writeSonars();
            robot->writePointsSonars();
            robot->writePointsSonarsOdom();
        }
        extApi_sleepMs(50);
    }
    vrep->stopSimulation();
    vrep->disconnect();
    exit(0);
}
