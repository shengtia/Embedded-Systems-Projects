//balance_bot.h
// This file is used to store constants and operators used by many files

#define RAD2GRAD 57.2957795
#define CAP(val, constraint) ((val > constraint) ? constraint : ((val<-constraint) ? -constraint : val))
#define MAX_CONTROL_OUTPUT 100
#define MAX_ANGLE 3
