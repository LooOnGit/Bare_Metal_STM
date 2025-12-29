#ifdef __GPIO__H__
#define __GPIO__C__
#include <stm32f411xe.h>

#endif // __GPIO__H__

:: tạo .gitkeep
type NUL > Header_CMSIS\STM32CubeF4\.gitkeep

:: stage .gitkeep (và nếu muốn, stage thay đổi gpio.h)
git add Header_CMSIS\STM32CubeF4\.gitkeep
git add Inc\gpio.h

:: commit và push
git commit -m "Keep Header_CMSIS/STM32CubeF4 directory"
git push