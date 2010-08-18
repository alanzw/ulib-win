/*
 * =====================================================================================
 *
 *       Filename:  umixer.h
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  2009-12-25 1:37:31
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#ifndef U_MIXER_H
#define U_MIXER_H

#include "ulib.h"

class ULIB_API UMixer
{
public:
    UMixer();
    ~UMixer();


private:
    HMIXER m_hMixer;
};

#endif // U_MIXER_H

