/*
 *  Ondrej Nedojedly 
 *     https://github.com/ondranedo/photoshell
 *  Created: 2024/12/26
 */

#include <base/string.h>

u128 string_count_front(const char * str, char ch) {
    u128 count;
    for(count = 0; count < (u128)(-1) && str[count] == ch; count++);
    return count;
}