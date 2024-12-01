#ifndef LM35_CFG_H_  // Header guard to prevent multiple inclusions of this header file
#define LM35_CFG_H_

#include "LM35.h"  // Include the LM35 sensor header file to access LM35_CfgType

// Declaration of the LM35_CfgParam array which holds the configuration parameters for the LM35 sensors
extern const LM35_CfgType LM35_CfgParam[SENSORS_NUM];  // 'SENSORS_NUM' specifies the number of LM35 sensors to configure

#endif /* LM35_CFG_H_ */  // End of the header guard
