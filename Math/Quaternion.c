#include "Quaternion.h"
#include "math.h"


#define M_PI   3.14159265358979323846


Quat quatNormalize(Quat *normQuat);
Quat quatMultQuat(Quat *a, Quat *b);

Quat quatFromEulerAngles(EulerAngles eulerAng);
EulerAngles quatToEulerAngles(Quat q);

EulerAngles myAngles = {0, 0, 0};
EulerAngles dAngles = {0, 0, 0};
EulerAngles calibrAngles = {0, 0, 0};


//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
Quat quatNormalize(Quat *normQuat)
{
	Quat result = {0};
	double temp = (normQuat->w * normQuat->w) + (normQuat->x * normQuat->x) + (normQuat->y * normQuat->y) + (normQuat->z * normQuat->z);
	double len = sqrt(temp);
	
	result.w = normQuat->w / len;
	result.x = normQuat->x / len;
	result.y = normQuat->y / len;
	result.z = normQuat->z / len;
	
	return result;
}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
Quat quatMultQuat(Quat *a, Quat *b)
{
	Quat result = {0};
	
	result.w = (a->w * b->w) - (a->x * b->x) - (a->y * b->y) - (a->z * b->z);
	result.x = (a->w * b->x) + (a->x * b->w) + (a->y * b->z) - (a->z * b->y);
	result.y = (a->w * b->y) - (a->x * b->z) + (a->y * b->w) + (a->z * b->x);
	result.z = (a->w * b->z) + (a->x * b->y) - (a->y * b->x) + (a->z * b->w);

	return result;
}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
Quat quatFromEulerAngles(EulerAngles eulerAng)
{
	Quat q_heading;
	Quat q_alt;
	Quat q_bank;
	
	Quat q_tmp;
	
	eulerAng.bank = (eulerAng.bank * M_PI) / 180;
	eulerAng.alt = (eulerAng.alt * M_PI) / 180;
	eulerAng.heading = (eulerAng.heading * M_PI) / 180;
	
	q_heading.w = cos(eulerAng.heading / 2);
	q_heading.x = 0;
	q_heading.y = 0;
	q_heading.z = sin(eulerAng.heading / 2);
	
	q_alt.w = cos(eulerAng.alt / 2);
	q_alt.x = 0;
	q_alt.y = sin(eulerAng.alt / 2);
	q_alt.z = 0;
	
	q_bank.w = cos(eulerAng.bank / 2);
	q_bank.x = sin(eulerAng.bank / 2);
	q_bank.y = 0;
	q_bank.z = 0;
	
	q_tmp = quatMultQuat(&q_heading, &q_alt);
	
	return quatMultQuat(&q_tmp, &q_bank);
}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
EulerAngles quatToEulerAngles(Quat q)
{
	q = quatNormalize(&q);						
	
	double qx2 = q.x * q.x;
	double qy2 = q.y * q.y;
	double qz2 = q.z * q.z;

	EulerAngles result;
	
	result.bank = atan2(2*(q.x * q.w + q.y * q.z), 1 - 2*(qx2 + qy2));
	result.alt = asin(2*(q.y * q.w - q.z * q.x));
	result.heading = atan2(2*(q.z * q.w + q.x * q.y), 1 - 2*(qy2 + qz2));
	
	result.bank = (result.bank * 180) / M_PI;
	result.alt = (result.alt * 180) / M_PI;
	result.heading = (result.heading * 180) / M_PI;
	
	return result;
}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
EulerAngles getNewAngles(EulerAngles deltaAng)
{
	EulerAngles newAngles;

	Quat myQuat;
	Quat quatRot;
	
	myQuat = quatFromEulerAngles(myAngles);
	quatRot = quatFromEulerAngles(deltaAng);
	
	myQuat = quatMultQuat(&myQuat, &quatRot);
	
	newAngles = quatToEulerAngles(myQuat);
	
	return newAngles;
}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//





