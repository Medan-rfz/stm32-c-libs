#ifndef __QUATERNION_H
#define __QUATERNION_H

typedef struct {
	double w;
	double x;
	double y;
	double z;
}Quat;


typedef struct {
	double heading;
	double alt;
	double bank;
}EulerAngles;


extern EulerAngles myAngles;
extern EulerAngles dAngles;
extern EulerAngles calibrAngles;

EulerAngles getNewAngles(EulerAngles deltaAng);


#endif













