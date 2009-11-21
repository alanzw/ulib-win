/*
 * =====================================================================================
 *
 *       Filename:  test_hys.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/01/2009 04:10:26 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#include "uengine.h"

int main()
{
    huys::ScriptEngine::UEngine use;

    use.doString("prt Hello");
    use.doString("prt Hello,2");
    use.doString("%prt Hello,2");
    use.doString("  ");

    use.doFile("test.hys");

    return 0;
}
