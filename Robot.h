#ifndef ROBOT_H
#define ROBOT_H

#define NUM_SONARS  16
#define LOG         1
#define rSonar      0.21
#include <fstream>
#include <iostream>
#include "Simulator.h"
#include <stdlib.h>
#include <math.h>
#include<vector>
#include "fl/Headers.h"


extern "C" {
   #include "extApi.h"
   #include "v_repLib.h"
}

enum state {stand, toGoal, wallfollow, avoidFuzzy };

class Robot
{
public:
    Robot(Simulator *sim, std::string name);
    void update();
    void updateSensors();
    void updatePose();
    void printPose();
    void writeGT();
    void writeOdom();
    void writeSonars();
    void move(float vLeft, float vRight);
    double vRToDrive(double vLinear, double vAngular);
    double vLToDrive(double vLinear, double vAngular);
    void drive(double vLinear, double vAngular);
    void stop();
    //------Métodos Adicionados--------------------------
    void updateOdom();
    std::vector<float> getOdometry();
    void initOdometry();
    void polarErrorCalc(float poseAtual[3]); //Essa função só recebe uma pose para que a gente escolha odometria ou groundTruth
    void goToGoal();
    void setRobotState(state e);
    void checkRobotState();
    bool obstaclesInWay();
    void writePointsSonars();
    void fuzzyController();
    void initFuzzyController();
    void followTheWall();
    float sonarMin(char lado);
    void writePointsSonarsOdom();
    
private:
    const float L = 0.381;                                   // distance between wheels
    const float R = 0.0975;                                  // wheel radius
    std::string name;
    Simulator *sim;

    simxInt handle;                                        // robot handle
    simxFloat velocity[2] = {1,1};                         // wheels' speed
    simxInt sonarHandle[16];                               // handle for sonars
    simxInt motorHandle[2] = {0,0};                        // [0]-> leftMotor [1]->rightMotor
    simxInt encoderHandle[2] = {0,0};
    simxInt goalHandle;
    simxFloat encoder[2] = {0,0};
    simxFloat lastEncoder[2] = {0,0};

    /* Robot Position  */
    simxFloat robotPosition[3] = {0,0,0};                    // current robot position
    simxFloat robotOrientation[3] = {0,0,0};                 // current robot orientation
    float initialPose[3] = {0,0,0};
    simxFloat robotLastPosition[3] = {0,0,0};                // last robot position
    float sonarReadings[NUM_SONARS];

//---------------Privates Adicionados---------------------
    /* Ground Truth */
    float robotPose[3] = {0,0,0};
    float robotLastPose[3] = {0,0,0};

    /* Robot Odometry  */
    float odomPose[3] = {0,0,0};                    // current odometry based pose
    float odomLastPose[3] = {0,0,0};                // last odometry based pose

    /*Robot Control*/
    float polarError[3] = {0,0,0};
    float goal[3] = {0,0,0};
    bool atGoal = false;

    /*Variável de estado*/
    state estado = toGoal;

    /*Fuzzy*/
    fl::Engine *engine = new fl::Engine;
    fl::InputVariable *sensorDir = new fl::InputVariable;
    fl::InputVariable *sensorEsq = new fl::InputVariable;
    fl::OutputVariable *vLinear = new fl::OutputVariable;
    fl::OutputVariable *omega = new fl::OutputVariable;
    fl::RuleBlock *mamdani = new fl::RuleBlock;

    /*Constantes*/
    const int sonarAngles[8] = {90, 50, 30, 10, -10, -30, -50, -90};
    const float limiar = 0.05, minSonarValue = 0.8, minwfDistance = 0.4;
};

#endif // ROBOT_H
