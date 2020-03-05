/*
 * unit_test_facilitator.h
 *
 *  Created on: Apr 27, 2019
 *      Author: Jay
 */

#ifndef UNIT_TEST_FACILITATOR_H_
#define UNIT_TEST_FACILITATOR_H_

#ifdef UNIT_TESTING
  FOREVER for(bool __once = true; __once; __once = !__once)
#else
  FOREVER(float speed, float kph, float pwm, float speed_l) while(speed < (kph) && pwm < speed_l)
#endif



#endif /* UNIT_TEST_FACILITATOR_H_ */
